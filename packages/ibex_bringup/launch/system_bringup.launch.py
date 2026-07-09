# Launch file for IBEX basic functionality
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import PathJoinSubstitution

from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    ld = LaunchDescription()

    #=========================# Sub Launches #=========================#

    static_tf_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution(
                [FindPackageShare("ibex_bringup"), "launch", "static_tf.launch.py"]
            )
        ),
    )
    ld.add_action(static_tf_launch)

    sensors_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution(
                [FindPackageShare("ibex_bringup"), "launch", "sensors.launch.py"]
            )
        ),
    )
    ld.add_action(sensors_launch)

    return ld