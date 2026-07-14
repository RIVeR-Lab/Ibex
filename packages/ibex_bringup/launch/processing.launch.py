# Launch file for IBEX sensors
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution

from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    ld = LaunchDescription()




    #=========================# KISS-ICP Config #=========================#

    kiss_icp_topic = "/ouster/points"
    kiss_icp_base_frame = "front_bumper"
    kiss_icp_odom_frame = "odom"
    visualize = "False"


    #=========================# Sub Launches #=========================#

    kiss_icp_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution(
                [FindPackageShare("kiss_icp"), "launch", "odometry.launch.py"]
            )
        ),
        launch_arguments={
            "topic": kiss_icp_topic,
            "base_frame": kiss_icp_base_frame,
            "lidar_odom_frame": kiss_icp_odom_frame,
            "visualize": visualize,
        }.items(),
    )

    ld.add_action(kiss_icp_launch)


    return ld