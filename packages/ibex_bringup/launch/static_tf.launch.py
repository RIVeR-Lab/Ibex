# Launch file for IBEX static transforms
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    ld = LaunchDescription()

    #=========================# Static Transforms #=========================#
    # arguments are: x y z yaw pitch roll parent_frame child_frame

    # front_bumper to sensor_rack
    ouster_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="ouster_static_tf",
        arguments=[
            "-1.850", "0.0", "1.250",
            "0.0", "0.0", "0.0",
            "front_bumper",
            "sensor_rack",
        ],
        output="screen",
    )
    ld.add_action(ouster_tf)


    #=====# Ouster Lidar #=====#

    # sensor_rack to os_mount
    insta360_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="insta360_static_tf",
        arguments=[
            ".830", "0.0", "-0.11",
            "0.0", "0.436", "0.0",
            "sensor_rack",
            "os_mount",
        ],
        output="screen",
    )
    ld.add_action(insta360_tf)

    # os_mount to os_lidar
    insta360_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="insta360_static_tf",
        arguments=[
            "0.0", "0.0", "0.0",
            "0.0", "0.0", "0.0",
            "os_mount",
            "os_sensor",
        ],
        output="screen",
    )
    ld.add_action(insta360_tf)


    #=====# Insta360 #=====#

    # sensor_rack to insta_mount
    insta360_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="insta360_static_tf",
        arguments=[
            ".650", "0.0", "0.20",
            "0.0", "0.0", "0.0",
            "sensor_rack",
            "insta_mount",
        ],
        output="screen",
    )
    ld.add_action(insta360_tf)

    # insta_mount to insta_sensor
    insta360_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="insta360_static_tf",
        arguments=[
            "0.0", "0.0", "0.13",
            "-1.5708", "0.0", "-1.5708",
            "insta_mount",
            "insta_sensor",
        ],
        output="screen",
    )
    ld.add_action(insta360_tf)

    return ld