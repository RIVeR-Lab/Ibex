from launch import LaunchDescription
from launch.actions import SetEnvironmentVariable, TimerAction
from launch_ros.actions import Node
import os

# Ordering rationale
# ------------------
# ambient_light_measurement's cubes_callback divides live cubes by the point-
# spectrometer terms (S_raw/S_white/S_dark), which are populated ONLY by
# spectra_callback when /combined_spectra arrives. If cubes arrive first, those
# terms are still their scalar 0 defaults and the correction collapses to a
# scalar (crashing on max()/min()). So the Ibsen spectrometer stack is brought
# up FIRST here, and the camera pipeline is delayed as a block so /combined_spectra
# is already flowing by the time cubes start.
#
# Spectrometer nodes are copied from spectrometer_drivers/ibsen_launch.py.
# NIR is staggered behind VNIR to avoid the FT4222 enumeration race (see that
# launch file's notes). Raw FT4222 USB access needs a udev rule or root:
#   SUBSYSTEM=="usb", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="601c", MODE="0666"

NIR_START_DELAY = 5.0     # VNIR at t=0, NIR at t=+5 (FTDI enumeration race)
CAMERA_START_DELAY = 8.0  # hold the camera block until /combined_spectra is up


def generate_launch_description():
    current_ld_path = os.environ.get('LD_LIBRARY_PATH', '')

    lib_paths = [
        '/opt/imec/hsi-mosaic/bin',
        '/opt/imec/hsi-mosaic/resources/installers/package/api/X64',
        '/opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib', # pleora ebus 6.5.3
        '/opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib/genicam/bin/Linux64_x64',
        # '/opt/pleora/ebus_sdk/Ubuntu-x86_64/lib', # pleora ebus 6.1.1
        # '/opt/pleora/ebus_sdk/Ubuntu-x86_64/lib/genicam/bin/Linux64_x64',
        '/opt/XIMEA/CamTool',
        '/opt/PFSDK_2025.1.0_Linux64/lib',
        current_ld_path
    ]

    ld_path = ':'.join(filter(None, lib_paths))

    # ---- Ibsen spectrometer stack (starts first) --------------------------

    # VNIR streamer (starts immediately)
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

    # NIR streamer (delayed to avoid the FTDI enumeration race)
    ibsen_nir = TimerAction(
        period=NIR_START_DELAY,
        actions=[
            Node(
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
            ),
        ],
    )

    # Combine VNIR + NIR into /combined_spectra (self-throttles until both arrive)
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

    return LaunchDescription([
        SetEnvironmentVariable('GENICAM_ROOT_V3_1', '/opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib/genicam'),
        SetEnvironmentVariable('GENICAM_ROOT_V3_4', '/opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib/genicam'),
        #SetEnvironmentVariable('RMW_IMPLEMENTATION', 'rmw_cyclonedds_cpp'),
        #SetEnvironmentVariable('CYCLONEDDS_URI', 'file:///home/river/cyclonedds.xml'),
        SetEnvironmentVariable('LD_LIBRARY_PATH', ld_path),

        # --- Spectrometer first ---
        ibsen_vnir,
        spectra_combiner,
        ibsen_nir,

        # RGB Camera Node (delayed so the spectrometer leads)
        TimerAction(
            period=CAMERA_START_DELAY,
            actions=[
                Node(
                    package='vimbax_camera',
                    executable='vimbax_camera_node',
                    name='alvium_camera',
                    namespace='alvium',
                    output='screen',
                    emulate_tty=True,
                    parameters=[{
                        'camera_id': 'DEV_1AB22C025217',  # your camera GUID
                        'settings_file': '/home/river/allied_vision_config.xml',
                    }],
                    remappings=[
                        ('/alvium/image_raw', '/camera/image_raw')  # remap to topic synchronous_cubes expects
                    ]
                ),
            ]
        ),

        # Synchronous cube generator for both IMEC and XIMEA cameras
        TimerAction(
            period=CAMERA_START_DELAY + 5.0,
            actions=[
                Node(
                    package='hyper_drive',
                    executable='synchronous_cubes',
                    name='cameraProcessors',
                    output='screen',
                    emulate_tty=True,
                    parameters=[{
                        'sleep': 0.001, # 0.001
                        'x_frame_rate': 15,
                        'x_integration_time': 10,
                        'i_frame_rate': 15,
                        'i_integration_time': 60, #60
                        'time_wait': 0,
                    }]
                ),
            ]
        ),

        # Ambient Light Measurement
        TimerAction(
            period=CAMERA_START_DELAY + 10.0,
            actions=[
                Node(
                    package='hyper_drive',
                    executable='ambient_light_measurement',
                    name='ambient_light',
                    output='screen',
                    emulate_tty=True,
                ),
            ]
        ),

        # Cube visualizer
        TimerAction(
            period=CAMERA_START_DELAY + 12.0,
            actions=[
                Node(
                    package='hyper_drive',
                    executable='corrected_cube_visualizer',
                    output='screen',
                    emulate_tty=True,
                ),
            ]
        ),

    ])
