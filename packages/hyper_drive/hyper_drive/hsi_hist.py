import io
import numpy as np
from PIL import Image
import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib.figure import Figure
from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas

# Messages
from std_msgs.msg import *
from sensor_msgs.msg import *
from hyper_drive_interfaces.msg import MultipleDataCubes
from hyper_drive_interfaces.msg import DataCube

# ROS Imports
import rclpy
from rclpy.node import Node
from rclpy.logging import get_logging_directory
from ament_index_python.packages import get_package_share_directory
import ros2_numpy

class HSI_HIST(Node):
    def __init__(self):
        super().__init__('hsi_hist')

        plt.switch_backend('agg')
        # Current wavelength to display
        self.lam = 0
        # Flag to set if new information is available
        self.new_cube = False
        # Placeholder for hypercube data
        self.cube = np.array([])
        # Placeholder ofr single channel grayscale image
        self.img = np.array([])

        self.declare_parameter('camera_model', '')
        self.cam_model = self.get_parameter('camera_model').get_parameter_value().string_value

        #get cam model
        self.model_sub = self.create_subscription(String, '/hsi_gui/camera', self.callback_model, 10)

        #get slider num
        self.slider_num = self.create_subscription(Int8, '/hsi_gui/channel', self.callback_slider, 10)

        #get cube data
        # TODO - make this a rosparam for the topic
        self.cubes_sub = self.create_subscription(MultipleDataCubes, '/synchronous_cubes', self.callback_cubes, 10)

        # self.imec_data_sub = rospy.Subscriber('/imec/undistort_data', DataCube, self.imec_callback)
        # self.ximea_data_sub = rospy.Subscriber('/ximea/undistort_data', DataCube, self.ximea_callback)
        # self.combined_data_sub = rospy.Subscriber('/combined/undistort_data', DataCube, self.combined_callback)

        #publish image
        self.pub_img = self.create_publisher(Image, '/hsi_gui/channel_img', 10)

        #publish histogram
        self.pub_hist = self.create_publisher(Image, '/hsi_gui/hist_img', 10)
        
    def callback_model(self, msg):
        '''
        Change model and allow only a single topic to run things
        '''
        self.cam_model = msg.data

    def callback_slider(self, msg):
        self.lam = msg.data

    def callback_cubes(self, msg):
        #self.ximea_callback(msg.cubes[0]) # UPDATE: uncomment when using all three cameras
        #self.imec_callback(msg.cubes[1])
        #self.vimba_callback(msg.im)

        self.imec_callback(msg.cubes[0])

    def imec_callback(self, msg):
        if self.cam_model == 'imec':
            # Mark that we've received a new cube
            self.new_cube = True
            self.cube = np.reshape(msg.data, (msg.width, msg.height, msg.lam))
            self.do_update()

    def ximea_callback(self, msg):
        if self.cam_model == 'ximea':
            # Mark that we've received a new cube
            self.new_cube = True
            self.cube = np.reshape(msg.data, (msg.width, msg.height, msg.lam))
            self.do_update()

    def vimba_callback(self, msg):
        if self.cam_model == 'vimba':
            # Mark that we've received a new cube
            self.new_cube = True
            self.cube = ros2_numpy.numpify(msg)

            # cv.imshow('vimba', self.cube[:,:,0])
            # cv.waitKey(1)

            #np.reshape(msg.data, (msg.width, msg.height, msg.lam))
            self.do_update()

    def do_update(self):
        '''
        Callback function for hypercube data
        '''

        # Every time we receive a new cube, we should republish the data to both channels
        self.update_histogram()
        self.update_image()

    def rescale_image(self, arr):
        '''
        Rescale image to 8-bit pixels for display with PyQT
        '''
        if self.cam_model == 'imec':
            # resize image
            scale_percent = 175 # percent of original size
            width = int(arr.shape[1] * scale_percent / 100)
            height = int(arr.shape[0] * scale_percent / 100)
            dim = (width, height)
            arr = cv.resize(arr,  dim, interpolation = cv.INTER_AREA)
        if self.cam_model == 'vimba':
             # resize image
            scale_percent = 25 # percent of original size
            width = int(arr.shape[1] * scale_percent / 100)
            height = int(arr.shape[0] * scale_percent / 100)
            dim = (width, height)
            arr = cv.resize(arr,  dim, interpolation = cv.INTER_AREA)
        
        # UPDATE: Added for contrast
        arr_min = arr.min()
        arr_max = arr.max()
        normalized = (arr - arr_min) / (arr_max - arr_min) * 255
        return normalized.astype('uint8')
        ##################################
        # return ((arr) * (1/((arr.max())) * 255)).astype('uint8')

    def update_histogram(self):
        '''
        Redraw the histogram on the main panel
        '''
        # configure and draw the histogram figure
        if self.img.size > 0:
            histogram, bin_edges = np.histogram(self.cube[:, :, self.lam], bins=50)
            fig = plt.figure(frameon=False)
            canvas = FigureCanvas(fig)
            ax = fig.gca()
            sns.histplot(
                self.cube[:,:,self.lam].flatten(),
                bins=100
            )
            ax.set_title("Grayscale Histogram")
            ax.set_xlabel("grayscale value")
            ax.set_ylabel("pixel count")
            canvas.draw()
            buf = io.BytesIO()
            plt.savefig(buf, format='png', transparent=True)
            buf.seek(0)
            file_bytes = np.asarray(bytearray(buf.read()), dtype=np.uint8)
            img = cv.imdecode(file_bytes, cv.IMREAD_UNCHANGED)
            ros_image = ros2_numpy.msgify(Image, img, encoding="rgba8") #UPDATE: 8UC4
            self.pub_hist.publish(ros_image)
            plt.close()

    def update_image(self, override = False):
        '''
        Update the value of the single channel image
        '''
        if self.cube.size == 0:
            return
        if self.new_cube == False and override == False:
            return
        slice_image = self.cube[:, :, self.lam]
        self.img = slice_image
        img = self.rescale_image(slice_image)
        self.img = img   
        backtorgb = cv.cvtColor(self.img,cv.COLOR_GRAY2RGB)
        ros_image = ros2_numpy.msgify(Image, backtorgb, encoding="rgb8") #UPDATE: 8UC3
        self.pub_img.publish(ros_image)
        self.new_cube = False

def main(args=None):
    rclpy.init(args=args)
    hsi_hist = HSI_HIST()
    rclpy.spin(hsi_hist)
    hsi_hist.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()