import cv2 as cv
import numpy as np
import rclpy
from rclpy.node import Node
import ros2_numpy
import PySimpleGUI as sg

from hyper_drive_interfaces.msg import DataCube
from sensor_msgs.msg import Image

from os import mkdir, chdir, scandir, environ
import copy


# Authored by Gary Lvov
class Calibration(Node):
    def __init__(self):
        super().__init__('calibration')
        self.ximea_frame = []
        self.imec_frame = []
        self.alvium_raw = []
        self.alvium_red = [] 
        self.create_folders()

        # Subscriptions
        self.ximea_sub = self.create_subscription(DataCube, '/ximea/cube_data', self.callback_ximea, 10)
        self.imec_sub = self.create_subscription(DataCube, '/imec/cube_data', self.callback_imec, 10)
        self.alvium_sub = self.create_subscription(Image, '/camera/image_raw', self.callback_alvium, 10)
        self.create_gui()

        # Run the gui
        self.timer = self.create_timer(0.1, self.do_gui)


    @staticmethod
    def reject_outliers(data, m=2):
        return data[abs(data - np.mean(data)) < m * np.std(data)]
        
    def callback_ximea(self, msg):
        #recontructs data cube
        ximea_cube = np.reshape(msg.data, (msg.width, msg.height, msg.lam))
        data = self.rescale_image(self.reject_outliers(ximea_cube[:, :, 0], m=4))
        self.ximea_frame = self.rescale_image(ximea_cube[:, :, 0])

    def callback_imec(self, msg):
        #reconstructs data cube
        imec_cube = np.reshape(msg.data, (msg.width, msg.height, msg.lam))
        self.imec_frame = self.rescale_image(imec_cube[:, :, 0])

    def callback_alvium(self, msg):
        frame = ros2_numpy.numpify(msg)
        self.alvium_raw = frame
        self.alvium_red = cv.resize(frame, (2464//5, 2056//5), interpolation=cv.INTER_AREA)

    def create_folders(self):
        try:
            mkdir("/home/river/Desktop/calibration")
        except FileExistsError:
            print("Calibrations exist in this directory, adding newest calibration in new folder")

        mkdir("/home/river/Desktop/calibration/imec")
        mkdir("/home/river/Desktop/calibration/ximea")
        mkdir("/home/river/Desktop/calibration/alvium")

    @staticmethod
    def rescale_image(arr):
        '''
        Rescale image to 8-bit pixels for display with PyQT
        '''
        return ((arr) * (1/((arr.max())) * 255)).astype('uint8')

    def update_config(self):
        chdir("../") # Now in parent folder
        chdir("/scripts/matlab/")
        chdir("../../") # change back to parent folder

    def create_gui(self):
        img_scale = .5

        sg.theme("DarkPurple")

        layout = [[sg.Text("Gary and Ben's Sick Stereo Calibration Helper Tool", size=(40, 1), justification='center', font='Helvetica 20')],
              [sg.Image(filename='', key='cam1'),sg.Image(filename='', key='cam2'),sg.Image(filename='', key='cam3')],
              [sg.Button('Save Image', size=(10, 1), font='Helvetica 14'),]]

        self.window = sg.Window("Gary and Ben's Camera Calibration Helper Tool",
                        layout, location=(800, 400))
        self.saved_count = 0

        
    def do_gui(self):
        event, values = self.window.read(timeout=100)

        if event == 'Save Image':
            cv.imwrite("/home/river/Desktop/calibration/ximea/" + str(self.saved_count) + ".png", self.ximea_frame)
            cv.imwrite("/home/river/Desktop/calibration/imec/" + str(self.saved_count) + ".png", self.imec_frame)
            cv.imwrite("/home/river/Desktop/calibration/alvium/" + str(self.saved_count) + ".png", self.alvium_raw)
        
            self.saved_count += 1
        
        elif event == sg.WIN_CLOSED:
            exit()
        if self.ximea_frame != []:
            imgbytes1 = cv.imencode('.png', self.ximea_frame)[1].tobytes()
            self.window['cam1'].update(data=imgbytes1)
        if self.imec_frame != []:
            imgbytes2 = cv.imencode('.png', self.imec_frame)[1].tobytes()
            self.window['cam2'].update(data=imgbytes2)
        if self.alvium_red != []:
            imgbytes3 = cv.imencode('.png', self.alvium_red)[1].tobytes()
            self.window['cam3'].update(data=imgbytes3)

def main(args=None):
    rclpy.init(args=args)
    calibration = Calibration()
    try:
        rclpy.spin(calibration)
    finally:
        calibration.window.close()
        calibration.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()