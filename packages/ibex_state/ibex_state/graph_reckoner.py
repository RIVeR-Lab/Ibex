#!/usr/bin/env python3
import rclpy
from rclpy.node import Node

from sensor_msgs.msg import Joy
from ackermann_msgs.msg import AckermannDriveStamped

from rclpy.executors import ExternalShutdownException


class GraphReckoner(Node):

    def __init__(self):
        super().__init__("graph_reckoner")

        #==========# Parameters #==========# (controlled in sim/real.yaml)

        # self.n_vehicles = self.declare_parameter("n_vehicles", 0).get_parameter_value().integer_value


        #==========# Publishers & Subscribers #==========#

        self.create_subscription(Joy, joy_topic, self.joy_cb, 1)

        self.joy_pubs = []
        self.drive_pubs = []
        for i in range(self.n_vehicles):
            self.joy_pubs.append(self.create_publisher(Joy, f"/veh_{i}/{joy_topic}", 1))
            self.drive_pubs.append(self.create_publisher(AckermannDriveStamped, f"/veh_{i}/{drive_topic}", 1))

        #==========# State #==========#

        self.active_vehicle = 0
        self.last_dpad_y = 0.0

        # self.get_logger().info(f"Subscribing to joy topic: {joy_topic}")
        # self.get_logger().info(f"n_vehicles: {self.n_vehicles}, selector_only_veh_0_joy: {self.selector_only_veh_0_joy}")


    def joy_cb(self, msg: Joy):
        if len(msg.buttons) <= self.teleop_deadman_button:
            return
 
        # Dpad up/down to cycle active vehicle
        if len(msg.axes) > self.dpad_y_axis:
            dpad_y = msg.axes[self.dpad_y_axis]
            if dpad_y == 1.0 and self.last_dpad_y != 1.0:  # up: previous vehicle
                self.active_vehicle = max(0, self.active_vehicle - 1)
                self.get_logger().info(f"Active vehicle: veh_{self.active_vehicle}")
            elif dpad_y == -1.0 and self.last_dpad_y != -1.0:  # down: next vehicle
                self.active_vehicle = min(self.n_vehicles - 1, self.active_vehicle + 1)
                self.get_logger().info(f"Active vehicle: veh_{self.active_vehicle}")
            self.last_dpad_y = dpad_y
 
        # Publish raw joy
        if self.selector_only_veh_0_joy: # always publish to /veh_0/joy (auto control) for deadman on all trail vehicles and publish to active vehicle (ekf)
            self.joy_pubs[0].publish(msg)
            if self.active_vehicle != 0:
                self.joy_pubs[self.active_vehicle].publish(msg)
        else: # publish to active vehicle's joy topic
            self.joy_pubs[self.active_vehicle].publish(msg)
 
        # Gate drive commands behind LB
        if not msg.buttons[self.teleop_deadman_button]:
            return
 
        if len(msg.axes) <= max(self.joy_speed_axis, self.joy_steer_axis):
            self.get_logger().warn("Joy message has fewer axes than expected")
            return
 
        drive_msg = AckermannDriveStamped()
        drive_msg.header.stamp = self.get_clock().now().to_msg()
        drive_msg.drive.speed = max(0.0,msg.axes[self.joy_speed_axis] * self.joy_speed_scale) # clamped because Gym crashes :/
        drive_msg.drive.steering_angle = msg.axes[self.joy_steer_axis] * self.joy_steer_scale
 
        if self.selector_only_veh_0_joy: # drive active vehicle, brake all others
            brake_msg = AckermannDriveStamped()
            brake_msg.header.stamp = self.get_clock().now().to_msg()
            for i in range(self.n_vehicles):
                if i == self.active_vehicle:
                    self.drive_pubs[i].publish(drive_msg)
                else:
                    self.drive_pubs[i].publish(brake_msg)
        else: # drive active vehicle only
            self.drive_pubs[self.active_vehicle].publish(drive_msg)


def main(args=None):
    rclpy.init(args=args)
    node = GraphReckoner()

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