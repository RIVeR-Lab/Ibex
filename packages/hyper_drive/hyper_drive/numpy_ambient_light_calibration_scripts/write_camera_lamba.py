#!/usr/bin/env python3
"""
write_camera_lamba.py

Write the camera band-center wavelength axes that ambient_light_measurement.py
loads:
    ximea_lamba.npy   (24 XIMEA/VNIR band centers, nm)
    imec_lamba.npy    ( 9 IMEC/SWIR  band centers, nm)

These are FIXED by sensor calibration -- they are the `bands_nm` arrays that
synchronous_cubes prints in its "Output Data Format" log line. Values below were
copied verbatim from that log, so this is authoritative and needs no bag.

Pure numpy -- no ROS required. Run anywhere:
    python3 write_camera_lamba.py \
        --out ~/ibex_ws/src/ibex/packages/hyper_drive/bag_files/numpy_files

NOTE: this does NOT touch spec_lamba.npy -- that one was correctly extracted
from /combined_spectra (305 bins, 451-1702 nm); keep it.
"""

import os
import argparse
import numpy as np

# XIMEA (VNIR) 24 band centers, nm -- from synchronous_cubes bands_nm
XIMEA_BANDS_NM = [
    662.74414484, 678.44427179, 691.47995842, 702.79743883,
    718.98022991, 730.09886825, 743.11429966, 757.61797177,
    770.56671122, 779.24140246, 794.57439591, 805.38484958,
    817.52315993, 831.92643985, 842.41924582, 853.91129525,
    867.76937301, 878.09883114, 887.16460955, 901.40985581,
    909.37019374, 919.05507754, 928.17011884, 932.90958447,
]

# IMEC (SWIR) 9 band centers, nm -- from synchronous_cubes bands_nm
IMEC_BANDS_NM = [
    1119.45746577, 1137.42703212, 1159.30855803, 1189.4666138,
    1207.39972145, 1295.01617225, 1378.54223445, 1461.43849505,
    1650.12658601,
]


def write_axis(name, values, out_dir, out_name, expected):
    arr = np.asarray(values, dtype=np.float64).ravel()
    assert arr.size == expected, f"{name}: {arr.size} values, expected {expected}"
    out_path = os.path.join(out_dir, out_name)
    np.save(out_path, arr)
    print(f"  {name}: {arr.size} bands, {arr.min():.1f}-{arr.max():.1f} nm "
          f"-> {out_path}")


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--out', required=True,
                    help='output dir, e.g. .../hyper_drive/bag_files/numpy_files')
    args = ap.parse_args()

    out_dir = os.path.expanduser(args.out)
    os.makedirs(out_dir, exist_ok=True)

    write_axis('ximea_lamba', XIMEA_BANDS_NM, out_dir, 'ximea_lamba.npy', 24)
    write_axis('imec_lamba', IMEC_BANDS_NM, out_dir, 'imec_lamba.npy', 9)

    print("\nDone. Rebuild so the .npy files install into share/:")
    print("  cd ~/ibex_ws && colcon build --packages-select hyper_drive")


if __name__ == '__main__':
    main()
