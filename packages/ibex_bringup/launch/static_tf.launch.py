# Launch file for IBEX static transforms
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    ld = LaunchDescription()

    #=========================# Static Transforms #=========================#
    # arguments are: x y z yaw pitch roll parent_frame child_frame

    # base_link to front_bumper
    base_to_bumper_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="base_to_bumper_static_tf",
        arguments=[
            "2.4638", "0.0", "0.5715",
            "0.0", "0.0", "0.0",
            "base_link",
            "front_bumper",
        ],
        output="screen",
    )
    ld.add_action(base_to_bumper_tf)

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
    ouster_mount_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="ouster_mount_static_tf",
        arguments=[
            ".830", "0.0", "-0.11",
            "0.0", "0.436", "0.0",
            "sensor_rack",
            "os_mount",
        ],
        output="screen",
    )
    ld.add_action(ouster_mount_tf)

    # os_mount to os_lidar
    ouster_mount_to_lidar_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="ouster_mount_to_lidar_static_tf",
        arguments=[
            "0.0", "0.0", "0.0",
            "0.0", "0.0", "0.0",
            "os_mount",
            "os_sensor",
        ],
        output="screen",
    )
    ld.add_action(ouster_mount_to_lidar_tf)


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
    insta360_to_sensor_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="insta360_to_sensor_static_tf",
        arguments=[
            "0.0", "0.0", "0.13",
            "-1.5708", "0.0", "-1.5708",
            "insta_mount",
            "insta_sensor",
        ],
        output="screen",
    )
    ld.add_action(insta360_to_sensor_tf)

    return ld