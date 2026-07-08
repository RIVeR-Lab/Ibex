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

import ctypes
from ctypes.util import find_library

# include the datatypes and enumerations for the API
from .hsi_common_types import *


# ---------------------------------------------------------------------
# testing for third party tools in PATH:
def __third_party_check():
    """! @private """
    if find_library ('hsi_api') is None:
        print("""

            Attention: The imec api libraries are not found in the path.

            For Windows:
                Please add it to the Windows PATH environment variable
                or add it to your python PATH environment at _top_ of your script:
                > import os
                > os.environ['PATH'] += os.pathsep + '<path to API folder>/bin'

            For Linux:
                The path to the hsi binaries have to be added to the LD_LIBRARY_PATH variable prior to
                running a Python script. The path has the form /opt/imec/<package>/bin, where 
                <package> is for instance hsi-mosaic or hsi-camera, depending on the installation.

                You can either set the environment variable permanently, or only for a
                specific run as follows:

                    LD_LIBRARY_PATH=/opt/imec/<package>/bin:LD_LIBRARY_PATH python3 <python_script>.py

        """)
        exit(1)
__third_party_check()


__api_cmm = None

# ---------------------------------------------------------------------
def __Dev_LoadDll():
    """! @private """
    global __api_cmm
    if __api_cmm is not None: return
    dllname = 'hsi_api'
    if len(sys.argv) > 1 and '--Debug' in sys.argv:
        dllname += '_d'
    dllname = find_library(dllname)
    __api_cmm = ctypes.CDLL(dllname)
    assert __api_cmm is not None

# ---------------------------------------------------------------------
import atexit

@atexit.register
def __Dev_UnloadDll():
    """! @private """
    global __api_cmm
    if __api_cmm is None: return
    del __api_cmm
    __api_cmm = None

__Dev_LoadDll() # load the dll now...


''' #---------------------------------------------------------------------------------
HSI_COMMON HSI_RETURN GetAPIVersion (int* o_p_major, int* o_p_minor, int* o_p_patch, int* o_p_beta);
'''
__api_cmm.commonGetAPIVersion.restype = ctypes.c_int
__api_cmm.commonGetAPIVersion.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int)]

def GetAPIVersion():
    major, minor, patch, beta = ctypes.c_int(0), ctypes.c_int(0), ctypes.c_int(0), ctypes.c_int(0)
    res = __api_cmm.commonGetAPIVersion(ctypes.byref(major),
                                        ctypes.byref(minor),
                                        ctypes.byref(patch),
                                        ctypes.byref(beta))
    return (major.value, minor.value, patch.value, beta.value)

''' #---------------------------------------------------------------------------------
HSI_RETURN InitializeLogger (char const* i_log_dir_path, LoggerVerbosity i_verbosity_level);
'''
__api_cmm.commonInitializeLogger.argtypes = [ctypes.c_char_p, ctypes.c_int]
__api_cmm.commonInitializeLogger.restype = ctypes.c_int

def InitializeLogger (log_dir_path:str, verbosity_level:int = LoggerVerbosity.LV_INFO):
    """! Initialize Logger
    @param log_dir_path [in] directory path to where the logger should put its output log files.
    @param verbosity_level [in] verbosity level to be used for all the internal loggers.
    """
    result = __api_cmm.commonInitializeLogger (log_dir_path.encode('utf-8'), verbosity_level)
    assert result == ReturnValue.HSI_OK, ToString (result)

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN AllocateFrame (FrameFloat*, FrameDataFormat );
'''
__api_cmm.commonAllocateFrame.restype = ctypes.c_int
__api_cmm.commonAllocateFrame.argtypes = [ctypes.POINTER(FrameFloat), FrameDataFormat]

def AllocateFrame (frame_dataformat:FrameDataFormat) -> FrameFloat:
    """! Create and Allocate (the internal buffers of) a frame object
         for the given frame Format
    @param frame_dataformat [in] required frame data format
    @result frame
    """
    frame = FrameFloat()
    result = __api_cmm.commonAllocateFrame (ctypes.byref(frame), frame_dataformat)
    assert result == ReturnValue.HSI_OK, ToString (result)
    return frame

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN DeallocateFrame (FrameFloat* );
'''
__api_cmm.commonDeallocateFrame.restype = ctypes.c_int
__api_cmm.commonDeallocateFrame.argtypes = [ctypes.POINTER(FrameFloat)]

def DeallocateFrame (frame:FrameFloat):
    """! Deallocate (the internal buffers of) the frame object indicated
    @param frame [in] frame object to deallocate
    """
    result = __api_cmm.commonDeallocateFrame (ctypes.byref(frame))
    assert result == ReturnValue.HSI_OK, ToString (result)

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN LoadFrame (FloatFrame*, char const* );
'''
__api_cmm.commonLoadFrame.argtypes = [ctypes.POINTER(FrameFloat), ctypes.c_char_p]
__api_cmm.commonLoadFrame.restypes = ctypes.c_int

def LoadFrame(filepath:str) -> FrameFloat:
    """! Create and Load a frame from disk, as indicated by the argument file path.
    @param filepath [in] file path from where to read and load a frame from disk.
    @return new frame object
    """
    frame = FrameFloat()
    result = __api_cmm.commonLoadFrame(ctypes.byref(frame), filepath.encode('utf-8'))
    assert result == ReturnValue.HSI_OK, ToString (result)
    return frame

''' #---------------------------------------------------------------------------------
HSI_RETURN SaveFrame (FrameFloat i_frame, char const* i_out_dir_path, char const* i_out_file_name, FileFormat i_out_file_format)
'''
__api_cmm.commonSaveFrame.argtypes = [FrameFloat, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]
__api_cmm.commonSaveFrame.restypes = ctypes.c_int

def SaveFrame(frame:FrameFloat, dirname:str, filename:str, format:int=FileFormat.FF_RAW):
    """! Save a frame to disk in a number of supported output file formats.
    @param frame [in] frame object to be saved to disk
    @param dirname [in] full path to the directory where the frame will be saved.
    @param filename [in] filename to be used to save the frame (without file extension!!!)
    @param format [in] which output file format to use (see FileFormat)
    """
    result = __api_cmm.commonSaveFrame(frame, dirname.encode('utf-8'), filename.encode('utf-8'), format)
    assert result == ReturnValue.HSI_OK, ToString (result)

''' #---------------------------------------------------------------------------------
HSI_API HSI_RETURN AllocateCube (CubeFloat* o_p_cube, CubeDataFormat i_data_format);
'''
__api_cmm.commonAllocateCube.argtypes = [ctypes.POINTER(CubeFloat), CubeDataFormat]
__api_cmm.commonAllocateCube.restypes = ctypes.c_int

def AllocateCube(cube_data_format:CubeDataFormat) -> CubeFloat:
    """! Create and Allocate (the internal buffers of) a cube object, according to the given cube data format.
    @param cube_data_format [in] data format to be used when allocating internal buffers (see CubeDataFormat)
    @return cube object (see CubeFloat)
    """
    cube = CubeFloat()
    result = __api_cmm.commonAllocateCube(ctypes.byref(cube), cube_data_format)
    assert result == ReturnValue.HSI_OK, ToString (result)
    return cube

''' #---------------------------------------------------------------------------------
HSI_MOSAIC_API HSI_RETURN DeallocateCube (HANDLE* io_p_frame);
'''
__api_cmm.commonDeallocateCube.argtypes = [ctypes.c_void_p,]
__api_cmm.commonDeallocateCube.restypes = ctypes.c_int

def DeallocateCube(cube:CubeFloat):
    """! Deallocate (the internal buffers of) a cube object.
    @param cube cube object to deallocate.
    """
    result = __api_cmm.commonDeallocateCube(ctypes.byref(cube))
    assert result == ReturnValue.HSI_OK, ToString (result)

''' #---------------------------------------------------------------------------------
HSI_API HSI_RETURN SaveCube (CubeFloat i_cube, char const* i_out_dir_path, char const* i_out_file_name, FileFormat i_out_file_format);
'''
__api_cmm.commonSaveCube.argtypes = [CubeFloat, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]
__api_cmm.commonSaveCube.restypes = ctypes.c_int

def SaveCube(cube:CubeFloat, dirname:str, filename:str, format:int=FileFormat.FF_ENVI):
    """! Save a cube to disk in a number of supported output file formats.
         In case an image format is provided (BMP, JPEG, ...) an image will be written for
         every band in this cube.
    @param cube [in] cube object to be saved to disk.
    @param dirname [in] full path to the directory where the cube will be saved.
    @param filename [in] filename to be used to save the cube (without file extension!!!).
    @param format [in] which output file format to use (see FileFormat).
    """
    result = __api_cmm.commonSaveCube(cube, dirname.encode('utf-8'), filename.encode('utf-8'), format)
    assert result == ReturnValue.HSI_OK, ToString (result)

''' #---------------------------------------------------------------------------------
HSI_RETURN AllocateSpectrum (SpectrumFloat* o_p_spectrum, CubeDataFormat i_data_format)
'''
__api_cmm.commonAllocateSpectrum.argtypes = [ctypes.POINTER(SpectrumFloat), CubeDataFormat]
__api_cmm.commonAllocateSpectrum.restypes = ctypes.c_int

def AllocateSpectrum(cube_data_format:CubeDataFormat) -> SpectrumFloat:
    """! Create and Allocate a spectrum object according to the provided cube data format.
    @param cube_data_format [in] cube data format to be used (see CubeDataFormat)
    @return spectrum object
    """
    spectrum = SpectrumFloat()
    result = __api_cmm.commonAllocateSpectrum(ctypes.byref(spectrum), cube_data_format)
    assert result == ReturnValue.HSI_OK, ToString (result)
    return spectrum

''' #---------------------------------------------------------------------------------
HSI_RETURN DeallocateSpectrum (SpectrumFloat* o_p_spectrum)
'''
__api_cmm.commonDeallocateSpectrum.argtypes = [ctypes.POINTER(SpectrumFloat)]
__api_cmm.commonDeallocateSpectrum.restypes = ctypes.c_int

def DeallocateSpectrum(spectrum:SpectrumFloat):
    """! Deallocate (the internal buffers of) a spectrum object.
    @param spectrum [in] spectrum object to deallocate
    """
    result = __api_cmm.commonDeallocateSpectrum(ctypes.byref(spectrum))
    assert result == ReturnValue.HSI_OK, ToString (result)

''' #---------------------------------------------------------------------------------
HSI_RETURN SaveSpectrum (SpectrumFloat i_p_spectrum, char const* i_out_file_path, char const* i_out_file_name);
'''
__api_cmm.commonSaveSpectrum.argtypes = [SpectrumFloat, ctypes.c_char_p, ctypes.c_char_p]
__api_cmm.commonSaveSpectrum.restypes = ctypes.c_int

def SaveSpectrum(spectrum:SpectrumFloat, dirname:str, filename:str):
    """! Save a spectrum to disk.  This is done in XML format.
    @param spectrum [in] spectrum object to be saved to disk.
    @param dirname [in] full path to the directory where the spectrum will be saved.
    @param filename [in] filename to be used to save the cube (without file extension!!!).
    """
    result = __api_cmm.commonSaveSpectrum(spectrum, dirname.encode('utf-8'), filename.encode('utf-8'))
    assert result == ReturnValue.HSI_OK, ToString (result)


''' #---------------------------------------------------------------------------------
HSI_RETURN LoadSpectrum (SpectrumFloat* o_p_spectrum, char const* i_in_file_path);
'''
__api_cmm.commonLoadSpectrum.argtypes = [ctypes.POINTER(SpectrumFloat), ctypes.c_char_p]
__api_cmm.commonLoadSpectrum.restypes = ctypes.c_int

def LoadSpectrum(filename:str) -> SpectrumFloat:
    """! Method to load a Spectrum file (in XML format) from disk into a spectrum object
    @param filename [in] filename to be used to load the spectrum data from.
    @returns spectrum object with the read in spectrum data
    """
    spectrum = SpectrumFloat()
    result = __api_cmm.commonLoadSpectrum(ctypes.byref(spectrum), filename.encode('utf-8'))
    assert result == ReturnValue.HSI_OK, ToString (result)
    return spectrum

''' #---------------------------------------------------------------------------------
HSI_RETURN LoadCube (CubeFloat* o_p_cube, char const* i_in_file_path)
'''
__api_cmm.commonLoadCube.argtypes = [ctypes.POINTER(CubeFloat), ctypes.c_char_p]
__api_cmm.commonLoadCube.restypes = ctypes.c_int

def LoadCube(filename:str) -> CubeFloat:
    """! Method to load a Cube file from disk into a cube object
    @param filename [in] filename to be used to load the cube data from (header file for ENVI/RAW formats).
    @returns cube object with the read in hyperspectral cube data
    """
    cube = CubeFloat()
    result = __api_cmm.commonLoadCube(ctypes.byref(cube), filename.encode('utf-8'))
    assert result == ReturnValue.HSI_OK, ToString (result)
    return cube

#---------------------------------------------------------------------------------
__api_cmm.commonGetMaxStringLength.argtypes = []
__api_cmm.commonGetMaxStringLength.restype = ctypes.c_int

def GetMaxStringLength() -> int:
    return __api_cmm.commonGetMaxStringLength()

