#!/usr/bin/env python3
"""
extract_references.py

Extract white / dark reference cubes from recorded ROS2 bags and save them as
the .npy files that ambient_light_measurement.py loads.

Background
----------
ambient_light_measurement.py loads static reference cubes once at startup:
    vnir_white_ref.npy   (XIMEA white, flattened)
    swir_white_ref.npy   (IMEC  white, flattened)
    vnir_dark_ref.npy    (XIMEA dark,  flattened)
    swir_dark_ref.npy    (IMEC  dark,  flattened)
and divides every live cube by these baselines. So we only need ONE cube per
reference -- but to reduce sensor noise this script AVERAGES all (or up to
--max-frames) MultipleDataCubes messages in the bag into a single mean cube per
camera before saving.

The bag is expected to contain /synchronous_cubes (MultipleDataCubes), where
    cubes[0] = XIMEA  (407 x 215 x 24)
    cubes[1] = IMEC   (211 x 168 x 9)

Usage
-----
Record a bag while imaging a white target, and another with the lens capped:
    ros2 bag record /synchronous_cubes -o white_bag
    ros2 bag record /synchronous_cubes -o dark_bag

Then:
    python3 extract_references.py --white-bag white_bag --dark-bag dark_bag \
        --out ~/ibex_ws/src/hyper_drive/bag_files/numpy_files

You can also run one at a time with just --white-bag or just --dark-bag.

Notes
-----
* Cube .data is decoded with np.frombuffer(..., dtype=float32), matching how the
  cubes are serialized/decoded elsewhere in the pipeline (the visualizer uses
  the same np.frombuffer decode). If your publisher serializes differently,
  adjust decode_cube().
* References are saved FLATTENED (the calibration script calls .flatten() on
  load anyway), but shape is validated against each cube's width/height/lam
  before saving so a mis-sized capture is caught early.
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

TOPIC = '/synchronous_cubes'
MSG_TYPE = 'hyper_drive_interfaces/msg/MultipleDataCubes'

# Expected cube geometry (total element counts) for validation
XIMEA_ELEMS = 407 * 215 * 24
IMEC_ELEMS = 211 * 168 * 9


def decode_cube(datacube_msg):
    """
    Decode a single DataCube message's .data into a flat float32 numpy array.
    Uses np.frombuffer to match the pipeline's serialization.
    """
    raw = datacube_msg.data
    # np.frombuffer handles bytes / bytearray / array.array('f') buffers.
    # If .data comes through as a plain python list, fall back to np.asarray.
    try:
        arr = np.frombuffer(bytes(raw), dtype=np.float32)
    except (TypeError, ValueError):
        arr = np.asarray(raw, dtype=np.float32)
    return arr.astype(np.float32)


def open_bag(bag_path):
    """Open a bag for sequential reading and return the reader."""
    storage_options = rosbag2_py.StorageOptions(uri=bag_path, storage_id='sqlite3')
    converter_options = rosbag2_py.ConverterOptions(
        input_serialization_format='cdr',
        output_serialization_format='cdr')
    reader = rosbag2_py.SequentialReader()
    reader.open(storage_options, converter_options)
    return reader


def accumulate_bag(bag_path, max_frames=None):
    """
    Read all MultipleDataCubes messages from a bag and return the mean XIMEA and
    IMEC cubes (flattened float32), plus the frame count used.
    """
    reader = open_bag(bag_path)
    msg_class = get_message(MSG_TYPE)

    ximea_sum = None
    imec_sum = None
    n = 0

    while reader.has_next():
        topic, data, _t = reader.read_next()
        if topic != TOPIC:
            continue
        msg = deserialize_message(data, msg_class)
        if len(msg.cubes) < 2:
            print(f"  [warn] message with only {len(msg.cubes)} cubes; skipping")
            continue

        ximea = decode_cube(msg.cubes[0])
        imec = decode_cube(msg.cubes[1])

        if ximea_sum is None:
            ximea_sum = np.zeros_like(ximea, dtype=np.float64)
            imec_sum = np.zeros_like(imec, dtype=np.float64)

        # Guard against a mid-bag size change (shouldn't happen, but be safe)
        if ximea.shape != ximea_sum.shape or imec.shape != imec_sum.shape:
            print(f"  [warn] frame {n} size mismatch "
                  f"(XIMEA {ximea.shape} vs {ximea_sum.shape}, "
                  f"IMEC {imec.shape} vs {imec_sum.shape}); skipping frame")
            continue

        ximea_sum += ximea
        imec_sum += imec
        n += 1

        if max_frames is not None and n >= max_frames:
            break

    if n == 0:
        raise RuntimeError(f"No usable {TOPIC} messages found in {bag_path}")

    ximea_mean = (ximea_sum / n).astype(np.float32)
    imec_mean = (imec_sum / n).astype(np.float32)
    return ximea_mean, imec_mean, n


def validate(name, arr, expected_elems):
    """Warn (do not abort) if the element count doesn't match expectations."""
    if arr.size != expected_elems:
        print(f"  [warn] {name}: {arr.size} elements, expected {expected_elems}. "
              f"Saving anyway, but check that this bag matches your current "
              f"camera resolution/band count.")
    else:
        print(f"  [ok]   {name}: {arr.size} elements")


def process(kind, bag_path, out_dir, ximea_name, imec_name, max_frames):
    print(f"\n=== {kind} reference from: {bag_path} ===")
    ximea_mean, imec_mean, n = accumulate_bag(bag_path, max_frames)
    print(f"  averaged {n} frame(s)")

    validate(f"XIMEA ({kind})", ximea_mean, XIMEA_ELEMS)
    validate(f"IMEC  ({kind})", imec_mean, IMEC_ELEMS)

    ximea_out = os.path.join(out_dir, ximea_name)
    imec_out = os.path.join(out_dir, imec_name)
    np.save(ximea_out, ximea_mean)
    np.save(imec_out, imec_mean)
    print(f"  saved {ximea_out}")
    print(f"  saved {imec_out}")


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--white-bag', help='path to bag recorded while imaging a white target')
    ap.add_argument('--dark-bag', help='path to bag recorded with the lens capped')
    ap.add_argument('--out', required=True,
                    help='output dir, e.g. ~/ibex_ws/src/hyper_drive/Bag_Files/Numpy_Files')
    ap.add_argument('--max-frames', type=int, default=None,
                    help='cap frames averaged per bag (default: use all)')
    args = ap.parse_args()

    if not args.white_bag and not args.dark_bag:
        ap.error("provide at least one of --white-bag / --dark-bag")

    out_dir = os.path.expanduser(args.out)
    os.makedirs(out_dir, exist_ok=True)

    if args.white_bag:
        process('WHITE', os.path.expanduser(args.white_bag), out_dir,
                'vnir_white_ref.npy', 'swir_white_ref.npy', args.max_frames)

    if args.dark_bag:
        process('DARK', os.path.expanduser(args.dark_bag), out_dir,
                'vnir_dark_ref.npy', 'swir_dark_ref.npy', args.max_frames)

    print("\nDone. Remember to rebuild so the .npy files install into share/:")
    print("  cd ~/ibex_ws && colcon build --packages-select hyper_drive")


if __name__ == '__main__':
    main()
