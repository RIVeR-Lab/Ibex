#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from rclpy.executors import ExternalShutdownException
import math

from nav_msgs.msg import Odometry
from geometry_msgs.msg import PoseWithCovarianceStamped


from ibex_state.estimator.estimators import GraphReckoner


class GraphFrontender(Node):

    def __init__(self):
        super().__init__("graph_frontender")

        #==========# Parameters #==========#

        self.wheelbase = self.declare_parameter("wheelbase", 1.2).get_parameter_value().double_value
        self.primary_period_s = self.declare_parameter("primary_period_s", 1.0/6.0).get_parameter_value().double_value

        self.output_topic = self.declare_parameter("output_topic", "/not_set").get_parameter_value().string_value
        self.lidar_odom_topic = self.declare_parameter("lidar_odom_topic", "/not_set").get_parameter_value().string_value
        # self.ouster_imu_topic = self.declare_parameter("ouster_imu_topic", "/not_set").get_parameter_value().string_value
        # self.insta_imu_topic = self.declare_parameter("insta_imu_topic", "/not_set").get_parameter_value().string_value
        # self.gps_topic = self.declare_parameter("gps_topic", "/not_set").get_parameter_value().string_value

        self.prior_noise_std = self.declare_parameter("prior_noise_std", 0.001).get_parameter_value().double_value
        self.dyn_noise_std = self.declare_parameter("dyn_noise_std", [0.005, 0.005, 1.0, 1.0, 1.0, 0.001]).get_parameter_value().double_array_value
        self.lidar_noise_std = self.declare_parameter("lidar_noise_std", [0.05, 0.05, 0.05, 0.02, 0.02, 0.02]).get_parameter_value().double_array_value
        self.lidar_drift_prior_std = self.declare_parameter("lidar_drift_prior_std", [0.01, 0.01, 0.01, 0.005, 0.005, 0.005]).get_parameter_value().double_array_value
        self.lidar_drift_process_noise_std = self.declare_parameter("lidar_drift_process_noise_std", [0.01, 0.01, 0.01, 0.005, 0.005, 0.005]).get_parameter_value().double_array_value
        self.residual_prop_noise_std = self.declare_parameter("residual_prop_noise_std", [0.01, 0.01, 1.0, 1.0, 1.0, 0.01]).get_parameter_value().double_array_value
        self.gps_prop_noise_std = self.declare_parameter("gps_prop_noise_std", [0.5, 0.5, 0.5, 0.5, 0.5, 0.5]).get_parameter_value().double_array_value
        self.nhc_noise_std = self.declare_parameter("nhc_noise_std", [0.001, 0.001]).get_parameter_value().double_array_value
        self.rate_prior_std = self.declare_parameter("rate_prior_std", [1.0, 1.0]).get_parameter_value().double_array_value
        self.rate_process_noise_std = self.declare_parameter("rate_process_noise_std", [0.5, 0.5]).get_parameter_value().double_array_value
        self.rate_tie_noise_std = self.declare_parameter("rate_tie_noise_std", [0.01, 0.01]).get_parameter_value().double_array_value
        self.gps_noise_std = self.declare_parameter("gps_noise_std", [1.5, 1.5, 3.0]).get_parameter_value().double_array_value
        self.lag_seconds = self.declare_parameter("lag_seconds", 5.0).get_parameter_value().double_value
        self.init_velocity_noise_std = self.declare_parameter("init_velocity_noise_std", 1.0).get_parameter_value().double_value
        self.fallback_vel_noise_std = self.declare_parameter("fallback_vel_noise_std", 5.0).get_parameter_value().double_value

        self.enable_lidar = self.declare_parameter("enable_lidar", True).get_parameter_value().bool_value
        self.enable_IMUs = self.declare_parameter("enable_IMUs", True).get_parameter_value().bool_value
        self.enable_NHC = self.declare_parameter("enable_NHC", True).get_parameter_value().bool_value
        self.enable_rate = self.declare_parameter("enable_rate", True).get_parameter_value().bool_value
        self.enable_gps = self.declare_parameter("enable_gps", True).get_parameter_value().bool_value
        self.enable_debugging = self.declare_parameter("enable_debugging", True).get_parameter_value().bool_value

        self.imu_ouster_gyro_noise_std = self.declare_parameter("imu_ouster_gyro_noise_std", 0.01).get_parameter_value().double_value
        self.imu_ouster_accel_noise_std = self.declare_parameter("imu_ouster_accel_noise_std", 0.05).get_parameter_value().double_value
        self.imu_ouster_integration_noise_std = self.declare_parameter("imu_ouster_integration_noise_std", 1e-4).get_parameter_value().double_value
        self.imu_ouster_gyro_bias_walk_std = self.declare_parameter("imu_ouster_gyro_bias_walk_std", 0.0005).get_parameter_value().double_value
        self.imu_ouster_accel_bias_walk_std = self.declare_parameter("imu_ouster_accel_bias_walk_std", 0.001).get_parameter_value().double_value

        self.imu_insta_gyro_noise_std = self.declare_parameter("imu_insta_gyro_noise_std", 0.01).get_parameter_value().double_value
        self.imu_insta_accel_noise_std = self.declare_parameter("imu_insta_accel_noise_std", 0.05).get_parameter_value().double_value
        self.imu_insta_integration_noise_std = self.declare_parameter("imu_insta_integration_noise_std", 1e-4).get_parameter_value().double_value
        self.imu_insta_gyro_bias_walk_std = self.declare_parameter("imu_insta_gyro_bias_walk_std", 0.0005).get_parameter_value().double_value
        self.imu_insta_accel_bias_walk_std = self.declare_parameter("imu_insta_accel_bias_walk_std", 0.001).get_parameter_value().double_value

        self.imu_configs = {
            "imu_ouster": {
                "gyro_noise_std": self.imu_ouster_gyro_noise_std,
                "accel_noise_std": self.imu_ouster_accel_noise_std,
                "integration_noise_std": self.imu_ouster_integration_noise_std,
                "gyro_bias_walk_std": self.imu_ouster_gyro_bias_walk_std,
                "accel_bias_walk_std": self.imu_ouster_accel_bias_walk_std,
            },
            "imu_insta": {
                "gyro_noise_std": self.imu_insta_gyro_noise_std,
                "accel_noise_std": self.imu_insta_accel_noise_std,
                "integration_noise_std": self.imu_insta_integration_noise_std,
                "gyro_bias_walk_std": self.imu_insta_gyro_bias_walk_std,
                "accel_bias_walk_std": self.imu_insta_accel_bias_walk_std,
            },
        }

        #==========# Estimator #==========#

        self._init_time = self.get_clock().now().nanoseconds * 1e-9

        self.estimator = GraphReckoner(
            wheelbase=self.wheelbase,
            init_time=self._init_time,
            prior_noise_std=self.prior_noise_std,
            init_velocity_noise_std=self.init_velocity_noise_std,
            dyn_noise_std=tuple(self.dyn_noise_std),
            lidar_noise_std=tuple(self.lidar_noise_std),
            lidar_drift_prior_std=tuple(self.lidar_drift_prior_std),
            lidar_drift_process_noise_std=tuple(self.lidar_drift_process_noise_std),
            residual_prop_noise_std=tuple(self.residual_prop_noise_std),
            gps_prop_noise_std=tuple(self.gps_prop_noise_std),
            imu_configs=self.imu_configs,
            nhc_noise_std=tuple(self.nhc_noise_std),
            rate_prior_std=tuple(self.rate_prior_std),
            rate_process_noise_std=tuple(self.rate_process_noise_std),
            rate_tie_noise_std=tuple(self.rate_tie_noise_std),
            fallback_vel_noise_std=self.fallback_vel_noise_std,
            gps_noise_std=tuple(self.gps_noise_std),
            lag_seconds=self.lag_seconds,
            enable_lidar=self.enable_lidar,
            enable_IMUs=self.enable_IMUs,
            enable_NHC=self.enable_NHC,
            enable_rate=self.enable_rate,
            enable_gps=self.enable_gps,
            debugging=self.enable_debugging,
        )

        #==========# Publishers & Subscribers #==========#

        self.primary_timer = self.create_timer(self.primary_period_s, self.primary_timer_cb)

        self.create_subscription(Odometry, self.lidar_odom_topic, self.lidar_cb, 10)
        # self.create_subscription(ImuMsgType, self.ouster_imu_topic, self.ouster_imu_cb, 10)
        # self.create_subscription(ImuMsgType, self.insta_imu_topic, self.insta_imu_cb, 10)
        # self.create_subscription(GpsMsgType, self.gps_topic, self.gps_cb, 10)

        self.pose_pub = self.create_publisher(PoseWithCovarianceStamped, self.output_topic, 10)

        #==========# State #==========#

        self.last_v = 0.0
        self.last_delta = 0.0


    #==========# Timer Callback #==========#

    def primary_timer_cb(self):
        t = self.get_clock().now().nanoseconds * 1e-9
        state = self.estimator.add_primary(t, self.last_v, self.last_delta)
        self._publish_estimate(t)


    #==========# Sensor Callback #==========#

    def lidar_cb(self, msg: Odometry):
        t_lidar = msg.header.stamp.sec + msg.header.stamp.nanosec * 1e-9

        p = msg.pose.pose.position
        q = msg.pose.pose.orientation
        roll, pitch, yaw = self._quat_to_euler(q.x, q.y, q.z, q.w)

        lidar_state_xyzrpy = (p.x, p.y, p.z, roll, pitch, yaw)
        self.estimator.add_lidar(t_lidar, lidar_state_xyzrpy)


    def ouster_imu_cb(self, msg):
        # omega, accel, dt = ...  # extract from msg
        # self.estimator.add_imu("imu_ouster", omega, accel, dt)
        pass

    def insta_imu_cb(self, msg):
        # omega, accel, dt = ...  # extract from msg
        # self.estimator.add_imu("imu_insta", omega, accel, dt)
        pass

    def gps_cb(self, msg):
        # t_gps, gps_xyz = ...  # extract from msg
        # self.estimator.add_gps(t_gps, gps_xyz)
        pass




    #==========# Helpers #==========#

    @staticmethod
    def _quat_to_euler(x, y, z, w):
        sinr_cosp = 2 * (w * x + y * z)
        cosr_cosp = 1 - 2 * (x * x + y * y)
        roll = math.atan2(sinr_cosp, cosr_cosp)

        sinp = 2 * (w * y - z * x)
        sinp = max(-1.0, min(1.0, sinp))
        pitch = math.asin(sinp)

        siny_cosp = 2 * (w * z + x * y)
        cosy_cosp = 1 - 2 * (y * y + z * z)
        yaw = math.atan2(siny_cosp, cosy_cosp)

        return roll, pitch, yaw

    @staticmethod
    def _euler_to_quat(roll, pitch, yaw):
        cr, sr = math.cos(roll * 0.5), math.sin(roll * 0.5)
        cp, sp = math.cos(pitch * 0.5), math.sin(pitch * 0.5)
        cy, sy = math.cos(yaw * 0.5), math.sin(yaw * 0.5)

        qw = cr * cp * cy + sr * sp * sy
        qx = sr * cp * cy - cr * sp * sy
        qy = cr * sp * cy + sr * cp * sy
        qz = cr * cp * sy - sr * sp * cy
        return qx, qy, qz, qw

    def _publish_estimate(self, t):
        state = self.estimator.get_estimate()
        cov = self.estimator.get_covariance()

        x, y, z, roll, pitch, yaw = state
        qx, qy, qz, qw = self._euler_to_quat(roll, pitch, yaw)

        msg = PoseWithCovarianceStamped()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = "odom"

        msg.pose.pose.position.x = x
        msg.pose.pose.position.y = y
        msg.pose.pose.position.z = z
        msg.pose.pose.orientation.x = qx
        msg.pose.pose.orientation.y = qy
        msg.pose.pose.orientation.z = qz
        msg.pose.pose.orientation.w = qw

        # ROS PoseWithCovariance order (x, y, z, rot_x, rot_y, rot_z) matches
        # our (x, y, z, roll, pitch, yaw) convention -- no permutation needed
        msg.pose.covariance = cov.flatten().tolist()

        self.pose_pub.publish(msg)

    


def main(args=None):
    rclpy.init(args=args)
    node = GraphFrontender()

    try:
        rclpy.spin(node)
    except (KeyboardInterrupt, ExternalShutdownException):
        pass
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == "__main__":
    main()