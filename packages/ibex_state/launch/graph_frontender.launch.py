# Launch file for IBEX state estimation
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution

from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    ld = LaunchDescription()

    #=========================# Launch Args #=========================#

    use_sim_time = LaunchConfiguration("use_sim_time") # For bag playback
    ld.add_action(DeclareLaunchArgument("use_sim_time", default_value="false", choices=["true", "false"]))



    #=========================# Config #=========================#

    graph_frontender_config = PathJoinSubstitution([FindPackageShare("ibex_state"), "config", "graph_frontender_config.yaml"])



    #=========================# Nodes #=========================#

    graph_frontender_node = Node(
        package="ibex_state",
        executable="graph_frontender",
        name="graph_frontender",
        output="screen",
        parameters=[
            graph_frontender_config,
            {"use_sim_time": use_sim_time},
        ],
    )
    ld.add_action(graph_frontender_node)


    return ld