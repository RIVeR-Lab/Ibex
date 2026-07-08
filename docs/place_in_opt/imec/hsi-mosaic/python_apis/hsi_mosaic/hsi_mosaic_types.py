'''//////////////////////////
//
// Date: 16 November, 2020
//
// Authors:
//    Geert Vanmeerbeeck
//    Bart Masschelein
//
//  Copyright 2020 imec. All rights reserved.
//
/////////////////'''

import sys
import logging
from enum import IntEnum
from typing import List

import ctypes
from ctypes.util import find_library

import hsi_common as HSI_COMMON

# ---------------------------------------------------------------------
class PipeFlag(IntEnum):
    """! Enumeration class with Mosaic Pipeline Stage flags. """
    ## no Mosaic pipeline stages enabled
    NONE = 0
    ## Bias Correction (aka dark correction) stage enabled
    BIAS_CORRECTION = 1
    ## Cube Generation (aka de-mosaicing) stage enabled
    CUBE_GENERATION = 2
    ## Band-Alignment stage enabled
    ALIGNMENT = 4
    ## Spatial resampling stage enabled
    SPATIAL_RESAMPLING = 8
    ## Median Filtering (on the band images) enabled
    SPATIAL_MEDIAN_FILTERING = 16
    ## White balancing (aka reference spectrum balancing) enabled
    WHITE_BALANCING = 32
    ## Spectral Correction stage enabled
    SPECTRAL_CORRECTION = 64
    ## All Mosaic Stages enabled
    ALL = 127

class KernelSize(IntEnum):
    """! Enumeration class with Median Filter Kernel Sizes. """
    ## Median filtering with a 3x3 Kernel
    KS_3X3 = 0
    ## Median filtering with a 5x5 Kernel
    KS_5X5 = 1
    ## No Median filtering
    KS_NONE = 2

class OpticalRange(IntEnum):
    """! Enumeration class for Optical Range of the Sensor. """
    ## Optical Range = Visual spectrum only
    OR_VIS = 0
    ## Optical Range = Near-Infrared spectrum only
    OR_NIR = 1
    ## Optical Range = Visual and Near-Infrared spectrum
    OR_VISNIR = 2
    ## Optical Range = Short-wave Infrared spectrum
    OR_SWIR = 3

# ---------------------------------------------------------------------

class ConfigurationParameters(HSI_COMMON.BaseStruct):
    """! This class is the python version of the C-level ConfigurationParameters class. \
         This class holds the Mosaic pipeline configuration parameters.

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> spatial_resampling_width </td><td> c_int </td><td> cube width after (spatial) resampling </td></tr>
    <tr><td> spatial_resampling_height </td><td> c_int </td><td> cube height after (spatial) resampling </td></tr>
    <tr><td> spatial_median_filter_enable </td><td> c_int </td><td> flag indicating if Median filtering is applied </td></tr>
    <tr><td> spatial_median_filter_kernel_size </td><td> c_int </td><td> kernel size to be used for Median Filtering (see KernelSize) </td></tr>
    <tr><td> white_balance_enable </td><td> c_int </td><td> flag indicating that spectral balancing (against a reference spectrum) is applied </td></tr>
    </table>
    """
    _fields_ = [
        ('spatial_resampling_width', ctypes.c_size_t),
        ('spatial_resampling_height', ctypes.c_size_t),
        ('spatial_median_filter_enable', ctypes.c_bool),
        ('spatial_median_filter_kernel_size', ctypes.c_int),
        ('white_balance_enable', ctypes.c_bool)
    ]

class ContextStatus(HSI_COMMON.BaseStruct):
    """! This class is the python version of the C-level ContextStatus class. \
         This class contains the status flags for a certain context object.

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> context_ready </td><td> c_int </td><td> flag indicating if a context can be used </td></tr>
    <tr><td> has_dark_reference </td><td> c_int </td><td> flag indiating the context contains at least one dark reference </td></tr>
    <tr><td> has_sensor_calibration </td><td> c_int </td><td> flag indicating the context contains the sensor calibration file </td></tr>
    <tr><td> has_nonuniformity </td><td> c_int </td><td> flag indicating the context contains non-uniformity data </td></tr>
    <tr><td> has_optical_setup </td><td> c_int </td><td> flag indicating the context contains optical setup information </td></tr>
    </table>
    """
    _fields_ = [
        ('context_ready', ctypes.c_int),
        ('has_dark_reference', ctypes.c_int),
        ('has_sensor_calibration', ctypes.c_int),
        ('has_nonuniformity', ctypes.c_int),
        ('has_optical_setup', ctypes.c_int)
    ]

class OpticalSetup(HSI_COMMON.BaseStruct):
    """! This class is the python version of the C-level OpticalSetup class. \
         This class contains the optical setup information that can be added to a context.

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> lens_f_number </td><td> c_double </td><td> the F-Number of the lens used </td></tr>
    <tr><td> focal_length_mm </td><td> c_double </td><td> focal length of the lens in millimeters </td></tr>
    <tr><td> exit_pupil_mm </td><td> c_double </td><td> exit pupil distance of the setup in millimeters </td></tr>
    <tr><td> manufacturer </td><td> string </td><td> text string indicating the manufacturer of the lens </td></tr>
    <tr><td> optical_range </td><td> c_int </td><td> enumerated value for the optical range of the setup (see OpticalRange) </td></tr>
    </table>
    """
    _fields_ = [
        ('lens_f_number', ctypes.c_double),
        ('focal_length_mm', ctypes.c_double),
        ('exit_pupil_mm', ctypes.c_double),
        ('manufacturer', ctypes.c_char_p),
        ('optical_range', ctypes.c_int)
    ]

