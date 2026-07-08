'''//////////////////////////
//
// Date: 16 November, 2020
//
// Authors:
//    Geert Vanmeerbeeck
//
//  Copyright 2020 imec. All rights reserved.
//
/////////////////'''

import ctypes
import numpy as np

from enum import IntEnum


# ---------------------------------------------------------------------
# enum Return Value
class ReturnValue(IntEnum):
    """! Enumeration class with API function return values
    """
    ## Function call successful.
    HSI_OK = 0
    ## Invalid device handle specified.
    HSI_HANDLE_INVALID = 1
    ## Invalid argument provided in function call.
    HSI_ARGUMENT_INVALID = 2
    ## Function call illegal given the current internal state.
    HSI_CALL_ILLEGAL = 3
    ## A file could not be found.
    HSI_FILE_NOT_FOUND = 10
    ## Sensor calibration file could not be found.
    HSI_CALIBRATION_FILE_NOT_FOUND = 11
    ## Camera could not be connected to the system.
    HSI_CONNECTION_FAILED = 12
    ## Allocation of resources failed.
    HSI_ALLOCATION_ERROR = 20
    ## An Acquisition timeout has occured.
    HSI_ACQUISITION_TIMEOUT = 21
    ## Acquisition failed during operation.
    HSI_ACQUISITION_FAILED = 30
    ## Provided data structure is not allocated.
    HSI_DATA_NOT_ALLOCATED = 32
    ## Data with valid flag false provided as input for operation.
    HSI_DATA_NOT_VALID = 33
    ## Data provided is not compatible.
    HSI_DATA_NOT_COMPATIBLE = 34
    ## Specified directory doesn't exist and could not be created.
    HSI_FILE_SYSTEM_ERROR = 40
    ## Could not read or write data from the filesystem.
    HSI_FILE_IO_ERROR = 41
    ## An undexpected internal error occurred.
    HSI_INTERNAL_ERROR = 100


class LoggerVerbosity(IntEnum):
    """! Enumeration class with Logger Verbosity levels.
    """
    ## Logs on error only
    LV_ERROR = 0
    ## Logs on error and warnings
    LV_WARNING = 1
    ## Logs on info and higher
    LV_INFO = 2
    ## Logs on debug and higher
    LV_DEBUG = 3
    ## Logs on verbose and higher
    LV_VERBOSE = 4
    ## No logging.
    LV_NONE = 5


# enum File Formats for Writing
class FileFormat(IntEnum):
    """! Enumeration class with supported output file formats.
    """
    ## Pixel Grayscale Map (PGM) file format
    FF_PGM = 0
    ## 8-bit TIFF file format
    FF_TIFF = 1
    ## 8-bit Portable Network Graphics (PNG) file format
    FF_PNG = 2
    ## 8-bit Bitmap (BMP) format
    FF_BMP = 3
    ## 8-bit JPEG file format
    FF_JPEG = 4
    ## Multi-band ENVI file format
    FF_ENVI = 10
    ## RAW file format
    FF_RAW = 20


# ---------------------------------------------------------------------

class BaseStruct (ctypes.Structure):
    """! This class serves as a base class for all C-struct types
    """
    def __init__(self, **kwargs:dict):
        """! Constructor for initialization with values.
        @param kwargs: dictionary with (member:value) pairs
        """
        super().__init__(**kwargs)
    def as_dict(self):
        """! convert struct to dictionary """
        result = dict([(field[0], getattr(self, field[0])) for field in self._fields_])
        for key,val in result.items():
            if isinstance(val, BaseStruct):
                result[key] = val.as_dict()
            if key == 'bands_nm':
                result[key] = np.ctypeslib.as_array(result[key],
                                                    shape=(int(result['nr_bands']),))
        return result
    def __str__(self):
        """! convert to string, via dictionary """
        return str(self.as_dict())
    def __getitem__(self, key):
        """! Python dictionary Get(key) interface """
        return self.as_dict()[key]
    def __setitem__(self, key, value):
        """! Python dictionary Set(key, value) interface """
        setattr(self, key, value)


class FrameDataFormat(BaseStruct):
    """! This class is the python version of the C-level FrameDataFormat class.
         This class holds the Data Format for a FrameFloat object.

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> width </td><td> c_int </td><td> width of the frame (number of columns) </td></tr>
    <tr><td> height </td><td> c_int </td><td> height of the frame (number of rows) </td></tr>
    <tr><td> size_bytes </td><td> c_longlong </td><td> total size (in bytes) for a single frame </td></tr>
    </table>
    """
    _fields_ = [
        ('width', ctypes.c_int),
        ('height', ctypes.c_int),
        ('size_bytes', ctypes.c_longlong),
        ('p_reserved', ctypes.c_void_p)]


class FrameDataInfo(BaseStruct):
    """! This class is the python version of the C-level FrameDataInfo class.
         This class holds the Data Info for a FrameFloat object.

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> analog_gain </td><td> c_double </td><td> analog gain setting of the sensor </td></tr>
    <tr><td> integration_time_ms </td><td> c_double </td><td> integration time setting of the sensor </td></tr>
    <tr><td> acquisition_time_us </td><td> c_unit64 </td><td> acquisition time (relative to EPOCH) </td></tr>
    <tr><td> value_max </td><td> c_double </td><td> maximum possible value </td></tr>
    <tr><td> value_saturation </td><td> c_double </td><td> value threshold for saturated pixels </td></tr>
    <tr><td> value_no_data </td><td> c_double </td><td> value indicating a no data condition </td></tr>
    <tr><td> value_min </td><td> c_double </td><td> minimum possible value </td></tr>
    <tr><td> system_id </td><td> string </td><td> System-ID string of the camera </td></tr>
    <tr><td> valid </td><td> c_bool </td><td> frame is valid flag </td></tr>
    </table>
    """
    _fields_ = [
        ('analog_gain', ctypes.c_double),
        ('integration_time_ms', ctypes.c_double),
        ('acquisition_time_us', ctypes.c_uint64),
        ('value_max', ctypes.c_double),
        ('value_saturation', ctypes.c_double),
        ('value_no_data', ctypes.c_double),
        ('value_min', ctypes.c_double),
        ('system_id', ctypes.c_char * 64),
        ('valid', ctypes.c_bool)]


class FrameFloat(BaseStruct):
    """! This class is the python version of the C-level FrameFloat class.
         This class holds all the Data structures of a FrameFloat object, including its
         format (see FrameDataFormat), its info (see FrameDataInfo) and its pixel values (c_float).

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> format </td><td> FrameDataFormat </td><td> full data format structure of the frame </td></tr>
    <tr><td> info </td><td> FrameDataInfo </td><td> full data info structure of the frame </td></tr>
    <tr><td> pp_data </td><td> **c_float </td><td> double pointer to the data values in this frame </td></tr>
    </table>
    """
    _fields_ = [
        ('format', FrameDataFormat),
        ('info', FrameDataInfo),
        ('pp_data', ctypes.POINTER(ctypes.POINTER(ctypes.c_float))),
        ('p_reserved', ctypes.c_void_p)]


class CubeDataFormat(BaseStruct):
    """! This class is the python version of the C-level CubeDataFormat class.
         This class holds the Data Format for a CubeFloat object.

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> width </td><td> c_int </td><td> width of the cube (number of columns) </td></tr>
    <tr><td> height </td><td> c_int </td><td> height of the cube (number of rows) </td></tr>
    <tr><td> nr_bands </td><td> c_int </td><td> number of spectral bands in the cube </td></tr>
    <tr><td> cube_kind </td><td> c_int </td><td> Band-Sequential or Pixel-Interleaved cube </td></tr>
    <tr><td> bands_nm </td><td> list[] </td><td> list of wavelength values (in nm) per band </td></tr>
    <tr><td> size_bytes </td><td> c_longlong </td><td> total size (in bytes) for a single frame </td></tr>
    </table>
    """
    _fields_ = [
        ('width', ctypes.c_int),
        ('height', ctypes.c_int),
        ('cube_kind', ctypes.c_int),
        ('nr_bands', ctypes.c_int),
        ('bands_nm', ctypes.POINTER(ctypes.c_double)),
        ('size_bytes', ctypes.c_longlong),
        ('p_reserved', ctypes.c_void_p)]

class CubeDataInfo(BaseStruct):
    """! This class is the python version of the C-level CubeDataInfo class.
         This class holds the Data Info for a CubeFloat object.

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> analog_gain </td><td> c_double </td><td> analog gain setting of the sensor </td></tr>
    <tr><td> integration_time_ms </td><td> c_double </td><td> integration time setting of the sensor </td></tr>
    <tr><td> acquisition_time_us </td><td> c_unit64 </td><td> acquisition time (relative to EPOCH) </td></tr>
    <tr><td> value_max </td><td> c_double </td><td> maximum possible value </td></tr>
    <tr><td> value_saturation </td><td> c_double </td><td> value threshold for saturated pixels </td></tr>
    <tr><td> value_no_data </td><td> c_double </td><td> value indicating a no data condition </td></tr>
    <tr><td> value_min </td><td> c_double </td><td> minimum possible value </td></tr>
    <tr><td> system_id </td><td> string </td><td> System-ID string of the camera </td></tr>
    <tr><td> valid </td><td> c_bool </td><td> frame is valid flag </td></tr>
    <tr><td> dark_field_corrected </td><td> c_bool </td><td> flag indicating this cube has been dark-field corrected </td></tr>
    <tr><td> vignetting_corrected </td><td> c_bool </td><td> flag indicating this cube has been vignetting corrected </td></tr>
    <tr><td> white_balanced </td><td> c_bool </td><td> flag indicating this cube has been balanced (with a reference spectrum) </td></tr>
    <tr><td> spectral_corrected </td><td> c_bool </td><td> flag indicating this cube has been spectrally corrected </td></tr>
    </table>
    """
    _fields_ = [
        ('analog_gain', ctypes.c_double),
        ('integration_time_ms', ctypes.c_double),
        ('acquisition_time_us', ctypes.c_longlong),
        ('value_max', ctypes.c_double),
        ('value_saturation', ctypes.c_double),
        ('value_no_data', ctypes.c_double),
        ('value_min', ctypes.c_double),
        ('system_id', ctypes.c_char * 64),
        ('valid', ctypes.c_bool),
        ('dark_field_corrected', ctypes.c_bool),
        ('vignetting_corrected', ctypes.c_bool),
        ('white_balanced', ctypes.c_bool),
        ('spectral_corrected', ctypes.c_bool)]


class CubeFloat(BaseStruct):
    """! This class is the python version of the C-level CubeFloat class.
         This class holds all the Data structures of a CubeFloat object, including its
         format (see CubeDataFormat), its info (see CubeDataInfo) and its cube values (c_float).

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> format </td><td> CubeDataFormat </td><td> full data format structure of the cube </td></tr>
    <tr><td> info </td><td> CubeDataInfo </td><td> full data info structure of the cube </td></tr>
    <tr><td> ppp_data </td><td> ***c_float </td><td> triple pointer to the data values in this cube </td></tr>
    </table>
    """
    _fields_ = [
        ('format', CubeDataFormat),
        ('info', CubeDataInfo),
        ('ppp_data', ctypes.POINTER(ctypes.POINTER(ctypes.POINTER(ctypes.c_float)))),
        ('p_reserved', ctypes.c_void_p)]


class RegionOfInterest(BaseStruct):
    """! This class is the python version of the C-level RegionOfInterest class.
        Its parameters are x (horizontal offset), y (vertial offset), width and height.

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> x </td><td> c_int </td><td> horizontal offset of this ROI in the frame/cube </td></tr>
    <tr><td> y </td><td> c_int </td><td> vertical offset of this ROI in the frame/cube </td></tr>
    <tr><td> width </td><td> c_longlong </td><td> width of this ROI (number of columns) </td></tr>
    <tr><td> height </td><td> c_longlong </td><td> height of this ROI (number of rows) </td></tr>
    </table>
    """
    _fields_ = [
        ('x', ctypes.c_int),
        ('y', ctypes.c_int),
        ('width', ctypes.c_int),
        ('height', ctypes.c_int)]


class SpectrumFloat(BaseStruct):
    """! This class is the python version of the C-level SpectrumFloat class.
         This class contains a data-info (see CubeDataInfo), the number of bands present in
         the spectrum (nr_bands), the list with wavelength values (in c_double), and the list
         of spectrum data values (in c_float)

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> info </td><td> CubeDataInfo </td><td> full data info structure of the originating cube </td></tr>
    <tr><td> nr_bands </td><td> c_int </td><td> number of spectral bands in this spectrum </td></tr>
    <tr><td> p_wavelength_nm </td><td> *c_double </td><td> list of c_double values indicating the wavelength values (in nm) per band </td></tr>
    <tr><td> p_data </td><td> *c_float </td><td> list of data values in this spectrum </td></tr>
    </table>
    """
    _fields_ = [
        ('info', CubeDataInfo),   # ///< Information on the specral data.
        ('nr_bands', ctypes.c_int), # ///< Nuber of bands.
        ('p_wavelength_nm', ctypes.POINTER(ctypes.c_double)),   # ///< Positions of the samples in wavelength (nm).
        ('p_data', ctypes.POINTER(ctypes.c_float))]  # ///< Data pointer to the sample data.


#---------------------------------------------------------------------------------
def ToString (ret_val:ReturnValue) -> str:
    """! function to translate API return values into meaningful messages.
    @param ret_val [in] return value to translate into a meaningful string.
    @return message (as string)
    """
    ErrorMap = {
          ReturnValue.HSI_OK                         : "Succeed."
        , ReturnValue.HSI_HANDLE_INVALID             : "Invalid device handle specified."
        , ReturnValue.HSI_ARGUMENT_INVALID           : "Invalid argument provided in function call."
        , ReturnValue.HSI_CALL_ILLEGAL               : "Function call illegal given the current snapscan state."
        , ReturnValue.HSI_FILE_NOT_FOUND             : "A file could not be found."
        , ReturnValue.HSI_CALIBRATION_FILE_NOT_FOUND : "Sensor calibration file could not be found."
        , ReturnValue.HSI_CONNECTION_FAILED          : "Snapscan system could not be connected."
        , ReturnValue.HSI_ALLOCATION_ERROR           : "Allocation of resources failed."
        , ReturnValue.HSI_ACQUISITION_TIMEOUT        : "An Acquisition timeout has occured."
        , ReturnValue.HSI_ACQUISITION_FAILED         : "Unable to fulfill acquisition."
        , ReturnValue.HSI_DATA_NOT_ALLOCATED         : "Provided data structure is not allocated."
        , ReturnValue.HSI_DATA_NOT_VALID             : "Data with valid flag false provided as input for operation."
        , ReturnValue.HSI_DATA_NOT_COMPATIBLE        : "Data provided is not compatible."
        , ReturnValue.HSI_FILE_SYSTEM_ERROR          : "Specified directory doesn't exist and could not be created."
        , ReturnValue.HSI_FILE_IO_ERROR              : "Could not read or write data from the filesystem."
        , ReturnValue.HSI_INTERNAL_ERROR             : "An unexpected internal error occurred."
    }
    if ret_val not in ErrorMap:
        raise ValueError("Unknown error return value: %s" % ret_val)

    return ErrorMap[ret_val]


#---------------------------------------------------------------------------------
def FrameAsArray(frame:FrameFloat):
    """! Method to convert a FrameFloat to a numpy array, while re-using the internally allocated data.
        So no new copy is made, only a numpy header around the existing data.
    @param frame [in] frame object (of type FrameFloat) to be returned as a numpy array
    @return numpy array representation of a frame object.
    """
    return np.ctypeslib.as_array(frame.pp_data[0],
                                 (frame.format.height,
                                  frame.format.width))


#---------------------------------------------------------------------------------
def CubeAsArray(cube:CubeFloat, BSQ=True):
    """! Method to convert a CubeFloat to a numpy array, while re-using the internally allocated data.
        So no new copy is made, only a numpy header around the existing data.
    @param cube [in] cube object (of type CubeFloat) to be returned as a numpy array
    @param BSQ [in] boolean flag to indicate a Band-Sequential Cube (BSQ=True) or a Band-Interleaved Cube (BSQ=False)
    @return numpy array representation of a cube object.
    """
    if BSQ:
        return np.ctypeslib.as_array(cube.ppp_data[0][0],
                                     (cube.format.nr_bands,
                                      cube.format.height,
                                      cube.format.width))
    # else it is BIP format
    return np.ctypeslib.as_array(cube.ppp_data[0][0],
                                    (cube.format.height,
                                     cube.format.width,
                                     cube.format.nr_bands))

def SpectrumAsDict(spectrum:SpectrumFloat) -> dict:
    """! Method to convert a SpectrumFloat to a numpy dictionary, while re-using the internally allocated data.
        So no new copy is made, only a numpy header around the existing data.
    @param spectrum [in] spectrum object
    @return dictionary with the contents of a spectrum: (wavelengths[] and values[])
    """
    return {
            'values' : np.ctypeslib.as_array(spectrum.p_data,
                                             (spectrum.nr_bands,)),
            'wavelengths' : np.ctypeslib.as_array(spectrum.p_wavelength_nm,
                                                  (spectrum.nr_bands,))
           }
