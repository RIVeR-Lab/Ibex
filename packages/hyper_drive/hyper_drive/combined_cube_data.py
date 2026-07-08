import cv2
import os
import numpy as np
import sys

# Messages
from std_msgs.msg import Header
from sensor_msgs.msg import Image
from hyper_drive_interfaces.msg import DataCube

# ROS Imports
import rclpy
from rclpy.node import Node
from rclpy.logging import get_logging_directory
from ament_index_python.packages import get_package_share_directory
import ros2_numpy

class CombineDataCube(Node):
    def __init__(self):
        
        super().__init__('combined_data_cube')
        # Place holder variable for received images
        self.ximea_cube = []
        self.imec_cube = []
        self.alvium = []
        self.imec_msg = None
        self.ximea_msg = None
        
        # Load homography matrices
        pkg_path = get_package_share_directory('hyper_drive')
        # IMEC to Alvium
        self.i2a = np.load(os.path.join(pkg_path, 'config', 'homographies', 'i2a.npy'))
        # XIMEA to Alvium
        self.x2a = np.load(os.path.join(pkg_path, 'config', 'homographies', 'x2a.npy'))
        # Load mask for cimbined data area
        self.mask = np.load(os.path.join(pkg_path, 'config', 'homographies', 'mask.npy'))

        # Subscribe to incoming data values
        self.ximea_sub = self.create_subscription(DataCube, '/ximea/undistort_data', self.callback_ximea, 10)
        self.imec_sub = self.create_subscription(DataCube, '/imec/undistort_data', self.callback_imec, 10)
        self.alvium_sub = self.create_subscription(Image, '/camera/image_raw', self.callback_alvium, 10)

        # Publish output data
        self.pub_merge = self.create_publisher(DataCube, '/combined/undistort_data', 10)
        self.pub_pan = self.create_publisher(Image, '/combined/pan', 10)

        # Run Start
        self.timer = self.create_timer(0.1, self.start)  # 10Hz

    def callback_imec(self, msg: DataCube) -> None:
        '''
        Callback function to receive undistorted IMEC datacube
        '''
        self.imec_msg = msg
        self.imec_cube = np.reshape(msg.data, (msg.width, msg.height, msg.lam))


    def callback_ximea(self, msg: DataCube) -> None:
        '''
        Callback function to receive undistorted XIMEA datacube
        '''
        self.ximea_msg = msg
        self.ximea_cube = np.reshape(msg.data, (msg.width, msg.height, msg.lam))

    
    def callback_alvium(self, msg: Image) -> None:
        '''
        Callback function to receive udistorted RGB image
        '''
        self.alvium = ros2_numpy.numpify(msg)


    def warp_and_merge(self, alvium: np.ndarray, ximea: np.ndarray, imec: np.ndarray, apply_filter: bool=False) -> np.ndarray:
        '''
        Create combined hypercube by using affine transformations
        '''
        print(f"ximea: {ximea.shape} alvium: {alvium.shape} imec: {imec.shape}")
        cube = np.zeros((*alvium.shape, ximea.shape[2]+imec.shape[2]))
        # Warp the ximea image
        cube[:,:,:ximea.shape[2]] = cv2.warpAffine(
            ximea,
            self.x2a, 
            (alvium.shape[1], alvium.shape[0]),
        )
        # Warp the imec image:
        cube[:,:,-imec.shape[2]:] = cv2.warpAffine(
            imec,
            self.i2a, 
            (alvium.shape[1], alvium.shape[0]),
        )
        # apply final crop to maximum available data
        return cube
    
    def combine_and_publish(self, out: np.ndarray) -> None:
        '''
        Take datacube, and generate metadata
        '''
        print('Combining')
        toSend = DataCube()
        # Create header
        h = Header()
        h.stamp = self.get_clock().now().to_msg()
        toSend.header = h
        toSend.data = out.flatten()
        toSend.width,toSend.height,toSend.lam = out.shape
        toSend.central_wavelengths = np.concatenate((self.ximea_msg.central_wavelengths, self.imec_msg.central_wavelengths), axis=None)
        toSend.qe = np.concatenate((self.ximea_msg.qe, self.imec_msg.qe), axis=None)
        toSend.fwhm_nm = np.concatenate((self.ximea_msg.fwhm_nm, self.imec_msg.fwhm_nm), axis=None)
        self.pub_merge.publish(toSend)


    def start(self) -> None:
        '''
        Main execution  loop to enable periodic publishing
        of combined hyperspectral datacube
        '''

        if self.ximea_cube != [] and self.imec_cube !=[] and self.alvium != []:
            out = self.warp_and_merge(self.alvium, self.ximea_cube, self.imec_cube)
            self.combine_and_publish(out)

def main(args=None):
    rclpy.init(args=args)
    combined_data_cube = CombineDataCube()
    rclpy.spin(combined_data_cube)
    combined_data_cube.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()