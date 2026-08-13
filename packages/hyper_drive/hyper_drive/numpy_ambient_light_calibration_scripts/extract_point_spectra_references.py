#!/usr/bin/env python3
"""
extract_point_spectra_references.py

Extract white / dark POINT-SPECTROMETER reference spectra from recorded ROS2
bags and save them as the .npy files that ambient_light_measurement.py loads.

Background
----------
ambient_light_measurement.py corrects the datacubes using a point-spectrometer
reference. In spectra_callback() it loads two static 1-D reference spectra:

    point_spectra_white_ref.npy   (S_white_spectra)
    point_spectra_dark_ref.npy    (S_dark_spectra)   # currently hardcoded

and then samples them at the wavelengths nearest each camera band:

    ximea_spec_lamba = [argmin(|spec_lamba - w|) for w in static_wavelengths_ximea]
    S_dark_ximea  = dark_data[ximea_spec_lamba]
    S_white_ximea = white_data[ximea_spec_lamba]

The KEY compatibility requirement is therefore:
    the white/dark reference arrays must be aligned element-for-element with
    spec_lamba.npy  (same length, same wavelength ordering).

Because the ambient node subscribes to /combined_spectra (produced by
combine_ibsen.py, which stitches the Ibsen VNIR + NIR spectra together), the
cleanest way to satisfy that requirement is to average the `data` field of
/combined_spectra messages and validate the result against spec_lamba.npy.

This is the point-spectrometer analogue of extract_references.py (which does the
same job for the XIMEA/IMEC datacubes). Same CLI shape, same averaging idea.

Usage
-----
Record a bag while imaging a white target, and another with the fiber capped:
    ros2 bag record /combined_spectra -o point_white_bag
    ros2 bag record /combined_spectra -o point_dark_bag

Then:
    python3 extract_point_spectra_references.py \
        --white-bag point_white_bag --dark-bag point_dark_bag \
        --out ~/ibex_ws/src/ibex/packages/hyper_drive/Bag_Files/Numpy_Files

You can also run one at a time with just --white-bag or just --dark-bag.

Notes
-----
* The reference spectra are saved as flat float32 arrays (the calibration script
  calls .flatten() on load).
* If spec_lamba.npy is found in the output dir (or pointed to with --spec-lamba),
  the script checks that the averaged spectrum has the same length AND that the
  bag's wavelength axis matches spec_lamba. A mismatch here is exactly what would
  silently misalign the dark/white subtraction, so it is surfaced loudly.
* Storage format (sqlite3 vs mcap) is auto-detected from the bag's metadata.yaml;
  override with --storage if needed.
"""

import os
import sys
import argparse
import numpy as np

# ROS2 bag + message imports (available on the ROS2 machine, e.g. volta)
try:
    import rosbag2_py
    from rclpy.serialization import deserialize_message
    from rosidl_runtime_py.utilities import get_message
except ImportError as e:
    sys.exit(f"ROS2 bag libraries not found -- run this on the ROS2 machine "
             f"with your workspace sourced. ({e})")

TOPIC = '/combined_spectra'
MSG_TYPE = 'spectrometer_interfaces/msg/Spectra'

# Soft sanity value: the current hardcoded dark spectrum in the ambient node is
# 305 bins (184 VNIR + 121 NIR). Used only for an informational warning.
EXPECTED_BINS = 305


def detect_storage_id(bag_path):
    """
    Determine the rosbag2 storage plugin id for a bag directory.
    Prefers metadata.yaml; falls back to sniffing file extensions; then sqlite3.
    """
    meta = os.path.join(bag_path, 'metadata.yaml')
    if os.path.isfile(meta):
        try:
            import yaml
            with open(meta) as f:
                d = yaml.safe_load(f)
            sid = d['rosbag2_bagfile_information']['storage_identifier']
            if sid:
                return sid
        except Exception:
            pass  # fall through to extension sniffing
    try:
        for fn in os.listdir(bag_path):
            if fn.endswith('.db3'):
                return 'sqlite3'
            if fn.endswith('.mcap'):
                return 'mcap'
    except OSError:
        pass
    return 'sqlite3'


def decode_spectrum(spectra_msg):
    """
    Return (data, wavelengths) as flat float64 numpy arrays from a Spectra msg.
    float32[] fields deserialize to array.array('f'); np.asarray handles that,
    plain lists, and bytes-like buffers alike.
    """
    data = np.asarray(spectra_msg.data, dtype=np.float64).ravel()
    wave = np.asarray(spectra_msg.wavelengths, dtype=np.float64).ravel()
    return data, wave


def open_bag(bag_path, storage_id):
    """Open a bag for sequential reading and return the reader."""
    storage_options = rosbag2_py.StorageOptions(uri=bag_path, storage_id=storage_id)
    converter_options = rosbag2_py.ConverterOptions(
        input_serialization_format='cdr',
        output_serialization_format='cdr')
    reader = rosbag2_py.SequentialReader()
    reader.open(storage_options, converter_options)
    return reader


def accumulate_bag(bag_path, topic, max_frames=None, storage_id=None):
    """
    Read all Spectra messages from `topic` in a bag and return
    (mean_spectrum float32, wavelengths float64, frame_count).

    The wavelength axis is taken from the first usable frame; later frames are
    checked for a matching axis (spectrometer bins should be static).
    """
    if storage_id is None:
        storage_id = detect_storage_id(bag_path)
    print(f"  storage id: {storage_id}")

    reader = open_bag(bag_path, storage_id)
    msg_class = get_message(MSG_TYPE)

    data_sum = None
    wavelengths = None
    n = 0

    while reader.has_next():
        rd_topic, rd_bytes, _t = reader.read_next()
        if rd_topic != topic:
            continue
        msg = deserialize_message(rd_bytes, msg_class)

        data, wave = decode_spectrum(msg)
        if data.size == 0:
            print(f"  [warn] frame {n}: empty data field; skipping")
            continue

        if data_sum is None:
            data_sum = np.zeros_like(data, dtype=np.float64)
            wavelengths = wave

        if data.shape != data_sum.shape:
            print(f"  [warn] frame {n} length {data.shape} != {data_sum.shape}; "
                  f"skipping frame")
            continue

        # Static bins expected; warn (once-ish) if the axis drifts.
        if wave.shape == wavelengths.shape and not np.allclose(wave, wavelengths, atol=1e-3):
            print(f"  [warn] frame {n}: wavelength axis differs from first frame")

        data_sum += data
        n += 1
        if max_frames is not None and n >= max_frames:
            break

    if n == 0:
        raise RuntimeError(f"No usable {topic} messages found in {bag_path}")

    mean = (data_sum / n).astype(np.float32)
    return mean, wavelengths, n


def load_spec_lamba(out_dir, explicit_path):
    """Return spec_lamba array if available (explicit path wins), else None."""
    path = explicit_path or os.path.join(out_dir, 'spec_lamba.npy')
    if os.path.isfile(path):
        return np.load(path).flatten().astype(np.float64), path
    return None, path


def validate(name, spectrum, wavelengths, spec_lamba, spec_lamba_path):
    """Print alignment diagnostics; warn (do not abort) on problems."""
    print(f"  [info] {name}: {spectrum.size} bins, "
          f"wavelength range {wavelengths.min():.1f}-{wavelengths.max():.1f} nm")

    if spectrum.size != EXPECTED_BINS:
        print(f"  [note] {name}: {spectrum.size} bins vs the ambient node's "
              f"current {EXPECTED_BINS}-bin hardcoded dark array -- fine if your "
              f"combined-spectra length has changed, just be consistent.")

    if spec_lamba is None:
        print(f"  [note] spec_lamba.npy not found at {spec_lamba_path}; skipping "
              f"grid-alignment check. The references MUST match spec_lamba's "
              f"length/ordering for the ambient node to sample them correctly.")
        return

    if spectrum.size != spec_lamba.size:
        print(f"  [WARN] {name}: length {spectrum.size} != spec_lamba length "
              f"{spec_lamba.size}. The ambient node indexes the reference by "
              f"spec_lamba position, so this WILL misalign. Re-check the bag / "
              f"spec_lamba.npy.")
    elif not np.allclose(wavelengths, spec_lamba, atol=0.5):
        max_diff = float(np.max(np.abs(wavelengths - spec_lamba)))
        print(f"  [WARN] {name}: length matches but the bag's wavelength axis "
              f"diverges from spec_lamba (max {max_diff:.2f} nm). The dark/white "
              f"subtraction assumes these line up; verify spec_lamba.npy came "
              f"from the same combined-spectra configuration.")
    else:
        print(f"  [ok]   {name}: aligned with spec_lamba ({spectrum.size} bins)")


def process(kind, bag_path, out_dir, out_name, topic, max_frames, storage_id,
            spec_lamba, spec_lamba_path):
    print(f"\n=== {kind} point-spectrometer reference from: {bag_path} ===")
    mean, wavelengths, n = accumulate_bag(bag_path, topic, max_frames, storage_id)
    print(f"  averaged {n} frame(s)")

    validate(f"{kind} spectrum", mean, wavelengths, spec_lamba, spec_lamba_path)

    out_path = os.path.join(out_dir, out_name)
    np.save(out_path, mean)
    print(f"  saved {out_path}")
    return mean, wavelengths


def main():
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--white-bag', help='bag recorded while imaging a white target')
    ap.add_argument('--dark-bag', help='bag recorded with the fiber/lens capped')
    ap.add_argument('--out', required=True,
                    help='output dir, e.g. '
                         '~/ibex_ws/src/ibex/packages/hyper_drive/Bag_Files/Numpy_Files')
    ap.add_argument('--topic', default=TOPIC,
                    help=f'spectra topic to read (default: {TOPIC})')
    ap.add_argument('--spec-lamba', default=None,
                    help='path to spec_lamba.npy for alignment validation '
                         '(default: <out>/spec_lamba.npy)')
    ap.add_argument('--storage', default=None,
                    help="rosbag2 storage id override, e.g. sqlite3 or mcap "
                         "(default: auto-detect from metadata.yaml)")
    ap.add_argument('--max-frames', type=int, default=None,
                    help='cap frames averaged per bag (default: use all)')
    args = ap.parse_args()

    if not args.white_bag and not args.dark_bag:
        ap.error("provide at least one of --white-bag / --dark-bag")

    out_dir = os.path.expanduser(args.out)
    os.makedirs(out_dir, exist_ok=True)

    spec_lamba, spec_lamba_path = load_spec_lamba(out_dir, args.spec_lamba)

    white = dark = None
    white_wave = dark_wave = None

    if args.white_bag:
        white, white_wave = process(
            'WHITE', os.path.expanduser(args.white_bag), out_dir,
            'point_spectra_white_ref.npy', args.topic, args.max_frames,
            args.storage, spec_lamba, spec_lamba_path)

    if args.dark_bag:
        dark, dark_wave = process(
            'DARK', os.path.expanduser(args.dark_bag), out_dir,
            'point_spectra_dark_ref.npy', args.topic, args.max_frames,
            args.storage, spec_lamba, spec_lamba_path)

    # Cross-check the two references against each other.
    if white is not None and dark is not None:
        if white.size != dark.size:
            print(f"\n[WARN] white ({white.size}) and dark ({dark.size}) have "
                  f"different lengths -- (S_white - S_dark) will fail in the "
                  f"ambient node. Re-record so both match spec_lamba.")
        elif not np.allclose(white_wave, dark_wave, atol=0.5):
            print("\n[WARN] white and dark wavelength axes differ; make sure both "
                  "bags used the same spectrometer configuration.")
        else:
            print("\n[ok] white and dark references share length and wavelength axis.")

    print("\nDone. Remember to rebuild so the .npy files install into share/:")
    print("  cd ~/ibex_ws && colcon build --packages-select hyper_drive")


if __name__ == '__main__':
    main()
