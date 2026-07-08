from launch import LaunchDescription
from launch.actions import SetEnvironmentVariable, DeclareLaunchArgument, GroupAction
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node, PushRosNamespace
import os


def generate_launch_description():
    # Declare arguments
    use_imec_arg = DeclareLaunchArgument('use_imec', default_value='true')
    use_ximea_arg = DeclareLaunchArgument('use_ximea', default_value='true')
    gui_arg = DeclareLaunchArgument('gui', default_value='true')
    use_bag_arg = DeclareLaunchArgument('use_bag', default_value='false')

    current_ld_path = os.environ.get('LD_LIBRARY_PATH','')

    lib_paths = [
        '/opt/imec/hsi-mosaic/bin', # imec 1.12.0.0
        '/opt/imec/hsi-mosaic/resources/installers/package/api/X64',
        '/opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib', # pleora ebus 6.5.3
        '/opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib/genicam/bin/Linux64_x64',
        '/opt/XIMEA/CamTool',
        '/opt/PFSDK_2025.1.0_Linux64/lib',
        current_ld_path
    ]

    ld_path = ':'.join(filter(None, lib_paths))

    # IMEC group
    imec_group = GroupAction(
        condition=IfCondition(LaunchConfiguration('use_imec')),
        actions=[
            PushRosNamespace('imec'),
            Node(
                package='hyper_drive',
                executable='hyper_drive_pub',
                name='cube_processor_imec',
                output='screen',
                parameters=[{
                    'camera_model': 'imec',
                    'frame_rate': 10,
                    'integration_time': 70
                }]
            ),
            # Node(
            #     package='hyper_drive',
            #     executable='undistort_hypercubes.py',
            #     name='cube_correct_imec',
            #     output='screen',
            #     parameters=[{'camera_model': 'imec'}]
            # ),
        ]
    )

    # XIMEA group
    ximea_group = GroupAction(
        condition=IfCondition(LaunchConfiguration('use_ximea')),
        actions=[
            PushRosNamespace('ximea'),
            Node(
                package='hyper_drive',
                executable='hyper_drive_pub',
                name='cube_processor_ximea',
                output='screen',
                parameters=[{
                    'camera_model': 'ximea',
                    'frame_rate': 30,
                    'integration_time': 15
                }]
            ),
            # Node(
            #     package='hyper_drive',
            #     executable='undistort_hypercubes.py',
            #     name='cube_correct_ximea',
            #     output='screen',
            #     parameters=[{'camera_model': 'ximea'}]
            # ),
        ]
    )

    # GUI renderer node
    gui_node = Node(
        package='hyper_drive',
        executable='hsi_hist',
        name='GUI_renderer',
        output='screen',
        condition=IfCondition(LaunchConfiguration('gui'))
    )

    # Cube combiner node
    combiner_node = Node(
        package='hyper_drive',
        executable='combined_cube_data',
        name='cube_combiner',
        output='screen'
    )

    # Commented out rviz node:
    # rviz_node = Node(
    #     package='rviz2',
    #     executable='rviz2',
    #     name='rviz',
    #     condition=IfCondition(LaunchConfiguration('gui')),
    #     arguments=['-d', os.path.join(get_package_share_directory('hyper_drive'), 'rviz_configs', 'hsi_default.rviz')]
    # )

    return LaunchDescription([
        SetEnvironmentVariable('GENICAM_ROOT_V3_1', '/opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib/genicam'),
        SetEnvironmentVariable('LD_LIBRARY_PATH', ld_path),
        use_imec_arg,
        use_ximea_arg,
        gui_arg,
        use_bag_arg,
        imec_group,
        ximea_group,
        gui_node,
        combiner_node,
    ])