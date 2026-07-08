import numpy as np
import cv2

# Messages
from std_msgs.msg import String
from sensor_msgs.msg import Image
from hyper_drive_interfaces.msg import DataCube

# ROS Imports
import rclpy
from rclpy.node import Node
import ros2_numpy

class CubeVisualizer(Node):
    def __init__(self):
        super().__init__('cube_visualizer')

        self.cube = np.array([])
        self.num_bands = 0

        # Subscribe to cube data
        self.cube_sub = self.create_subscription(
            DataCube,
            'cube_data',
            self.callback_cube,
            10
        )

        # Publisher for each band
        self.band_pubs = []

        # Publisher for false color RGB image
        self.pub_false_color = self.create_publisher(Image, '/visualizer/false_color', 10)

        # Publisher for all bands as a grid
        self.pub_grid = self.create_publisher(Image, '/visualizer/band_grid', 10)

        self.get_logger().info('Cube Visualizer started, waiting for data...')

    def normalize_band(self, band: np.ndarray) -> np.ndarray:
        '''
        Contrast stretch a single band to 8-bit
        '''
        band_min = band.min()
        band_max = band.max()
        if band_max == band_min:
            return np.zeros_like(band, dtype=np.uint8)
        normalized = (band - band_min) / (band_max - band_min) * 255
        return normalized.astype(np.uint8)

    def publish_band(self, band: np.ndarray, pub):
        '''
        Normalize and publish a single band as a grayscale image
        '''
        normalized = self.normalize_band(band)
        rgb = cv2.cvtColor(normalized, cv2.COLOR_GRAY2RGB)
        ros_image = ros2_numpy.msgify(Image, rgb, encoding='rgb8')
        ros_image.header.stamp = self.get_clock().now().to_msg()
        ros_image.header.frame_id = 'camera'
        pub.publish(ros_image)

    def publish_false_color(self, cube: np.ndarray):
        '''
        Map 3 evenly spaced bands to R, G, B for a false color image
        '''
        num_bands = cube.shape[2]
        r_idx = 0
        g_idx = num_bands // 2
        b_idx = num_bands - 1

        r = self.normalize_band(cube[:, :, r_idx])
        g = self.normalize_band(cube[:, :, g_idx])
        b = self.normalize_band(cube[:, :, b_idx])

        false_color = np.stack([r, g, b], axis=2)
        ros_image = ros2_numpy.msgify(Image, false_color, encoding='rgb8')
        ros_image.header.stamp = self.get_clock().now().to_msg()
        ros_image.header.frame_id = 'camera'
        self.pub_false_color.publish(ros_image)
        self.get_logger().info(f'False color: R=band{r_idx}, G=band{g_idx}, B=band{b_idx}')

    def publish_band_grid(self, cube: np.ndarray):
        '''
        Arrange all bands into a single grid image for overview
        '''
        num_bands = cube.shape[2]
        h, w = cube.shape[:2]

        # Arrange bands in a row
        grid_row = []
        for i in range(num_bands):
            band = self.normalize_band(cube[:, :, i])
            # Add band label
            labeled = cv2.putText(
                band.copy(), f'B{i}',
                (5, 20), cv2.FONT_HERSHEY_SIMPLEX,
                0.5, 255, 1
            )
            grid_row.append(labeled)

        # Concatenate all bands horizontally
        grid = np.concatenate(grid_row, axis=1)
        grid_rgb = cv2.cvtColor(grid, cv2.COLOR_GRAY2RGB)

        ros_image = ros2_numpy.msgify(Image, grid_rgb, encoding='rgb8')
        ros_image.header.stamp = self.get_clock().now().to_msg()
        ros_image.header.frame_id = 'camera'
        self.pub_grid.publish(ros_image)

    def callback_cube(self, msg: DataCube):
        '''
        Receive cube and publish all visualizations
        '''
        # Reshape flat data back into cube
        cube = np.frombuffer(bytes(msg.data), dtype=np.float32).reshape(
            (msg.width, msg.height, msg.lam)
        )
        # cube = np.reshape(
        #     np.array(msg.data),
        #     (msg.width, msg.height, msg.lam)
        # ).astype(np.float32)

        num_bands = msg.lam

        # Create per-band publishers if not already created
        # if len(self.band_pubs) != num_bands:
        #     self.band_pubs = []
        #     for i in range(num_bands):
        #         pub = self.create_publisher(Image, f'/visualizer/band_{i}', 10)
        #         self.band_pubs.append(pub)
        #     self.get_logger().info(f'Created publishers for {num_bands} bands')

        # Publish each individual band
        # for i in range(num_bands):
        #     self.publish_band(cube[:, :, i], self.band_pubs[i])

        # Publish false color composite
        # self.publish_false_color(cube)

        # Publish band grid overview
        self.publish_band_grid(cube)

        self.get_logger().info(
            f'Published {num_bands} bands | '
            f'min: {cube.min():.1f} max: {cube.max():.1f} mean: {cube.mean():.1f}'
        )

def main(args=None):
    rclpy.init(args=args)
    visualizer = CubeVisualizer()
    try:
        rclpy.spin(visualizer)
    finally:
        visualizer.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()