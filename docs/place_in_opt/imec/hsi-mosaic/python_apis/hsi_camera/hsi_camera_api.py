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

import ctypes
from ctypes.util import find_library
from typing import Tuple

import hsi_common as HSI_COMMON

__api_dll = None

# ---------------------------------------------------------------------
def __Dev_LoadDll():
    """! @private """
    global __api_dll
    if __api_dll is not None: return
    dllname = r'hsi_camera_api'
    if len(sys.argv) > 1 and '--Debug' in sys.argv:
        dllname += '_d'
    dllname = find_library(dllname)
    __api_dll = ctypes.CDLL(dllname)
    assert __api_dll is not None

# ---------------------------------------------------------------------
import atexit

@atexit.register
def __Dev_UnloadDll():
    """! @private """
    global __api_dll
    if __api_dll is None: return
    del __api_dll
    __api_dll = None


__Dev_LoadDll() # load the dll now...


class Manufacturer(IntEnum):
    """! Enumeration class with Camera Manufacturers. """
    ## Select and include cameras from all manufacturers
    EM_ALL = 0
    ## Select and include only Ximea cameras
    EM_XIMEA = 1
    ## Select and include only Photon-Focus cameras
    EM_PHOTONFOCUS = 2
    ## Select and include only IMEC-made cameras
    EM_IMEC = 3
    ## Do not select any manufacturer
    EM_NONE = 4
    ## Special value only used for debugging!
    EM_OCV = 666

class Model(IntEnum):
    """! Enumeration class with Camera Manufacturers. """
    ## Select and include cameras from all models
    MO_ALL = 0
    ## Select and include only snapshot SWIR cameras (when manufacturer is EM_IMEC)
    MO_SNAPSHOT_SWIR = 1
    ## Select and include only dummy cameras (when manufacturer is EM_IMEC)
    MO_DUMMY = 2

class TriggerMode(IntEnum):
    """! Enumeration class with Camera Trigger Modes. """
    ## Triggering is disabled
    TM_NO_TRIGGERING = 0
    ## The camera captures frames on receiving a software trigger signal.
    TM_SOFTWARE_TRIGGERED = 1
    ## The camera captures frames on receiving a hardware trigger signal.
    TM_HARDWARE_TRIGGERED = 2
    TM_INVALID = 3

class ShutterState(IntEnum):
    """! Enumeration class with Camera Sutter States. """
    ## Close the internal camera shutter
    SS_CLOSE = 0
    ## Open the internal camera shutter
    SS_OPEN = 1


# ---------------------------------------------------------------------
_MAX_STRING_LENGTH = HSI_COMMON.GetMaxStringLength()


class CameraInfo (HSI_COMMON.BaseStruct):
    """! This class is the python version of the C-level CameraInfo class. \
         This class holds the camera info members.

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> manufacturer </td><td> c_int </td><td> enumerated value indicating the manufacturer of the camera (see Manufacturer) </td></tr>
    <tr><td> model </td><td> string </td><td> text string indicating the model of the Camera </td></tr>
    <tr><td> serial_number </td><td> string </td><td> text string indicating the serial number of the Camera </td></tr>
    <tr><td> identification_string </td><td> string </td><td> unique identification string of the Camera </td></tr>
    </table>
    """
    _fields_ = [
        ('manufacturer', ctypes.c_int),
        ('model', ctypes.c_char * _MAX_STRING_LENGTH),
        ('serial_number', ctypes.c_char * _MAX_STRING_LENGTH),
        ('identification_string', ctypes.c_char * _MAX_STRING_LENGTH)]


class ConfigurationParameters(HSI_COMMON.BaseStruct):
    """! This class is the python version of the C-level ConfigurationParameters class. \
         This class holds the camera configuration parameters.

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> bit_depth </td><td> c_uint </td><td> value indicating how many bits are used per pixel </td></tr>
    <tr><td> nr_me_frames </td><td> c_int </td><td> value how many multi-exposure frames are used (1 means NO multi-exposure) </td></tr>
    <tr><td> multi_exposure_scaling </td><td> c_bool </td><td> flag to indicate if multi-exposure scaling needs to be applied </td></tr>
    </table>
    """
    _fields_ = [
        ('bit_depth', ctypes.c_uint),
        ('nr_me_frames', ctypes.c_int),
        ('multi_exposure_scaling', ctypes.c_bool)
        ]


class RuntimeParameters(HSI_COMMON.BaseStruct):
    """! This class is the python version of the C-level RuntimeParameters class. \
         This class holds the camera runtime parameters.

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> acquisition_timeout_ms </td><td> c_int </td><td> the internal acquisition timeout value in milliseconds </td></tr>
    <tr><td> black_level_offset </td><td> c_int </td><td> black level offset value </td></tr>
    <tr><td> exposure_time_ms </td><td> c_double </td><td> the exposure time to be used in milliseconds </td></tr>
    <tr><td> flip_horizontal </td><td> c_bool </td><td> flag to indicate if horizontal frame flipping is enabled </td></tr>
    <tr><td> flip_vertical </td><td> c_bool </td><td> flag to indicate if vertical frame flipping is enabled </td></tr>
    <tr><td> frame_rate_hz </td><td> c_double </td><td> the frame rate during the acquisition (in Hertz) </td></tr>
    <tr><td> requested_temperature_dc </td><td> c_double </td><td> the requested temperature indicated in degrees Celcius </td></tr>
    <tr><td> tec_enabled </td><td> c_bool </td><td> flag indicating that temperature control (TEC) was enabled </td></tr>
    <tr><td> trigger_mode </td><td> c_uint </td><td> enumerator value to indicate the trigger configuration of the camera (see TriggerMode) </td></tr>
    <tr><td> hdr_scaling </td><td> c_bool </td><td> flag indicating if HDR scaling was applied </td></tr>
    </table>
    """
    _fields_ = [
        ('black_level_offset', ctypes.c_int ),          ## [r] The black level offset in DN
        ('exposure_time_ms', ctypes.c_double ),         ## [r] The image exposure time in milliseconds.
        ('flip_horizontal', ctypes.c_bool ),            ## [r] Aka. up-down
        ('flip_vertical', ctypes.c_bool ),              ## [r] Aka. left-right
        ('frame_rate_hz', ctypes.c_double ),            ## [r] The frame rate at which images are created by the camera or 0 for maximum possible framerate. The maximum frame rate is limited by the exposure time. \see exposure_time_ms
        ('requested_temperature_dc', ctypes.c_double ), ## [r] requested temperature to TEC
        ('tec_enabled', ctypes.c_bool ),                ## [r] TEC
        ('trigger_mode', ctypes.c_uint ),               ## [r] The trigger mode of the camera.
        ('hdr_scaling', ctypes.c_bool )]                ## [r] Scale the HDR image according to the integration time ratios


class HDRParameters(HSI_COMMON.BaseStruct):
    """! This class is the python version of the C-level HDRParameters class. \
         This class holds the camera High Dynamic Range (HDR) parameters.

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> width </td><td> c_int </td><td> width of the frames used for HDR </td></tr>
    <tr><td> height </td><td> c_int </td><td> height of the frames used for HDR </td></tr>
    <tr><td> num_entries </td><td> c_int </td><td> number of frames used for HDR </td></tr>
    <tr><td> integration_time_ratios </td><td> List[c_double] </td><td> list of integration time ratios used for HDR </td></tr>
    </table>
    """
    _fields_ = [
        ('width', ctypes.c_int),
        ('height', ctypes.c_int),
        ('num_entries', ctypes.c_int),
        ('integration_time_ratios', ctypes.POINTER(ctypes.c_double)),
        ('p_data', ctypes.POINTER(ctypes.c_int)),
        ('pp_rows', ctypes.POINTER(ctypes.POINTER(ctypes.c_int)))]

class DeviceArgs (HSI_COMMON.BaseStruct):
    """! This class is the python version of the C-level DeviceArgs class. \
         This class holds the device argument members.

    The members of this wrapper class are:
    <table>
    <tr><th> member name </th><th> type </th><th> meaning </th></tr>
    <tr><td> manufacturer </td><td> c_int </td><td> enumerated value indicating the manufacturer of the camera (see Manufacturer) </td></tr>
    <tr><td> model </td><td> string </td><td> enumerated value indicating the model of the camera (see Model) </td></tr>
    </table>
    """
    _fields_ = [
        ('manufacturer', ctypes.c_int),
        ('model', ctypes.c_int)]


# ---------------------------------------------------------------------
## type aliasing
Handle = ctypes.c_void_p

''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN EnumerateConnectedDevices (CameraInfo* , int* , int , Manufacturer );
'''
__api_dll.cameraEnumerateConnectedDevices.argtypes = [ctypes.POINTER(CameraInfo * 10), ctypes.POINTER(ctypes.c_int), ctypes.c_int, DeviceArgs]
__api_dll.cameraEnumerateConnectedDevices.restype = ctypes.c_int

def EnumerateConnectedDevices(max_num_devices:int=10, manufacturer:int=Manufacturer.EM_ALL, model:int=Model.MO_ALL) -> list:
    """! Enumerates all connected HSI Camera devices and returns them as a list.
    @param max_num_devices [in] indicating the maximum amount of cameras to return in the list.
    @param manufacturer    [in] function argument to filter available cameras by manufacturer (see Manufacturer ).
    @param model           [in] function argument to filter available models (see Model ).
    @return List of all found HSI Cameras, filtered by manufacturer, with the indicated max length.
    """
    device_list = (CameraInfo * max_num_devices)()
    nr_devices = ctypes.c_int(0)
    device_args = DeviceArgs()
    device_args.manufacturer = manufacturer
    device_args.model = model
    result = __api_dll.cameraEnumerateConnectedDevices(ctypes.byref(device_list),
                                                       ctypes.byref(nr_devices),
                                                       max_num_devices, device_args)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    if nr_devices.value > 0:
        for dev in device_list[:nr_devices.value]: logging.info (dev)
        return device_list[:nr_devices.value] # slice array, only contain valid cameras
    return []

''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN OpenDevice (HANDLE* , CameraInfo const& );
'''
__api_dll.cameraOpenDevice.restype = ctypes.c_int
__api_dll.cameraOpenDevice.argtypes = [ctypes.POINTER(Handle), CameraInfo]

def OpenDevice(dev_info:CameraInfo) -> Handle:
    """! Opens a connection to the device as indicated by the device-info argument.
    @param dev_info [in] camera device information to open the connection to (see CameraInfo ).
    @return handle to the device
    """
    handle = Handle()
    result = __api_dll.cameraOpenDevice(ctypes.byref(handle), dev_info)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    logging.info(f'=== Connected to Device: {dev_info}')
    logging.info(f'=== Device-Handle {handle}')
    return handle

''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN CloseDevice (HANDLE);
'''
__api_dll.cameraCloseDevice.restype = ctypes.c_int
__api_dll.cameraCloseDevice.argtypes = [ctypes.POINTER(Handle),]

def CloseDevice (handle:Handle):
    """! Closes a connection to the device indicated by the handle.
    @param handle [in] handle to the connected camera device.
    """
    result = __api_dll.cameraCloseDevice (ctypes.byref(handle))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN Initialize (HANDLE);
'''
__api_dll.cameraInitialize.restype = ctypes.c_int
__api_dll.cameraInitialize.argtypes = [Handle,]

def Initialize (handle:Handle):
    """! Initializes the camera indicated by the handle.
    @param handle [in] handle to the connected camera device.
    """
    result = __api_dll.cameraInitialize (handle)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN Start (HANDLE);
'''
__api_dll.cameraStart.restype = ctypes.c_int
__api_dll.cameraStart.argtypes = [Handle,]

def Start (handle:Handle):
    """! Start the camera indicated by the handle.
    @param handle [in] handle to the connected camera device.
    """
    result = __api_dll.cameraStart (handle)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN Pause (HANDLE);
'''
__api_dll.cameraPause.restype = ctypes.c_int
__api_dll.cameraPause.argtypes = [Handle,]

def Pause (handle:Handle):
    """! Pause the camera indicated by the handle.
    @param handle [in] handle to the connected camera device.
    """
    result = __api_dll.cameraPause (handle)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN Stop (HANDLE);
'''
__api_dll.cameraStop.restype = ctypes.c_int
__api_dll.cameraStop.argtypes = [Handle,]

def Stop (handle:Handle):
    """! Stop the camera indicated by the handle.
    @param handle [in] handle to the connected camera device.
    """
    result = __api_dll.cameraStop (handle)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN GetRuntimeParameters (HANDLE, RuntimeParameters*);
'''
__api_dll.cameraGetRuntimeParameters.restype = ctypes.c_int
__api_dll.cameraGetRuntimeParameters.argtypes = [Handle, ctypes.POINTER(RuntimeParameters)]

def GetRuntimeParameters (handle:Handle) -> RuntimeParameters:
    """! Get the runtime parameters for the camera indicated by the handle.
         @param handle [in] handle to the connected camera device.
         @return runtime-parameters (see RuntimeParameters)
    """
    params = RuntimeParameters()
    result = __api_dll.cameraGetRuntimeParameters (handle, ctypes.byref(params))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return params


''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN SetRuntimeParameters (HANDLE, RuntimeParameters);
'''
__api_dll.cameraSetRuntimeParameters.restype = ctypes.c_int
__api_dll.cameraSetRuntimeParameters.argtypes = [Handle, RuntimeParameters]

def SetRuntimeParameters (handle:Handle, params:RuntimeParameters):
    """! Set the argument runtime parameters for the camera indicated by the handle.
    @param handle [in] handle to the connected camera device.
    @param params [in] new set of RuntimeParameters to apply on the camera.
    """
    result = __api_dll.cameraSetRuntimeParameters (handle, params)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN GetConfigurationParameters (HANDLE, ConfigurationParameters*);
'''
__api_dll.cameraGetConfigurationParameters.restype = ctypes.c_int
__api_dll.cameraGetConfigurationParameters.argtypes = [Handle, ctypes.POINTER(ConfigurationParameters)]

def GetConfigurationParameters (handle:Handle) -> ConfigurationParameters:
    """! Get the configuration parameters for the camera indicated by the handle.
    @param handle [in] handle to the connected camera device.
    @return configuration-parameters (see ConfigurationParameters)
    """
    params = ConfigurationParameters()
    result = __api_dll.cameraGetConfigurationParameters (handle, ctypes.byref(params))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return params


''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN SetConfigurationParameters (HANDLE, ConfigurationParameters);
'''
__api_dll.cameraSetConfigurationParameters.restype = ctypes.c_int
__api_dll.cameraSetConfigurationParameters.argtypes = [Handle, ConfigurationParameters]

def SetConfigurationParameters (handle:Handle, params:ConfigurationParameters):
    """! Set the argument Configuration parameters for the camera indicated by the handle.
    @param handle [in] handle to the connected camera device.
    @param params [in] new set of ConfigurationParameters to apply on the camera.
    """
    result = __api_dll.cameraSetConfigurationParameters (handle, params)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN GetOutputFrameDataFormat  (HANDLE, FrameDataFormat* );
'''
__api_dll.cameraGetOutputFrameDataFormat.restype = ctypes.c_int
__api_dll.cameraGetOutputFrameDataFormat.argtypes = [Handle, ctypes.POINTER(HSI_COMMON.FrameDataFormat)]

def GetOutputFrameDataFormat (handle:Handle) -> HSI_COMMON.FrameDataFormat:
    """! Get the output frame-data-format for the camera indicated by the handle.
    @param handle [in] handle to the connected camera device.
    @return data format of the output of this camera (see FrameDataFormat)
    """
    dataformat = HSI_COMMON.FrameDataFormat()
    result = __api_dll.cameraGetOutputFrameDataFormat  (handle, ctypes.byref(dataformat))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return dataformat


''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN AllocateFrameForCamera (HANDLE, FrameFloat* );
'''
__api_dll.cameraAllocateFrameForCamera.restype = ctypes.c_int
__api_dll.cameraAllocateFrameForCamera.argtypes = [Handle, ctypes.POINTER(HSI_COMMON.FrameFloat)]

def AllocateFrameForCamera (handle:Handle) -> HSI_COMMON.FrameFloat:
    """! Allocate a FrameFloat for the camera indicated by the handle, this call will
         internally get the output frame dataformat and allocate a FrameFloat according
         to that format.
    @param handle [in] handle to the connected camera device.
    @return FrameFloat object with internally allocated buffers
    """
    frame = HSI_COMMON.FrameFloat()
    result = __api_dll.cameraAllocateFrameForCamera (handle, ctypes.byref(frame))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return frame


''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN AcquireFrame (HANDLE, FrameFloat*, int);
'''
__api_dll.cameraAcquireFrame.restype = ctypes.c_int
__api_dll.cameraAcquireFrame.argtypes = [Handle, ctypes.POINTER(HSI_COMMON.FrameFloat), ctypes.c_int]

def AcquireFrame (handle:Handle, frame:HSI_COMMON.FrameFloat=None, timeout_ms:int=0) -> Tuple[int, HSI_COMMON.FrameFloat]:
    """! Acquire a frame from the camera indicated by the handle. Optionally an existing FrameFloat
         argument can be provided. Also a timeout value in milliseconds can be provided, if a frame
         is not available after this timeout, a timeout status value will be returned.
    @param handle     [in] handle to the connected camera device
    @param frame      [in] [OPTIONAL] exsting FrameFloat object to be re-used.  If not provided a new FrameFloat will be allocated.
    @param timeout_ms [in] [OPTIONAL] if positive and non-zero, the maximum amount of time (in milliseconds) to wait for an incoming frame.
    @return tuple(result, frame) where result is the return status of the call (see ReturnValue ), frame is the resulting FrameFloat object
    """
    if frame is None:
        frame = AllocateFrameForCamera ( handle )
    result = __api_dll.cameraAcquireFrame (handle, ctypes.byref(frame), timeout_ms)
    assert result in (HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ReturnValue.HSI_ACQUISITION_TIMEOUT), HSI_COMMON.ToString (result)
    return result, frame

''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN SetRegionOfInterestArray (HANDLE , RegionOfInterest* i_p_roi_array, int i_nr_rois);
'''
__api_dll.cameraSetRegionOfInterestArray.argtypes = [Handle, ctypes.POINTER(HSI_COMMON.RegionOfInterest), ctypes.c_int]
__api_dll.cameraSetRegionOfInterestArray.restype = ctypes.c_int

def SetRegionOfInterestArray(handle:Handle, roi:HSI_COMMON.RegionOfInterest):
    """! Apply the argument Region of Interest onto the camera indicated by the handle.
    @param handle [in] handle to the connected camera device
    @param roi    [in] Region of Interest to apply (see RegionOfInterest)
    """
    roi_list = (HSI_COMMON.RegionOfInterest * len(roi))(*roi)
    result = __api_dll.cameraSetRegionOfInterestArray(handle, roi_list, len(roi))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)

''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN AllocateHDRParameters (int width, int height, int num_entries, HDRParameters* o_p_hdr_parameters);
'''
__api_dll.cameraAllocateHDRParameters.restype = ctypes.c_int
__api_dll.cameraAllocateHDRParameters.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.POINTER(HDRParameters)]

def AllocateHDRParameters(width:int, height:int, num_entries:int) -> HDRParameters:
    """! Allocate a set of High Hynamic Range (HDR) Camera parameters according to the arguments
    @param width  [in] width of the sensor to be used for HDR
    @param height [in] height of the sensor to be used for HDR
    @param num_entries [in] number of HDR-entries (frames) to be used for HDR
    @return set of HDR parameters (see HDRParameters)
    """
    hdr_params = HDRParameters()
    result = __api_dll.cameraAllocateHDRParameters (width, height, num_entries, ctypes.byref(hdr_params))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return hdr_params

''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN DeallocateHDRParameters (HDRParameters* io_p_hdr_parameters);
'''
__api_dll.cameraDeallocateHDRParameters.restype = ctypes.c_int
__api_dll.cameraDeallocateHDRParameters.argtypes = [ctypes.POINTER(HDRParameters),]

def DeallocateHDRParameters(hdr_parameters:HDRParameters):
    """! Deallocate the argument HDRParameters object, and its internal buffers.
    @param hdr_parameters [in] HDR parameters object that needs to be deallocated.
    """
    result = __api_dll.cameraDeallocateHDRParameters (ctypes.byref(hdr_parameters))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)

''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN SetHdrSettings (HANDLE, HDRParameters*);
'''
__api_dll.cameraSetHdrSettings.argtypes = [Handle, ctypes.POINTER(HDRParameters)]
__api_dll.cameraSetHdrSettings.restype = ctypes.c_int

def SetHdrSettings(handle:Handle, hdr_parameters:HDRParameters):
    """! Apply the argument HDR settings on the camera indicated by the handle.
    @param handle     [in] handle to the connected camera device.
    @param hdr_parameters [in] HDR parameters object that needs to be deallocated.
    """
    result = __api_dll.cameraSetHdrSettings(handle, ctypes.byref(hdr_parameters))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)

''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN Trigger (HANDLE);
'''
__api_dll.cameraTrigger.argtypes = [Handle,]
__api_dll.cameraTrigger.restype = ctypes.c_int

def Trigger(handle:Handle):
    """! Trigger the camera indicated by the handle.
    @param handle [in] handle to the connected camera device.
    """
    result = __api_dll.cameraTrigger(handle)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)

''' #---------------------------------------------------------------------------------
HSI_CAMERA_API HSI_RETURN SetShutterState (HANDLE i_device_handle, bool i_shutter_state);
'''
__api_dll.cameraSetShutterState.argtypes = [Handle, ctypes.c_bool]
__api_dll.cameraSetShutterState.restype = ctypes.c_int

def SetShutterState(handle:Handle, shutter_state:int):
    """! Set/Change the state of the internal shutter of the camera.
    @param handle [in] handle to the connected camera device.
    @param shutter_state [in] new state of the shutter (see ShutterState)
    """
    result = __api_dll.cameraSetShutterState(handle, shutter_state)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)

#---------------------------------------------------------------------------------
__api_dll.cameraGetAPIVersion.restype = ctypes.c_int
__api_dll.cameraGetAPIVersion.argtypes = [ctypes.POINTER(ctypes.c_int),
                                    ctypes.POINTER(ctypes.c_int),
                                    ctypes.POINTER(ctypes.c_int),
                                    ctypes.POINTER(ctypes.c_int)]

def GetAPIVersion() -> Tuple[int,int,int,int]:
    """! Get the version information on this Camera API,
    The returned tuple contains 4 values: [major,minor,patch,build]
    to exactly and uniquely mark the version you are using.
    """
    M,m,p,b = ctypes.c_int(0),ctypes.c_int(0),ctypes.c_int(0),ctypes.c_int(0)
    result = __api_dll.cameraGetAPIVersion(ctypes.byref(M),ctypes.byref(m),
                                  ctypes.byref(p),ctypes.byref(b))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return (M.value, m.value, p.value, b.value)

