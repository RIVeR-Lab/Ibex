# Launch IBEX vehicle control (Shared_link Bridge).
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import PathJoinSubstitution

from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    ld = LaunchDescription()

    #=========================# Sub Launches #=========================#

    shared_link_bridge_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution(
                [FindPackageShare("shared_link_bridge"), "launch", "bringup.launch.py"]
            )
        ),
        # System-level overrides. Uncomment each once the matching
        # DeclareLaunchArgument exists in shared_link_bridge's launch file.
        # launch_arguments={
        #     # Force a safe initial e-stop state at the system level, rather
        #     # than the submodule's testing default.
        #     "estop_initial_state": "ESTOP",
        #     # Interactive TUI handling: launch controller_teleop in its own
        #     # terminal so it doesn't fight the shared launch output stream.
        #     "teleop_in_xterm": "true",
        # }.items(),
    )
    ld.add_action(shared_link_bridge_launch)

    # Future Kairos motion/control launch files get added here as additional
    # IncludeLaunchDescription blocks, keeping this wrapper the single seam for
    # the vehicle-control subsystem.

    return ld