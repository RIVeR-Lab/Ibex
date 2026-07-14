#!/usr/bin/env python3
from launch import LaunchDescription
from launch.actions import TimerAction
from launch_ros.actions import Node
 
# ROS2 port of ibsen.launch
#
# Pipeline:
#   spectral_data_streamer (x2: ibsen_vnir, ibsen_nir)  ->  combine_ibsen  ->  spectral_plot
#
# Namespacing: each streamer runs in its own namespace so its relative
# 'spectral_data' topic resolves to '/ibsen_vnir/spectral_data' and
# '/ibsen_nir/spectral_data', matching what the combiner subscribes to.
#
# STAGGERED STARTUP: each physical FT4222 exposes several sub-interfaces, and
# if both streamer nodes enumerate/open the FTDI devices at the same instant
# they collide -- device descriptions come back blank and one node aborts with
# "INVALID NUMBER OF DETECTED DEVICES". Starting them a few seconds apart lets
# the first node finish claiming its device before the second enumerates.
# The VNIR node starts immediately; the NIR node is delayed via TimerAction.
# If you still see collisions, increase NIR_START_DELAY.
#
# NOTE ON sudo/permissions: raw FT4222 USB access (vendor 0403, product 601c)
# requires either a udev rule or running as root. A one-off `sudo chmod 666
# /dev/bus/usb/BUS/DEV` works until replug/reboot; the persistent fix is a udev
# rule:
#   SUBSYSTEM=="usb", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="601c", MODE="0666"
# placed in /etc/udev/rules.d/, then `sudo udevadm control --reload-rules &&
# sudo udevadm trigger` and replug.
 
NIR_START_DELAY = 5.0  # seconds to wait before starting the NIR streamer
 
 
def generate_launch_description():
    # --- Ibsen VNIR streamer (starts immediately) ---
    ibsen_vnir = Node(
        package='spectrometer_drivers',
        executable='spectral_data_streamer',
        name='ibsen_vnir',
        namespace='ibsen_vnir',
        output='screen',
        parameters=[{
            'min_wavelength': 500.0,
            'max_wavelength': 1100.0,
            'wavelength_range': 'vnir',
            'integration_time': 25.0,
        }],
    )
 
    # --- Ibsen NIR streamer (delayed to avoid FTDI enumeration race) ---
    ibsen_nir = Node(
        package='spectrometer_drivers',
        executable='spectral_data_streamer',
        name='ibsen_nir',
        namespace='ibsen_nir',
        output='screen',
        parameters=[{
            'min_wavelength': 900.0,
            'max_wavelength': 1700.0,
            'wavelength_range': 'nir',
            'integration_time': 250.0,
        }],
    )
 
    ibsen_nir_delayed = TimerAction(
        period=NIR_START_DELAY,
        actions=[ibsen_nir],
    )
 
    # --- Combine VNIR + NIR into one continuous spectrum ---
    spectra_combiner = Node(
        package='spectrometer_drivers',
        executable='combine_ibsen.py',
        name='spectra_combiner',
        output='screen',
        parameters=[{
            'vnir_topic': '/ibsen_vnir/spectral_data',
            'nir_topic': '/ibsen_nir/spectral_data',
            'rate': 20.0,
        }],
    )
 
    # --- Visualize the combined spectrum ---
    combined_plot = Node(
        package='spectrometer_drivers',
        executable='spectral_plot.py',
        name='combined_plot',
        output='screen',
        parameters=[{
            'data_topic': '/combined_spectra',
            'min_wavelength': 500.0,
            'max_wavelength': 1700.0,
            'min_count': 0.0,
            'max_count': 65536.0,
            'spectrometer_model': 'Ibsen Combined',
        }],
    )
 
    return LaunchDescription([
        ibsen_vnir,
        ibsen_nir_delayed,
        spectra_combiner,
        combined_plot,
 
        # --- Optional per-spectrometer plots (disabled, matching ROS1 launch) ---
        # Node(
        #     package='spectrometer_drivers',
        #     executable='spectral_plot.py',
        #     name='ibsen_vis_plot',
        #     output='screen',
        #     parameters=[{
        #         'data_topic': '/ibsen_vnir/spectral_data',
        #         'min_wavelength': 500.0,
        #         'max_wavelength': 1100.0,
        #         'min_count': 0.0,
        #         'max_count': 65536.0,
        #         'spectrometer_model': 'Ibsen VNIR',
        #     }],
        # ),
        # Node(
        #     package='spectrometer_drivers',
        #     executable='spectral_plot.py',
        #     name='ibsen_nir_plot',
        #     output='screen',
        #     parameters=[{
        #         'data_topic': '/ibsen_nir/spectral_data',
        #         'min_wavelength': 900.0,
        #         'max_wavelength': 1700.0,
        #         'min_count': 0.0,
        #         'max_count': 65536.0,
        #         'spectrometer_model': 'Ibsen NIR',
        #     }],
        # ),
    ])

