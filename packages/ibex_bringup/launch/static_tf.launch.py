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
    # Pitch derived empirically from a stationary os_imu accelerometer
    # reading against a known-level vehicle (see docs/ben_notes.md) --
    # the previous 0.436 rad (25 deg) left a ~2.6 deg residual in the
    # wrong (nose-up) direction.
    ouster_mount_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="ouster_mount_static_tf",
        arguments=[
            ".830", "0.0", "-0.11",
            "0.0", "0.391698", "0.0",
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

    # insta_sensor to insta_imu -- rotation derived empirically from a
    # stationary insta_imu accelerometer reading against a known-level
    # vehicle (see docs/ben_notes.md): the IMU die is evidently not
    # axis-aligned with insta_sensor's optical-frame convention (a ~77 deg
    # yaw offset, not just collocated). This is the shortest-arc rotation
    # that levels gravity correctly -- roll/pitch are pinned by that, but
    # yaw about the gravity axis is fundamentally unobservable from a
    # single accelerometer reading alone, so this yaw value is our best
    # working estimate, not a verified physical measurement.
    insta360_imu_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="insta360_imu_static_tf",
        arguments=[
            "0.0", "0.0", "0.0",
            "1.347411", "-0.034602", "-0.223388",
            "insta_sensor",
            "insta_imu",
        ],
        output="screen",
    )
    ld.add_action(insta360_imu_tf)

    return ld



    #=====# GPS #=====#

    # base_link to gps_mount
    gps_mount_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="gps_mount_static_tf",
        arguments=[
            "0.70", "0.60", "0.85",
            "0.0", "0.0", "0.0",
            "base_link",
            "gps_mount",
        ],
        output="screen",
    )
    ld.add_action(gps_mount_tf)