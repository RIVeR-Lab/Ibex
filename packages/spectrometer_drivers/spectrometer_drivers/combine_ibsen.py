#!/usr/bin/env python3

import traceback
import numpy as np
from typing import Tuple

import rclpy
from rclpy.node import Node
from spectrometer_interfaces.msg import Spectra

# Combine Ibsen VNIR and NIR spectrums into a single continuous spectrum
# Author: Nathaniel Hanson
# Date: 01/27/23
# Purpose: ROS visualization tool for spectral data
# ROS2 (rclpy) port.


class SpectrumCombiner(Node):
    def __init__(self):
        super().__init__('spectrum_combiner')

        # Parameters (ROS2: declare then get)
        self.declare_parameter('vnir_topic', '/ibsen_vnir/spectral_data')
        self.declare_parameter('nir_topic', '/ibsen_nir/spectral_data')
        self.declare_parameter('rate', 20.0)

        self.ibsen_vnir_data_topic = self.get_parameter('vnir_topic').get_parameter_value().string_value
        self.ibsen_nir_data_topic = self.get_parameter('nir_topic').get_parameter_value().string_value
        rate_hz = self.get_parameter('rate').get_parameter_value().double_value

        # Create empty variable initialization
        self.ibsen_nir = None
        self.ibsen_vnir = None

        # Subscribe to collected data topics
        self.sub_vnir = self.create_subscription(
            Spectra, self.ibsen_vnir_data_topic, self.process_ibsen_vnir, 10)
        self.sub_nir = self.create_subscription(
            Spectra, self.ibsen_nir_data_topic, self.process_ibsen_nir, 10)

        # Create combined data publisher
        self.pub = self.create_publisher(Spectra, '/combined_spectra', 10)

        # Drive the combine/publish loop on a timer instead of a blocking while loop
        self.timer = self.create_timer(1.0 / rate_hz, self.timer_callback)

    def process_ibsen_vnir(self, data: Spectra) -> None:
        '''
        Take in a single spectrometer reading and prepare to plot
        '''
        self.ibsen_vnir = data

    def process_ibsen_nir(self, data: Spectra) -> None:
        '''
        Take in a single spectrometer reading and prepare to plot
        '''
        self.ibsen_nir = data

    def combine_spectra(self) -> Tuple[np.ndarray, np.ndarray]:
        '''
        Combine spectra, accounting for differences in Quantum efficiencies
        '''
        # Trim the VNIR data < 950 nm
        mask = np.array(self.ibsen_vnir.wavelengths) < 950
        data_tmp_vnir = np.array(self.ibsen_vnir.data)[mask]
        wave_tmp_vnir = np.array(self.ibsen_vnir.wavelengths)[mask]
        # Trim the NIR data > 950 nm
        mask = np.array(self.ibsen_nir.wavelengths) > 950
        data_tmp_nir = np.array(self.ibsen_nir.data)[mask]
        wave_tmp_nir = np.array(self.ibsen_nir.wavelengths)[mask]

        combined_data = np.concatenate((data_tmp_vnir, data_tmp_nir), axis=None)
        combined_wave = np.concatenate((wave_tmp_vnir, wave_tmp_nir), axis=None)
        return combined_data, combined_wave

    def send_combined(self, combined_data: np.ndarray, combined_wave: np.ndarray) -> None:
        '''
        Publish a combined spectrum
        '''
        toSend = Spectra()
        # msg float32[] fields expect plain python lists of floats
        toSend.wavelengths = [float(x) for x in combined_wave]
        toSend.data = [float(x) for x in combined_data]
        self.pub.publish(toSend)

    def timer_callback(self) -> None:
        '''
        Combine and publish composite spectra (runs at the configured rate)
        '''
        try:
            if self.ibsen_nir is not None and self.ibsen_vnir is not None:
                combined_data, combined_wave = self.combine_spectra()
                self.send_combined(combined_data, combined_wave)
            else:
                self.get_logger().warn('SKIP! Waiting for both VNIR and NIR spectra...',
                                       throttle_duration_sec=5.0)
        except Exception:
            self.get_logger().error(f'Error combining spectra: {traceback.format_exc()}')


def main(args=None):
    rclpy.init(args=args)
    controller = SpectrumCombiner()
    try:
        rclpy.spin(controller)
    except KeyboardInterrupt:
        controller.get_logger().info('Shutting down spectral combiner')
    finally:
        controller.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
