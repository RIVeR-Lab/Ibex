#!/usr/bin/env python3
"""
extract_wavelength_axes.py

Regenerate the wavelength-axis ("lamba" == lambda == wavelength) .npy files that
ambient_light_measurement.py loads:

    spec_lamba.npy    point-spectrometer wavelength grid  (~305 bins, ~451-1702 nm)
    ximea_lamba.npy   XIMEA camera band centers           (24 bands, ~663-933 nm)
    imec_lamba.npy    IMEC  camera band centers            (9  bands, ~1119-1650 nm)

These are NOT bag-averaged references like the white/dark files -- they are just
axes. They travel inside the messages, so we can recover them from bags you
already recorded:

    spec_lamba  <- /combined_spectra  .wavelengths           (point_*_bag)
    ximea_lamba <- /synchronous_cubes cubes[0].central_wavelengths (white_bag/dark_bag)
    imec_lamba  <- /synchronous_cubes cubes[1].central_wavelengths (white_bag/dark_bag)

Axes are static, so we read the FIRST usable message from each bag (and warn if a
later frame's axis drifts).

Usage
-----
    python3 extract_wavelength_axes.py \
        --spectra-bag ~/ibex_ws/rosbag_library/point_white_bag \
        --cubes-bag   ~/ibex_ws/rosbag_library/white_bag \
        --out ~/ibex_ws/src/ibex/packages/hyper_drive/bag_files/numpy_files

Either bag may be omitted (e.g. only --spectra-bag to refresh spec_lamba).
Then rebuild:
    cd ~/ibex_ws && colcon build --packages-select hyper_drive
"""

import os
import sys
import argparse
import numpy as np

try:
    import rosbag2_py
    from rclpy.serialization import deserialize_message
    from rosidl_runtime_py.utilities import get_message
except ImportError as e:
    sys.exit(f"ROS2 bag libraries not found -- run this on the ROS2 machine "
             f"with your workspace sourced. ({e})")

SPECTRA_TOPIC = '/combined_spectra'
SPECTRA_TYPE = 'spectrometer_interfaces/msg/Spectra'
CUBES_TOPIC = '/synchronous_cubes'
CUBES_TYPE = 'hyper_drive_interfaces/msg/MultipleDataCubes'


def detect_storage_id(bag_path):
    """rosbag2 storage id: prefer metadata.yaml, else sniff extensions, else sqlite3."""
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
            pass
    try:
        for fn in os.listdir(bag_path):
            if fn.endswith('.db3'):
                return 'sqlite3'
            if fn.endswith('.mcap'):
                return 'mcap'
    except OSError:
        pass
    return 'sqlite3'


def open_bag(bag_path, storage_id):
    storage_options = rosbag2_py.StorageOptions(uri=bag_path, storage_id=storage_id)
    converter_options = rosbag2_py.ConverterOptions(
        input_serialization_format='cdr', output_serialization_format='cdr')
    reader = rosbag2_py.SequentialReader()
    reader.open(storage_options, converter_options)
    return reader


def iter_messages(bag_path, topic, msg_type):
    """Yield deserialized messages on `topic` from a bag."""
    storage_id = detect_storage_id(bag_path)
    print(f"  storage id: {storage_id}")
    reader = open_bag(bag_path, storage_id)
    msg_class = get_message(msg_type)
    while reader.has_next():
        rd_topic, rd_bytes, _t = reader.read_next()
        if rd_topic != topic:
            continue
        yield deserialize_message(rd_bytes, msg_class)


def check_drift(name, axes):
    """Warn if collected axes are not all (near-)identical."""
    ref = axes[0]
    for i, a in enumerate(axes[1:], 1):
        if a.shape != ref.shape or not np.allclose(a, ref, atol=1e-3):
            print(f"  [warn] {name}: frame {i} axis differs from frame 0 "
                  f"(axes are supposed to be static)")
            break


def save_axis(name, axis, out_dir, out_name, expected=None):
    axis = np.asarray(axis, dtype=np.float64).ravel()
    print(f"  [info] {name}: {axis.size} values, "
          f"{axis.min():.1f}-{axis.max():.1f} nm")
    if expected is not None and axis.size != expected:
        print(f"  [note] {name}: got {axis.size}, expected ~{expected}; "
              f"check the bag matches your current camera band count.")
    out_path = os.path.join(out_dir, out_name)
    np.save(out_path, axis.astype(np.float64))
    print(f"  saved {out_path}")


def do_spectra(bag_path, out_dir, max_check=10):
    print(f"\n=== spec_lamba from: {bag_path} ({SPECTRA_TOPIC}) ===")
    axes = []
    for i, msg in enumerate(iter_messages(bag_path, SPECTRA_TOPIC, SPECTRA_TYPE)):
        w = np.asarray(msg.wavelengths, dtype=np.float64).ravel()
        if w.size:
            axes.append(w)
        if i + 1 >= max_check:
            break
    if not axes:
        raise RuntimeError(f"No {SPECTRA_TOPIC} messages with wavelengths in {bag_path}")
    check_drift('spec_lamba', axes)
    save_axis('spec_lamba', axes[0], out_dir, 'spec_lamba.npy', expected=305)


def do_cubes(bag_path, out_dir, max_check=10):
    print(f"\n=== ximea_lamba / imec_lamba from: {bag_path} ({CUBES_TOPIC}) ===")
    ximea_axes, imec_axes = [], []
    for i, msg in enumerate(iter_messages(bag_path, CUBES_TOPIC, CUBES_TYPE)):
        if len(msg.cubes) < 2:
            continue
        xw = np.asarray(msg.cubes[0].central_wavelengths, dtype=np.float64).ravel()
        iw = np.asarray(msg.cubes[1].central_wavelengths, dtype=np.float64).ravel()
        if xw.size and iw.size:
            ximea_axes.append(xw)
            imec_axes.append(iw)
        if i + 1 >= max_check:
            break
    if not ximea_axes:
        raise RuntimeError(
            f"No usable central_wavelengths in {bag_path}. If this field is empty "
            f"in your cubes, the camera band centers must come from the pipeline "
            f"config instead (the bands_nm printed by synchronous_cubes).")
    check_drift('ximea_lamba', ximea_axes)
    check_drift('imec_lamba', imec_axes)
    save_axis('ximea_lamba', ximea_axes[0], out_dir, 'ximea_lamba.npy', expected=24)
    save_axis('imec_lamba', imec_axes[0], out_dir, 'imec_lamba.npy', expected=9)


def main():
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--spectra-bag', help=f'bag with {SPECTRA_TOPIC} (for spec_lamba)')
    ap.add_argument('--cubes-bag', help=f'bag with {CUBES_TOPIC} (for ximea/imec_lamba)')
    ap.add_argument('--out', required=True,
                    help='output dir, e.g. .../hyper_drive/bag_files/numpy_files')
    args = ap.parse_args()

    if not args.spectra_bag and not args.cubes_bag:
        ap.error("provide at least one of --spectra-bag / --cubes-bag")

    out_dir = os.path.expanduser(args.out)
    os.makedirs(out_dir, exist_ok=True)

    if args.spectra_bag:
        do_spectra(os.path.expanduser(args.spectra_bag), out_dir)
    if args.cubes_bag:
        do_cubes(os.path.expanduser(args.cubes_bag), out_dir)

    print("\nDone. Rebuild so the .npy files install into share/:")
    print("  cd ~/ibex_ws && colcon build --packages-select hyper_drive")


if __name__ == '__main__':
    main()
