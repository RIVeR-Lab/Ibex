# Launch file for IBEX sensors
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution

from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    ld = LaunchDescription()

    #=========================# Launch Args #=========================#

    equirectangular = LaunchConfiguration("equirectangular")
    ld.add_action(DeclareLaunchArgument("equirectangular", default_value="true", choices=["true", "false"]))

    imu_filter = LaunchConfiguration("imu_filter")
    ld.add_action(DeclareLaunchArgument("imu_filter", default_value="true", choices=["true", "false"]))



    #=========================# Configs #=========================#

    ouster_config = PathJoinSubstitution(
        [FindPackageShare("ibex_bringup"), "config", "ibex_ouster_sensor_config.yaml"]
    )




    #=========================# Sub Launches #=========================#

    insta_bringup_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution(
                [FindPackageShare("insta360_ros_driver"), "launch", "insta_bringup.launch.py"]
            )
        ),
        launch_arguments=[
            ("equirectangular", equirectangular),
            ("imu_filter", imu_filter),
        ],
    )
    ld.add_action(insta_bringup_launch)

    

    ouster_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution(
                [FindPackageShare("ouster_ros"), "launch", "driver.launch.py"]
            )
        ),
        launch_arguments=[
            ("params_file", ouster_config),
            ("viz", "false"),
        ],
    )
    ld.add_action(ouster_launch)


    return ld