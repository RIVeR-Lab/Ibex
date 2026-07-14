#!/usr/bin/env python3

import threading
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

import rclpy
from rclpy.node import Node
from spectrometer_interfaces.msg import Spectra

# Matplotlib-based visualization for spectrometer data
# Author: Nathaniel Hanson
# Date: 01/27/23
# Purpose: ROS visualization tool for spectral data
# ROS2 (rclpy) port.
#
# Note on threading: matplotlib's FuncAnimation must own the main thread, so
# ROS2 spinning is done on a background thread. The subscription callback only
# updates shared xdata/ydata; the animation reads them on the main thread.


class SpectrumVisualizer(Node):
    def __init__(self):
        super().__init__('spectral_visualizer')

        # Parameters (ROS2: declare then get)
        self.declare_parameter('spectrometer_model', 'Spectrometer')
        self.declare_parameter('data_topic', '/combined_spectra')
        self.declare_parameter('max_wavelength', 1700.0)
        self.declare_parameter('min_wavelength', 500.0)
        self.declare_parameter('max_count', 65536.0)
        self.declare_parameter('min_count', 0.0)

        self.model = self.get_parameter('spectrometer_model').get_parameter_value().string_value
        self.data_topic = self.get_parameter('data_topic').get_parameter_value().string_value
        self.max_wave = self.get_parameter('max_wavelength').get_parameter_value().double_value
        self.min_wave = self.get_parameter('min_wavelength').get_parameter_value().double_value
        self.max_count = self.get_parameter('max_count').get_parameter_value().double_value
        self.min_count = self.get_parameter('min_count').get_parameter_value().double_value

        # Shared plot data (updated in callback, read by animation)
        self.xdata = []
        self.ydata = []

        # Subscribe to collected data topic
        self.sub = self.create_subscription(
            Spectra, self.data_topic, self.process_single, 10)

    def process_single(self, data: Spectra) -> None:
        '''
        Take in a single spectrometer reading and prepare to plot
        '''
        self.xdata = data.wavelengths
        self.ydata = data.data

    def setup_plot(self) -> None:
        '''
        Initialize plot to visualize the live spectrum
        '''
        self.fig = plt.figure()
        # Change font to Times New Roman because we fancy :)
        plt.rcParams["font.family"] = "Times New Roman"
        self.ax = plt.axes(xlim=(self.min_wave, self.max_wave),
                           ylim=(self.min_count, self.max_count))
        self.ax.set_xlabel('Wavelength (nm)')
        self.ax.set_ylabel('Counts')
        self.ax.set_title(f'{self.model} Spectrometer Live View')
        self.ax.grid(True)
        self.line, = self.ax.plot([], [], lw=3)
        self.ani = FuncAnimation(self.fig, self.update_plot,
                                 init_func=self.init_plot, blit=True,
                                 cache_frame_data=False)

    def init_plot(self):
        self.line.set_data([], [])
        return self.line,

    def update_plot(self, _):
        '''
        Animate graph with current data
        '''
        self.line.set_data(self.xdata, self.ydata)
        return self.line,

    def shutdown(self) -> None:
        '''
        Custom shutdown behavior
        '''
        try:
            self.ani.event_source.stop()
        except Exception:
            pass
        plt.close('all')


def main(args=None):
    rclpy.init(args=args)
    node = SpectrumVisualizer()

    # Spin ROS on a background thread so matplotlib can own the main thread
    executor = rclpy.executors.SingleThreadedExecutor()
    executor.add_node(node)
    spin_thread = threading.Thread(target=executor.spin, daemon=True)
    spin_thread.start()

    try:
        node.setup_plot()
        plt.show()  # blocks main thread until the window is closed
    except KeyboardInterrupt:
        pass
    finally:
        node.get_logger().info('Shutting down visualization window')
        node.shutdown()
        executor.shutdown()
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
