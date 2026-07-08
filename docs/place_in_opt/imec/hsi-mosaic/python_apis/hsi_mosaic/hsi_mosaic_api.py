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

from .hsi_mosaic_types import *

__api_dll = None

# ---------------------------------------------------------------------
def __Dev_LoadDll():
    """! @private """
    global __api_dll
    if __api_dll is not None: return
    dllname = r'hsi_mosaic_api'
    if len(sys.argv) > 1 and '--Debug' in sys.argv:
        dllname += '_d'
    dllname = find_library(dllname)
    __api_dll = ctypes.CDLL(dllname)
    assert __api_dll is not None

__Dev_LoadDll() # load the dll now...

# ---------------------------------------------------------------------
import atexit

@atexit.register
def __Dev_UnloadDll():
    """! @private """
    global __api_dll
    if __api_dll is None: return
    del __api_dll
    __api_dll = None

# ---------------------------------------------------------------------

## API type alias
Handle = ctypes.c_void_p

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN GetAPIVersion (int* o_p_major, int* o_p_minor, int* o_p_patch, int* o_p_beta);
'''
__api_dll.mosaicGetAPIVersion.restype = ctypes.c_int
__api_dll.mosaicGetAPIVersion.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int)]

def GetAPIVersion():
    major, minor, patch, beta = ctypes.c_int(0), ctypes.c_int(0), ctypes.c_int(0), ctypes.c_int(0)
    res = __api_dll.mosaicGetAPIVersion(ctypes.byref(major),
                                  ctypes.byref(minor),
                                  ctypes.byref(patch),
                                  ctypes.byref(beta))
    return (major.value, minor.value, patch.value, beta.value)

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN Initialize (HANDLE);
'''
__api_dll.mosaicInitialize.restype = ctypes.c_int
__api_dll.mosaicInitialize.argtypes = [Handle,]

def Initialize (handle:Handle):
    """! Initialize the Mosaic data-processing pipeline indicated by the handle.
    This call will also allocate a set of internal buffers based on the provided
    input data format (derived from the context). After this call also the output
    data format is available (see GetOutputDataFormat()).  Calling Destroy() on
    this pipeline object will free all internally allocated buffers again.
    @param handle [in] handle to the Mosaic Data-processing Pipeline.
    """
    result = __api_dll.mosaicInitialize (handle)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN Start (HANDLE);
'''
__api_dll.mosaicStart.restype = ctypes.c_int
__api_dll.mosaicStart.argtypes = [Handle,]

def Start (handle:Handle):
    """! Start the Mosaic data-processing pipeline indicated by the handle.
    @param handle [in] handle to the Mosaic Data-processing Pipeline.
    """
    result = __api_dll.mosaicStart (handle)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN Pause (HANDLE);
'''
__api_dll.mosaicPause.restype = ctypes.c_int
__api_dll.mosaicPause.argtypes = [Handle,]

def Pause (handle:Handle):
    """! Pause the Mosaic data-processing pipeline indicated by the handle.
    @param handle [in] handle to the Mosaic Data-processing Pipeline.
    """
    result = __api_dll.mosaicPause (handle)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN Stop (HANDLE);
'''
__api_dll.mosaicStop.restype = ctypes.c_int
__api_dll.mosaicStop.argtypes = [Handle,]

def Stop (handle:Handle):
    """! Stop the Mosaic data-processing pipeline indicated by the handle.
    @param handle [in] handle to the Mosaic Data-processing Pipeline.
    """
    result = __api_dll.mosaicStop (handle)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN Create (HANDLE*, HANDLE );
'''
__api_dll.mosaicCreate.restype = ctypes.c_int
__api_dll.mosaicCreate.argtypes = [ctypes.POINTER(Handle), Handle]

def Create(context:Handle) -> Handle:
    """! Create a new Mosaic data-processing pipeline, based on the contents
    of the context object indicated by the argument.
    @param context [in] handle to a Mosaic Data-processing Context object.
    @returns handle to the newly created Mosaic data-processing Pipeline object.
    """
    handle = Handle()
    result = __api_dll.mosaicCreate(ctypes.byref(handle), context)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    logging.info(f'=== Created new PL-Handle {handle}')
    return handle

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN Destroy (HANDLE*);
'''
__api_dll.mosaicDestroy.restype = ctypes.c_int
__api_dll.mosaicDestroy.argtypes = [Handle,]

def Destroy (handle:Handle):
    """! Destroy the Mosaic data-processing pipeline indicated by the handle.
    This will internally clean up all allocated buffers.  This function must be
    called on a pipeline object handle to avoid memory leaks.
    @param handle [in] handle to the Mosaic Data-processing Pipeline.
    """
    result = __api_dll.mosaicDestroy (ctypes.byref(handle))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN GetOutputDataFormat (HANDLE, CubeDataFormat* );
'''
__api_dll.mosaicGetOutputDataFormat.argtypes = [Handle, ctypes.POINTER(HSI_COMMON.CubeDataFormat)]
__api_dll.mosaicGetOutputDataFormat.restype = ctypes.c_int

def GetOutputDataFormat(handle:Handle) -> HSI_COMMON.CubeDataFormat:
    """! Get the output data format of this Mosaic data processing pipeline
    object. This data format is available only after setting the input data
    format (see SetInputDataFormat()) and after calling Initialize().
    @param handle [in] handle to the Mosaic Data-processing Pipeline.
    @returns the output cube data format of this pipeline.
    """
    dataformat = HSI_COMMON.CubeDataFormat()
    result = __api_dll.mosaicGetOutputDataFormat (handle, ctypes.byref(dataformat))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return dataformat

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN PushFrame (HANDLE, FrameFloat* );
'''
__api_dll.mosaicPushFrame.argtypes = [Handle, HSI_COMMON.FrameFloat]
__api_dll.mosaicPushFrame.restypes = ctypes.c_int

def PushFrame(handle:Handle, frame:HSI_COMMON.FrameFloat):
    """! Method to push a valid frame object in the data processing pipeline to be converted
    into a hyperspectral cube. This frame needs to meet the input data format as set by
    SetInputDataFormat().
    @param handle [in] handle to the Mosaic Data-processing Pipeline.
    @param frame [in] frame to push as input of the data processing pipeline.
    """
    result = __api_dll.mosaicPushFrame(handle, frame)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN GetCube (HANDLE, HANDLE, int );
'''
__api_dll.mosaicGetCube.argtypes = [Handle, Handle, ctypes.c_int]
__api_dll.mosaicGetCube.restypes = ctypes.c_int

def GetCube(handle:Handle, cube:HSI_COMMON.CubeFloat, timeout_ms:int=5000):
    """! Method to wait and get an output hyperspectral cube from the data processing pipeline.
    This call will wait (in a blocking way) for the pipeline to produce a valid output cube. The
    method accepts a timeout value to limit the blocking wait time of this call.  If no valid output
    cube is produced within the indicated timeout window, the call will return with a TIMEOUT return
    value (see ReturnValue), and an invalid output cube.
    The cube argument needs to be a cube object with already pre-allocated memory. The output format
    of the data processing pipeline needs to be querried with GetOutputDataFormat().
    @param handle [in] handle to the Mosaic Data-processing Pipeline.
    @param cube [in] pre-allocated cube where the data processing pipeline can put the resulting cube-data.
    @param timeout_ms [in] [OPTIONAL] maximum wait time in milliseconds to wait for an output cube.
    @returns hyperspectral output cube (with pre-allocated memory)
    """
    result = __api_dll.mosaicGetCube(handle, ctypes.byref(cube), timeout_ms)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return cube

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN GetConfigurationParameters (HANDLE , ConfigurationParameters* );
'''
__api_dll.mosaicGetConfigurationParameters.argtypes = [Handle, ctypes.POINTER(ConfigurationParameters)]
__api_dll.mosaicGetConfigurationParameters.restypes = ctypes.c_int

def GetConfigurationParameters(handle:Handle):
    """! Method to get all the configuration parameters from the data processing pipeline
    @param handle [in] handle to the Mosaic Data-processing Pipeline.
    @returns ConfigurationParameters object with parameters.
    """
    params = ConfigurationParameters()
    result = __api_dll.mosaicGetConfigurationParameters(handle, ctypes.byref(params))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return params

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN SetConfigurationParameters (HANDLE , ConfigurationParameters );
'''
__api_dll.mosaicSetConfigurationParameters.argtypes = [Handle, ConfigurationParameters]
__api_dll.mosaicSetConfigurationParameters.restypes = ctypes.c_int

def SetConfigurationParameters(handle:Handle, params:ConfigurationParameters):
    """! Method to apply a (modified) set of configuration parameters to the data processing pipeline.
    @param handle [in] handle to the Mosaic Data-processing Pipeline.
    @param params [in] parameter object with the configuration parameters that need to be applied.
    """
    result = __api_dll.mosaicSetConfigurationParameters(handle, params)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN LoadContext (HANDLE*, char const* );
'''
__api_dll.mosaicLoadContext.argtypes = [Handle, ctypes.c_char_p]
__api_dll.mosaicLoadContext.restypes = ctypes.c_int

def LoadContext(pathname:str) -> Handle:
    """! Method to load an entire context (including all files and subfolders) into a
    context object.  This context object can be used to create and configure a new pipeline
    object.
    @param pathname [in] string indicating the (full) path to the location of the base context folder.
    @returns context object
    """
    context = Handle()
    result = __api_dll.mosaicLoadContext(ctypes.byref(context), pathname.encode('utf-8'))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return context

''' #---------------------------------------------------------------------------------
HSI_RETURN SaveContext (HANDLE i_context_handle, char const* i_pathname)
'''
__api_dll.mosaicSaveContext.argtypes = [Handle, ctypes.c_char_p]
__api_dll.mosaicSaveContext.restypes = ctypes.c_int

def SaveContext(context:Handle, pathname:str):
    """! Method to save an entire context object, including calibration file, dark-refs,
    white-refs, ... to disk in a structured way.  The resulting structured folder can be
    read back by the LoadContext() method.
    @param context [in] handle to the (internal) context object
    @param pathname [in] string indicating the (full) path to the output context folder.
    """
    result = __api_dll.mosaicSaveContext(context, pathname.encode('utf-8'))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN AllocateContext (HANDLE*, FrameFloat);
'''
__api_dll.mosaicAllocateContext.argtypes = [Handle,HSI_COMMON.FrameFloat]
__api_dll.mosaicAllocateContext.restypes = ctypes.c_int

def AllocateContext(frame:HSI_COMMON.FrameFloat) -> Handle:
    """! Method to allocate an internal context object. A context object can be
    modified and/or saved for later re-use.  Make sure to also de-allocate a
    context object to avoid memory leaks. (see DeallocateContext() )
    @param frame [in] frame to declare the pipeline meta-data information
    @returns a handle to the newly allocated context object.
    """
    type(frame)
    context = Handle()
    result = __api_dll.mosaicAllocateContext(ctypes.byref(context), frame)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return context

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN DeallocateContext (HANDLE* );
'''
__api_dll.mosaicDeallocateContext.argtypes = [Handle,]
__api_dll.mosaicDeallocateContext.restypes = ctypes.c_int

def DeallocateContext(context:Handle) -> Handle:
    """! Method to de-allocate all internal buffers associated with a context object.
    Context objects need to be allocated (see AllocateContext()) or created from a
    stored context on disk (see LoadContext()). In both cases, a context needs to
    be de-allocated with this call.
    @returns handle to the deallocated context
    """
    result = __api_dll.mosaicDeallocateContext(ctypes.byref(context))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return context

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN ContextReset (HANDLE i_p_context);
'''
__api_dll.mosaicContextReset.argtypes = [Handle,]
__api_dll.mosaicContextReset.restypes = ctypes.c_int

def ContextReset(context:Handle):
    """! Method to reset a context object.  After this reset operation, the context
    object will no longer contain any data!
    @param context [in] handle to a context object that needs to be reset
    """
    result = __api_dll.mosaicContextReset(context)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN ContextGetStatus (HANDLE i_p_context, ContextStatus* o_p_status_flags);
'''
__api_dll.mosaicContextGetStatus.ContextGetStatus = [Handle, ctypes.POINTER(ContextStatus)]
__api_dll.mosaicContextGetStatus.ContextGetStatus = ctypes.c_int

def ContextGetStatus(context:Handle) -> ContextStatus:
    """! Method to get the internal status of a context object.  The internal status
    is a set of flags to indicate which stages can be configured using this current
    context object.
    @param context [in] handle to a context object.
    @returns context status object.
    """
    context_status = ContextStatus()
    result = __api_dll.mosaicContextGetStatus(context, ctypes.byref(context_status))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return context_status

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN ContextIntegrationTimeInDark (HANDLE i_p_context, int* o_p_status_flag, float i_integration_time_ms);
'''
__api_dll.mosaicContextIntegrationTimeInDark.argtypes = [Handle, ctypes.POINTER(ctypes.c_int), ctypes.c_float]
__api_dll.mosaicContextIntegrationTimeInDark.restypes = ctypes.c_int

def ContextIntegrationTimeInDark(context:Handle, integration_time_ms:int):
    """! Method to check if for a certain argument integration time there is a dark reference
    present in the intenals of a context object.  A context object should always contain
    a dark reference image with the _same_ integration time when converting a RAW frame into
    a Hyperspectral cube.
    @param context [in] handle to a context object.
    @param integration_time_ms [in] the value of the integration time (in milliseconds) to look for.
    @returns boolean flag indicating if the argument integration time is present in the context (or not)
    """
    status_flag = ctypes.c_int(-1)
    result = __api_dll.mosaicContextIntegrationTimeInDark(context, ctypes.byref(status_flag), ctypes.c_float(integration_time_ms))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return status_flag.value

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN ContextSetCalibrationFile (HANDLE i_p_context, char const* i_in_file_path);
'''
__api_dll.mosaicContextSetCalibrationFile.argtypes = [Handle, ctypes.c_char_p]
__api_dll.mosaicContextSetCalibrationFile.restypes = ctypes.c_int

def ContextSetCalibrationFile(context:Handle, filename:str):
    """! Method to Set the calibration file for a specific context object.
    A context object must contain one and only one calibration file.
    @param context [in] handle to a context object.
    @param filename [in] string with a (full) path to the calibration file that needs to be added to the argument context object.
    """
    result = __api_dll.mosaicContextSetCalibrationFile(context, filename.encode('utf-8'))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_RETURN ContextSetDarkFieldReferences (HANDLE i_context_handle, FrameFloat* i_p_dark_field_array, int i_array_length);
'''
__api_dll.mosaicContextSetDarkFieldReferences.argtypes = [Handle, ctypes.POINTER(HSI_COMMON.FrameFloat), ctypes.c_int]
__api_dll.mosaicContextSetDarkFieldReferences.restypes = ctypes.c_int

def ContextSetDarkFieldReferences(context:Handle, darkref_list:List[HSI_COMMON.FrameFloat]):
    """! Method to add a list of dark field images to the argument context object.
    Multiple dark field frames need to be added to a context if multiple exposure
    times are (or will be) used when processing RAW frames to cubes. In the context
    one dark reference is required for every integration time you (will) need.
    @param context [in] handle to a context object.
    @param darkref_list [in] list with one or more dark frames, to be used as dark references inside a context object
    """
    nr_dark_refs = len(darkref_list)
    dark_refs = (HSI_COMMON.FrameFloat * nr_dark_refs)(*darkref_list)
    result = __api_dll.mosaicContextSetDarkFieldReferences(context, dark_refs, nr_dark_refs)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_RETURN ContextSetOpticalSetup (HANDLE i_context_handle, OpticalSetup i_optical_setup);
'''
__api_dll.mosaicContextSetOpticalSetup.argtypes = [Handle, OpticalSetup]
__api_dll.mosaicContextSetOpticalSetup.restypes = ctypes.c_int

def ContextSetOpticalSetup(context:Handle, optical_setup:OpticalSetup):
    """! Method to add information about the optical setup of a Hyperspectral Camera
    to the argument context.
    @param context [in] handle to a context object.
    @param optical_setup [in] object with required information on the optical setup.
    """
    result = __api_dll.mosaicContextSetOpticalSetup(context, optical_setup)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)


''' #---------------------------------------------------------------------------------
HSI_RETURN ExtractSpectrumFromCube (SpectrumFloat* o_p_reference_spectrum, CubeFloat i_cube, RegionOfInterest i_roi);
'''
__api_dll.mosaicExtractSpectrumFromCube.argtypes = [ctypes.POINTER(HSI_COMMON.SpectrumFloat), HSI_COMMON.CubeFloat, HSI_COMMON.RegionOfInterest]
__api_dll.mosaicExtractSpectrumFromCube.restypes = ctypes.c_int

def ExtractSpectrumFromCube(cube:HSI_COMMON.CubeFloat, roi:HSI_COMMON.RegionOfInterest) -> HSI_COMMON.SpectrumFloat:
    """! Method to extract a spectrum from a processed cube. The spectrum is extracted
    from the argument cube.  The argument region of interest is applied onto the argument
    cube.  The resulting spectral values are averaged (per wavelength) resulting in the
    return spectrum object.
    @param cube [in] cube from which to extract a spectrum
    @param roi [in] region of interest specifying from which part of the argument cube to extract a spectrum.
    @returns spectrum object.
    """
    spectrum = HSI_COMMON.AllocateSpectrum(cube.format)
    result = __api_dll.mosaicExtractSpectrumFromCube(ctypes.byref(spectrum), cube, roi)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return spectrum


''' #---------------------------------------------------------------------------------
HSI_RETURN SetReferenceSpectrum (HANDLE i_pipeline_handle, SpectrumFloat i_p_balancing_spectrum, double i_balancing_coefficient);
'''
__api_dll.mosaicSetReferenceSpectrum.argtypes = [Handle, HSI_COMMON.SpectrumFloat, ctypes.c_double]
__api_dll.mosaicSetReferenceSpectrum.restypes = ctypes.c_int

def SetReferenceSpectrum(pipeline:Handle, balancing_spectrum:HSI_COMMON.SpectrumFloat, balancing_coefficient:float):
    """! Method to set a reference spectrum to a data processing pipeline.
    When setting a reference spectrum, or a balancing spectrum, the output values will be
    made relative to this reference spectrum.  The balancing coefficient is used to
    weight the values when calculating the relative output values.  For example a fully
    white reference should have a balancing coefficient of 1.0, a 50% gray reference spectrum
    should have a balancing coefficient of 0.50.
    @param pipeline [in] handle to a data processing object
    @param balancing_spectrum [in] spectrum object to be used when doing spectral balancing
    @param balancing_coefficient [in] weight coefficient to be applied with doing spectral balancing.
    """
    result = __api_dll.mosaicSetReferenceSpectrum(pipeline, balancing_spectrum, balancing_coefficient)
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)

''' #---------------------------------------------------------------------------------
HSI_RETURN LoadOpticalSetup (char const* i_in_file_path, OpticalSetup* o_p_optical_setup);
'''
__api_dll.mosaicLoadOpticalSetup.argtypes = [ctypes.c_wchar_p, ctypes.POINTER(OpticalSetup)]
__api_dll.mosaicLoadOpticalSetup.restypes = ctypes.c_int

def LoadOpticalSetup(filename:str) -> OpticalSetup:
    """! Method to load an optical setup file (XML) from disk into an optical setup object.
    @param filename [in] path to the filename to load the optical setup information from
    @return object with the parsed optical information.    
    """
    optical_setup = OpticalSetup()
    result = __api_dll.mosaicLoadOpticalSetup(filename.encode('utf-8'), ctypes.byref(optical_setup))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
    return optical_setup

''' #---------------------------------------------------------------------------------
HSI_RETURN ContextSetNonUniformity (HANDLE i_p_context, FrameFloat* i_p_dark_frame, FrameFloat* i_p_white_frame)
'''
__api_dll.mosaicContextSetNonUniformity.argtypes = [Handle, ctypes.POINTER(HSI_COMMON.FrameFloat), ctypes.POINTER(HSI_COMMON.FrameFloat)]
__api_dll.mosaicContextSetNonUniformity.restypes = ctypes.c_int

def ContextSetNonUniformity(context:Handle, dark_frame:HSI_COMMON.FrameFloat, white_frame:HSI_COMMON.FrameFloat):
    """! Method to load an optical setup file (XML) from disk into an optical setup object.
    @param context [in] handle to a context object.
    @param dark_frame [in] Frame (object) to be used as Non-Uniformity Dark image
    @param white_frame [in] Frame (object) to be used as Non-Uniformity White image
    """
    result = __api_dll.mosaicContextSetNonUniformity(context, ctypes.byref(dark_frame), ctypes.byref(white_frame))
    assert result == HSI_COMMON.ReturnValue.HSI_OK, HSI_COMMON.ToString (result)
