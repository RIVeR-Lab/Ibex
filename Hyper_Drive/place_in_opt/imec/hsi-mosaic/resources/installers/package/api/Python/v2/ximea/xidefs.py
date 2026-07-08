from ctypes import *
from .xi_wintypes import *

ERROR_CODES = {    
     0: "Function call succeeded",
     1: "Invalid handle",
     2: "Register read error",
     3: "Register write error",
     4: "Freeing resources error",
     5: "Freeing channel error",
     6: "Freeing bandwith error",
     7: "Read block error",
     8: "Write block error",
     9: "No image",
    10: "Timeout",
    11: "Invalid arguments supplied",
    12: "Not supported",
    13: "Attach buffers error",
    14: "Overlapped result",
    15: "Memory allocation error",
    16: "DLL context is NULL",
    17: "DLL context is non zero",
    18: "DLL context exists",
    19: "Too many devices connected",
    20: "Camera context error",
    21: "Unknown hardware",
    22: "Invalid TM file",
    23: "Invalid TM tag",
    24: "Incomplete TM",
    25: "Bus reset error",
    26: "Not implemented",
    27: "Shading is too bright",
    28: "Shading is too dark",
    29: "Gain is too low",
    30: "Invalid sensor defect correction list",
    31: "Error while sensor defect correction list reallocation",
    32: "Invalid pixel list",
    33: "Invalid Flash File System",
    34: "Invalid profile",
    35: "Invalid calibration",
    36: "Invalid buffer",
    38: "Invalid data",
    39: "Timing generator is busy",
    40: "Wrong operation open/write/read/close",
    41: "Acquisition already started",
    42: "Old version of device driver installed to the system.",
    43: "To get error code please call GetLastError function.",
    44: "Data cannot be processed",
    45: "Acquisition is stopped. It needs to be started to perform operation.",
    46: "Acquisition has been stopped with an error.",
    47: "Input ICC profile missing or corrupted",
    48: "Output ICC profile missing or corrupted",
    49: "Device not ready to operate",
    50: "Shading is too contrast",
    51: "Module already initialized",
    52: "Application does not have enough privileges (one or more app)",
    53: "Installed driver is not compatible with current software",
    54: "TM file was not loaded successfully from resources",
    55: "Device has been reset, abnormal initial state",
    56: "No Devices Found",
    57: "Resource (device) or function locked by mutex",
    58: "Buffer provided by user is too small",
    59: "Could not initialize processor.",
    60: "The object/module/procedure/process being referred to has not been started.",
    61: "Resource not found(could be processor, file, item...).",
    0: "Function call succeeded",
    100: "Unknown parameter",
    101: "Wrong parameter value",
    103: "Wrong parameter type",
    104: "Wrong parameter size",
    105: "Input buffer is too small",
    106: "Parameter is not supported",
    107: "Parameter info not supported",
    108: "Data format is not supported",
    109: "Read only parameter",
    111: "This camera does not support currently available bandwidth",
    112: "FFS file selector is invalid or NULL",
    113: "FFS file not found",
    114: "Parameter value cannot be set (might be out of range or invalid).",
    115: "Safe buffer policy is not supported. E.g. when transport target is set to GPU (GPUDirect).",
    116: "GPUDirect is not available. E.g. platform isn't supported or CUDA toolkit isn't installed.",
    117: "Incorrect sensor board unique identifier checksum.",
    118: "Incorrect or unknown FPGA firmware type used for camera.",
    119: "Parameter is not available in current context. Available only if another feature is turned on.",
    120: "Frame buffer RAM initialization error.",
    121: "Parameter is not supported while acquisition is running.",
    201: "Processing error - other",
    202: "Error while image processing.",
    203: "Input format is not supported for processing.",
    204: "Output format is not supported for processing.",
    205: "Parameter value is out of range",
       0: "Result OK",
    1001: "Invalid argument detected (out of range)",
    1002: "Implementation is not yet done",
    1003: "This value is not supported",
    1004: "Using of unknown parameter",
    1005: "Input buffer is not allocated",
    1006: "Output buffer is not allocated",
    1007: "Output buffer allocation error",
    1008: "There are no processors in chain",
    1009: "Some requirement is not met",
    1010: "Processors chain is full",
    1011: "Processor is not initialized yet",
    1012: "Processor generates error",
    1013: "Unknown data format",
    1014: "Unknown color filter array pattern type",
    1020: "Couldn't load library",
    1021: "Couldn't load function",
    1022: "Couldn't open process",
    1023: "Couldn't open process",
    1024: "Unsupported input format",
    1025: "Unsupported output format",
    1026: "Image intensity is our of range",
    1027: "Timeout waiting for event",
    1028: "Processor execution disabled",
    1029: "Defect already in list",
    1030: "Couldn't open input file",
    1031: "Couldn't allocate memory",
    1032: "Couldn't read",
    1033: "Couldn't write",
    1034: "Operation is read only",
    1035: "Input data not compatible with current FFC processor settings",
    1036: "Image can't be processed due to image width alignment is not supported by Image Processor.",
    }
# Enumerators

#Downsampling value enumerator.
XI_DOWNSAMPLING_VALUE = { 
    "XI_DWN_1x1": c_uint(1),    #1 sensor pixel = 1 image pixel
    "XI_DWN_2x2": c_uint(2),    #2x2 sensor pixels = 1 image pixel
    "XI_DWN_3x3": c_uint(3),    #Downsampling 3x3.
    "XI_DWN_4x4": c_uint(4),    #4x4 sensor pixels = 1 image pixel
    "XI_DWN_5x5": c_uint(5),    #Downsampling 5x5.
    "XI_DWN_6x6": c_uint(6),    #Downsampling 6x6.
    "XI_DWN_7x7": c_uint(7),    #Downsampling 7x7.
    "XI_DWN_8x8": c_uint(8),    #Downsampling 8x8.
    "XI_DWN_9x9": c_uint(9),    #Downsampling 9x9.
    "XI_DWN_10x10": c_uint(10),    #Downsampling 10x10.
    "XI_DWN_16x16": c_uint(16),    #Downsampling 16x16.
    }

#Test Pattern Generator
XI_TEST_PATTERN_GENERATOR = { 
    "XI_TESTPAT_GEN_SENSOR": c_uint(0),    #Sensor test pattern generator
    "XI_TESTPAT_GEN_FPGA": c_uint(1),    # FPGA Test Pattern Generator
    "XI_TESTPAT_GEN_MCU": c_uint(2),    # MCU Test Pattern Generator
    }

#Module/Unit version selector
XI_VERSION = { 
    "XI_VER_API": c_uint(0),    #version of API
    "XI_VER_DRV": c_uint(1),    #version of device driver
    "XI_VER_MCU1": c_uint(2),    #version of MCU1 firmware.
    "XI_VER_MCU2": c_uint(3),    #version of MCU2 firmware.
    "XI_VER_MCU3": c_uint(4),    #version of MCU3 firmware.
    "XI_VER_FPGA1": c_uint(5),    #version of FPGA1 firmware.
    "XI_VER_XMLMAN": c_uint(6),    #version of XML manifest.
    "XI_VER_HW_REV": c_uint(7),    #version of hardware revision.
    "XI_VER_FACTORY_SET": c_uint(8),    #version of factory set.
    }

#Test Pattern Type
XI_TEST_PATTERN = { 
    "XI_TESTPAT_OFF": c_uint(0),    # Testpattern turned off.
    "XI_TESTPAT_BLACK": c_uint(1),    # Image is filled with darkest possible image.
    "XI_TESTPAT_WHITE": c_uint(2),    # Image is filled with brightest possible image.
    "XI_TESTPAT_GREY_HORIZ_RAMP": c_uint(3),    # Image is filled horizontally with an image that goes from the darkest possible value to the brightest.
    "XI_TESTPAT_GREY_VERT_RAMP": c_uint(4),    # Image is filled vertically with an image that goes from the darkest possible value to the brightest.
    "XI_TESTPAT_GREY_HORIZ_RAMP_MOVING": c_uint(5),    # Image is filled horizontally with an image that goes from the darkest possible value to the brightest and moves from left to right.
    "XI_TESTPAT_GREY_VERT_RAMP_MOVING": c_uint(6),    # Image is filled vertically with an image that goes from the darkest possible value to the brightest and moves from left to right.
    "XI_TESTPAT_HORIZ_LINE_MOVING": c_uint(7),    # A moving horizontal line is superimposed on the live image.
    "XI_TESTPAT_VERT_LINE_MOVING": c_uint(8),    # A moving vertical line is superimposed on the live image.
    "XI_TESTPAT_COLOR_BAR": c_uint(9),    # Image is filled with stripes of color including White, Black, Red, Green, Blue, Cyan, Magenta and Yellow.
    "XI_TESTPAT_FRAME_COUNTER": c_uint(10),    # A frame counter is superimposed on the live image.
    "XI_TESTPAT_DEVICE_SPEC_COUNTER": c_uint(11),    # 128bit counter.
    }

#Decimation Pattern Format
XI_DEC_PATTERN = { 
    "XI_DEC_MONO": c_uint(1),    #adjacent pixels are decimated
    "XI_DEC_BAYER": c_uint(2),    #	Bayer pattern is preserved during pixel decimation
    }

#Binning Pattern Format
XI_BIN_PATTERN = { 
    "XI_BIN_MONO": c_uint(1),    #adjacent pixels are combined
    "XI_BIN_BAYER": c_uint(2),    #Bayer pattern is preserved during pixel combining
    }

#Binning Engine Selector
XI_BIN_SELECTOR = { 
    "XI_BIN_SELECT_SENSOR": c_uint(0),    #parameters for image sensor binning are selected
    "XI_BIN_SELECT_DEVICE_FPGA": c_uint(1),    #parameters for device (camera) FPGA decimation are selected
    "XI_BIN_SELECT_HOST_CPU": c_uint(2),    #parameters for Host CPU binning are selected
    }

#Selects binning mode; to be used with
XI_BIN_MODE = { 
    "XI_BIN_MODE_SUM": c_uint(0),    #The response from the combined pixels will be added, resulting in increased sensitivity.
    "XI_BIN_MODE_AVERAGE": c_uint(1),    #The response from the combined pixels will be averaged, resulting in increased signal/noise ratio.
    }

#Decimation Engine Selector
XI_DEC_SELECTOR = { 
    "XI_DEC_SELECT_SENSOR": c_uint(0),    #parameters for image sensor decimation are selected
    "XI_DEC_SELECT_DEVICE_FPGA": c_uint(1),    #parameters for device (camera) FPGA decimation are selected
    "XI_DEC_SELECT_HOST_CPU": c_uint(2),    #parameters for Host CPU decimation are selected
    }

#Sensor tap count enumerator.
XI_SENSOR_TAP_CNT = { 
    "XI_TAP_CNT_1": c_uint(1),    #1 sensor tap selected.
    "XI_TAP_CNT_2": c_uint(2),    #2 sensor taps selected.
    "XI_TAP_CNT_4": c_uint(4),    #4 sensor taps selected.
    }

#Bit depth enumerator.
XI_BIT_DEPTH = { 
    "XI_BPP_8": c_uint(8),    #8 bit per pixel
    "XI_BPP_9": c_uint(9),    #9 bit per pixel
    "XI_BPP_10": c_uint(10),    #10 bit per pixel
    "XI_BPP_11": c_uint(11),    #11 bit per pixel
    "XI_BPP_12": c_uint(12),    #12 bit per pixel
    "XI_BPP_13": c_uint(13),    #13 bit per pixel
    "XI_BPP_14": c_uint(14),    #14 bit per pixel
    "XI_BPP_15": c_uint(15),    #15 bit per pixel
    "XI_BPP_16": c_uint(16),    #16 bit per pixel
    "XI_BPP_24": c_uint(24),    #24 bit per pixel
    "XI_BPP_32": c_uint(32),    #32 bit per pixel
    }

#Debug level enumerator.
XI_DEBUG_LEVEL = { 
    "XI_DL_DETAIL": c_uint(0),    #(see Note1)
    "XI_DL_TRACE": c_uint(1),    #Prints errors, warnings and important informations
    "XI_DL_WARNING": c_uint(2),    #Prints all errors and warnings
    "XI_DL_ERROR": c_uint(3),    #Prints all errors
    "XI_DL_FATAL": c_uint(4),    #Prints only important errors
    "XI_DL_DISABLED": c_uint(100),    #Prints no messages
    }

#Image output format enumerator.
XI_IMG_FORMAT = { 
    "XI_MONO8": c_uint(0),    #8 bits per pixel. 	[Intensity] (see Note5,Note6)
    "XI_MONO16": c_uint(1),    #16 bits per pixel. [Intensity LSB] [Intensity MSB] (see Note5,Note6)
    "XI_RGB24": c_uint(2),    #RGB data format. [Blue][Green][Red] (see Note5)
    "XI_RGB32": c_uint(3),    #RGBA data format. 	[Blue][Green][Red][0] (see Note5)
    "XI_RGB_PLANAR": c_uint(4),    #RGB planar data format. [Red][Red]...[Green][Green]...[Blue][Blue]... (see Note5)
    "XI_RAW8": c_uint(5),    #8 bits per pixel raw data from sensor. 	[pixel byte] raw data from transport (camera output)
    "XI_RAW16": c_uint(6),    #16 bits per pixel raw data from sensor. 	[pixel byte low] [pixel byte high] 16 bits (depacked) raw data
    "XI_FRM_TRANSPORT_DATA": c_uint(7),    #Data from transport layer (e.g. packed). Depends on data on the transport layer (see Note7)
    "XI_RGB48": c_uint(8),    #RGB data format. [Blue low byte][Blue high byte][Green low][Green high][Red low][Red high] (see Note5)
    "XI_RGB64": c_uint(9),    #RGBA data format. [Blue low byte][Blue high byte][Green low][Green high][Red low][Red high][0][0] (Note5)
    "XI_RGB16_PLANAR": c_uint(10),    #RGB16 planar data format
    "XI_RAW8X2": c_uint(11),    #8 bits per pixel raw data from sensor(2 components in a row). [ch1 pixel byte] [ch2 pixel byte] 8 bits raw data from 2 channels (e.g. high gain and low gain channels of sCMOS cameras)
    "XI_RAW8X4": c_uint(12),    #8 bits per pixel raw data from sensor(4 components in a row). 	[ch1 pixel byte [ch2 pixel byte] [ch3 pixel byte] [ch4 pixel byte] 8 bits raw data from 4 channels (e.g. sCMOS cameras)
    "XI_RAW16X2": c_uint(13),    #16 bits per pixel raw data from sensor(2 components in a row). 	[ch1 pixel byte low] [ch1 pixel byte high] [ch2 pixel byte low] [ch2 pixel byte high] 16 bits (depacked) raw data from 2 channels (e.g. high gain and low gain channels of sCMOS cameras)
    "XI_RAW16X4": c_uint(14),    #16 bits per pixel raw data from sensor(4 components in a row). 	[ch1 pixel byte low] [ch1 pixel byte high] [ch2 pixel byte low] [ch2 pixel byte high] [ch3 pixel byte low] [ch3 pixel byte high] [ch4 pixel byte low] [ch4 pixel byte high] 16 bits (depacked) raw data from 4 channels (e.g. sCMOS cameras)
    "XI_RAW32": c_uint(15),    #32 bits per pixel raw data from sensor in integer format (LSB first). 4 bytes (LSB first) pixel (depacked) raw data
    "XI_RAW32FLOAT": c_uint(16),    #32 bits per pixel raw data from sensor in single-precision floating point format. 4 bytes per pixel (depacked) raw data
    "XI_RAW8X3": c_uint(17),    #8 bits per pixel raw data from sensor(3 components in a row). 	[ch1 pixel byte] [ch2 pixel byte] [ch3 pixel byte] raw data from 3 channels (e.g. ToF cameras)
    "XI_RAW16X3": c_uint(18),    #16 bits per pixel raw data from sensor(3 components in a row). [ch1 pixel byte low] [ch1 pixel byte high] [ch2 pixel byte low] [ch2 pixel byte high] [ch3 pixel byte low] [ch3 pixel byte high] 16 bits (depacked) raw data from 3 channels (e.g. ToF cameras)
    }

#Bayer color matrix enumerator.
XI_COLOR_FILTER_ARRAY = { 
    "XI_CFA_NONE": c_uint(0),    #Result pixels have no filters applied in this format
    "XI_CFA_BAYER_RGGB": c_uint(1),    #Regular RGGB
    "XI_CFA_CMYG": c_uint(2),    #AK Sony sens
    "XI_CFA_RGR": c_uint(3),    #2R+G readout
    "XI_CFA_BAYER_BGGR": c_uint(4),    #BGGR readout
    "XI_CFA_BAYER_GRBG": c_uint(5),    #GRBG readout
    "XI_CFA_BAYER_GBRG": c_uint(6),    #GBRG readout
    "XI_CFA_POLAR_A_BAYER_BGGR": c_uint(7),    #BGGR polarized 4x4 macropixel
    "XI_CFA_POLAR_A": c_uint(8),    #Polarized 2x2 macropixel
    "XI_CFA_TOF_ANB": c_uint(9),    #ToF A and B
    "XI_CFA_TOF_AMB": c_uint(10),    #ToF A minus B
    "XI_CFA_TOF_APB": c_uint(11),    #ToF A plus B
    "XI_CFA_TOF_A": c_uint(12),    #ToF A only
    "XI_CFA_TOF_B": c_uint(13),    #ToF B only
    "XI_CFA_TOF_X1X2X3": c_uint(14),    #ToF X1,X2,X3
    }

#Image sequence type.
XI_IMAGE_SEQUENCE_TYPE = { 
    "XI_IMAGE_SEQUENCE_TYPE_DISABLED": c_uint(0),    #No image sequence.
    "XI_IMAGE_SEQUENCE_TYPE_EXPOSURES": c_uint(1),    #Image sequence off different exposures. E.g. for HDR.
    }

#structure containing information about buffer policy(can be safe, data will be copied to user/app buffer or unsafe, user will get internally allocated buffer without data copy).
XI_BP = { 
    "XI_BP_UNSAFE": c_uint(0),    #User gets pointer to internally allocated circle buffer and data may be overwritten by device.
    "XI_BP_SAFE": c_uint(1),    #Data from device will be copied to user allocated buffer or xiApi allocated memory.
    }

#structure containing information about trigger source
XI_TRG_SOURCE = { 
    "XI_TRG_OFF": c_uint(0),    #Capture of next image is automatically started after previous.
    "XI_TRG_EDGE_RISING": c_uint(1),    #Capture is started on rising edge of selected input.
    "XI_TRG_EDGE_FALLING": c_uint(2),    #Capture is started on falling edge of selected input
    "XI_TRG_SOFTWARE": c_uint(3),    #Capture is started with software trigger.
    "XI_TRG_LEVEL_HIGH": c_uint(4),    #Specifies that the trigger is considered valid as long as the level of the source signal is high.
    "XI_TRG_LEVEL_LOW": c_uint(5),    #Specifies that the trigger is considered valid as long as the level of the source signal is low.
    }

#structure containing information about trigger functionality
XI_TRG_SELECTOR = { 
    "XI_TRG_SEL_FRAME_START": c_uint(0),    #Trigger starts the capture of one frame
    "XI_TRG_SEL_EXPOSURE_ACTIVE": c_uint(1),    #Trigger controls the start and length of the exposure.
    "XI_TRG_SEL_FRAME_BURST_START": c_uint(2),    #Trigger starts the capture of the bursts of frames in an acquisition.
    "XI_TRG_SEL_FRAME_BURST_ACTIVE": c_uint(3),    #Trigger controls the duration of the capture of the bursts of frames in an acquisition.
    "XI_TRG_SEL_MULTIPLE_EXPOSURES": c_uint(4),    #Trigger which when first trigger starts exposure and consequent pulses are gating exposure(active HI)
    "XI_TRG_SEL_EXPOSURE_START": c_uint(5),    #Trigger controls the start of the exposure of one Frame.
    "XI_TRG_SEL_MULTI_SLOPE_PHASE_CHANGE": c_uint(6),    #Trigger controls the multi slope phase in one Frame (phase0 -> phase1) or (phase1 -> phase2).
    "XI_TRG_SEL_ACQUISITION_START": c_uint(7),    #Selects a trigger that starts the Acquisition.
    }

#Trigger overlap modes
XI_TRG_OVERLAP = { 
    "XI_TRG_OVERLAP_OFF": c_uint(0),    #No trigger overlap is permitted.
    "XI_TRG_OVERLAP_READ_OUT": c_uint(1),    #Trigger is accepted immediately after the exposure period. (see Note1)
    "XI_TRG_OVERLAP_PREV_FRAME": c_uint(2),    #Trigger is accepted (latched) at any time during the capture of the previous frame.
    }

#structure containing information about acquisition timing modes
XI_ACQ_TIMING_MODE = { 
    "XI_ACQ_TIMING_MODE_FREE_RUN": c_uint(0),    #camera acquires images at a maximum possible framerate
    "XI_ACQ_TIMING_MODE_FRAME_RATE": c_uint(1),    #Selects a mode when sensor frame acquisition frequency is set to parameter FRAMERATE
    "XI_ACQ_TIMING_MODE_FRAME_RATE_LIMIT": c_uint(2),    #Selects a mode when sensor frame acquisition frequency is limited by parameter FRAMERATE
    }

#Enumerator for data target modes
XI_TRANSPORT_DATA_TARGET_MODE = { 
    "XI_TRANSPORT_DATA_TARGET_CPU_RAM": c_uint(0),    #normal CPU memory buffer is used for image data
    "XI_TRANSPORT_DATA_TARGET_GPU_RAM": c_uint(1),    #data is delivered straight to GPU memory using GPUDirect technology
    "XI_TRANSPORT_DATA_TARGET_UNIFIED": c_uint(2),    #CUDA managed memory is used for image data.
    "XI_TRANSPORT_DATA_TARGET_ZEROCOPY": c_uint(3),    #CUDA zerocopy memory is used for image data.
    }

#Enumeration for XI_PRM_GPI_SELECTOR for CB cameras.
XI_GPI_SEL_CB = { 
    "XI_GPI_SEL_CB_IN1": c_uint(1),    #Input1 - Pin3 (Opto Isolated).
    "XI_GPI_SEL_CB_IN2": c_uint(2),    #Input2 - Pin4 (Opto Isolated).
    "XI_GPI_SEL_CB_INOUT1": c_uint(3),    #Input/Output1 - Pin6
    "XI_GPI_SEL_CB_INOUT2": c_uint(4),    #Input/Output2 - Pin7
    "XI_GPI_SEL_CB_INOUT3": c_uint(5),    #Input/Output3 - Pin11
    "XI_GPI_SEL_CB_INOUT4": c_uint(6),    #Input/Output4 - Pin12
    }

#Enumeration for XI_PRM_GPO_SELECTOR for CB cameras.
XI_GPO_SEL_CB = { 
    "XI_GPO_SEL_CB_OUT1": c_uint(1),    #Output1 - Pin8 (Opto Isolated).
    "XI_GPO_SEL_CB_OUT2": c_uint(2),    #Output2 - Pin9 (Opto Isolated).
    "XI_GPO_SEL_CB_INOUT1": c_uint(3),    #Input/Output1 - Pin6
    "XI_GPO_SEL_CB_INOUT2": c_uint(4),    #Input/Output2 - Pin7
    "XI_GPO_SEL_CB_INOUT3": c_uint(5),    #Input/Output3 - Pin11
    "XI_GPO_SEL_CB_INOUT4": c_uint(6),    #Input/Output4 - Pin12
    }

#structure containing information about GPI functionality
XI_GPI_MODE = { 
    "XI_GPI_OFF": c_uint(0),    #Input is not used for triggering, but can be used to get parameter GPI_LEVEL. This can be used to switch I/O line on some cameras to input mode.
    "XI_GPI_TRIGGER": c_uint(1),    #Input can be used for triggering.
    "XI_GPI_EXT_EVENT": c_uint(2),    #External signal input (not implemented)
    }

#Enumerator for GPI port selection.
XI_GPI_SELECTOR = { 
    "XI_GPI_PORT1": c_uint(1),    #GPI port 1
    "XI_GPI_PORT2": c_uint(2),    #GPI port 2
    "XI_GPI_PORT3": c_uint(3),    #GPI port 3
    "XI_GPI_PORT4": c_uint(4),    #GPI port 4
    "XI_GPI_PORT5": c_uint(5),    #GPI port 5
    "XI_GPI_PORT6": c_uint(6),    #GPI port 6
    "XI_GPI_PORT7": c_uint(7),    #GPI port 7
    "XI_GPI_PORT8": c_uint(8),    #GPI port 8
    "XI_GPI_PORT9": c_uint(9),    #GPI port 9
    "XI_GPI_PORT10": c_uint(10),    #GPI port 10
    "XI_GPI_PORT11": c_uint(11),    #GPI port 11
    "XI_GPI_PORT12": c_uint(12),    #GPI port 12
    }

#structure containing information about GPO functionality
XI_GPO_MODE = { 
    "XI_GPO_OFF": c_uint(0),    #Output is off (zero voltage or switched_off)
    "XI_GPO_ON": c_uint(1),    #Output is on (voltage or switched_on)
    "XI_GPO_FRAME_ACTIVE": c_uint(2),    #Output is on while frame exposure,read,transfer.
    "XI_GPO_FRAME_ACTIVE_NEG": c_uint(3),    #Output is off while frame exposure,read,transfer.
    "XI_GPO_EXPOSURE_ACTIVE": c_uint(4),    #Output is on while frame exposure
    "XI_GPO_EXPOSURE_ACTIVE_NEG": c_uint(5),    #Output is off while frame exposure
    "XI_GPO_FRAME_TRIGGER_WAIT": c_uint(6),    #Output is on while camera is ready for trigger
    "XI_GPO_FRAME_TRIGGER_WAIT_NEG": c_uint(7),    #Output is off while camera is ready for trigger.
    "XI_GPO_EXPOSURE_PULSE": c_uint(8),    #Output is on short pulse at the beginning of frame exposure.
    "XI_GPO_EXPOSURE_PULSE_NEG": c_uint(9),    #Output is off short pulse at the beginning of frame exposure.
    "XI_GPO_BUSY": c_uint(10),    #Output is on when camera has received trigger until end of transfer
    "XI_GPO_BUSY_NEG": c_uint(11),    #Output is off when camera has received trigger until end of transfer
    "XI_GPO_HIGH_IMPEDANCE": c_uint(12),    #Associated pin is in high impedance (tri-stated) and can be driven externally. E.g. for triggering or reading status by GPI_LEVEL.
    "XI_GPO_FRAME_BUFFER_OVERFLOW": c_uint(13),    #Frame buffer overflow status.
    "XI_GPO_EXPOSURE_ACTIVE_FIRST_ROW": c_uint(14),    #Output is on while the first row exposure.
    "XI_GPO_EXPOSURE_ACTIVE_FIRST_ROW_NEG": c_uint(15),    #Output is off while the first row exposure.
    "XI_GPO_EXPOSURE_ACTIVE_ALL_ROWS": c_uint(16),    #Output is on while all rows exposure together.
    "XI_GPO_EXPOSURE_ACTIVE_ALL_ROWS_NEG": c_uint(17),    #Output is off while all rows exposure together.
    "XI_GPO_TXD": c_uint(18),    #Output is connected to TXD of UART module
    }

#Enumerator for GPO port selection.
XI_GPO_SELECTOR = { 
    "XI_GPO_PORT1": c_uint(1),    #GPO port 1
    "XI_GPO_PORT2": c_uint(2),    #GPO port 2
    "XI_GPO_PORT3": c_uint(3),    #GPO port 3
    "XI_GPO_PORT4": c_uint(4),    #GPO port 4
    "XI_GPO_PORT5": c_uint(5),    #GPO port 5
    "XI_GPO_PORT6": c_uint(6),    #GPO port 6
    "XI_GPO_PORT7": c_uint(7),    #GPO port 7
    "XI_GPO_PORT8": c_uint(8),    #GPO port 8
    "XI_GPO_PORT9": c_uint(9),    #GPO port 9
    "XI_GPO_PORT10": c_uint(10),    #GPO port 10
    "XI_GPO_PORT11": c_uint(11),    #GPO port 11
    "XI_GPO_PORT12": c_uint(12),    #GPO port 12
    }

#Metadata sampling mode selector
XI_METADATA_SAMPLING_MODE = { 
    "XI_METADATA_SAMPLING_OFF": c_uint(0),    #Metadata sampling turned off
    "XI_METADATA_SAMPLING_POINT_EXPOSURE_START": c_uint(1),    #Selects the sampling point to exposure start
    "XI_METADATA_SAMPLING_POINT_EXPOSURE_END": c_uint(2),    #Selects the sampling point to exposure end
    "XI_METADATA_SAMPLING_POINT_EXPOSURE_START_END": c_uint(3),    #Selects the sampling point to exposure start and end
    "XI_METADATA_SAMPLING_POINT_READOUT_END": c_uint(4),    #Selects the sampling point to readout end
    }

#structure containing information about LED functionality
XI_LED_MODE = { 
    "XI_LED_HEARTBEAT": c_uint(0),    #Set led to blink (1 Hz) if link is OK.
    "XI_LED_TRIGGER_ACTIVE": c_uint(1),    #Set led to blink if trigger detected.
    "XI_LED_EXT_EVENT_ACTIVE": c_uint(2),    #Set led to blink if external signal detected.
    "XI_LED_LINK": c_uint(3),    #Set led to blink if link is OK.
    "XI_LED_ACQUISITION": c_uint(4),    #Set led to blink if data streaming
    "XI_LED_EXPOSURE_ACTIVE": c_uint(5),    #Set led to blink if sensor integration time.
    "XI_LED_FRAME_ACTIVE": c_uint(6),    #Set led to blink if device busy/not busy.
    "XI_LED_OFF": c_uint(7),    #Set led to off.
    "XI_LED_ON": c_uint(8),    #Set led to on.
    "XI_LED_BLINK": c_uint(9),    #Blinking (1Hz).
    }

#Enumerator for LED selection.
XI_LED_SELECTOR = { 
    "XI_LED_SEL1": c_uint(1),    #LED 1
    "XI_LED_SEL2": c_uint(2),    #LED 2
    "XI_LED_SEL3": c_uint(3),    #LED 3
    "XI_LED_SEL4": c_uint(4),    #LED 4
    "XI_LED_SEL5": c_uint(5),    #LED 5
    }

#structure contains frames counter
XI_COUNTER_SELECTOR = { 
    "XI_CNT_SEL_TRANSPORT_SKIPPED_FRAMES": c_uint(0),    #Number of skipped frames on transport layer (e.g. when image gets lost while transmission). Occur when capacity of transport channel does not allow to transfer all data.
    "XI_CNT_SEL_API_SKIPPED_FRAMES": c_uint(1),    #Number of skipped frames on API layer. Occur when application does not process the images as quick as they are received from the camera.
    "XI_CNT_SEL_TRANSPORT_TRANSFERRED_FRAMES": c_uint(2),    #Number of delivered buffers since last acquisition start.
    "XI_CNT_SEL_FRAME_MISSED_TRIGGER_DUETO_OVERLAP": c_uint(3),    #Number of missed triggers overlapped with exposure or read-out stage of previous frame – see XI_PRM_TRG_OVERLAP. (see Note1)
    "XI_CNT_SEL_FRAME_MISSED_TRIGGER_DUETO_FRAME_BUFFER_OVR": c_uint(4),    #Number of missed triggers due to frame buffer full. (see Note1)
    "XI_CNT_SEL_FRAME_BUFFER_OVERFLOW": c_uint(5),    #Internal camera frame buffer memory (RAM) full events counter. It can be incremented multiple times per one frame. (see Note1)
    "XI_CNT_SEL_TRANSPORT_QUEUE_UNDERRUN": c_uint(6),    #Incremented when camera starts to transfer new image, however no target buffer is queued in the transport queue. Connected to GenTL.STREAM_INFO_NUM_UNDERRUN. (see Note1)
    "XI_CNT_SEL_ACQUISITION_AUTO_RESTARTED_ON_FAILURE": c_uint(7),    #Acquisition can be restarted, due to failures on bus
    }

#structure containing information about timestamp reset arming
XI_TS_RST_MODE = { 
    "XI_TS_RST_ARM_ONCE": c_uint(0),    #Engine is disabled after TimeStamp has been reset after selected event.
    "XI_TS_RST_ARM_PERSIST": c_uint(1),    #Engine is armed permanently so each selected event will trigger TimeStamp reset. 
    }

#structure containing information about possible timestamp reset sources
XI_TS_RST_SOURCE = { 
    "XI_TS_RST_OFF": c_uint(0),    #No source selected TimeStamp reset is not armed.
    "XI_TS_RST_SRC_GPI_1": c_uint(1),    #GPI1 rising edge is active (signal after de-bounce module)
    "XI_TS_RST_SRC_GPI_2": c_uint(2),    #GPI2 rising edge is active
    "XI_TS_RST_SRC_GPI_3": c_uint(3),    #GPI3 rising edge is active
    "XI_TS_RST_SRC_GPI_4": c_uint(4),    #GPI4 rising edge is active
    "XI_TS_RST_SRC_GPI_1_INV": c_uint(5),    #GPI1 falling edge is active
    "XI_TS_RST_SRC_GPI_2_INV": c_uint(6),    #GPI2 falling edge is active
    "XI_TS_RST_SRC_GPI_3_INV": c_uint(7),    #GPI3 falling edge is active
    "XI_TS_RST_SRC_GPI_4_INV": c_uint(8),    #GPI4 falling edge is active
    "XI_TS_RST_SRC_GPO_1": c_uint(9),    #TimeStamp reset source selected GPO1
    "XI_TS_RST_SRC_GPO_2": c_uint(10),    #TimeStamp reset source selected GPO2
    "XI_TS_RST_SRC_GPO_3": c_uint(11),    #TimeStamp reset source selected GPO3
    "XI_TS_RST_SRC_GPO_4": c_uint(12),    #TimeStamp reset source selected GPO4
    "XI_TS_RST_SRC_GPO_1_INV": c_uint(13),    #TimeStamp reset source selected GPO1 inverted
    "XI_TS_RST_SRC_GPO_2_INV": c_uint(14),    #TimeStamp reset source selected GPO2 inverted
    "XI_TS_RST_SRC_GPO_3_INV": c_uint(15),    #TimeStamp reset source selected GPO3 inverted
    "XI_TS_RST_SRC_GPO_4_INV": c_uint(16),    #TimeStamp reset source selected GPO4 inverted
    "XI_TS_RST_SRC_TRIGGER": c_uint(17),    #TRIGGER to sensor rising edge is active
    "XI_TS_RST_SRC_TRIGGER_INV": c_uint(18),    #TRIGGER to sensor rising edge is active
    "XI_TS_RST_SRC_SW": c_uint(19),    #TRIGGER to sensor rising edge is active. TimeStamp is reset by software take effect imminently.
    "XI_TS_RST_SRC_EXPACTIVE": c_uint(20),    #Exposure Active signal rising edge 
    "XI_TS_RST_SRC_EXPACTIVE_INV": c_uint(21),    #Exposure Active signal falling edge 
    "XI_TS_RST_SRC_FVAL": c_uint(22),    #Frame valid signal rising edge (internal signal in camera)
    "XI_TS_RST_SRC_FVAL_INV": c_uint(23),    #Frame valid signal falling edge (internal signal in camera)
    "XI_TS_RST_SRC_GPI_5": c_uint(24),    #GPI5 rising edge is active
    "XI_TS_RST_SRC_GPI_6": c_uint(25),    #GPI6 rising edge is active
    "XI_TS_RST_SRC_GPI_5_INV": c_uint(26),    #GPI5 falling edge is active
    "XI_TS_RST_SRC_GPI_6_INV": c_uint(27),    #GPI6 falling edge is active
    "XI_TS_RST_SRC_GPI_7": c_uint(28),    #TimeStamp reset source selected GPI7 (after de bounce)
    "XI_TS_RST_SRC_GPI_8": c_uint(29),    #TimeStamp reset source selected GPI8 (after de bounce)
    "XI_TS_RST_SRC_GPI_9": c_uint(30),    #TimeStamp reset source selected GPI9 (after de bounce)
    "XI_TS_RST_SRC_GPI_10": c_uint(31),    #TimeStamp reset source selected GPI10 (after de bounce)
    "XI_TS_RST_SRC_GPI_11": c_uint(32),    #TimeStamp reset source selected GPI11 (after de bounce)
    "XI_TS_RST_SRC_GPI_7_INV": c_uint(33),    #TimeStamp reset source selected GPI7 inverted (after de bounce)
    "XI_TS_RST_SRC_GPI_8_INV": c_uint(34),    #TimeStamp reset source selected GPI8 inverted (after de bounce)
    "XI_TS_RST_SRC_GPI_9_INV": c_uint(35),    #TimeStamp reset source selected GPI9 inverted (after de bounce)
    "XI_TS_RST_SRC_GPI_10_INV": c_uint(36),    #TimeStamp reset source selected GPI10 inverted (after de bounce)
    "XI_TS_RST_SRC_GPI_11_INV": c_uint(37),    #TimeStamp reset source selected GPI11 inverted (after de bounce)
    }

#structure containing information about parameters type
XI_PRM_TYPE = { 
    "xiTypeInteger": c_uint(0),    #integer parameter type
    "xiTypeFloat": c_uint(1),    #float parameter type
    "xiTypeString": c_uint(2),    #string parameter type
    "xiTypeEnum": c_uint(0),    #enumerator parameter type
    "xiTypeBoolean": c_uint(0),    #boolean parameter type
    "xiTypeCommand": c_uint(0),    #command parameter type
    "xiTypeInteger64": c_uint(6),    #64bit integer parameter type
    }

#Turn parameter On/Off
XI_SWITCH = { 
    "XI_OFF": c_uint(0),    #Turn parameter off
    "XI_ON": c_uint(1),    #Turn parameter on
    }

#Temperature selector
XI_TEMP_SELECTOR = { 
    "XI_TEMP_IMAGE_SENSOR_DIE_RAW": c_uint(0),    #Image sensor die (non-calibrated)
    "XI_TEMP_IMAGE_SENSOR_DIE": c_uint(1),    #Image sensor die (calibrated)
    "XI_TEMP_SENSOR_BOARD": c_uint(2),    #Image sensor PCB
    "XI_TEMP_INTERFACE_BOARD": c_uint(3),    #Data interface PCB
    "XI_TEMP_FRONT_HOUSING": c_uint(4),    #Front part of camera housing
    "XI_TEMP_REAR_HOUSING": c_uint(5),    #Rear part of camera housing
    "XI_TEMP_TEC1_COLD": c_uint(6),    #TEC1 cold side temperature
    "XI_TEMP_TEC1_HOT": c_uint(7),    #TEC1 hot side temperature
    "XI_TEMP_VCSEL_BOARD_A": c_uint(8),    #VCSEL board temperature 
    }

#Temperature selector
XI_TEMP_CTRL_MODE_SELECTOR = { 
    "XI_TEMP_CTRL_MODE_OFF": c_uint(0),    #Controlling of elements (TEC/Peltier, Fans) is turned off
    "XI_TEMP_CTRL_MODE_AUTO": c_uint(1),    #Controlling of elements is performed automatically by API or camera in order to reach parameter TARGET_TEMP.
    "XI_TEMP_CTRL_MODE_MANUAL": c_uint(2),    #Controlling of elements is done manually by application.
    }

#Temperature element selector
XI_TEMP_ELEMENT_SELECTOR = { 
    "XI_TEMP_ELEM_TEC1": c_uint(11),    #TEC1 = TEC/Peltier that is closest to the image sensor
    "XI_TEMP_ELEM_TEC2": c_uint(12),    #TEC2 = TEC/Peltier location depends on camera model
    "XI_TEMP_ELEM_FAN1": c_uint(31),    #Temperature element fan current or rotation (FAN1 = Fan)
    "XI_TEMP_ELEM_FAN1_THRS_TEMP": c_uint(32),    #Temperature element fan start rotation threshold temperature
    }

#Data packing(grouping) types.
XI_OUTPUT_DATA_PACKING_TYPE = { 
    "XI_DATA_PACK_XI_GROUPING": c_uint(0),    #Data grouping (10g160, 12g192, 14g224).
    "XI_DATA_PACK_PFNC_LSB_PACKING": c_uint(1),    #Data packing (10p, 12p)
    }

#Downsampling types
XI_DOWNSAMPLING_TYPE = { 
    "XI_BINNING": c_uint(0),    #pixels are interpolated - better image
    "XI_SKIPPING": c_uint(1),    #pixels are skipped - higher frame rate
    }

#Selector of processing engine(instructions set)
XI_PROC_ENGINE = { 
    "XI_PE_ALL": c_uint(0),    #Use all available instructions
    "XI_PE_C": c_uint(1),    #Use C(C++) code
    "XI_PE_SSE2": c_uint(3),    #Use SSE2 instructions
    "XI_PE_AVX": c_uint(4),    #Use AVX instructions
    "XI_PE_AVX2": c_uint(5),    #Use AVX2 instructions
    }

#Image correction function
XI_IMAGE_CORRECTION_SELECTOR = { 
    "XI_CORRECTION_TYPE_SELECTOR": c_uint(0),    #Correction Type selected see XI_TYPE_CORRECTION_SELECTOR
    "XI_DEFECT_ID": c_uint(1),    #Select defect id
    "XI_DEFECTS_COUNT_BY_TYPE": c_uint(2),    #Count of defects selected by current XI_DEFECT_TYPE
    "XI_DEFECT_TYPE": c_uint(3),    #Type of defect see XI_IMAGE_DEFECT_TYPE
    "XI_DEFECT_SUB_TYPE": c_uint(4),    #Defect sub type see XI_IMAGE_DEFECT_SUB_TYPE
    "XI_DEFECT_POS_X": c_uint(5),    #Defect position x
    "XI_DEFECT_POS_Y": c_uint(6),    #Defect position y
    "XI_DEFECT_CMD_ADD": c_uint(7),    #Write cached defect to the list
    "XI_DEFECT_CMD_DELETE": c_uint(8),    #Delete defect to the list
    "XI_DEFECT_CMD_APPLY_CHANGES": c_uint(9),    #Apply changes
    "XI_DEFECT_CMD_LIST_CLEAR": c_uint(10),    #Clear list
    "XI_DEFECT_CMD_LISTS_CLEAR": c_uint(11),    #Clear lists
    "XI_DEFECT_CMD_SAVE": c_uint(12),    #Save list to device
    "XI_CORRECTION_TYPE_ENABLED": c_uint(13),    #Enable or disable correction type
    "XI_DEFECT_ID_BY_TYPE": c_uint(14),    #Select defect id by type
    "XI_LIST_ID": c_uint(15),    #Select list id
    "XI_DEFECT_CMD_APPLY_CHANGES_ALL": c_uint(16),    #Apply changes to all lists
    "XI_LIST_STATUS": c_uint(17),    #Current list status (Read-only). Result is mask of bits XI_LIST_STATUS_GENERATED, XI_LIST_STATUS_...
    "XI_IMG_COR_TAP_SELECTOR": c_uint(64),    #Selected tap id (0-N) for image correction
    "XI_IMG_COR_GAIN_TUNE": c_uint(65),    #Adjustment of gain in dB. For multitap sensors, active tap is selected by XI_IMG_COR_TAP_SELECTOR.
    "XI_IMG_COR_OFFSET_TUNE": c_uint(66),    #Adjustment of pixel values offset. For multitap sensors, active tap is selected by XI_IMG_COR_TAP_SELECTOR.
    }

#Define image  correction type
XI_TYPE_CORRECTION_SELECTOR = { 
    "XI_CORR_TYPE_SENSOR_DEFECTS_FACTORY": c_uint(0),    #Factory defect list
    "XI_CORR_TYPE_SENSOR_COLUMN_FPN": c_uint(1),    #Select Fixed Pattern Noise Correction for Columns
    "XI_CORR_TYPE_SENSOR_ADC_BLO": c_uint(2),    #ADC gain and black level offset sensor register correction
    "XI_CORR_TYPE_SENSOR_ROW_FPN": c_uint(3),    #Select Fixed Pattern Noise Correction for Rows
    "XI_CORR_TYPE_SENSOR_DEFECTS_USER0": c_uint(4),    #User defect list
    "XI_CORR_TYPE_SENSOR_CHANNELS_TUNE": c_uint(5),    #Image channel/tap intensity correction
    "XI_CORR_TYPE_SENSOR_COLUMN_BLACK_OFFSET": c_uint(6),    #Select image black offset Correction for Columns
    "XI_CORR_TYPE_SENSOR_ROW_BLACK_OFFSET": c_uint(7),    #Select image black offset Correction for Rows
    "XI_CORR_TYPE_SENSOR_DEFECTS_IN_CAMERA": c_uint(8),    #Custom defect list(specific cameras)
    }

#Define image defect types
XI_IMAGE_DEFECT_TYPE = { 
    "XI_IMAGE_DEFECT_TYPE_PIXEL": c_uint(0),    #Defect is pixel
    "XI_IMAGE_DEFECT_TYPE_COLUMN": c_uint(1),    #Defect is column
    "XI_IMAGE_DEFECT_TYPE_ROW": c_uint(2),    #Defect is row
    }

#Define image defect sub types
XI_IMAGE_DEFECT_SUB_TYPE = { 
    "XI_IMAGE_DEFECT_SUB_TYPE_DARK": c_uint(0),    #Defect pixel(s) is(are) too dark
    "XI_IMAGE_DEFECT_SUB_TYPE_BRIGHT": c_uint(1),    #Defect pixel(s) is(are) out of range
    "XI_IMAGE_DEFECT_SUB_TYPE_HOT": c_uint(2),    #Defect pixel(s) is(are) too bright
    }

#Exposure time selector
XI_EXPOSURE_TIME_SELECTOR_TYPE = { 
    "XI_EXPOSURE_TIME_SELECTOR_COMMON": c_uint(0),    #Selects the common Exposure Time
    "XI_EXPOSURE_TIME_SELECTOR_GROUP1": c_uint(1),    #Selects the common Exposure Time for pixel group 1 (for InterlineExposureMode)
    "XI_EXPOSURE_TIME_SELECTOR_GROUP2": c_uint(2),    #Selects the common Exposure Time for pixel group 2 (for InterlineExposureMode)
    "XI_EXPOSURE_TIME_SELECTOR_DUAL_TRG_EXP_ZONE_1": c_uint(3),    #Selects the Exposure Time for Zone 1 (for Dual Trigger Exposure feature)
    "XI_EXPOSURE_TIME_SELECTOR_DUAL_TRG_EXP_ZONE_2": c_uint(4),    #Selects the Exposure Time for Zone 2 (for Dual Trigger Exposure feature)
    }

#Interline exposure mode
XI_INTERLINE_EXPOSURE_MODE_TYPE = { 
    "XI_INTERLINE_EXPOSURE_MODE_OFF": c_uint(0),    #Disabled
    "XI_INTERLINE_EXPOSURE_MODE_ON": c_uint(1),    #Enabled
    }

#Gain selector
XI_GAIN_SELECTOR_TYPE = { 
    "XI_GAIN_SELECTOR_ALL": c_uint(0),    #Gain selector selects all channels. Implementation of gain type depends on camera.
    "XI_GAIN_SELECTOR_ANALOG_ALL": c_uint(1),    #Gain selector selects all analog channels. This is available only on some cameras.
    "XI_GAIN_SELECTOR_DIGITAL_ALL": c_uint(2),    #Gain selector selects all digital channels. This is available only on some cameras.
    "XI_GAIN_SELECTOR_ANALOG_TAP1": c_uint(3),    #Gain selector selects tap 1. This is available only on some cameras.
    "XI_GAIN_SELECTOR_ANALOG_TAP2": c_uint(4),    #Gain selector selects tap 2. This is available only on some cameras.
    "XI_GAIN_SELECTOR_ANALOG_TAP3": c_uint(5),    #Gain selector selects tap 3. This is available only on some cameras.
    "XI_GAIN_SELECTOR_ANALOG_TAP4": c_uint(6),    #Gain selector selects tap 4. This is available only on some cameras.
    "XI_GAIN_SELECTOR_ANALOG_N": c_uint(7),    #First of two channels of programmable gain control (PGC) function - Gain setting of R, B pixels (North column analog gain). This is available only on some cameras.
    "XI_GAIN_SELECTOR_ANALOG_S": c_uint(8),    #Second of two channels of programmable gain control (PGC) function - Gain setting of Gr, Gb pixels (South column analog gain). This is available only on some cameras.
    }

#Shutter mode types
XI_SHUTTER_TYPE = { 
    "XI_SHUTTER_GLOBAL": c_uint(0),    #Sensor Global Shutter(CMOS sensor)
    "XI_SHUTTER_ROLLING": c_uint(1),    #Sensor Electronic Rolling Shutter(CMOS sensor)
    "XI_SHUTTER_GLOBAL_RESET_RELEASE": c_uint(2),    #Sensor Global Reset Release Shutter(CMOS sensor)
    }

#structure containing information about CMS functionality
XI_CMS_MODE = { 
    "XI_CMS_DIS": c_uint(0),    #disables color management
    "XI_CMS_EN": c_uint(1),    #enables color management (high CPU usage)
    "XI_CMS_EN_FAST": c_uint(2),    #enables fast color management (high RAM usage)
    }

#structure containing information about ICC Intents
XI_CMS_INTENT = { 
    "XI_CMS_INTENT_PERCEPTUAL": c_uint(0),    #CMS intent perceptual
    "XI_CMS_INTENT_RELATIVE_COLORIMETRIC": c_uint(1),    #CMS intent relative colorimetry
    "XI_CMS_INTENT_SATURATION": c_uint(2),    #CMS intent saturation
    "XI_CMS_INTENT_ABSOLUTE_COLORIMETRIC": c_uint(3),    #CMS intent absolute colorimetry
    }

#structure containing information about options for selection of camera before opening
XI_OPEN_BY = { 
    "XI_OPEN_BY_INST_PATH": c_uint(0),    #Open camera by its hardware path
    "XI_OPEN_BY_SN": c_uint(1),    #Open camera by its serial number
    "XI_OPEN_BY_USER_ID": c_uint(2),    #open camera by its custom user ID
    "XI_OPEN_BY_LOC_PATH": c_uint(3),    #Open camera by its hardware location path
    }

#Lens feature selector selects which feature will be accessed.
XI_LENS_FEATURE = { 
    "XI_LENS_FEATURE_MOTORIZED_FOCUS_SWITCH": c_uint(1),    #Status of lens motorized focus switch
    "XI_LENS_FEATURE_MOTORIZED_FOCUS_BOUNDED": c_uint(2),    #On read = 1 if motorized focus is on one of limits.
    "XI_LENS_FEATURE_MOTORIZED_FOCUS_CALIBRATION": c_uint(3),    #(planned feature) On read = 1 if motorized focus is calibrated. Write 1 to start calibration.
    "XI_LENS_FEATURE_IMAGE_STABILIZATION_ENABLED": c_uint(4),    #On read = 1 if image stabilization is enabled. Write 1 to enable image stabilization.
    "XI_LENS_FEATURE_IMAGE_STABILIZATION_SWITCH_STATUS": c_uint(5),    #On read = 1 if image stabilization switch is in position On.
    "XI_LENS_FEATURE_IMAGE_ZOOM_SUPPORTED": c_uint(6),    #On read = 1 if lens supports zoom = are not prime.
    }

#Sensor feature selector selects which feature will be accessed.
XI_SENSOR_FEATURE_SELECTOR = { 
    "XI_SENSOR_FEATURE_ZEROROT_ENABLE": c_uint(0),    #Sensor Zero ROT enable for ONSEMI PYTHON family. For camera model:MQ013xG-ON (on/off)
    "XI_SENSOR_FEATURE_BLACK_LEVEL_CLAMP": c_uint(1),    #Black level offset clamping (value). for Camera model:MD
    "XI_SENSOR_FEATURE_MD_FPGA_DIGITAL_GAIN_DISABLE": c_uint(2),    #Disable digital component of gain for MD family (1=disabled/0=enabled)
    "XI_SENSOR_FEATURE_ACQUISITION_RUNNING": c_uint(3),    #Sensor acquisition is running status (0/1). Could be stopped by setting of 0. For camera model:CB,MC,MX,MT
    "XI_SENSOR_FEATURE_TIMING_MODE": c_uint(4),    #Sensor timing mode (value depends on sensor)
    "XI_SENSOR_FEATURE_PARALLEL_ADC": c_uint(5),    # Enables the parallel ADC readout mode, where all exposed pixels undergo dual sampling, leading to reduced readout noise at the cost of increased readout time 
    "XI_SENSOR_FEATURE_BLACK_LEVEL_OFFSET_RAW": c_uint(6),    #Sensor specific register raw black level offset (value)
    "XI_SENSOR_FEATURE_SHORT_INTERVAL_SHUTTER": c_uint(7),    #Sensor short Interval Shutter (on/off)
    "XI_SENSOR_FEATURE_AUTO_LOW_POWER_MODE_AUTO": c_uint(8),    #Sensor low power mode (on/off)
    "XI_SENSOR_FEATURE_HIGH_CONVERSION_GAIN": c_uint(9),    #Enables high conversion gain feature which applies additional gain to the signal at the pixel level. This leads to a reduction in read noise and a boost in sensitivity and signal-to-noise ratio, particularly in low-light situations. Consequently, the camera exhibits superior performance in dark environments, capturing images with minimal noise and enhanced detail.
    "XI_SENSOR_FEATURE_DUAL_TRG_EXP_ZONE_DIVIDER_POSITION": c_uint(10),    #Sensor Dual Trigger Exposure Zone Divider Position
    "XI_SENSOR_FEATURE_TOF_VCSEL_CTRL_VOLTAGE_MV": c_uint(11),    #ToF VCSEL Control Voltage in mV
    "XI_SENSOR_FEATURE_MULTIPLE_ADC": c_uint(12),    #Multiple ADC
    "XI_SENSOR_FEATURE_FAST_TRIGGER_MODE_ENABLE": c_uint(13),    #Enables a sensor trigger mode that starts exposure immediately at the beginning of the trigger signal. When fast trigger mode is disabled, the camera operates in a trigger mode that allows exposure and readout to overlap, resulting in a higher FPS. For camera model: MU051xG. (on/off)
    }

#Extended feature selector.
XI_EXT_FEATURE_SELECTOR = { 
    "XI_EXT_FEATURE_SEL_SIMULATOR_GENERATOR_FRAME_LOST_PERIOD_MIN": c_uint(1),    #Camera simulator lost frame generation minimum period (in frames).
    "XI_EXT_FEATURE_SEL_SIMULATOR_GENERATOR_FRAME_LOST_PERIOD_MAX": c_uint(2),    #Camera simulator lost frame generation random period (in frames).
    "XI_EXT_FEATURE_SEL_SIMULATOR_IMAGE_DATA_FORMAT": c_uint(3),    #Camera simulator image data format.
    "XI_EXT_FEATURE_SEL_BANDWIDTH_MEASUREMENT_TIME_SECONDS": c_uint(4),    #Number of seconds for bandwidth measurement. Default = 1.
    "XI_EXT_FEATURE_SEL_IMAGE_INTENSIFIER_VOLTAGE": c_uint(5),    #Input voltage for image intensifier. Default = 0.
    "XI_EXT_FEATURE_SEL_TRIG_FRAME": c_uint(6),    #Triggers frame(s) on internal event. Default = 0.
    "XI_EXT_FEATURE_SEL_IMAGE_OVERSAMPLING": c_uint(7),    #Enable/disable image pixels oversampling. Default = 0.
    "XI_EXT_FEATURE_SEL_APPLY_DATA_FINAL": c_uint(8),    #Enable/disable applying data final. Default = 1.
    "XI_EXT_FEATURE_SEL_FAN_RPM": c_uint(9),    #Sets camera cooling fan rpm (% from max). Default = 100.
    "XI_EXT_FEATURE_SEL_DITHERING_HOST": c_uint(10),    #Enables/Disables shifted(left/up) image data dithering on HOST side. Default = 0(off).
    "XI_EXT_FEATURE_SEL_DITHERING_DEVICE": c_uint(11),    #Enables/Disables shifted(left/up) image data dithering on DEVICE side. Default = 0(off).
    "XI_EXT_FEATURE_SEL_FAN_THR_TEMP": c_uint(12),    #Sets camera fan/back side threshold temperature. Default = 35.
    "XI_EXT_FEATURE_PCIE_IOCTL_GLOBAL_LOCK_ENABLED": c_uint(13),    #Controls if PCIe IOCTL global locking is enabled. If disabled, concurrent operation (e.g. using filesystem is running faster in multiple threads)
    "XI_EXT_FEATURE_SEL_EXTERNAL_POWER_SOURCE_VOLTAGE": c_uint(14),    #Input voltage from external power source.
    "XI_EXT_FEATURE_SEL_TOL_INV_SBSN": c_uint(15),    #Tolerance to invalid Sensor board SN while open on certain cameras.
    "XI_EXT_FEATURE_SEL_TOL_FRAME_BUFFER_RAM_INIT_ERROR": c_uint(16),    #Tolerance FrameBuffer RAM initialization error while open.
    "XI_EXT_FEATURE_SEL_TRANSPARENT_RETURN_CODES_ENABLED": c_uint(17),    #Enables transparent return codes(MC family).
    "XI_EXT_FEATURE_SEL_TARGET_TEMPERATURE_MINIMUM": c_uint(18),    #Allows to set minimum limit of target temperature.
    "XI_EXT_FEATURE_SEL_PRINT_STATUS_TO_DEBUG": c_uint(19),    #Prints to debug status information on some scientific cameras e.g. MJ (tepmerature, consumption and other parameters).
    "XI_EXT_FEATURE_SEL_BUFFER_HEADER_CHECK_ENABLED": c_uint(20),    #Input buffer integrity check.
    "XI_EXT_FEATURE_SEL_TRANSPORT_IMAGE_BUFFER_SIZE_ALIGNMENT_BYTES": c_uint(21),    #Size automatic aligment for transport image buffer size to be automatically aligned. E.g. 65536 for 64KiB, useful if storing (e.g. to SSD) requires larger sizes, than image payload. Default is 4096 bytes.
    "XI_EXT_FEATURE_SEL_PROCESSING_IBD_CROP_MSB_EN": c_uint(22),    #Crop MSB bits of resizer IP data output (if enabled output data format bitdepth of resize IP equal to input format)
    "XI_EXT_FEATURE_API_SETPARAM_CALLS_LOG_ENABLED": c_uint(23),    #Allows enabling of xiSetParam calls to be logged automatically to file xiapi_call_log.txt
    }

#Device unit selector
XI_DEVICE_UNIT_SELECTOR = { 
    "XI_DEVICE_UNIT_SENSOR1": c_uint(0),    #Selects first sensor on device
    "XI_DEVICE_UNIT_FPGA1": c_uint(1),    #Selects first FPGA on device
    "XI_DEVICE_UNIT_SAL": c_uint(2),    #Selects sensor abstraction layer
    "XI_DEVICE_UNIT_DAL": c_uint(3),    #Selects driver abstraction layer
    "XI_DEVICE_UNIT_SCM": c_uint(4),    #Selects sensor correction module
    "XI_DEVICE_UNIT_FGENTL": c_uint(5),    #Selects register in underlying GenTL layer
    "XI_DEVICE_UNIT_MCU1": c_uint(6),    #Selects first MCU on device
    "XI_DEVICE_UNIT_MCU2": c_uint(7),    #Selects second MCU on device
    "XI_DEVICE_UNIT_CHF": c_uint(8),    #Selects Camera High Features Model
    "XI_DEVICE_UNIT_CALIB_DATA": c_uint(9),    #Selects calibration data structure
    "XI_DEVICE_UNIT_XIFAPI_ACQ": c_uint(10),    #Selects acquisition unit
    "XI_DEVICE_UNIT_I2C_BUS": c_uint(11),    #Selects i2c bus
    }

#Device unit register type
XI_DEVICE_UNIT_REGISTER_TYPE = { 
    "XI_REGISTER_TYPE_UNKNOWN": c_uint(0),    #Unknown register type
    "XI_REGISTER_TYPE_INT": c_uint(1),    #Integer value register type
    "XI_REGISTER_TYPE_INT100X": c_uint(2),    #Integer value register type which is a 100 fold of the target value
    "XI_REGISTER_TYPE_INT1000X": c_uint(3),    #Integer value register type which is a 1000 fold of the target value
    "XI_REGISTER_TYPE_INT32768X": c_uint(4),    #Integer value register type which is a 32768 fold of the target value
    "XI_REGISTER_TYPE_INT_HEX": c_uint(5),    #Integer value register type represented in hex format
    "XI_REGISTER_TYPE_ENUM_INT1000x": c_uint(6),    #Enumerated value register type representing 1000 fold of gain values
    "XI_REGISTER_TYPE_ENUM_RAW": c_uint(7),    #Enumerated value register type representing raw gain value stored in EEPROM
    }

#Device unit register
XI_DEVICE_UNIT_MCU1_REGS = { 
    "XI_DEVICE_UNIT_MCU1_ACQ_RESTARTED": c_uint(0),    #How many times acquisition was restarted, due to errors.
    }

#Device unit register
XI_DEVICE_UNIT_I2C_BUS_REGS = { 
    "XI_DEVICE_UNIT_I2C_BUS_SLAVE_ADDR": c_uint(0),    #i2c bus slave address.
    "XI_DEVICE_UNIT_I2C_BUS_SLAVE_SUB_ADDR_LEN": c_uint(1),    #i2c bus slave sub address lenght in bytes.
    "XI_DEVICE_UNIT_I2C_BUS_SLAVE_SUB_ADDR": c_uint(2),    #i2c bus slave sub address (LSB first).
    "XI_DEVICE_UNIT_I2C_BUS_SLAVE_DATA_LEN": c_uint(3),    #i2c bus data read/write lenght.
    "XI_DEVICE_UNIT_I2C_BUS_SLAVE_DATA": c_uint(4),    #i2c bus data read/write. Transaction will be executed on last byte written or first byte read.
    }

#Device unit register type
XI_DEVICE_UNIT_XIFAPI_ACQ_REGS = { 
    "XI_DEVICE_UNIT_XIFAPI_ACQ_LOOP_TIME_ENABLE": c_uint(0),    #Enables calculation of worker loops time
    "XI_DEVICE_UNIT_XIFAPI_ACQ_RESET": c_uint(1),    #Resets all calculated,cached data
    "XI_DEVICE_UNIT_XIFAPI_ACQ_LOOP_TIME": c_uint(2),    #Selects register of one worker loop time
    "XI_DEVICE_UNIT_XIFAPI_ACQ_LOOPS_TIME": c_uint(3),    #Selects register of worker loops time(since last reset)
    "XI_DEVICE_UNIT_XIFAPI_ACQ_LOOPS": c_uint(4),    #Selects register of worker number of loops(since last reset)
    "XI_DEVICE_UNIT_XIFAPI_WORKER_INTERRUPT_SIM_TIME": c_uint(5),    #Simulates waorker interruption (in ms)
    }

#Camera sensor mode enumerator.
XI_SENSOR_MODE = { 
    "XI_SENS_MD0": c_uint(0),    #Sensor mode number 0
    "XI_SENS_MD1": c_uint(1),    #Sensor mode number 1
    "XI_SENS_MD2": c_uint(2),    #Sensor mode number 2
    "XI_SENS_MD3": c_uint(3),    #Sensor mode number 3
    "XI_SENS_MD4": c_uint(4),    #Sensor mode number 4
    "XI_SENS_MD5": c_uint(5),    #Sensor mode number 5
    "XI_SENS_MD6": c_uint(6),    #Sensor mode number 6
    "XI_SENS_MD7": c_uint(7),    #Sensor mode number 7
    "XI_SENS_MD8": c_uint(8),    #Sensor mode number 8
    "XI_SENS_MD9": c_uint(9),    #Sensor mode number 9
    "XI_SENS_MD10": c_uint(10),    #Sensor mode number 10
    "XI_SENS_MD11": c_uint(11),    #Sensor mode number 11
    "XI_SENS_MD12": c_uint(12),    #Sensor mode number 12
    "XI_SENS_MD13": c_uint(13),    #Sensor mode number 13
    "XI_SENS_MD14": c_uint(14),    #Sensor mode number 14
    "XI_SENS_MD15": c_uint(15),    #Sensor mode number 15
    }

#Defines image sensor area as output.
XI_IMAGE_AREA_SELECTOR = { 
    "XI_IMAGE_AREA_ACTIVE": c_uint(0),    #All light sensitive pixels suggested by image vendor.
    "XI_IMAGE_AREA_ACTIVE_AND_MASKED": c_uint(1),    #All Active pixels plus masked pixels surrounding the Active area.
    }

#Camera channel count enumerator.
XI_SENSOR_OUTPUT_CHANNEL_COUNT = { 
    "XI_CHANN_CNT2": c_uint(2),    #2 sensor readout channels.
    "XI_CHANN_CNT4": c_uint(4),    #4 sensor readout channels.
    "XI_CHANN_CNT8": c_uint(8),    #8 sensor readout channels.
    "XI_CHANN_CNT16": c_uint(16),    #16 sensor readout channels.
    "XI_CHANN_CNT24": c_uint(24),    #24 sensor readout channels.
    "XI_CHANN_CNT32": c_uint(32),    #32 sensor readout channels.
    "XI_CHANN_CNT48": c_uint(48),    #48 sensor readout channels.
    }

#Sensor defects correction list selector
XI_SENS_DEFFECTS_CORR_LIST_SELECTOR = { 
    "XI_SENS_DEFFECTS_CORR_LIST_SEL_FACTORY": c_uint(0),    #Factory defect correction list
    "XI_SENS_DEFFECTS_CORR_LIST_SEL_USER0": c_uint(1),    #User defect correction list
    "XI_SENS_DEFFECTS_CORR_LIST_SEL_IN_CAMERA": c_uint(2),    #Device specific defect correction list
    }

#Acquisition status Selector
XI_ACQUISITION_STATUS_SELECTOR = { 
    "XI_ACQUISITION_STATUS_ACQ_ACTIVE": c_uint(0),    # Device is currently doing an acquisition of one or many frames.
    }

#Select unit where data-pipe is configured
XI_DP_UNIT_SELECTOR = { 
    "XI_DP_UNIT_SENSOR": c_uint(0),    #Selects device image sensor
    "XI_DP_UNIT_FPGA": c_uint(1),    #Selects device image FPGA
    }

#Select unit processor
XI_DP_PROC_SELECTOR = { 
    "XI_DP_PROC_NONE": c_uint(0),    #Default empty processor
    "XI_DP_PROC_CHANNEL_MUXER": c_uint(1),    #Channel Muxer (selected processor combines multiple input channels)
    "XI_DP_PROC_PIXEL_SEQUENCER": c_uint(2),    #Selects pixel data output sequence
    "XI_DP_PROC_CHANNEL_1": c_uint(3),    #Selects sensor output channel 1
    "XI_DP_PROC_CHANNEL_2": c_uint(4),    #Selects sensor output channel 2
    "XI_DP_PROC_FRAME_BUFFER": c_uint(5),    #Selects frame buffer memory
    }

#Select processor parameter
XI_DP_PARAM_SELECTOR = { 
    "XI_DP_PARAM_NONE": c_uint(0),    #Empty parameter
    "XI_DP_PARAM_CHMUX_CHANNEL_SELECTOR": c_uint(1),    #Defines output of Channel Muxer processor
    "XI_DP_PARAM_CHMUX_ALPHA": c_uint(2),    #Channel merger coefficient Alpha
    "XI_DP_PARAM_CHMUX_BETA": c_uint(3),    #Channel merger coefficient Beta
    "XI_DP_PARAM_PIXSEQ_SELECTOR": c_uint(4),    #PixSeq Selector
    "XI_DP_PARAM_CHANNEL_TIMING": c_uint(5),    #Selected channel timing
    "XI_DP_PARAM_FRAMEBUF_MODE": c_uint(6),    #Frame Buffer Mode
    "XI_DP_PARAM_FRAMEBUF_SIZE": c_uint(7),    #Frame Buffer Size Bytes
    }

#Select processor parameter value
XI_DP_PARAM_VALUE = { 
    "XI_DP_PARAM_VALUE_CHMUX_CHANNEL_1": c_uint(0),    #Selected source channel 1
    "XI_DP_PARAM_VALUE_CHMUX_CHANNEL_2": c_uint(1),    #Selected source channel 2
    "XI_DP_PARAM_VALUE_CHMUX_CHANNEL_1_2": c_uint(2),    #Selected source channel 1 and 2
    "XI_DP_PARAM_VALUE_CHMUX_MERGED": c_uint(3),    #Merged data of two channels
    "XI_DP_PARAM_VALUE_CHMUX_CMS_S": c_uint(4),    #Correlated Multiple Sampling(summing)
    "XI_DP_PARAM_VALUE_PIXSEQ_ONE_VALUE": c_uint(5),    #Output is one value per pixel
    "XI_DP_PARAM_VALUE_PIXSEQ_TWO_VALUES": c_uint(6),    #Output are two values per pixel
    "XI_DP_PARAM_VALUE_CHTIM_HG": c_uint(7),    #High Gain channel timing
    "XI_DP_PARAM_VALUE_CHTIM_LG": c_uint(8),    #Low Gain channel timing
    "XI_DP_PARAM_VALUE_FRAMEBUF_MODE_DISABLED": c_uint(9),    #Frame buffer is disabled
    "XI_DP_PARAM_VALUE_FRAMEBUF_MODE_ENABLED": c_uint(10),    #Frame buffer is on
    "XI_DP_PARAM_VALUE_PIXSEQ_FOUR_VALUES": c_uint(11),    #Output are four values per pixel
    "XI_DP_PARAM_VALUE_CHMUX_CMS_A": c_uint(12),    #Correlated Multiple Sampling(averaging)
    }

#User Set selector options.
XI_USER_SET_SELECTOR = { 
    "XI_US_12_STD_L": c_uint(10),    #12bit per channel STD Low Gain mode preset.
    "XI_US_12_STD_H": c_uint(11),    #12bit per channel STD High Gain mode preset.
    "XI_US_14_STD_L": c_uint(12),    #14bit per channel STD Low Gain mode preset.
    "XI_US_NONE": c_uint(999),    #No preset selected.
    "XI_US_14_STD_H": c_uint(13),    #14bit per channel STD High Gain mode preset.
    "XI_US_2_12_CMS_S_L": c_uint(14),    #12bit per channel, 2 samples,  CMS(summing) Low Gain mode preset.
    "XI_US_2_12_CMS_S_H": c_uint(15),    #12bit per channel, 2 samples,  CMS(summing) High Gain mode preset.
    "XI_US_2_14_CMS_S_L": c_uint(16),    #14bit per channel, 2 samples,  CMS(summing) Low Gain mode preset.
    "XI_US_2_14_CMS_S_H": c_uint(17),    #14bit per channel, 2 samples,  CMS(summing) High Gain mode preset.
    "XI_US_4_12_CMS_S_L": c_uint(18),    #12bit per channel, 4 samples,  CMS(summing) Low Gain mode preset.
    "XI_US_4_12_CMS_S_H": c_uint(19),    #12bit per channel, 4 samples,  CMS(summing) High Gain mode preset.
    "XI_US_4_14_CMS_S_L": c_uint(20),    #14bit per channel, 4 samples,  CMS(summing) Low Gain mode preset.
    "XI_US_4_14_CMS_S_H": c_uint(21),    #14bit per channel, 4 samples,  CMS(summing) High Gain mode preset.
    "XI_US_2_12_HDR_HL": c_uint(22),    #12bit per channel, 2 samples,  HDR High Low Gain mode preset.
    "XI_US_2_12_HDR_L": c_uint(23),    #12bit per channel, 2 samples,  HDR Low Gain mode preset.
    "XI_US_2_12_HDR_H": c_uint(24),    #12bit per channel, 2 samples,  HDR High Gain mode preset.
    "XI_US_4_12_CMS_HDR_HL": c_uint(25),    #12bit per channel, 4 samples,  CMS + HDR High Low Gain mode preset.
    "XI_US_2_14_HDR_L": c_uint(26),    #14bit per channel, 2 samples,  HDR Low Gain mode preset.
    "XI_US_2_14_HDR_H": c_uint(27),    #14bit per channel, 2 samples,  HDR High Gain mode preset.
    "XI_US_2_12_CMS_A_L": c_uint(28),    #12bit per channel, 2 samples,  CMS(averaging) Low Gain mode preset.
    "XI_US_2_12_CMS_A_H": c_uint(29),    #12bit per channel, 2 samples,  CMS(averaging) High Gain mode preset.
    "XI_US_TOF_DEFAULT": c_uint(30),    #
    "XI_US_TOF_LONG_RANGE": c_uint(31),    #
    "XI_US_TOF_SINGLE_READOUT": c_uint(32),    #
    "XI_US_TOF_DEFAULT_EXT_CDS": c_uint(33),    #
    "XI_US_TOF_LONG_RANGE_EXT_CDS": c_uint(34),    #
    "XI_US_TOF_SINGLE_READOUT_EXT_CDS": c_uint(35),    #
    }

#Mode of DualADC feature
XI_DUAL_ADC_MODE = { 
    "XI_DUAL_ADC_MODE_OFF": c_uint(0),    #Disable DualADC feature
    "XI_DUAL_ADC_MODE_COMBINED": c_uint(1),    #Set Combined mode
    "XI_DUAL_ADC_MODE_NON_COMBINED": c_uint(2),    #Set NonCombined mode
    }

#Probe Selector
XI_PROBE_SELECTOR = { 
    "XI_PROBE_SELECTOR_CURRENT_MAINBOARD_VCC_IN": c_uint(0),    #Current probe on Main Board at VCC_IN power supply
    "XI_PROBE_SELECTOR_VOLTAGE_MAINBOARD_VCC_IN": c_uint(1),    #Voltage probe on Main Board at VCC_IN power supply
    "XI_PROBE_SELECTOR_CURRENT_MAINBOARD_VCC_ADJ2": c_uint(2),    #Current probe on Main Board at VCC_ADJ2 power supply
    "XI_PROBE_SELECTOR_VOLTAGE_MAINBOARD_VCC_ADJ2": c_uint(3),    #Voltage probe on Main Board at VCC_ADJ2 power supply
    "XI_PROBE_SELECTOR_CURRENT_MAINBOARD_VCC_ADJ1": c_uint(4),    #Current probe on Main Board at VCC_ADJ1 power supply
    "XI_PROBE_SELECTOR_VOLTAGE_MAINBOARD_VCC_ADJ1": c_uint(5),    #Voltage probe on Main Board at VCC_ADJ1 power supply
    "XI_PROBE_SELECTOR_CURRENT_MAINBOARD_VCC_PLT": c_uint(6),    #Current probe on Main Board at VCC_PLT power supply
    "XI_PROBE_SELECTOR_VOLTAGE_MAINBOARD_VCC_PLT": c_uint(7),    #Voltage probe on Main Board at VCC_PLT power supply
    "XI_PROBE_SELECTOR_VOLTAGE_SENSORBOARD_VCC_ADJ1": c_uint(8),    #Voltage probe on Sensor Board at VCC_ADJ1
    "XI_PROBE_SELECTOR_VOLTAGE_SENSORBOARD_VCC_ADJ2": c_uint(9),    #Voltage probe on Sensor Board at VCC_ADJ2
    "XI_PROBE_SELECTOR_VOLTAGE_SENSORBOARD_VCC_5V0": c_uint(10),    #Voltage probe on Sensor Board at VCC_5V0
    "XI_PROBE_SELECTOR_VOLTAGE_SENSORBOARD_VCC_3V3": c_uint(11),    #Voltage probe on Sensor Board at VCC_3V3
    "XI_PROBE_SELECTOR_VOLTAGE_DATA_CON_INPUT": c_uint(12),    #Voltage probe on device input, if device is bus powered
    "XI_PROBE_SELECTOR_VOLTAGE_PELTIER1": c_uint(13),    #Voltage probe on peltier #1
    "XI_PROBE_SELECTOR_CURRENT_PELTIER1": c_uint(14),    #Current probe on peltier #1
    "XI_PROBE_SELECTOR_VOLTAGE_PELTIER2": c_uint(15),    #Voltage probe on peltier #2
    "XI_PROBE_SELECTOR_CURRENT_PELTIER2": c_uint(16),    #Current probe on peltier #2
    "XI_PROBE_SELECTOR_VOLTAGE_MAINBOARD_VCC_5V0": c_uint(17),    #Voltage probe on Main Board at VCC_5V0 power supply 
    "XI_PROBE_SELECTOR_CURRENT_MAINBOARD_VCC_5V0": c_uint(18),    #Current probe on Main Board at VCC_5V0 power supply
    }

#ToF Readout Mode feature
XI_TOF_READOUT_MODE = { 
    "XI_TOF_READOUT_MODE_A_ONLY": c_uint(0),    #A Only readout mode
    "XI_TOF_READOUT_MODE_B_ONLY": c_uint(1),    #B Only readout mode
    "XI_TOF_READOUT_MODE_A_MINUS_B": c_uint(2),    #A Minus B readout mode
    "XI_TOF_READOUT_MODE_A_PLUS_B": c_uint(3),    #A Plus B readout mode
    "XI_TOF_READOUT_MODE_A_AND_B": c_uint(4),    #A And B readout mode
    }

#Image data signedness
XI_DATA_SM = { 
    "XI_DATA_SM_UNSIGNED": c_uint(0),    #Unsigned if it can only represent non-negative numbers (zero or positive numbers).
    "XI_DATA_SM_SIGNED_2C": c_uint(1),    #Signed if it can represent both positive and negative numbers (two's complement).
    "XI_DATA_SM_SIGNED_FLOATING": c_uint(2),    #Signed floating point data type.
    }


XI_GenTL_Image_Format_e = { 
    "XI_GenTL_Image_Format_Mono8": c_uint(0x01080001),    
    }
	
# Parameters

XI_PRM_EXPOSURE = "exposure"    #Exposure time in microseconds
XI_PRM_EXPOSURE_TIME_SELECTOR = "exposure_time_selector"    #Selector for Exposure parameter
XI_PRM_EXPOSURE_BURST_COUNT = "exposure_burst_count"    #Sets the number of times of exposure in one frame.
XI_PRM_GAIN_SELECTOR = "gain_selector"    #Gain selector for parameter Gain allows to select different type of gains.
XI_PRM_GAIN = "gain"    #Gain in dB
XI_PRM_DOWNSAMPLING = "downsampling"    #Change image resolution by binning or skipping.
XI_PRM_DOWNSAMPLING_TYPE = "downsampling_type"    #Change image downsampling type.
XI_PRM_TEST_PATTERN_GENERATOR_SELECTOR = "test_pattern_generator_selector"    #Selects which test pattern generator is controlled by the test pattern feature.
XI_PRM_TEST_PATTERN = "test_pattern"    #Selects which test pattern type is generated by the selected generator.
XI_PRM_IMAGE_DATA_FORMAT = "imgdataformat"    #Output data format.
XI_PRM_IMAGE_DATA_SIGN = "image_data_sign"    #Signedness of image data.
XI_PRM_SHUTTER_TYPE = "shutter_type"    #Change sensor shutter type(CMOS sensor).
XI_PRM_SENSOR_TAPS = "sensor_taps"    #Number of taps
XI_PRM_AEAG = "aeag"    #Automatic exposure/gain
XI_PRM_AEAG_ROI_OFFSET_X = "aeag_roi_offset_x"    #Automatic exposure/gain ROI offset X
XI_PRM_AEAG_ROI_OFFSET_Y = "aeag_roi_offset_y"    #Automatic exposure/gain ROI offset Y
XI_PRM_AEAG_ROI_WIDTH = "aeag_roi_width"    #Automatic exposure/gain ROI Width
XI_PRM_AEAG_ROI_HEIGHT = "aeag_roi_height"    #Automatic exposure/gain ROI Height
XI_PRM_SENS_DEFECTS_CORR_LIST_SELECTOR = "bpc_list_selector"    #Selector of list used by Sensor Defects Correction parameter
XI_PRM_SENS_DEFECTS_CORR_LIST_CONTENT = "sens_defects_corr_list_content"    #Sets/Gets sensor defects list in special text format
XI_PRM_SENS_DEFECTS_CORR = "bpc"    #Correction of sensor defects (pixels, columns, rows) enable/disable
XI_PRM_AUTO_WB = "auto_wb"    #Automatic white balance
XI_PRM_MANUAL_WB = "manual_wb"    #Calculates White Balance(xiGetImage function must be called)
XI_PRM_WB_ROI_OFFSET_X = "wb_roi_offset_x"    #White balance offset X
XI_PRM_WB_ROI_OFFSET_Y = "wb_roi_offset_y"    #White balance offset Y
XI_PRM_WB_ROI_WIDTH = "wb_roi_width"    #White balance width
XI_PRM_WB_ROI_HEIGHT = "wb_roi_height"    #White balance height
XI_PRM_WB_KR = "wb_kr"    #White balance red coefficient
XI_PRM_WB_KG = "wb_kg"    #White balance green coefficient
XI_PRM_WB_KB = "wb_kb"    #White balance blue coefficient
XI_PRM_WIDTH = "width"    #Width of the Image provided by the device (in pixels).
XI_PRM_HEIGHT = "height"    #Height of the Image provided by the device (in pixels).
XI_PRM_OFFSET_X = "offsetX"    #Horizontal offset from the origin to the area of interest (in pixels).
XI_PRM_OFFSET_Y = "offsetY"    #Vertical offset from the origin to the area of interest (in pixels).
XI_PRM_REGION_SELECTOR = "region_selector"    #Selects Region in Multiple ROI which parameters are set by width, height, ... ,region mode
XI_PRM_REGION_MODE = "region_mode"    #Activates/deactivates Region selected by Region Selector
XI_PRM_HORIZONTAL_FLIP = "horizontal_flip"    #Horizontal flip enable
XI_PRM_VERTICAL_FLIP = "vertical_flip"    #Vertical flip enable
XI_PRM_INTERLINE_EXPOSURE_MODE = "interline_exposure_mode"    #Selector for Exposure parameter
XI_PRM_FFC = "ffc"    #Image flat field correction
XI_PRM_FFC_FLAT_FIELD_FILE_NAME = "ffc_flat_field_file_name"    #Set name of file to be applied for FFC processor.
XI_PRM_FFC_DARK_FIELD_FILE_NAME = "ffc_dark_field_file_name"    #Set name of file to be applied for FFC processor.
XI_PRM_TOF_READOUT_MODE = "tof_readout_mode"    #Sets ToF Readout Mode
XI_PRM_TOF_MODULATION_FREQUENCY = "tof_modulation_frequency"    #Sets ToF Modulation Frequency in MHz
XI_PRM_TOF_MULTIPLE_PHASES_IN_BUFFER = "tof_multiple_phases_in_buffer"    #is multiple ToF phases concatenated in buffer
XI_PRM_TOF_PHASES_COUNT = "tof_phases_count"    #Sets the number of tof phases. E.g. 4 for four phases.
XI_PRM_TOF_PHASE_ANGLE = "tof_phase_angle"    #Sets Illumination angle for selected ToF phase
XI_PRM_TOF_PHASE_EXPOSURE_TIME = "tof_phase_exposure_time"    #Sets Exposure time for selected ToF phase in microseconds.
XI_PRM_TOF_PHASE_SELECTOR = "tof_phase_selector"    #Selects tof phase
XI_PRM_BINNING_SELECTOR = "binning_selector"    #Binning engine selector.
XI_PRM_BINNING_VERTICAL_MODE = "binning_vertical_mode"    #Sets the mode to use to combine vertical pixel together.
XI_PRM_BINNING_VERTICAL = "binning_vertical"    #Vertical Binning - number of vertical photo-sensitive cells to combine together.
XI_PRM_BINNING_VERTICAL_FLOAT = "binning_vertical_float"    #Vertical Binning Float - number of vertical photo-sensitive cells to combine together.
XI_PRM_BINNING_HORIZONTAL_MODE = "binning_horizontal_mode"    #Sets the mode to use to combine horizontal pixel together.
XI_PRM_BINNING_HORIZONTAL = "binning_horizontal"    #Horizontal Binning - number of horizontal photo-sensitive cells to combine together.
XI_PRM_BINNING_HORIZONTAL_FLOAT = "binning_horizontal_float"    #Horizontal Binning Float - number of horizontal photo-sensitive cells to combine together.
XI_PRM_BINNING_HORIZONTAL_PATTERN = "binning_horizontal_pattern"    #Binning horizontal pattern type.
XI_PRM_BINNING_VERTICAL_PATTERN = "binning_vertical_pattern"    #Binning vertical pattern type.
XI_PRM_DECIMATION_SELECTOR = "decimation_selector"    #Decimation engine selector.
XI_PRM_DECIMATION_VERTICAL = "decimation_vertical"    #Vertical Decimation - vertical sub-sampling of the image - reduces the vertical resolution of the image by the specified vertical decimation factor.
XI_PRM_DECIMATION_HORIZONTAL = "decimation_horizontal"    #Horizontal Decimation - horizontal sub-sampling of the image - reduces the horizontal resolution of the image by the specified vertical decimation factor.
XI_PRM_DECIMATION_HORIZONTAL_PATTERN = "decimation_horizontal_pattern"    #Decimation horizontal pattern type.
XI_PRM_DECIMATION_VERTICAL_PATTERN = "decimation_vertical_pattern"    #Decimation vertical pattern type.
XI_PRM_EXP_PRIORITY = "exp_priority"    #Exposure priority (0.8 - exposure 80%, gain 20%).
XI_PRM_AG_MAX_LIMIT = "ag_max_limit"    #Maximum limit of gain in AEAG procedure
XI_PRM_AE_MAX_LIMIT = "ae_max_limit"    #Maximum time (us) used for exposure in AEAG procedure
XI_PRM_AEAG_LEVEL = "aeag_level"    #Average intensity of output signal AEAG should achieve(in %)
XI_PRM_AEAG_SKIP_FRAMES_COUNT = "aeag_skip_frames_count"    #Minimum number of frames to be skipped between AEAG compensation and AEAG re-evaluation
XI_PRM_LIMIT_BANDWIDTH = "limit_bandwidth"    #Set/get bandwidth(data rate in Megabits)
XI_PRM_LIMIT_BANDWIDTH_MODE = "limit_bandwidth_mode"    #Bandwidth limit enabled
XI_PRM_SENSOR_LINE_PERIOD = "sensor_line_period"    #Image sensor line period in us
XI_PRM_SENSOR_DATA_BIT_DEPTH = "sensor_bit_depth"    #Sensor output data bit depth.
XI_PRM_OUTPUT_DATA_BIT_DEPTH = "output_bit_depth"    #Device output data bit depth.
XI_PRM_IMAGE_DATA_BIT_DEPTH = "image_data_bit_depth"    #bit depth of data returned by function xiGetImage
XI_PRM_OUTPUT_DATA_PACKING = "output_bit_packing"    #Device output data packing (or grouping) enabled. Packing could be enabled if output_data_bit_depth > 8 and packing capability is available.
XI_PRM_OUTPUT_DATA_PACKING_TYPE = "output_bit_packing_type"    #Data packing type. Some cameras supports only specific packing type.
XI_PRM_IS_COOLED = "iscooled"    #Returns 1 for cameras that support cooling.
XI_PRM_COOLING = "cooling"    #Temperature control mode.
XI_PRM_TARGET_TEMP = "target_temp"    #Set sensor target temperature for cooling.
XI_PRM_TEMP_SELECTOR = "temp_selector"    #Selector of mechanical point where thermometer is located.
XI_PRM_TEMP = "temp"    #Camera temperature (selected by XI_PRM_TEMP_SELECTOR)
XI_PRM_TEMP_CONTROL_MODE = "device_temperature_ctrl_mode"    #Temperature control mode.
XI_PRM_CHIP_TEMP = "chip_temp"    #Camera sensor temperature
XI_PRM_HOUS_TEMP = "hous_temp"    #Camera housing temperature
XI_PRM_HOUS_BACK_SIDE_TEMP = "hous_back_side_temp"    #Camera housing back side temperature
XI_PRM_SENSOR_BOARD_TEMP = "sensor_board_temp"    #Camera sensor board temperature
XI_PRM_TEMP_ELEMENT_SEL = "device_temperature_element_sel"    #Temperature element selector (TEC(Peltier), Fan).
XI_PRM_TEMP_ELEMENT_VALUE = "device_temperature_element_val"    #Temperature element value in percents of full control range
XI_PRM_CMS = "cms"    #Mode of color management system.
XI_PRM_CMS_INTENT = "cms_intent"    #Intent of color management system.
XI_PRM_APPLY_CMS = "apply_cms"    #Enable applying of CMS profiles to xiGetImage (see XI_PRM_INPUT_CMS_PROFILE, XI_PRM_OUTPUT_CMS_PROFILE).
XI_PRM_INPUT_CMS_PROFILE = "input_cms_profile"    #Filename for input cms profile (e.g. input.icc)
XI_PRM_OUTPUT_CMS_PROFILE = "output_cms_profile"    #Filename for output cms profile (e.g. input.icc)
XI_PRM_IMAGE_IS_COLOR = "iscolor"    #Returns 1 for color cameras.
XI_PRM_COLOR_FILTER_ARRAY = "cfa"    #Returns color filter array type of RAW data.
XI_PRM_GAMMAY = "gammaY"    #Luminosity gamma
XI_PRM_GAMMAC = "gammaC"    #Chromaticity gamma
XI_PRM_SHARPNESS = "sharpness"    #Sharpness strength
XI_PRM_CC_MATRIX_00 = "ccMTX00"    #Color Correction Matrix element [0][0]
XI_PRM_CC_MATRIX_01 = "ccMTX01"    #Color Correction Matrix element [0][1]
XI_PRM_CC_MATRIX_02 = "ccMTX02"    #Color Correction Matrix element [0][2]
XI_PRM_CC_MATRIX_03 = "ccMTX03"    #Color Correction Matrix element [0][3]
XI_PRM_CC_MATRIX_10 = "ccMTX10"    #Color Correction Matrix element [1][0]
XI_PRM_CC_MATRIX_11 = "ccMTX11"    #Color Correction Matrix element [1][1]
XI_PRM_CC_MATRIX_12 = "ccMTX12"    #Color Correction Matrix element [1][2]
XI_PRM_CC_MATRIX_13 = "ccMTX13"    #Color Correction Matrix element [1][3]
XI_PRM_CC_MATRIX_20 = "ccMTX20"    #Color Correction Matrix element [2][0]
XI_PRM_CC_MATRIX_21 = "ccMTX21"    #Color Correction Matrix element [2][1]
XI_PRM_CC_MATRIX_22 = "ccMTX22"    #Color Correction Matrix element [2][2]
XI_PRM_CC_MATRIX_23 = "ccMTX23"    #Color Correction Matrix element [2][3]
XI_PRM_CC_MATRIX_30 = "ccMTX30"    #Color Correction Matrix element [3][0]
XI_PRM_CC_MATRIX_31 = "ccMTX31"    #Color Correction Matrix element [3][1]
XI_PRM_CC_MATRIX_32 = "ccMTX32"    #Color Correction Matrix element [3][2]
XI_PRM_CC_MATRIX_33 = "ccMTX33"    #Color Correction Matrix element [3][3]
XI_PRM_DEFAULT_CC_MATRIX = "defccMTX"    #Set default Color Correction Matrix
XI_PRM_CC_MATRIX_NORM = "ccMTXnorm"    #Normalize color correction matrix
XI_PRM_TRG_SOURCE = "trigger_source"    #Defines source of trigger.
XI_PRM_TRG_SOFTWARE = "trigger_software"    #Generates an internal trigger. XI_PRM_TRG_SOURCE must be set to TRG_SOFTWARE.
XI_PRM_TRG_SELECTOR = "trigger_selector"    #Selects the type of trigger.
XI_PRM_TRG_OVERLAP = "trigger_overlap"    #The mode of Trigger Overlap. This influences of trigger acception/rejection policy
XI_PRM_ACQ_FRAME_BURST_COUNT = "acq_frame_burst_count"    #Sets number of frames acquired by burst. This burst is used only if trigger is set to FrameBurstStart
XI_PRM_TIMESTAMP = "timestamp"    #Current value of the device timestamp counter
XI_PRM_GPI_SELECTOR = "gpi_selector"    #Selects GPI
XI_PRM_GPI_MODE = "gpi_mode"    #Defines GPI functionality
XI_PRM_GPI_LEVEL = "gpi_level"    #GPI level
XI_PRM_GPI_LEVEL_AT_IMAGE_EXP_START = "gpi_level_at_image_exp_start"    #GPI Level at image exposure start
XI_PRM_GPI_LEVEL_AT_IMAGE_EXP_END = "gpi_level_at_image_exp_end"    #GPI Level at image exposure end
XI_PRM_GPI_LEVEL_AT_IMAGE_READOUT_END = "gpi_level_at_image_readout_end"    #GPI Level at image readout end
XI_PRM_GPO_SELECTOR = "gpo_selector"    #Selects GPO
XI_PRM_GPO_MODE = "gpo_mode"    #Defines GPO functionality
XI_PRM_METADATA_SAMPLING_MODE = "metadata_sampling_mode"    #Defines metadata sampling mode
XI_PRM_LED_SELECTOR = "led_selector"    #Selects LED
XI_PRM_LED_MODE = "led_mode"    #Defines LED functionality
XI_PRM_DEBOUNCE_EN = "dbnc_en"    #Enable/Disable debounce to selected GPI
XI_PRM_DEBOUNCE_T0 = "dbnc_t0"    #Debounce time (x * 10us)
XI_PRM_DEBOUNCE_T1 = "dbnc_t1"    #Debounce time (x * 10us)
XI_PRM_DEBOUNCE_POL = "dbnc_pol"    #Debounce polarity (pol = 1 t0 - falling edge, t1 - rising edge)
XI_PRM_LENS_MODE = "lens_mode"    #Status of lens control interface. This shall be set to XI_ON before any Lens operations.
XI_PRM_LENS_APERTURE_VALUE = "lens_aperture_value"    #Current lens aperture value in stops. Examples: 2.8, 4, 5.6, 8, 11
XI_PRM_LENS_APERTURE_INDEX = "lens_aperture_index"    #Current aperture index as reported by lens.
XI_PRM_LENS_FOCUS_MOVEMENT_VALUE = "lens_focus_movement_value"    #Lens current focus movement value to be used by XI_PRM_LENS_FOCUS_MOVE in motor steps.
XI_PRM_LENS_FOCUS_MOVE = "lens_focus_move"    #Moves lens focus motor by steps set in XI_PRM_LENS_FOCUS_MOVEMENT_VALUE.
XI_PRM_LENS_FOCUS_DISTANCE = "lens_focus_distance"    #(Planned feature Issue#6958). Lens focus distance in cm.
XI_PRM_LENS_FOCAL_LENGTH = "lens_focal_length"    #Lens focal distance in mm.
XI_PRM_LENS_FEATURE_SELECTOR = "lens_feature_selector"    #Selects the current feature which is accessible by XI_PRM_LENS_FEATURE.
XI_PRM_LENS_FEATURE = "lens_feature"    #Allows access to lens feature value currently selected by XI_PRM_LENS_FEATURE_SELECTOR.
XI_PRM_LENS_COMM_DATA = "lens_comm_data"    #Write/Read data sequences to/from lens
XI_PRM_DEVICE_NAME = "device_name"    #Return device name
XI_PRM_DEVICE_TYPE = "device_type"    #Return device type
XI_PRM_DEVICE_MODEL_ID = "device_model_id"    #Return device model id
XI_PRM_SENSOR_MODEL_ID = "sensor_model_id"    #Return device sensor model id
XI_PRM_DEVICE_SN = "device_sn"    #Return device serial number
XI_PRM_DEVICE_SENS_SN = "device_sens_sn"    #Return sensor serial number
XI_PRM_DEVICE_ID = "device_id"    #Return unique device ID
XI_PRM_DEVICE_INSTANCE_PATH = "device_inst_path"    #Return device system instance path.
XI_PRM_DEVICE_LOCATION_PATH = "device_loc_path"    #Represents the location of the device in the device tree.
XI_PRM_DEVICE_USER_ID = "device_user_id"    #Return custom ID of camera.
XI_PRM_DEVICE_MANIFEST = "device_manifest"    #Return device capability description XML.
XI_PRM_IMAGE_USER_DATA = "image_user_data"    #User image data at image header to track parameters synchronization.
XI_PRM_IMAGE_DATA_FORMAT_RGB32_ALPHA = "imgdataformatrgb32alpha"    #The alpha channel of RGB32 output image format.
XI_PRM_IMAGE_PAYLOAD_SIZE = "imgpayloadsize"    #Buffer size in bytes sufficient for output image returned by xiGetImage
XI_PRM_TRANSPORT_PIXEL_FORMAT = "transport_pixel_format"    #Current format of pixels on transport layer.
XI_PRM_TRANSPORT_DATA_TARGET = "transport_data_target"    #Target selector for data - CPU RAM or GPU RAM
XI_PRM_SENSOR_CLOCK_FREQ_HZ = "sensor_clock_freq_hz"    #Sensor clock frequency in Hz.
XI_PRM_SENSOR_CLOCK_FREQ_INDEX = "sensor_clock_freq_index"    #Sensor clock frequency index. Sensor with selected frequencies have possibility to set the frequency only by this index.
XI_PRM_SENSOR_OUTPUT_CHANNEL_COUNT = "sensor_output_channel_count"    #Number of output channels from sensor used for data transfer.
XI_PRM_FRAMERATE = "framerate"    #Define framerate in Hz
XI_PRM_COUNTER_SELECTOR = "counter_selector"    #Select counter
XI_PRM_COUNTER_VALUE = "counter_value"    #Counter status
XI_PRM_ACQ_TIMING_MODE = "acq_timing_mode"    #Type of sensor frames timing.
XI_PRM_AVAILABLE_BANDWIDTH = "available_bandwidth"    #Measure available interface bandwidth (in Megabits)
XI_PRM_BUFFER_POLICY = "buffer_policy"    #Data move policy
XI_PRM_LUT_EN = "LUTEnable"    #Activates LUT.
XI_PRM_LUT_INDEX = "LUTIndex"    #Control the index (offset) of the coefficient to access in the LUT.
XI_PRM_LUT_VALUE = "LUTValue"    #Value at entry LUTIndex of the LUT
XI_PRM_TRG_DELAY = "trigger_delay"    #Specifies the delay in microseconds (us) to apply after the trigger reception before activating it.
XI_PRM_TS_RST_MODE = "ts_rst_mode"    #Defines how TimeStamp reset engine will be armed
XI_PRM_TS_RST_SOURCE = "ts_rst_source"    #Defines which source will be used for timestamp reset. Writing this parameter will trigger settings of engine (arming)
XI_PRM_IS_DEVICE_EXIST = "isexist"    #Returns 1 if camera connected and works properly.
XI_PRM_ACQ_BUFFER_SIZE = "acq_buffer_size"    #Acquisition buffer size in buffer_size_unit. Default bytes.
XI_PRM_ACQ_BUFFER_SIZE_UNIT = "acq_buffer_size_unit"    #Acquisition buffer size unit in bytes. Default 1. E.g. Value 1024 means that buffer_size is in KiBytes
XI_PRM_ACQ_TRANSPORT_BUFFER_SIZE = "acq_transport_buffer_size"    #Acquisition transport buffer size in bytes
XI_PRM_ACQ_TRANSPORT_PACKET_SIZE = "acq_transport_packet_size"    #Acquisition transport packet size in bytes
XI_PRM_BUFFERS_QUEUE_SIZE = "buffers_queue_size"    #Queue of field/frame buffers
XI_PRM_ACQ_TRANSPORT_BUFFER_COMMIT = "acq_transport_buffer_commit"    #Total number of buffers to be committed to transport layer. Increasing can enhance transport capacity. E.g. on USB
XI_PRM_ACQ_TRANSPORT_DATA_COMMIT_TOTAL_SIZE = "acq_transport_data_commit_total_size"    #Total number of bytes to be commit in one time on transport (all transport buffers together). Increasing can enhance transport capacity. E.g. on USB
XI_PRM_RECENT_FRAME = "recent_frame"    #GetImage returns most recent frame
XI_PRM_DEVICE_RESET = "device_reset"    #Resets the camera to default state.
XI_PRM_CONCAT_IMG_MODE = "concat_img_mode"    #Enable/disable the Concatenated Images in One Buffer feature
XI_PRM_CONCAT_IMG_COUNT = "concat_img_count"    #Number of Concatenated Images in One Buffer
XI_PRM_CONCAT_IMG_TRANSPORT_IMG_OFFSET = "concat_img_transport_img_offset"    #Offset between images data in transport buffer when feature Concatenated Images in One Buffer is enabled
XI_PRM_PROBE_SELECTOR = "probe_selector"    #Select Probe
XI_PRM_PROBE_VALUE = "probe_value"    #Returns Value of the selected Probe
XI_PRM_COLUMN_FPN_CORRECTION = "column_fpn_correction"    #Correction of column FPN
XI_PRM_ROW_FPN_CORRECTION = "row_fpn_correction"    #Correction of row FPN
XI_PRM_COLUMN_BLACK_OFFSET_CORRECTION = "column_black_offset_correction"    #Correction of column black offset
XI_PRM_ROW_BLACK_OFFSET_CORRECTION = "row_black_offset_correction"    #Correction of row black offset
XI_PRM_IMAGE_CORRECTION_SELECTOR = "image_correction_selector"    #Select image correction function
XI_PRM_IMAGE_CORRECTION_VALUE = "image_correction_value"    #Select image correction selected function value
XI_PRM_SENSOR_MODE = "sensor_mode"    #Current sensor mode. Allows to select sensor mode by one integer. Setting of this parameter affects: image dimensions and downsampling.
XI_PRM_HDR = "hdr"    #Enable High Dynamic Range feature.
XI_PRM_HDR_KNEEPOINT_COUNT = "hdr_kneepoint_count"    #The number of kneepoints in the PWLR.
XI_PRM_HDR_T1 = "hdr_t1"    #position of first kneepoint(in % of XI_PRM_EXPOSURE)
XI_PRM_HDR_T2 = "hdr_t2"    #position of second kneepoint (in % of XI_PRM_EXPOSURE)
XI_PRM_KNEEPOINT1 = "hdr_kneepoint1"    #value of first kneepoint (% of sensor saturation)
XI_PRM_KNEEPOINT2 = "hdr_kneepoint2"    #value of second kneepoint (% of sensor saturation)
XI_PRM_TRANS_DATA_BLACK_LEVEL_OVR = "trans_data_black_level_ovr"    #Overwrites black level comming from transport data.
XI_PRM_TRANS_DATA_BLACK_LEVEL_OVR_EN = "trans_data_black_level_ovr_en"    #Enables/disables black level overwrite.
XI_PRM_IMAGE_BLACK_LEVEL = "image_black_level"    #Last image black level counts (same as in XI_IMG). Setting can be used only for Offline Processing.
XI_PRM_IMAGE_AREA = "image_area"    #Defines image area of sensor as output.
XI_PRM_DUAL_ADC_MODE = "dual_adc_mode"    #Sets DualADC Mode
XI_PRM_DUAL_ADC_GAIN_RATIO = "dual_adc_gain_ratio"    #Sets DualADC Gain Ratio in dB
XI_PRM_DUAL_ADC_THRESHOLD = "dual_adc_threshold"    #Sets DualADC Threshold value
XI_PRM_COMPRESSION_REGION_SELECTOR = "compression_region_selector"    #Sets Compression Region Selector
XI_PRM_COMPRESSION_REGION_START = "compression_region_start"    #Sets Compression Region Start
XI_PRM_COMPRESSION_REGION_GAIN = "compression_region_gain"    #Sets Compression Region Gain
XI_PRM_VERSION_SELECTOR = "version_selector"    #Selects module/unit, which version we get.
XI_PRM_VERSION = "version"    #Returns version of selected module/unit(XI_PRM_VERSION_SELECTOR).
XI_PRM_API_VERSION = "api_version"    #Returns version of API.
XI_PRM_DRV_VERSION = "drv_version"    #Returns version of current device driver.
XI_PRM_MCU1_VERSION = "version_mcu1"    #Returns version of MCU1 firmware.
XI_PRM_MCU2_VERSION = "version_mcu2"    #Returns version of MCU2 firmware.
XI_PRM_MCU3_VERSION = "version_mcu3"    #Returns version of MCU3 firmware.
XI_PRM_FPGA1_VERSION = "version_fpga1"    #Returns version of FPGA firmware currently running.
XI_PRM_XMLMAN_VERSION = "version_xmlman"    #Returns version of XML manifest.
XI_PRM_HW_REVISION = "hw_revision"    #Returns hardware revision number.
XI_PRM_FACTORY_SET_VERSION = "factory_set_version"    #Returns version of factory set.
XI_PRM_DEBUG_LEVEL = "debug_level"    #Set debug level
XI_PRM_AUTO_BANDWIDTH_CALCULATION = "auto_bandwidth_calculation"    #Automatic bandwidth calculation,
XI_PRM_NEW_PROCESS_CHAIN_ENABLE = "new_process_chain_enable"    #Enables (2015/FAPI) processing chain for MQ MU cameras. If disabled - legacy processing 2006 is used.
XI_PRM_CAM_ENUM_GOLDEN_ENABLED = "cam_enum_golden_enabled"    #Enable enumeration of golden devices
XI_PRM_RESET_USB_IF_BOOTLOADER = "reset_usb_if_bootloader"    #Resets USB device if started as bootloader
XI_PRM_CAM_SIMULATORS_COUNT = "cam_simulators_count"    #Number of camera simulators to be available.
XI_PRM_CAM_SENSOR_INIT_DISABLED = "cam_sensor_init_disabled"    #Camera sensor will not be initialized when 1=XI_ON is set.
XI_PRM_PROC_NUM_THREADS = "proc_num_threads"    #Number of threads per image processor
XI_PRM_PROC_ENGINE = "proc_engine"    #Set processing engine
XI_PRM_READ_FILE_FFS = "read_file_ffs"    #Read file from camera flash filesystem.
XI_PRM_WRITE_FILE_FFS = "write_file_ffs"    #Write file to camera flash filesystem.
XI_PRM_FFS_FILE_NAME = "ffs_file_name"    #Set name of file to be written/read from camera FFS.
XI_PRM_FFS_FILE_ID = "ffs_file_id"    #File number.
XI_PRM_FFS_FILE_OFFSET = "ffs_file_offset"    #Offset of data in file.
XI_PRM_FFS_FILE_SIZE = "ffs_file_size"    #Size of file.
XI_PRM_FREE_FFS_SIZE = "free_ffs_size"    #Size of free camera FFS.
XI_PRM_USED_FFS_SIZE = "used_ffs_size"    #Size of used camera FFS.
XI_PRM_FFS_ACCESS_KEY = "ffs_access_key"    #Setting of key enables file operations on some cameras.
XI_PRM_API_CONTEXT_LIST = "xiapi_context_list"    #List of current parameters settings context - parameters with values. Used for offline processing.
XI_PRM_SENSOR_FEATURE_SELECTOR = "sensor_feature_selector"    #Selects the current feature which is accessible by XI_PRM_SENSOR_FEATURE_VALUE.
XI_PRM_SENSOR_FEATURE_VALUE = "sensor_feature_value"    #Allows access to sensor feature value currently selected by XI_PRM_SENSOR_FEATURE_SELECTOR.
XI_PRM_EXTENDED_FEATURE_SELECTOR = "ext_feature_selector"    #Selection of extended feature.
XI_PRM_EXTENDED_FEATURE = "ext_feature"    #Extended feature value.
XI_PRM_DEVICE_UNIT_SELECTOR = "device_unit_selector"    #Selects device unit.
XI_PRM_DEVICE_UNIT_REGISTER_SELECTOR = "device_unit_register_selector"    #Selects register of selected device unit(XI_PRM_DEVICE_UNIT_SELECTOR).
XI_PRM_DEVICE_UNIT_REGISTER_SELECTOR_NAME = "device_unit_register_selector_name"    #Selects register of selected device unit by name.
XI_PRM_DEVICE_UNIT_REGISTER_SELECTOR_DESCRIPTION = "device_unit_register_selector_desc"    #Read register description of selected device unit by name.
XI_PRM_DEVICE_UNIT_REGISTER_TYPE = "device_unit_register_type"    #Get type of device unit register for correct data interpretation.
XI_PRM_DEVICE_UNIT_REGISTER_VALUE = "device_unit_register_value"    #Sets/gets register value of selected device unit(XI_PRM_DEVICE_UNIT_SELECTOR).
XI_PRM_DEVICE_UNIT_REGISTER_ENUM_VALUE_COUNT = "device_unit_register_enum_value_count"    #Read register enumerated values count.
XI_PRM_DEVICE_UNIT_REGISTER_ENUM_VALUE_INDEX = "device_unit_register_enum_value_index"    #Sets/gets register enumerated value index.
XI_PRM_DEVICE_UNIT_REGISTER_ENUM_VALUE = "device_unit_register_enum_value"    #Read register enumerated value.
XI_PRM_API_PROGRESS_CALLBACK = "api_progress_callback"    #Callback address of pointer that is called upon long tasks (e.g. XI_PRM_WRITE_FILE_FFS).
XI_PRM_ACQUISITION_STATUS_SELECTOR = "acquisition_status_selector"    #Selects the internal acquisition signal to read using XI_PRM_ACQUISITION_STATUS.
XI_PRM_ACQUISITION_STATUS = "acquisition_status"    #Acquisition status(True/False)
XI_PRM_DP_UNIT_SELECTOR = "dp_unit_selector"    #Data Pipe Unit Selector.
XI_PRM_DP_PROC_SELECTOR = "dp_proc_selector"    #Data Pipe Processor Selector.
XI_PRM_DP_PARAM_SELECTOR = "dp_param_selector"    #Data Pipe Processor parameter Selector.
XI_PRM_DP_PARAM_VALUE = "dp_param_value"    #Data Pipe processor parameter value
XI_PRM_GENTL_DATASTREAM_ENABLED = "gentl_stream_en"    #Enable or disable low level streaming via GenTL.
XI_PRM_GENTL_DATASTREAM_CONTEXT = "gentl_stream_context"    #Get GenTL stream context pointer for low level streaming
XI_PRM_USER_SET_SELECTOR = "user_set_selector"    #Selects the feature User Set to load, save or configure.
XI_PRM_USER_SET_LOAD = "user_set_load"    #Loads the User Set specified by User Set Selector to the device and makes it active.
XI_PRM_USER_SET_DEFAULT = "user_set_default"    #Selects the feature User Set to load and make active by default when the device is reset. Change might affect default mode in other applications, e.g. CamTool.

# VAL_TYPE is a dictionary with all xiapi parameter names as keys and their types as values
VAL_TYPE = {
    "exposure": "xiTypeFloat",    #Exposure time in microseconds
    "exposure_time_selector": "xiTypeEnum",    #Selector for Exposure parameter
    "exposure_burst_count": "xiTypeInteger",    #Sets the number of times of exposure in one frame.
    "gain_selector": "xiTypeEnum",    #Gain selector for parameter Gain allows to select different type of gains.
    "gain": "xiTypeFloat",    #Gain in dB
    "downsampling": "xiTypeEnum",    #Change image resolution by binning or skipping.
    "downsampling_type": "xiTypeEnum",    #Change image downsampling type.
    "test_pattern_generator_selector": "xiTypeEnum",    #Selects which test pattern generator is controlled by the test pattern feature.
    "test_pattern": "xiTypeEnum",    #Selects which test pattern type is generated by the selected generator.
    "imgdataformat": "xiTypeEnum",    #Output data format.
    "image_data_sign": "xiTypeEnum",    #Signedness of image data.
    "shutter_type": "xiTypeEnum",    #Change sensor shutter type(CMOS sensor).
    "sensor_taps": "xiTypeEnum",    #Number of taps
    "aeag": "xiTypeBoolean",    #Automatic exposure/gain
    "aeag_roi_offset_x": "xiTypeInteger",    #Automatic exposure/gain ROI offset X
    "aeag_roi_offset_y": "xiTypeInteger",    #Automatic exposure/gain ROI offset Y
    "aeag_roi_width": "xiTypeInteger",    #Automatic exposure/gain ROI Width
    "aeag_roi_height": "xiTypeInteger",    #Automatic exposure/gain ROI Height
    "bpc_list_selector": "xiTypeEnum",    #Selector of list used by Sensor Defects Correction parameter
    "sens_defects_corr_list_content": "xiTypeString",    #Sets/Gets sensor defects list in special text format
    "bpc": "xiTypeBoolean",    #Correction of sensor defects (pixels, columns, rows) enable/disable
    "auto_wb": "xiTypeBoolean",    #Automatic white balance
    "manual_wb": "xiTypeCommand",    #Calculates White Balance(xiGetImage function must be called)
    "wb_roi_offset_x": "xiTypeInteger",    #White balance offset X
    "wb_roi_offset_y": "xiTypeInteger",    #White balance offset Y
    "wb_roi_width": "xiTypeInteger",    #White balance width
    "wb_roi_height": "xiTypeInteger",    #White balance height
    "wb_kr": "xiTypeFloat",    #White balance red coefficient
    "wb_kg": "xiTypeFloat",    #White balance green coefficient
    "wb_kb": "xiTypeFloat",    #White balance blue coefficient
    "width": "xiTypeInteger",    #Width of the Image provided by the device (in pixels).
    "height": "xiTypeInteger",    #Height of the Image provided by the device (in pixels).
    "offsetX": "xiTypeInteger",    #Horizontal offset from the origin to the area of interest (in pixels).
    "offsetY": "xiTypeInteger",    #Vertical offset from the origin to the area of interest (in pixels).
    "region_selector": "xiTypeInteger",    #Selects Region in Multiple ROI which parameters are set by width, height, ... ,region mode
    "region_mode": "xiTypeInteger",    #Activates/deactivates Region selected by Region Selector
    "horizontal_flip": "xiTypeBoolean",    #Horizontal flip enable
    "vertical_flip": "xiTypeBoolean",    #Vertical flip enable
    "interline_exposure_mode": "xiTypeEnum",    #Selector for Exposure parameter
    "ffc": "xiTypeBoolean",    #Image flat field correction
    "ffc_flat_field_file_name": "xiTypeString",    #Set name of file to be applied for FFC processor.
    "ffc_dark_field_file_name": "xiTypeString",    #Set name of file to be applied for FFC processor.
    "tof_readout_mode": "xiTypeEnum",    #Sets ToF Readout Mode
    "tof_modulation_frequency": "xiTypeFloat",    #Sets ToF Modulation Frequency in MHz
    "tof_multiple_phases_in_buffer": "xiTypeInteger",    #is multiple ToF phases concatenated in buffer
    "tof_phases_count": "xiTypeInteger",    #Sets the number of tof phases. E.g. 4 for four phases.
    "tof_phase_angle": "xiTypeFloat",    #Sets Illumination angle for selected ToF phase
    "tof_phase_exposure_time": "xiTypeFloat",    #Sets Exposure time for selected ToF phase in microseconds.
    "tof_phase_selector": "xiTypeInteger",    #Selects tof phase
    "binning_selector": "xiTypeEnum",    #Binning engine selector.
    "binning_vertical_mode": "xiTypeEnum",    #Sets the mode to use to combine vertical pixel together.
    "binning_vertical": "xiTypeInteger",    #Vertical Binning - number of vertical photo-sensitive cells to combine together.
    "binning_vertical_float": "xiTypeFloat",    #Vertical Binning Float - number of vertical photo-sensitive cells to combine together.
    "binning_horizontal_mode": "xiTypeEnum",    #Sets the mode to use to combine horizontal pixel together.
    "binning_horizontal": "xiTypeInteger",    #Horizontal Binning - number of horizontal photo-sensitive cells to combine together.
    "binning_horizontal_float": "xiTypeFloat",    #Horizontal Binning Float - number of horizontal photo-sensitive cells to combine together.
    "binning_horizontal_pattern": "xiTypeEnum",    #Binning horizontal pattern type.
    "binning_vertical_pattern": "xiTypeEnum",    #Binning vertical pattern type.
    "decimation_selector": "xiTypeEnum",    #Decimation engine selector.
    "decimation_vertical": "xiTypeInteger",    #Vertical Decimation - vertical sub-sampling of the image - reduces the vertical resolution of the image by the specified vertical decimation factor.
    "decimation_horizontal": "xiTypeInteger",    #Horizontal Decimation - horizontal sub-sampling of the image - reduces the horizontal resolution of the image by the specified vertical decimation factor.
    "decimation_horizontal_pattern": "xiTypeEnum",    #Decimation horizontal pattern type.
    "decimation_vertical_pattern": "xiTypeEnum",    #Decimation vertical pattern type.
    "exp_priority": "xiTypeFloat",    #Exposure priority (0.8 - exposure 80%, gain 20%).
    "ag_max_limit": "xiTypeFloat",    #Maximum limit of gain in AEAG procedure
    "ae_max_limit": "xiTypeInteger",    #Maximum time (us) used for exposure in AEAG procedure
    "aeag_level": "xiTypeInteger",    #Average intensity of output signal AEAG should achieve(in %)
    "aeag_skip_frames_count": "xiTypeInteger",    #Minimum number of frames to be skipped between AEAG compensation and AEAG re-evaluation
    "limit_bandwidth": "xiTypeInteger",    #Set/get bandwidth(data rate in Megabits)
    "limit_bandwidth_mode": "xiTypeEnum",    #Bandwidth limit enabled
    "sensor_line_period": "xiTypeFloat",    #Image sensor line period in us
    "sensor_bit_depth": "xiTypeEnum",    #Sensor output data bit depth.
    "output_bit_depth": "xiTypeEnum",    #Device output data bit depth.
    "image_data_bit_depth": "xiTypeEnum",    #bit depth of data returned by function xiGetImage
    "output_bit_packing": "xiTypeBoolean",    #Device output data packing (or grouping) enabled. Packing could be enabled if output_data_bit_depth > 8 and packing capability is available.
    "output_bit_packing_type": "xiTypeEnum",    #Data packing type. Some cameras supports only specific packing type.
    "iscooled": "xiTypeBoolean",    #Returns 1 for cameras that support cooling.
    "cooling": "xiTypeEnum",    #Temperature control mode.
    "target_temp": "xiTypeFloat",    #Set sensor target temperature for cooling.
    "temp_selector": "xiTypeEnum",    #Selector of mechanical point where thermometer is located.
    "temp": "xiTypeFloat",    #Camera temperature (selected by XI_PRM_TEMP_SELECTOR)
    "device_temperature_ctrl_mode": "xiTypeEnum",    #Temperature control mode.
    "chip_temp": "xiTypeFloat",    #Camera sensor temperature
    "hous_temp": "xiTypeFloat",    #Camera housing temperature
    "hous_back_side_temp": "xiTypeFloat",    #Camera housing back side temperature
    "sensor_board_temp": "xiTypeFloat",    #Camera sensor board temperature
    "device_temperature_element_sel": "xiTypeEnum",    #Temperature element selector (TEC(Peltier), Fan).
    "device_temperature_element_val": "xiTypeFloat",    #Temperature element value in percents of full control range
    "cms": "xiTypeEnum",    #Mode of color management system.
    "cms_intent": "xiTypeEnum",    #Intent of color management system.
    "apply_cms": "xiTypeBoolean",    #Enable applying of CMS profiles to xiGetImage (see XI_PRM_INPUT_CMS_PROFILE, XI_PRM_OUTPUT_CMS_PROFILE).
    "input_cms_profile": "xiTypeString",    #Filename for input cms profile (e.g. input.icc)
    "output_cms_profile": "xiTypeString",    #Filename for output cms profile (e.g. input.icc)
    "iscolor": "xiTypeBoolean",    #Returns 1 for color cameras.
    "cfa": "xiTypeEnum",    #Returns color filter array type of RAW data.
    "gammaY": "xiTypeFloat",    #Luminosity gamma
    "gammaC": "xiTypeFloat",    #Chromaticity gamma
    "sharpness": "xiTypeFloat",    #Sharpness strength
    "ccMTX00": "xiTypeFloat",    #Color Correction Matrix element [0][0]
    "ccMTX01": "xiTypeFloat",    #Color Correction Matrix element [0][1]
    "ccMTX02": "xiTypeFloat",    #Color Correction Matrix element [0][2]
    "ccMTX03": "xiTypeFloat",    #Color Correction Matrix element [0][3]
    "ccMTX10": "xiTypeFloat",    #Color Correction Matrix element [1][0]
    "ccMTX11": "xiTypeFloat",    #Color Correction Matrix element [1][1]
    "ccMTX12": "xiTypeFloat",    #Color Correction Matrix element [1][2]
    "ccMTX13": "xiTypeFloat",    #Color Correction Matrix element [1][3]
    "ccMTX20": "xiTypeFloat",    #Color Correction Matrix element [2][0]
    "ccMTX21": "xiTypeFloat",    #Color Correction Matrix element [2][1]
    "ccMTX22": "xiTypeFloat",    #Color Correction Matrix element [2][2]
    "ccMTX23": "xiTypeFloat",    #Color Correction Matrix element [2][3]
    "ccMTX30": "xiTypeFloat",    #Color Correction Matrix element [3][0]
    "ccMTX31": "xiTypeFloat",    #Color Correction Matrix element [3][1]
    "ccMTX32": "xiTypeFloat",    #Color Correction Matrix element [3][2]
    "ccMTX33": "xiTypeFloat",    #Color Correction Matrix element [3][3]
    "defccMTX": "xiTypeCommand",    #Set default Color Correction Matrix
    "ccMTXnorm": "xiTypeBoolean",    #Normalize color correction matrix
    "trigger_source": "xiTypeEnum",    #Defines source of trigger.
    "trigger_software": "xiTypeCommand",    #Generates an internal trigger. XI_PRM_TRG_SOURCE must be set to TRG_SOFTWARE.
    "trigger_selector": "xiTypeEnum",    #Selects the type of trigger.
    "trigger_overlap": "xiTypeEnum",    #The mode of Trigger Overlap. This influences of trigger acception/rejection policy
    "acq_frame_burst_count": "xiTypeInteger",    #Sets number of frames acquired by burst. This burst is used only if trigger is set to FrameBurstStart
    "timestamp": "xiTypeInteger64",    #Current value of the device timestamp counter
    "gpi_selector": "xiTypeEnum",    #Selects GPI
    "gpi_mode": "xiTypeEnum",    #Defines GPI functionality
    "gpi_level": "xiTypeInteger",    #GPI level
    "gpi_level_at_image_exp_start": "xiTypeInteger",    #GPI Level at image exposure start
    "gpi_level_at_image_exp_end": "xiTypeInteger",    #GPI Level at image exposure end
    "gpi_level_at_image_readout_end": "xiTypeInteger",    #GPI Level at image readout end
    "gpo_selector": "xiTypeEnum",    #Selects GPO
    "gpo_mode": "xiTypeEnum",    #Defines GPO functionality
    "metadata_sampling_mode": "xiTypeEnum",    #Defines metadata sampling mode
    "led_selector": "xiTypeEnum",    #Selects LED
    "led_mode": "xiTypeEnum",    #Defines LED functionality
    "dbnc_en": "xiTypeBoolean",    #Enable/Disable debounce to selected GPI
    "dbnc_t0": "xiTypeInteger",    #Debounce time (x * 10us)
    "dbnc_t1": "xiTypeInteger",    #Debounce time (x * 10us)
    "dbnc_pol": "xiTypeInteger",    #Debounce polarity (pol = 1 t0 - falling edge, t1 - rising edge)
    "lens_mode": "xiTypeBoolean",    #Status of lens control interface. This shall be set to XI_ON before any Lens operations.
    "lens_aperture_value": "xiTypeFloat",    #Current lens aperture value in stops. Examples: 2.8, 4, 5.6, 8, 11
    "lens_aperture_index": "xiTypeInteger",    #Current aperture index as reported by lens.
    "lens_focus_movement_value": "xiTypeInteger",    #Lens current focus movement value to be used by XI_PRM_LENS_FOCUS_MOVE in motor steps.
    "lens_focus_move": "xiTypeCommand",    #Moves lens focus motor by steps set in XI_PRM_LENS_FOCUS_MOVEMENT_VALUE.
    "lens_focus_distance": "xiTypeFloat",    #(Planned feature Issue#6958). Lens focus distance in cm.
    "lens_focal_length": "xiTypeFloat",    #Lens focal distance in mm.
    "lens_feature_selector": "xiTypeEnum",    #Selects the current feature which is accessible by XI_PRM_LENS_FEATURE.
    "lens_feature": "xiTypeFloat",    #Allows access to lens feature value currently selected by XI_PRM_LENS_FEATURE_SELECTOR.
    "lens_comm_data": "xiTypeString",    #Write/Read data sequences to/from lens
    "device_name": "xiTypeString",    #Return device name
    "device_type": "xiTypeString",    #Return device type
    "device_model_id": "xiTypeInteger",    #Return device model id
    "sensor_model_id": "xiTypeInteger",    #Return device sensor model id
    "device_sn": "xiTypeString",    #Return device serial number
    "device_sens_sn": "xiTypeString",    #Return sensor serial number
    "device_id": "xiTypeString",    #Return unique device ID
    "device_inst_path": "xiTypeString",    #Return device system instance path.
    "device_loc_path": "xiTypeString",    #Represents the location of the device in the device tree.
    "device_user_id": "xiTypeString",    #Return custom ID of camera.
    "device_manifest": "xiTypeString",    #Return device capability description XML.
    "image_user_data": "xiTypeInteger",    #User image data at image header to track parameters synchronization.
    "imgdataformatrgb32alpha": "xiTypeInteger",    #The alpha channel of RGB32 output image format.
    "imgpayloadsize": "xiTypeInteger",    #Buffer size in bytes sufficient for output image returned by xiGetImage
    "transport_pixel_format": "xiTypeEnum",    #Current format of pixels on transport layer.
    "transport_data_target": "xiTypeEnum",    #Target selector for data - CPU RAM or GPU RAM
    "sensor_clock_freq_hz": "xiTypeFloat",    #Sensor clock frequency in Hz.
    "sensor_clock_freq_index": "xiTypeInteger",    #Sensor clock frequency index. Sensor with selected frequencies have possibility to set the frequency only by this index.
    "sensor_output_channel_count": "xiTypeEnum",    #Number of output channels from sensor used for data transfer.
    "framerate": "xiTypeFloat",    #Define framerate in Hz
    "counter_selector": "xiTypeEnum",    #Select counter
    "counter_value": "xiTypeInteger",    #Counter status
    "acq_timing_mode": "xiTypeEnum",    #Type of sensor frames timing.
    "available_bandwidth": "xiTypeInteger",    #Measure available interface bandwidth (in Megabits)
    "buffer_policy": "xiTypeEnum",    #Data move policy
    "LUTEnable": "xiTypeBoolean",    #Activates LUT.
    "LUTIndex": "xiTypeInteger",    #Control the index (offset) of the coefficient to access in the LUT.
    "LUTValue": "xiTypeInteger",    #Value at entry LUTIndex of the LUT
    "trigger_delay": "xiTypeInteger",    #Specifies the delay in microseconds (us) to apply after the trigger reception before activating it.
    "ts_rst_mode": "xiTypeEnum",    #Defines how TimeStamp reset engine will be armed
    "ts_rst_source": "xiTypeEnum",    #Defines which source will be used for timestamp reset. Writing this parameter will trigger settings of engine (arming)
    "isexist": "xiTypeBoolean",    #Returns 1 if camera connected and works properly.
    "acq_buffer_size": "xiTypeInteger",    #Acquisition buffer size in buffer_size_unit. Default bytes.
    "acq_buffer_size_unit": "xiTypeInteger",    #Acquisition buffer size unit in bytes. Default 1. E.g. Value 1024 means that buffer_size is in KiBytes
    "acq_transport_buffer_size": "xiTypeInteger",    #Acquisition transport buffer size in bytes
    "acq_transport_packet_size": "xiTypeInteger",    #Acquisition transport packet size in bytes
    "buffers_queue_size": "xiTypeInteger",    #Queue of field/frame buffers
    "acq_transport_buffer_commit": "xiTypeInteger",    #Total number of buffers to be committed to transport layer. Increasing can enhance transport capacity. E.g. on USB
    "acq_transport_data_commit_total_size": "xiTypeInteger",    #Total number of bytes to be commit in one time on transport (all transport buffers together). Increasing can enhance transport capacity. E.g. on USB
    "recent_frame": "xiTypeBoolean",    #GetImage returns most recent frame
    "device_reset": "xiTypeCommand",    #Resets the camera to default state.
    "concat_img_mode": "xiTypeBoolean",    #Enable/disable the Concatenated Images in One Buffer feature
    "concat_img_count": "xiTypeInteger",    #Number of Concatenated Images in One Buffer
    "concat_img_transport_img_offset": "xiTypeInteger",    #Offset between images data in transport buffer when feature Concatenated Images in One Buffer is enabled
    "probe_selector": "xiTypeEnum",    #Select Probe
    "probe_value": "xiTypeFloat",    #Returns Value of the selected Probe
    "column_fpn_correction": "xiTypeEnum",    #Correction of column FPN
    "row_fpn_correction": "xiTypeEnum",    #Correction of row FPN
    "column_black_offset_correction": "xiTypeEnum",    #Correction of column black offset
    "row_black_offset_correction": "xiTypeEnum",    #Correction of row black offset
    "image_correction_selector": "xiTypeEnum",    #Select image correction function
    "image_correction_value": "xiTypeFloat",    #Select image correction selected function value
    "sensor_mode": "xiTypeEnum",    #Current sensor mode. Allows to select sensor mode by one integer. Setting of this parameter affects: image dimensions and downsampling.
    "hdr": "xiTypeBoolean",    #Enable High Dynamic Range feature.
    "hdr_kneepoint_count": "xiTypeInteger",    #The number of kneepoints in the PWLR.
    "hdr_t1": "xiTypeInteger",    #position of first kneepoint(in % of XI_PRM_EXPOSURE)
    "hdr_t2": "xiTypeInteger",    #position of second kneepoint (in % of XI_PRM_EXPOSURE)
    "hdr_kneepoint1": "xiTypeInteger",    #value of first kneepoint (% of sensor saturation)
    "hdr_kneepoint2": "xiTypeInteger",    #value of second kneepoint (% of sensor saturation)
    "trans_data_black_level_ovr": "xiTypeFloat",    #Overwrites black level comming from transport data.
    "trans_data_black_level_ovr_en": "xiTypeBoolean",    #Enables/disables black level overwrite.
    "image_black_level": "xiTypeInteger",    #Last image black level counts (same as in XI_IMG). Setting can be used only for Offline Processing.
    "image_area": "xiTypeEnum",    #Defines image area of sensor as output.
    "dual_adc_mode": "xiTypeEnum",    #Sets DualADC Mode
    "dual_adc_gain_ratio": "xiTypeFloat",    #Sets DualADC Gain Ratio in dB
    "dual_adc_threshold": "xiTypeInteger",    #Sets DualADC Threshold value
    "compression_region_selector": "xiTypeInteger",    #Sets Compression Region Selector
    "compression_region_start": "xiTypeFloat",    #Sets Compression Region Start
    "compression_region_gain": "xiTypeFloat",    #Sets Compression Region Gain
    "version_selector": "xiTypeEnum",    #Selects module/unit, which version we get.
    "version": "xiTypeString",    #Returns version of selected module/unit(XI_PRM_VERSION_SELECTOR).
    "api_version": "xiTypeString",    #Returns version of API.
    "drv_version": "xiTypeString",    #Returns version of current device driver.
    "version_mcu1": "xiTypeString",    #Returns version of MCU1 firmware.
    "version_mcu2": "xiTypeString",    #Returns version of MCU2 firmware.
    "version_mcu3": "xiTypeString",    #Returns version of MCU3 firmware.
    "version_fpga1": "xiTypeString",    #Returns version of FPGA firmware currently running.
    "version_xmlman": "xiTypeString",    #Returns version of XML manifest.
    "hw_revision": "xiTypeString",    #Returns hardware revision number.
    "factory_set_version": "xiTypeString",    #Returns version of factory set.
    "debug_level": "xiTypeEnum",    #Set debug level
    "auto_bandwidth_calculation": "xiTypeBoolean",    #Automatic bandwidth calculation,
    "new_process_chain_enable": "xiTypeBoolean",    #Enables (2015/FAPI) processing chain for MQ MU cameras. If disabled - legacy processing 2006 is used.
    "cam_enum_golden_enabled": "xiTypeBoolean",    #Enable enumeration of golden devices
    "reset_usb_if_bootloader": "xiTypeBoolean",    #Resets USB device if started as bootloader
    "cam_simulators_count": "xiTypeInteger",    #Number of camera simulators to be available.
    "cam_sensor_init_disabled": "xiTypeBoolean",    #Camera sensor will not be initialized when 1=XI_ON is set.
    "proc_num_threads": "xiTypeInteger",    #Number of threads per image processor
    "proc_engine": "xiTypeEnum",    #Set processing engine
    "read_file_ffs": "xiTypeString",    #Read file from camera flash filesystem.
    "write_file_ffs": "xiTypeString",    #Write file to camera flash filesystem.
    "ffs_file_name": "xiTypeString",    #Set name of file to be written/read from camera FFS.
    "ffs_file_id": "xiTypeInteger",    #File number.
    "ffs_file_offset": "xiTypeInteger",    #Offset of data in file.
    "ffs_file_size": "xiTypeInteger",    #Size of file.
    "free_ffs_size": "xiTypeInteger64",    #Size of free camera FFS.
    "used_ffs_size": "xiTypeInteger64",    #Size of used camera FFS.
    "ffs_access_key": "xiTypeInteger",    #Setting of key enables file operations on some cameras.
    "xiapi_context_list": "xiTypeString",    #List of current parameters settings context - parameters with values. Used for offline processing.
    "sensor_feature_selector": "xiTypeEnum",    #Selects the current feature which is accessible by XI_PRM_SENSOR_FEATURE_VALUE.
    "sensor_feature_value": "xiTypeInteger",    #Allows access to sensor feature value currently selected by XI_PRM_SENSOR_FEATURE_SELECTOR.
    "ext_feature_selector": "xiTypeEnum",    #Selection of extended feature.
    "ext_feature": "xiTypeInteger",    #Extended feature value.
    "device_unit_selector": "xiTypeEnum",    #Selects device unit.
    "device_unit_register_selector": "xiTypeInteger",    #Selects register of selected device unit(XI_PRM_DEVICE_UNIT_SELECTOR).
    "device_unit_register_selector_name": "xiTypeString",    #Selects register of selected device unit by name.
    "device_unit_register_selector_desc": "xiTypeString",    #Read register description of selected device unit by name.
    "device_unit_register_type": "xiTypeEnum",    #Get type of device unit register for correct data interpretation.
    "device_unit_register_value": "xiTypeInteger",    #Sets/gets register value of selected device unit(XI_PRM_DEVICE_UNIT_SELECTOR).
    "device_unit_register_enum_value_count": "xiTypeInteger",    #Read register enumerated values count.
    "device_unit_register_enum_value_index": "xiTypeInteger",    #Sets/gets register enumerated value index.
    "device_unit_register_enum_value": "xiTypeInteger",    #Read register enumerated value.
    "api_progress_callback": "xiTypeString",    #Callback address of pointer that is called upon long tasks (e.g. XI_PRM_WRITE_FILE_FFS).
    "acquisition_status_selector": "xiTypeEnum",    #Selects the internal acquisition signal to read using XI_PRM_ACQUISITION_STATUS.
    "acquisition_status": "xiTypeEnum",    #Acquisition status(True/False)
    "dp_unit_selector": "xiTypeEnum",    #Data Pipe Unit Selector.
    "dp_proc_selector": "xiTypeEnum",    #Data Pipe Processor Selector.
    "dp_param_selector": "xiTypeEnum",    #Data Pipe Processor parameter Selector.
    "dp_param_value": "xiTypeFloat",    #Data Pipe processor parameter value
    "gentl_stream_en": "xiTypeBoolean",    #Enable or disable low level streaming via GenTL.
    "gentl_stream_context": "xiTypeString",    #Get GenTL stream context pointer for low level streaming
    "user_set_selector": "xiTypeEnum",    #Selects the feature User Set to load, save or configure.
    "user_set_load": "xiTypeCommand",    #Loads the User Set specified by User Set Selector to the device and makes it active.
    "user_set_default": "xiTypeEnum",    #Selects the feature User Set to load and make active by default when the device is reset. Change might affect default mode in other applications, e.g. CamTool.
    }

ASSOC_ENUM = {
    "exposure_time_selector": XI_EXPOSURE_TIME_SELECTOR_TYPE,    #Selector for Exposure parameter
    "gain_selector": XI_GAIN_SELECTOR_TYPE,    #Gain selector for parameter Gain allows to select different type of gains.
    "downsampling": XI_DOWNSAMPLING_VALUE,    #Change image resolution by binning or skipping.
    "downsampling_type": XI_DOWNSAMPLING_TYPE,    #Change image downsampling type.
    "test_pattern_generator_selector": XI_TEST_PATTERN_GENERATOR,    #Selects which test pattern generator is controlled by the test pattern feature.
    "test_pattern": XI_TEST_PATTERN,    #Selects which test pattern type is generated by the selected generator.
    "imgdataformat": XI_IMG_FORMAT,    #Output data format.
    "image_data_sign": XI_DATA_SM,    #Signedness of image data.
    "shutter_type": XI_SHUTTER_TYPE,    #Change sensor shutter type(CMOS sensor).
    "sensor_taps": XI_SENSOR_TAP_CNT,    #Number of taps
    "bpc_list_selector": XI_SENS_DEFFECTS_CORR_LIST_SELECTOR,    #Selector of list used by Sensor Defects Correction parameter
    "interline_exposure_mode": XI_INTERLINE_EXPOSURE_MODE_TYPE,    #Selector for Exposure parameter
    "tof_readout_mode": XI_TOF_READOUT_MODE,    #Sets ToF Readout Mode
    "binning_selector": XI_BIN_SELECTOR,    #Binning engine selector.
    "binning_vertical_mode": XI_BIN_MODE,    #Sets the mode to use to combine vertical pixel together.
    "binning_horizontal_mode": XI_BIN_MODE,    #Sets the mode to use to combine horizontal pixel together.
    "binning_horizontal_pattern": XI_BIN_PATTERN,    #Binning horizontal pattern type.
    "binning_vertical_pattern": XI_BIN_PATTERN,    #Binning vertical pattern type.
    "decimation_selector": XI_DEC_SELECTOR,    #Decimation engine selector.
    "decimation_horizontal_pattern": XI_DEC_PATTERN,    #Decimation horizontal pattern type.
    "decimation_vertical_pattern": XI_DEC_PATTERN,    #Decimation vertical pattern type.
    "limit_bandwidth_mode": XI_SWITCH,    #Bandwidth limit enabled
    "sensor_bit_depth": XI_BIT_DEPTH,    #Sensor output data bit depth.
    "output_bit_depth": XI_BIT_DEPTH,    #Device output data bit depth.
    "image_data_bit_depth": XI_BIT_DEPTH,    #bit depth of data returned by function xiGetImage
    "output_bit_packing_type": XI_OUTPUT_DATA_PACKING_TYPE,    #Data packing type. Some cameras supports only specific packing type.
    "cooling": XI_TEMP_CTRL_MODE_SELECTOR,    #Temperature control mode.
    "temp_selector": XI_TEMP_SELECTOR,    #Selector of mechanical point where thermometer is located.
    "device_temperature_ctrl_mode": XI_TEMP_CTRL_MODE_SELECTOR,    #Temperature control mode.
    "device_temperature_element_sel": XI_TEMP_ELEMENT_SELECTOR,    #Temperature element selector (TEC(Peltier), Fan).
    "cms": XI_CMS_MODE,    #Mode of color management system.
    "cms_intent": XI_CMS_INTENT,    #Intent of color management system.
    "cfa": XI_COLOR_FILTER_ARRAY,    #Returns color filter array type of RAW data.
    "trigger_source": XI_TRG_SOURCE,    #Defines source of trigger.
    "trigger_selector": XI_TRG_SELECTOR,    #Selects the type of trigger.
    "trigger_overlap": XI_TRG_OVERLAP,    #The mode of Trigger Overlap. This influences of trigger acception/rejection policy
    "gpi_selector": XI_GPI_SELECTOR,    #Selects GPI
    "gpi_mode": XI_GPI_MODE,    #Defines GPI functionality
    "gpo_selector": XI_GPO_SELECTOR,    #Selects GPO
    "gpo_mode": XI_GPO_MODE,    #Defines GPO functionality
    "metadata_sampling_mode": XI_METADATA_SAMPLING_MODE,    #Defines metadata sampling mode
    "led_selector": XI_LED_SELECTOR,    #Selects LED
    "led_mode": XI_LED_MODE,    #Defines LED functionality
    "lens_feature_selector": XI_LENS_FEATURE,    #Selects the current feature which is accessible by XI_PRM_LENS_FEATURE.
    "transport_pixel_format": XI_GenTL_Image_Format_e,    #Current format of pixels on transport layer.
    "transport_data_target": XI_TRANSPORT_DATA_TARGET_MODE,    #Target selector for data - CPU RAM or GPU RAM
    "sensor_output_channel_count": XI_SENSOR_OUTPUT_CHANNEL_COUNT,    #Number of output channels from sensor used for data transfer.
    "counter_selector": XI_COUNTER_SELECTOR,    #Select counter
    "acq_timing_mode": XI_ACQ_TIMING_MODE,    #Type of sensor frames timing.
    "buffer_policy": XI_BP,    #Data move policy
    "ts_rst_mode": XI_TS_RST_MODE,    #Defines how TimeStamp reset engine will be armed
    "ts_rst_source": XI_TS_RST_SOURCE,    #Defines which source will be used for timestamp reset. Writing this parameter will trigger settings of engine (arming)
    "probe_selector": XI_PROBE_SELECTOR,    #Select Probe
    "column_fpn_correction": XI_SWITCH,    #Correction of column FPN
    "row_fpn_correction": XI_SWITCH,    #Correction of row FPN
    "column_black_offset_correction": XI_SWITCH,    #Correction of column black offset
    "row_black_offset_correction": XI_SWITCH,    #Correction of row black offset
    "image_correction_selector": XI_IMAGE_CORRECTION_SELECTOR,    #Select image correction function
    "sensor_mode": XI_SENSOR_MODE,    #Current sensor mode. Allows to select sensor mode by one integer. Setting of this parameter affects: image dimensions and downsampling.
    "image_area": XI_IMAGE_AREA_SELECTOR,    #Defines image area of sensor as output.
    "dual_adc_mode": XI_DUAL_ADC_MODE,    #Sets DualADC Mode
    "version_selector": XI_VERSION,    #Selects module/unit, which version we get.
    "debug_level": XI_DEBUG_LEVEL,    #Set debug level
    "proc_engine": XI_PROC_ENGINE,    #Set processing engine
    "sensor_feature_selector": XI_SENSOR_FEATURE_SELECTOR,    #Selects the current feature which is accessible by XI_PRM_SENSOR_FEATURE_VALUE.
    "ext_feature_selector": XI_EXT_FEATURE_SELECTOR,    #Selection of extended feature.
    "device_unit_selector": XI_DEVICE_UNIT_SELECTOR,    #Selects device unit.
    "device_unit_register_type": XI_DEVICE_UNIT_REGISTER_TYPE,    #Get type of device unit register for correct data interpretation.
    "acquisition_status_selector": XI_ACQUISITION_STATUS_SELECTOR,    #Selects the internal acquisition signal to read using XI_PRM_ACQUISITION_STATUS.
    "acquisition_status": XI_SWITCH,    #Acquisition status(True/False)
    "dp_unit_selector": XI_DP_UNIT_SELECTOR,    #Data Pipe Unit Selector.
    "dp_proc_selector": XI_DP_PROC_SELECTOR,    #Data Pipe Processor Selector.
    "dp_param_selector": XI_DP_PARAM_SELECTOR,    #Data Pipe Processor parameter Selector.
    "user_set_selector": XI_USER_SET_SELECTOR,    #Selects the feature User Set to load, save or configure.
    "user_set_default": XI_USER_SET_SELECTOR,    #Selects the feature User Set to load and make active by default when the device is reset. Change might affect default mode in other applications, e.g. CamTool.
    }

# Structures

class XI_IMG_DESC(Structure):
    '''
    structure containing description of image areas and format.
    '''
    _fields_ = [
        ("Area0Left",    DWORD),        #Pixels of Area0 of image left.
        ("Area1Left",    DWORD),        #Pixels of Area1 of image left.
        ("Area2Left",    DWORD),        #Pixels of Area2 of image left.
        ("Area3Left",    DWORD),        #Pixels of Area3 of image left.
        ("Area4Left",    DWORD),        #Pixels of Area4 of image left.
        ("Area5Left",    DWORD),        #Pixels of Area5 of image left.
        ("ActiveAreaWidth",    DWORD),        #Width of active area.
        ("Area5Right",    DWORD),        #Pixels of Area5 of image right.
        ("Area4Right",    DWORD),        #Pixels of Area4 of image right.
        ("Area3Right",    DWORD),        #Pixels of Area3 of image right.
        ("Area2Right",    DWORD),        #Pixels of Area2 of image right.
        ("Area1Right",    DWORD),        #Pixels of Area1 of image right.
        ("Area0Right",    DWORD),        #Pixels of Area0 of image right.
        ("Area0Top",    DWORD),        #Pixels of Area0 of image top.
        ("Area1Top",    DWORD),        #Pixels of Area1 of image top.
        ("Area2Top",    DWORD),        #Pixels of Area2 of image top.
        ("Area3Top",    DWORD),        #Pixels of Area3 of image top.
        ("Area4Top",    DWORD),        #Pixels of Area4 of image top.
        ("Area5Top",    DWORD),        #Pixels of Area5 of image top.
        ("ActiveAreaHeight",    DWORD),        #Height of active area.
        ("Area5Bottom",    DWORD),        #Pixels of Area5 of image bottom.
        ("Area4Bottom",    DWORD),        #Pixels of Area4 of image bottom.
        ("Area3Bottom",    DWORD),        #Pixels of Area3 of image bottom.
        ("Area2Bottom",    DWORD),        #Pixels of Area2 of image bottom.
        ("Area1Bottom",    DWORD),        #Pixels of Area1 of image bottom.
        ("Area0Bottom",    DWORD),        #Pixels of Area0 of image bottom.
        ("format",    DWORD),        #Current format of pixels. XI_GenTL_Image_Format_e.
        ("flags",    DWORD),        #description of areas and image.
        ]

class XI_IMG(Structure):
    '''
    structure containing information about incoming image.
    '''
    _fields_ = [
        ("size",    DWORD),        #Size of current structure on application side. When xiGetImage is called and size>=SIZE_XI_IMG_V2 then GPI_level, tsSec and tsUSec are filled.
        ("bp",    LPVOID),        #Pointer to data. (see Note1)
        ("bp_size",    DWORD),        #Filled buffer size. (see Note2)
        ("frm",    c_uint),        #Format of image data get from GetImage.
        ("width",    DWORD),        #width of incoming image.
        ("height",    DWORD),        #height of incoming image.
        ("nframe",    DWORD),        #Frame number. On some cameras it is reset by exposure, gain, downsampling change, auto exposure (AEAG).
        ("tsSec",    DWORD),        #Seconds part of image timestamp (see Note3).
        ("tsUSec",    DWORD),        #Micro-seconds part image timestamp (see Note3). Range 0-999999 us.
        ("GPI_level",    DWORD),        #Levels of digital inputs/outputs of the camera at time of exposure start/end (sample time and bits are specific for each camera model)
        ("black_level",    DWORD),        #Black level of image (ONLY for MONO and RAW formats). (see Note4)
        ("padding_x",    DWORD),        #Number of extra bytes provided at the end of each line to facilitate image alignment in buffers.
        ("AbsoluteOffsetX",    DWORD),        #Horizontal offset of origin of sensor and buffer image first pixel.
        ("AbsoluteOffsetY",    DWORD),        #Vertical offset of origin of sensor and buffer image first pixel.
        ("transport_frm",    DWORD),        #Current format of pixels on transport layer.
        ("img_desc",    XI_IMG_DESC),        #description of image areas and format.
        ("DownsamplingX",    DWORD),        #Horizontal downsampling
        ("DownsamplingY",    DWORD),        #Vertical downsampling
        ("flags",    DWORD),        #description of XI_IMG.
        ("exposure_time_us",    DWORD),        #Exposure time of this image in microseconds. (see Note5)
        ("gain_db",    FLOAT),        #Gain used for this image in deci-bells. (see Note6)
        ("acq_nframe",    DWORD),        #Frame number. Reset only by acquisition start. NOT reset by change of exposure, gain, downsampling, auto exposure (AEAG).
        ("image_user_data",    DWORD),        #(see Note7)
        ("exposure_sub_times_us",    DWORD*5),         #Array with five sub exposures times in microseconds used by XI_TRG_SEL_MULTIPLE_EXPOSURES or hardware controlled HDR        #(see Note8)
        ("data_saturation",    DOUBLE),        #Pixel value of saturation
        ("wb_red",    FLOAT),        #Red coefficient of white balance
        ("wb_green",    FLOAT),        #Green coefficient of white balance
        ("wb_blue",    FLOAT),        #Blue coefficient of white balance
        ("lg_black_level",    DWORD),        #In case of multi gain channel readout, the black level low gain channel
        ("hg_black_level",    DWORD),        #In case of multi gain channel readout, the black level high gain channel
        ("lg_range",    DWORD),        #In case of multi gain channel readout, the valid range of low gain channel
        ("hg_range",    DWORD),        #In case of multi gain channel readout, the valid range of high gain channel
        ("gain_ratio",    FLOAT),        #Gain ratio for dual-channel modes (high_gain_channel/low_gain_channel). Unitless.
        ("fDownsamplingX",    FLOAT),        #Horizontal downsampling
        ("fDownsamplingY",    FLOAT),        #Vertical downsampling
        ("color_filter_array",    c_uint),        # Mosaic of tiny color filters placed over the pixel sensors of an image sensor.
        ("tof_phases_count",    DWORD),        #Number of phases of ToF sensor. E.g. 4 for four phases.
        ("tof_phase_id",    DWORD),        #Current phase ID of ToF sensor data starting from 1
        ("tof_multiple_phases_in_buffer",    DWORD),        #Is multiple phases in buffer (bp)
        ("data_sign_mode",    c_uint),        #Sign mode or signedness is a property of data.
        ("sequence_type",    c_uint),        #Type of image sequence (e.g. seq_disabled, seq_multiple_exposures)
        ("sequence_image_id",    DWORD),        #Current image in sequence. Starting from 1. Zero if sequence is disabled.
        ("sequence_length",    DWORD),        #Count of images in sequence. Zero for sequence disabled.
        ]


