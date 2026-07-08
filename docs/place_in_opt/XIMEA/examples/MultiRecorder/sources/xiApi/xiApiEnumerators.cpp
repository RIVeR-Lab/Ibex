
#include <xiApiEnumerators.h>
//-------------------------------------------------------------------------------------------------------------------
// 
// xiAPI enumerators
std::vector<xi_enum_item_t> XI_DOWNSAMPLING_VALUE_ITEMS;                            // Downsampling value enumerator.
std::vector<xi_enum_item_t> XI_TEST_PATTERN_GENERATOR_ITEMS;                        // Test Pattern Generator
std::vector<xi_enum_item_t> XI_VERSION_ITEMS;                                       // Module/Unit version selector
std::vector<xi_enum_item_t> XI_TEST_PATTERN_ITEMS;                                  // Test Pattern Type
std::vector<xi_enum_item_t> XI_DEC_PATTERN_ITEMS;                                   // Decimation Pattern Format
std::vector<xi_enum_item_t> XI_BIN_PATTERN_ITEMS;                                   // Binning Pattern Format
std::vector<xi_enum_item_t> XI_BIN_SELECTOR_ITEMS;                                  // Binning Engine Selector
std::vector<xi_enum_item_t> XI_BIN_MODE_ITEMS;                                      // Selects binning mode; to be used with
std::vector<xi_enum_item_t> XI_DEC_SELECTOR_ITEMS;                                  // Decimation Engine Selector
std::vector<xi_enum_item_t> XI_SENSOR_TAP_CNT_ITEMS;                                // Sensor tap count enumerator.
std::vector<xi_enum_item_t> XI_BIT_DEPTH_ITEMS;                                     // Bit depth enumerator.
std::vector<xi_enum_item_t> XI_DEBUG_LEVEL_ITEMS;                                   // Debug level enumerator.
std::vector<xi_enum_item_t> XI_IMG_FORMAT_ITEMS;                                    // Image output format enumerator.
std::vector<xi_enum_item_t> XI_COLOR_FILTER_ARRAY_ITEMS;                            // Bayer color matrix enumerator.
std::vector<xi_enum_item_t> XI_IMAGE_SEQUENCE_TYPE_ITEMS;                           // Image sequence type.
std::vector<xi_enum_item_t> XI_BP_ITEMS;                                            // structure containing information about buffer policy(can be safe, data will be copied to user/app buffer or unsafe, user will get internally allocated buffer without data copy).
std::vector<xi_enum_item_t> XI_TRG_SOURCE_ITEMS;                                    // structure containing information about trigger source
std::vector<xi_enum_item_t> XI_TRG_SELECTOR_ITEMS;                                  // structure containing information about trigger functionality
std::vector<xi_enum_item_t> XI_TRG_OVERLAP_ITEMS;                                   // Trigger overlap modes
std::vector<xi_enum_item_t> XI_ACQ_TIMING_MODE_ITEMS;                               // structure containing information about acquisition timing modes
std::vector<xi_enum_item_t> XI_TRANSPORT_DATA_TARGET_MODE_ITEMS;                    // Enumerator for data target modes
std::vector<xi_enum_item_t> XI_GPI_SEL_CB_ITEMS;                                    // Enumeration for XI_PRM_GPI_SELECTOR for CB cameras.
std::vector<xi_enum_item_t> XI_GPO_SEL_CB_ITEMS;                                    // Enumeration for XI_PRM_GPO_SELECTOR for CB cameras.
std::vector<xi_enum_item_t> XI_GPI_MODE_ITEMS;                                      // structure containing information about GPI functionality
std::vector<xi_enum_item_t> XI_GPI_SELECTOR_ITEMS;                                  // Enumerator for GPI port selection.
std::vector<xi_enum_item_t> XI_GPO_MODE_ITEMS;                                      // structure containing information about GPO functionality
std::vector<xi_enum_item_t> XI_GPO_SELECTOR_ITEMS;                                  // Enumerator for GPO port selection.
std::vector<xi_enum_item_t> XI_METADATA_SAMPLING_MODE_ITEMS;                        // Metadata sampling mode selector
std::vector<xi_enum_item_t> XI_LED_MODE_ITEMS;                                      // structure containing information about LED functionality
std::vector<xi_enum_item_t> XI_LED_SELECTOR_ITEMS;                                  // Enumerator for LED selection.
std::vector<xi_enum_item_t> XI_COUNTER_SELECTOR_ITEMS;                              // structure contains frames counter
std::vector<xi_enum_item_t> XI_TS_RST_MODE_ITEMS;                                   // structure containing information about timestamp reset arming
std::vector<xi_enum_item_t> XI_TS_RST_SOURCE_ITEMS;                                 // structure containing information about possible timestamp reset sources
std::vector<xi_enum_item_t> XI_PRM_TYPE_ITEMS;                                      // structure containing information about parameters type
std::vector<xi_enum_item_t> XI_SWITCH_ITEMS;                                        // Turn parameter On/Off
std::vector<xi_enum_item_t> XI_TEMP_SELECTOR_ITEMS;                                 // Temperature selector
std::vector<xi_enum_item_t> XI_TEMP_CTRL_MODE_SELECTOR_ITEMS;                       // Temperature selector
std::vector<xi_enum_item_t> XI_TEMP_ELEMENT_SELECTOR_ITEMS;                         // Temperature element selector
std::vector<xi_enum_item_t> XI_OUTPUT_DATA_PACKING_TYPE_ITEMS;                      // Data packing(grouping) types.
std::vector<xi_enum_item_t> XI_DOWNSAMPLING_TYPE_ITEMS;                             // Downsampling types
std::vector<xi_enum_item_t> XI_EXPOSURE_TIME_SELECTOR_TYPE_ITEMS;                   // Exposure time selector
std::vector<xi_enum_item_t> XI_INTERLINE_EXPOSURE_MODE_TYPE_ITEMS;                  // Interline exposure mode
std::vector<xi_enum_item_t> XI_GAIN_SELECTOR_TYPE_ITEMS;                            // Gain selector
std::vector<xi_enum_item_t> XI_SHUTTER_TYPE_ITEMS;                                  // Shutter mode types
std::vector<xi_enum_item_t> XI_CMS_MODE_ITEMS;                                      // structure containing information about CMS functionality
std::vector<xi_enum_item_t> XI_CMS_INTENT_ITEMS;                                    // structure containing information about ICC Intents
std::vector<xi_enum_item_t> XI_OPEN_BY_ITEMS;                                       // structure containing information about options for selection of camera before opening
std::vector<xi_enum_item_t> XI_LENS_FEATURE_ITEMS;                                  // Lens feature selector selects which feature will be accessed.
std::vector<xi_enum_item_t> XI_SENSOR_FEATURE_SELECTOR_ITEMS;                       // Sensor feature selector selects which feature will be accessed.
std::vector<xi_enum_item_t> XI_SENSOR_MODE_ITEMS;                                   // Camera sensor mode enumerator.
std::vector<xi_enum_item_t> XI_IMAGE_AREA_SELECTOR_ITEMS;                           // Defines image sensor area as output.
std::vector<xi_enum_item_t> XI_SENSOR_OUTPUT_CHANNEL_COUNT_ITEMS;                   // Camera channel count enumerator.
std::vector<xi_enum_item_t> XI_SENS_DEFFECTS_CORR_LIST_SELECTOR_ITEMS;              // Sensor defects correction list selector
std::vector<xi_enum_item_t> XI_ACQUISITION_STATUS_SELECTOR_ITEMS;                   // Acquisition status Selector
std::vector<xi_enum_item_t> XI_DP_UNIT_SELECTOR_ITEMS;                              // Select unit where data-pipe is configured
std::vector<xi_enum_item_t> XI_DP_PROC_SELECTOR_ITEMS;                              // Select unit processor
std::vector<xi_enum_item_t> XI_DP_PARAM_SELECTOR_ITEMS;                             // Select processor parameter
std::vector<xi_enum_item_t> XI_DP_PARAM_VALUE_ITEMS;                                // Select processor parameter value
std::vector<xi_enum_item_t> XI_USER_SET_SELECTOR_ITEMS;                             // User Set selector options.
std::vector<xi_enum_item_t> XI_DUAL_ADC_MODE_ITEMS;                                 // Mode of DualADC feature
std::vector<xi_enum_item_t> XI_PROBE_SELECTOR_ITEMS;                                // Probe Selector
std::vector<xi_enum_item_t> XI_TOF_READOUT_MODE_ITEMS;                              // ToF Readout Mode feature
std::vector<xi_enum_item_t> XI_DATA_SM_ITEMS;                                       // Image data signedness

std::vector<xi_enum_item_t>  XI_RET_ITEMS;
std::vector<xi_enum_item_t>  XI_PROC_RET_ITEMS;

//-------------------------------------------------------------------------------------------------------------------
void xiInitEnumerators()
{
// Downsampling value enumerator.
	XI_DOWNSAMPLING_VALUE_ITEMS.push_back({"XI_DWN_1x1",                             1, "1 sensor pixel = 1 image pixel"});
	XI_DOWNSAMPLING_VALUE_ITEMS.push_back({"XI_DWN_2x2",                             2, "2x2 sensor pixels = 1 image pixel"});
	XI_DOWNSAMPLING_VALUE_ITEMS.push_back({"XI_DWN_3x3",                             3, "Downsampling 3x3."});
	XI_DOWNSAMPLING_VALUE_ITEMS.push_back({"XI_DWN_4x4",                             4, "4x4 sensor pixels = 1 image pixel"});
	XI_DOWNSAMPLING_VALUE_ITEMS.push_back({"XI_DWN_5x5",                             5, "Downsampling 5x5."});
	XI_DOWNSAMPLING_VALUE_ITEMS.push_back({"XI_DWN_6x6",                             6, "Downsampling 6x6."});
	XI_DOWNSAMPLING_VALUE_ITEMS.push_back({"XI_DWN_7x7",                             7, "Downsampling 7x7."});
	XI_DOWNSAMPLING_VALUE_ITEMS.push_back({"XI_DWN_8x8",                             8, "Downsampling 8x8."});
	XI_DOWNSAMPLING_VALUE_ITEMS.push_back({"XI_DWN_9x9",                             9, "Downsampling 9x9."});
	XI_DOWNSAMPLING_VALUE_ITEMS.push_back({"XI_DWN_10x10",                           10, "Downsampling 10x10."});
	XI_DOWNSAMPLING_VALUE_ITEMS.push_back({"XI_DWN_16x16",                           16, "Downsampling 16x16."});

// Test Pattern Generator
	XI_TEST_PATTERN_GENERATOR_ITEMS.push_back({"XI_TESTPAT_GEN_SENSOR",                  0, "Sensor test pattern generator"});
	XI_TEST_PATTERN_GENERATOR_ITEMS.push_back({"XI_TESTPAT_GEN_FPGA",                    1, " FPGA Test Pattern Generator"});
	XI_TEST_PATTERN_GENERATOR_ITEMS.push_back({"XI_TESTPAT_GEN_MCU",                     2, " MCU Test Pattern Generator"});

// Module/Unit version selector
	XI_VERSION_ITEMS.push_back({"XI_VER_API",                             0, "version of API"});
	XI_VERSION_ITEMS.push_back({"XI_VER_DRV",                             1, "version of device driver"});
	XI_VERSION_ITEMS.push_back({"XI_VER_MCU1",                            2, "version of MCU1 firmware."});
	XI_VERSION_ITEMS.push_back({"XI_VER_MCU2",                            3, "version of MCU2 firmware."});
	XI_VERSION_ITEMS.push_back({"XI_VER_MCU3",                            4, "version of MCU3 firmware."});
	XI_VERSION_ITEMS.push_back({"XI_VER_FPGA1",                           5, "version of FPGA1 firmware."});
	XI_VERSION_ITEMS.push_back({"XI_VER_XMLMAN",                          6, "version of XML manifest."});
	XI_VERSION_ITEMS.push_back({"XI_VER_HW_REV",                          7, "version of hardware revision."});
	XI_VERSION_ITEMS.push_back({"XI_VER_FACTORY_SET",                     8, "version of factory set."});

// Test Pattern Type
	XI_TEST_PATTERN_ITEMS.push_back({"XI_TESTPAT_OFF",                         0, " Testpattern turned off."});
	XI_TEST_PATTERN_ITEMS.push_back({"XI_TESTPAT_BLACK",                       1, " Image is filled with darkest possible image."});
	XI_TEST_PATTERN_ITEMS.push_back({"XI_TESTPAT_WHITE",                       2, " Image is filled with brightest possible image."});
	XI_TEST_PATTERN_ITEMS.push_back({"XI_TESTPAT_GREY_HORIZ_RAMP",             3, " Image is filled horizontally with an image that goes from the darkest possible value to the brightest."});
	XI_TEST_PATTERN_ITEMS.push_back({"XI_TESTPAT_GREY_VERT_RAMP",              4, " Image is filled vertically with an image that goes from the darkest possible value to the brightest."});
	XI_TEST_PATTERN_ITEMS.push_back({"XI_TESTPAT_GREY_HORIZ_RAMP_MOVING",      5, " Image is filled horizontally with an image that goes from the darkest possible value to the brightest and moves from left to right."});
	XI_TEST_PATTERN_ITEMS.push_back({"XI_TESTPAT_GREY_VERT_RAMP_MOVING",       6, " Image is filled vertically with an image that goes from the darkest possible value to the brightest and moves from left to right."});
	XI_TEST_PATTERN_ITEMS.push_back({"XI_TESTPAT_HORIZ_LINE_MOVING",           7, " A moving horizontal line is superimposed on the live image."});
	XI_TEST_PATTERN_ITEMS.push_back({"XI_TESTPAT_VERT_LINE_MOVING",            8, " A moving vertical line is superimposed on the live image."});
	XI_TEST_PATTERN_ITEMS.push_back({"XI_TESTPAT_COLOR_BAR",                   9, " Image is filled with stripes of color including White, Black, Red, Green, Blue, Cyan, Magenta and Yellow."});
	XI_TEST_PATTERN_ITEMS.push_back({"XI_TESTPAT_FRAME_COUNTER",               10, " A frame counter is superimposed on the live image."});
	XI_TEST_PATTERN_ITEMS.push_back({"XI_TESTPAT_DEVICE_SPEC_COUNTER",         11, " 128bit counter."});

// Decimation Pattern Format
	XI_DEC_PATTERN_ITEMS.push_back({"XI_DEC_MONO",                            1, "adjacent pixels are decimated"});
	XI_DEC_PATTERN_ITEMS.push_back({"XI_DEC_BAYER",                           2, "	Bayer pattern is preserved during pixel decimation"});

// Binning Pattern Format
	XI_BIN_PATTERN_ITEMS.push_back({"XI_BIN_MONO",                            1, "adjacent pixels are combined"});
	XI_BIN_PATTERN_ITEMS.push_back({"XI_BIN_BAYER",                           2, "Bayer pattern is preserved during pixel combining"});

// Binning Engine Selector
	XI_BIN_SELECTOR_ITEMS.push_back({"XI_BIN_SELECT_SENSOR",                   0, "parameters for image sensor binning are selected"});
	XI_BIN_SELECTOR_ITEMS.push_back({"XI_BIN_SELECT_DEVICE_FPGA",              1, "parameters for device (camera) FPGA decimation are selected"});
	XI_BIN_SELECTOR_ITEMS.push_back({"XI_BIN_SELECT_HOST_CPU",                 2, "parameters for Host CPU binning are selected"});

// Selects binning mode; to be used with
	XI_BIN_MODE_ITEMS.push_back({"XI_BIN_MODE_SUM",                        0, "The response from the combined pixels will be added, resulting in increased sensitivity."});
	XI_BIN_MODE_ITEMS.push_back({"XI_BIN_MODE_AVERAGE",                    1, "The response from the combined pixels will be averaged, resulting in increased signal/noise ratio."});

// Decimation Engine Selector
	XI_DEC_SELECTOR_ITEMS.push_back({"XI_DEC_SELECT_SENSOR",                   0, "parameters for image sensor decimation are selected"});
	XI_DEC_SELECTOR_ITEMS.push_back({"XI_DEC_SELECT_DEVICE_FPGA",              1, "parameters for device (camera) FPGA decimation are selected"});
	XI_DEC_SELECTOR_ITEMS.push_back({"XI_DEC_SELECT_HOST_CPU",                 2, "parameters for Host CPU decimation are selected"});

// Sensor tap count enumerator.
	XI_SENSOR_TAP_CNT_ITEMS.push_back({"XI_TAP_CNT_1",                           1, "1 sensor tap selected."});
	XI_SENSOR_TAP_CNT_ITEMS.push_back({"XI_TAP_CNT_2",                           2, "2 sensor taps selected."});
	XI_SENSOR_TAP_CNT_ITEMS.push_back({"XI_TAP_CNT_4",                           4, "4 sensor taps selected."});

// Bit depth enumerator.
	XI_BIT_DEPTH_ITEMS.push_back({"XI_BPP_8",                               8, "8 bit per pixel"});
	XI_BIT_DEPTH_ITEMS.push_back({"XI_BPP_9",                               9, "9 bit per pixel"});
	XI_BIT_DEPTH_ITEMS.push_back({"XI_BPP_10",                              10, "10 bit per pixel"});
	XI_BIT_DEPTH_ITEMS.push_back({"XI_BPP_11",                              11, "11 bit per pixel"});
	XI_BIT_DEPTH_ITEMS.push_back({"XI_BPP_12",                              12, "12 bit per pixel"});
	XI_BIT_DEPTH_ITEMS.push_back({"XI_BPP_13",                              13, "13 bit per pixel"});
	XI_BIT_DEPTH_ITEMS.push_back({"XI_BPP_14",                              14, "14 bit per pixel"});
	XI_BIT_DEPTH_ITEMS.push_back({"XI_BPP_15",                              15, "15 bit per pixel"});
	XI_BIT_DEPTH_ITEMS.push_back({"XI_BPP_16",                              16, "16 bit per pixel"});
	XI_BIT_DEPTH_ITEMS.push_back({"XI_BPP_24",                              24, "24 bit per pixel"});
	XI_BIT_DEPTH_ITEMS.push_back({"XI_BPP_32",                              32, "32 bit per pixel"});

// Debug level enumerator.
	XI_DEBUG_LEVEL_ITEMS.push_back({"XI_DL_DETAIL",                           0, "(see Note1)"});
	XI_DEBUG_LEVEL_ITEMS.push_back({"XI_DL_TRACE",                            1, "Prints errors, warnings and important informations"});
	XI_DEBUG_LEVEL_ITEMS.push_back({"XI_DL_WARNING",                          2, "Prints all errors and warnings"});
	XI_DEBUG_LEVEL_ITEMS.push_back({"XI_DL_ERROR",                            3, "Prints all errors"});
	XI_DEBUG_LEVEL_ITEMS.push_back({"XI_DL_FATAL",                            4, "Prints only important errors"});
	XI_DEBUG_LEVEL_ITEMS.push_back({"XI_DL_DISABLED",                         100, "Prints no messages"});

// Image output format enumerator.
	XI_IMG_FORMAT_ITEMS.push_back({"XI_MONO8",                               0, "8 bits per pixel. 	[Intensity] (see Note5,Note6)"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_MONO16",                              1, "16 bits per pixel. [Intensity LSB] [Intensity MSB] (see Note5,Note6)"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RGB24",                               2, "RGB data format. [Blue][Green][Red] (see Note5)"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RGB32",                               3, "RGBA data format. 	[Blue][Green][Red][0] (see Note5)"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RGB_PLANAR",                          4, "RGB planar data format. [Red][Red]...[Green][Green]...[Blue][Blue]... (see Note5)"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RAW8",                                5, "8 bits per pixel raw data from sensor. 	[pixel byte] raw data from transport (camera output)"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RAW16",                               6, "16 bits per pixel raw data from sensor. 	[pixel byte low] [pixel byte high] 16 bits (depacked) raw data"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_FRM_TRANSPORT_DATA",                  7, "Data from transport layer (e.g. packed). Depends on data on the transport layer (see Note7)"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RGB48",                               8, "RGB data format. [Blue low byte][Blue high byte][Green low][Green high][Red low][Red high] (see Note5)"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RGB64",                               9, "RGBA data format. [Blue low byte][Blue high byte][Green low][Green high][Red low][Red high][0][0] (Note5)"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RGB16_PLANAR",                        10, "RGB16 planar data format"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RAW8X2",                              11, "8 bits per pixel raw data from sensor(2 components in a row). [ch1 pixel byte] [ch2 pixel byte] 8 bits raw data from 2 channels (e.g. high gain and low gain channels of sCMOS cameras)"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RAW8X4",                              12, "8 bits per pixel raw data from sensor(4 components in a row). 	[ch1 pixel byte [ch2 pixel byte] [ch3 pixel byte] [ch4 pixel byte] 8 bits raw data from 4 channels (e.g. sCMOS cameras)"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RAW16X2",                             13, "16 bits per pixel raw data from sensor(2 components in a row). 	[ch1 pixel byte low] [ch1 pixel byte high] [ch2 pixel byte low] [ch2 pixel byte high] 16 bits (depacked) raw data from 2 channels (e.g. high gain and low gain channels of sCMOS cameras)"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RAW16X4",                             14, "16 bits per pixel raw data from sensor(4 components in a row). 	[ch1 pixel byte low] [ch1 pixel byte high] [ch2 pixel byte low] [ch2 pixel byte high] [ch3 pixel byte low] [ch3 pixel byte high] [ch4 pixel byte low] [ch4 pixel byte high] 16 bits (depacked) raw data from 4 channels (e.g. sCMOS cameras)"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RAW32",                               15, "32 bits per pixel raw data from sensor in integer format (LSB first). 4 bytes (LSB first) pixel (depacked) raw data"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RAW32FLOAT",                          16, "32 bits per pixel raw data from sensor in single-precision floating point format. 4 bytes per pixel (depacked) raw data"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RAW8X3",                              17, "8 bits per pixel raw data from sensor(3 components in a row). 	[ch1 pixel byte] [ch2 pixel byte] [ch3 pixel byte] raw data from 3 channels (e.g. ToF cameras)"});
	XI_IMG_FORMAT_ITEMS.push_back({"XI_RAW16X3",                             18, "16 bits per pixel raw data from sensor(3 components in a row). [ch1 pixel byte low] [ch1 pixel byte high] [ch2 pixel byte low] [ch2 pixel byte high] [ch3 pixel byte low] [ch3 pixel byte high] 16 bits (depacked) raw data from 3 channels (e.g. ToF cameras)"});

// Bayer color matrix enumerator.
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_NONE",                            0, "Result pixels have no filters applied in this format"});
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_BAYER_RGGB",                      1, "Regular RGGB"});
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_CMYG",                            2, "AK Sony sens"});
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_RGR",                             3, "2R+G readout"});
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_BAYER_BGGR",                      4, "BGGR readout"});
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_BAYER_GRBG",                      5, "GRBG readout"});
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_BAYER_GBRG",                      6, "GBRG readout"});
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_POLAR_A_BAYER_BGGR",              7, "BGGR polarized 4x4 macropixel"});
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_POLAR_A",                         8, "Polarized 2x2 macropixel"});
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_TOF_ANB",                         9, "ToF A and B"});
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_TOF_AMB",                         10, "ToF A minus B"});
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_TOF_APB",                         11, "ToF A plus B"});
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_TOF_A",                           12, "ToF A only"});
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_TOF_B",                           13, "ToF B only"});
	XI_COLOR_FILTER_ARRAY_ITEMS.push_back({"XI_CFA_TOF_X1X2X3",                      14, "ToF X1,X2,X3"});

// Image sequence type.
	XI_IMAGE_SEQUENCE_TYPE_ITEMS.push_back({"XI_IMAGE_SEQUENCE_TYPE_DISABLED",        0, "No image sequence."});
	XI_IMAGE_SEQUENCE_TYPE_ITEMS.push_back({"XI_IMAGE_SEQUENCE_TYPE_EXPOSURES",       1, "Image sequence off different exposures. E.g. for HDR."});

// structure containing information about buffer policy(can be safe, data will be copied to user/app buffer or unsafe, user will get internally allocated buffer without data copy).
	XI_BP_ITEMS.push_back({"XI_BP_UNSAFE",                           0, "User gets pointer to internally allocated circle buffer and data may be overwritten by device."});
	XI_BP_ITEMS.push_back({"XI_BP_SAFE",                             1, "Data from device will be copied to user allocated buffer or xiApi allocated memory."});

// structure containing information about trigger source
	XI_TRG_SOURCE_ITEMS.push_back({"XI_TRG_OFF",                             0, "Capture of next image is automatically started after previous."});
	XI_TRG_SOURCE_ITEMS.push_back({"XI_TRG_EDGE_RISING",                     1, "Capture is started on rising edge of selected input."});
	XI_TRG_SOURCE_ITEMS.push_back({"XI_TRG_EDGE_FALLING",                    2, "Capture is started on falling edge of selected input"});
	XI_TRG_SOURCE_ITEMS.push_back({"XI_TRG_SOFTWARE",                        3, "Capture is started with software trigger."});
	XI_TRG_SOURCE_ITEMS.push_back({"XI_TRG_LEVEL_HIGH",                      4, "Specifies that the trigger is considered valid as long as the level of the source signal is high."});
	XI_TRG_SOURCE_ITEMS.push_back({"XI_TRG_LEVEL_LOW",                       5, "Specifies that the trigger is considered valid as long as the level of the source signal is low."});

// structure containing information about trigger functionality
	XI_TRG_SELECTOR_ITEMS.push_back({"XI_TRG_SEL_FRAME_START",                 0, "Trigger starts the capture of one frame"});
	XI_TRG_SELECTOR_ITEMS.push_back({"XI_TRG_SEL_EXPOSURE_ACTIVE",             1, "Trigger controls the start and length of the exposure."});
	XI_TRG_SELECTOR_ITEMS.push_back({"XI_TRG_SEL_FRAME_BURST_START",           2, "Trigger starts the capture of the bursts of frames in an acquisition."});
	XI_TRG_SELECTOR_ITEMS.push_back({"XI_TRG_SEL_FRAME_BURST_ACTIVE",          3, "Trigger controls the duration of the capture of the bursts of frames in an acquisition."});
	XI_TRG_SELECTOR_ITEMS.push_back({"XI_TRG_SEL_MULTIPLE_EXPOSURES",          4, "Trigger which when first trigger starts exposure and consequent pulses are gating exposure(active HI)"});
	XI_TRG_SELECTOR_ITEMS.push_back({"XI_TRG_SEL_EXPOSURE_START",              5, "Trigger controls the start of the exposure of one Frame."});
	XI_TRG_SELECTOR_ITEMS.push_back({"XI_TRG_SEL_MULTI_SLOPE_PHASE_CHANGE",    6, "Trigger controls the multi slope phase in one Frame (phase0 -> phase1) or (phase1 -> phase2)."});
	XI_TRG_SELECTOR_ITEMS.push_back({"XI_TRG_SEL_ACQUISITION_START",           7, "Selects a trigger that starts the Acquisition."});

// Trigger overlap modes
	XI_TRG_OVERLAP_ITEMS.push_back({"XI_TRG_OVERLAP_OFF",                     0, "No trigger overlap is permitted."});
	XI_TRG_OVERLAP_ITEMS.push_back({"XI_TRG_OVERLAP_READ_OUT",                1, "Trigger is accepted immediately after the exposure period. (see Note1)"});
	XI_TRG_OVERLAP_ITEMS.push_back({"XI_TRG_OVERLAP_PREV_FRAME",              2, "Trigger is accepted (latched) at any time during the capture of the previous frame."});

// structure containing information about acquisition timing modes
	XI_ACQ_TIMING_MODE_ITEMS.push_back({"XI_ACQ_TIMING_MODE_FREE_RUN",            0, "camera acquires images at a maximum possible framerate"});
	XI_ACQ_TIMING_MODE_ITEMS.push_back({"XI_ACQ_TIMING_MODE_FRAME_RATE",          1, "Selects a mode when sensor frame acquisition frequency is set to parameter FRAMERATE"});
	XI_ACQ_TIMING_MODE_ITEMS.push_back({"XI_ACQ_TIMING_MODE_FRAME_RATE_LIMIT",    2, "Selects a mode when sensor frame acquisition frequency is limited by parameter FRAMERATE"});

// Enumerator for data target modes
	XI_TRANSPORT_DATA_TARGET_MODE_ITEMS.push_back({"XI_TRANSPORT_DATA_TARGET_CPU_RAM",       0, "normal CPU memory buffer is used for image data"});
	XI_TRANSPORT_DATA_TARGET_MODE_ITEMS.push_back({"XI_TRANSPORT_DATA_TARGET_GPU_RAM",       1, "data is delivered straight to GPU memory using GPUDirect technology"});
	XI_TRANSPORT_DATA_TARGET_MODE_ITEMS.push_back({"XI_TRANSPORT_DATA_TARGET_UNIFIED",       2, "CUDA managed memory is used for image data."});
	XI_TRANSPORT_DATA_TARGET_MODE_ITEMS.push_back({"XI_TRANSPORT_DATA_TARGET_ZEROCOPY",      3, "CUDA zerocopy memory is used for image data."});

// Enumeration for XI_PRM_GPI_SELECTOR for CB cameras.
	XI_GPI_SEL_CB_ITEMS.push_back({"XI_GPI_SEL_CB_IN1",                      1, "Input1 - Pin3 (Opto Isolated)."});
	XI_GPI_SEL_CB_ITEMS.push_back({"XI_GPI_SEL_CB_IN2",                      2, "Input2 - Pin4 (Opto Isolated)."});
	XI_GPI_SEL_CB_ITEMS.push_back({"XI_GPI_SEL_CB_INOUT1",                   3, "Input/Output1 - Pin6"});
	XI_GPI_SEL_CB_ITEMS.push_back({"XI_GPI_SEL_CB_INOUT2",                   4, "Input/Output2 - Pin7"});
	XI_GPI_SEL_CB_ITEMS.push_back({"XI_GPI_SEL_CB_INOUT3",                   5, "Input/Output3 - Pin11"});
	XI_GPI_SEL_CB_ITEMS.push_back({"XI_GPI_SEL_CB_INOUT4",                   6, "Input/Output4 - Pin12"});

// Enumeration for XI_PRM_GPO_SELECTOR for CB cameras.
	XI_GPO_SEL_CB_ITEMS.push_back({"XI_GPO_SEL_CB_OUT1",                     1, "Output1 - Pin8 (Opto Isolated)."});
	XI_GPO_SEL_CB_ITEMS.push_back({"XI_GPO_SEL_CB_OUT2",                     2, "Output2 - Pin9 (Opto Isolated)."});
	XI_GPO_SEL_CB_ITEMS.push_back({"XI_GPO_SEL_CB_INOUT1",                   3, "Input/Output1 - Pin6"});
	XI_GPO_SEL_CB_ITEMS.push_back({"XI_GPO_SEL_CB_INOUT2",                   4, "Input/Output2 - Pin7"});
	XI_GPO_SEL_CB_ITEMS.push_back({"XI_GPO_SEL_CB_INOUT3",                   5, "Input/Output3 - Pin11"});
	XI_GPO_SEL_CB_ITEMS.push_back({"XI_GPO_SEL_CB_INOUT4",                   6, "Input/Output4 - Pin12"});

// structure containing information about GPI functionality
	XI_GPI_MODE_ITEMS.push_back({"XI_GPI_OFF",                             0, "Input is not used for triggering, but can be used to get parameter GPI_LEVEL. This can be used to switch I/O line on some cameras to input mode."});
	XI_GPI_MODE_ITEMS.push_back({"XI_GPI_TRIGGER",                         1, "Input can be used for triggering."});
	XI_GPI_MODE_ITEMS.push_back({"XI_GPI_EXT_EVENT",                       2, "External signal input (not implemented)"});

// Enumerator for GPI port selection.
	XI_GPI_SELECTOR_ITEMS.push_back({"XI_GPI_PORT1",                           1, "GPI port 1"});
	XI_GPI_SELECTOR_ITEMS.push_back({"XI_GPI_PORT2",                           2, "GPI port 2"});
	XI_GPI_SELECTOR_ITEMS.push_back({"XI_GPI_PORT3",                           3, "GPI port 3"});
	XI_GPI_SELECTOR_ITEMS.push_back({"XI_GPI_PORT4",                           4, "GPI port 4"});
	XI_GPI_SELECTOR_ITEMS.push_back({"XI_GPI_PORT5",                           5, "GPI port 5"});
	XI_GPI_SELECTOR_ITEMS.push_back({"XI_GPI_PORT6",                           6, "GPI port 6"});
	XI_GPI_SELECTOR_ITEMS.push_back({"XI_GPI_PORT7",                           7, "GPI port 7"});
	XI_GPI_SELECTOR_ITEMS.push_back({"XI_GPI_PORT8",                           8, "GPI port 8"});
	XI_GPI_SELECTOR_ITEMS.push_back({"XI_GPI_PORT9",                           9, "GPI port 9"});
	XI_GPI_SELECTOR_ITEMS.push_back({"XI_GPI_PORT10",                          10, "GPI port 10"});
	XI_GPI_SELECTOR_ITEMS.push_back({"XI_GPI_PORT11",                          11, "GPI port 11"});
	XI_GPI_SELECTOR_ITEMS.push_back({"XI_GPI_PORT12",                          12, "GPI port 12"});

// structure containing information about GPO functionality
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_OFF",                             0, "Output is off (zero voltage or switched_off)"});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_ON",                              1, "Output is on (voltage or switched_on)"});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_FRAME_ACTIVE",                    2, "Output is on while frame exposure,read,transfer."});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_FRAME_ACTIVE_NEG",                3, "Output is off while frame exposure,read,transfer."});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_EXPOSURE_ACTIVE",                 4, "Output is on while frame exposure"});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_EXPOSURE_ACTIVE_NEG",             5, "Output is off while frame exposure"});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_FRAME_TRIGGER_WAIT",              6, "Output is on while camera is ready for trigger"});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_FRAME_TRIGGER_WAIT_NEG",          7, "Output is off while camera is ready for trigger."});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_EXPOSURE_PULSE",                  8, "Output is on short pulse at the beginning of frame exposure."});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_EXPOSURE_PULSE_NEG",              9, "Output is off short pulse at the beginning of frame exposure."});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_BUSY",                            10, "Output is on when camera has received trigger until end of transfer"});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_BUSY_NEG",                        11, "Output is off when camera has received trigger until end of transfer"});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_HIGH_IMPEDANCE",                  12, "Associated pin is in high impedance (tri-stated) and can be driven externally. E.g. for triggering or reading status by GPI_LEVEL."});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_FRAME_BUFFER_OVERFLOW",           13, "Frame buffer overflow status."});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_EXPOSURE_ACTIVE_FIRST_ROW",       14, "Output is on while the first row exposure."});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_EXPOSURE_ACTIVE_FIRST_ROW_NEG",   15, "Output is off while the first row exposure."});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_EXPOSURE_ACTIVE_ALL_ROWS",        16, "Output is on while all rows exposure together."});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_EXPOSURE_ACTIVE_ALL_ROWS_NEG",    17, "Output is off while all rows exposure together."});
	XI_GPO_MODE_ITEMS.push_back({"XI_GPO_TXD",                             18, "Output is connected to TXD of UART module"});

// Enumerator for GPO port selection.
	XI_GPO_SELECTOR_ITEMS.push_back({"XI_GPO_PORT1",                           1, "GPO port 1"});
	XI_GPO_SELECTOR_ITEMS.push_back({"XI_GPO_PORT2",                           2, "GPO port 2"});
	XI_GPO_SELECTOR_ITEMS.push_back({"XI_GPO_PORT3",                           3, "GPO port 3"});
	XI_GPO_SELECTOR_ITEMS.push_back({"XI_GPO_PORT4",                           4, "GPO port 4"});
	XI_GPO_SELECTOR_ITEMS.push_back({"XI_GPO_PORT5",                           5, "GPO port 5"});
	XI_GPO_SELECTOR_ITEMS.push_back({"XI_GPO_PORT6",                           6, "GPO port 6"});
	XI_GPO_SELECTOR_ITEMS.push_back({"XI_GPO_PORT7",                           7, "GPO port 7"});
	XI_GPO_SELECTOR_ITEMS.push_back({"XI_GPO_PORT8",                           8, "GPO port 8"});
	XI_GPO_SELECTOR_ITEMS.push_back({"XI_GPO_PORT9",                           9, "GPO port 9"});
	XI_GPO_SELECTOR_ITEMS.push_back({"XI_GPO_PORT10",                          10, "GPO port 10"});
	XI_GPO_SELECTOR_ITEMS.push_back({"XI_GPO_PORT11",                          11, "GPO port 11"});
	XI_GPO_SELECTOR_ITEMS.push_back({"XI_GPO_PORT12",                          12, "GPO port 12"});

// Metadata sampling mode selector
	XI_METADATA_SAMPLING_MODE_ITEMS.push_back({"XI_METADATA_SAMPLING_OFF",               0, "Metadata sampling turned off"});
	XI_METADATA_SAMPLING_MODE_ITEMS.push_back({"XI_METADATA_SAMPLING_POINT_EXPOSURE_START",1, "Selects the sampling point to exposure start"});
	XI_METADATA_SAMPLING_MODE_ITEMS.push_back({"XI_METADATA_SAMPLING_POINT_EXPOSURE_END",2, "Selects the sampling point to exposure end"});
	XI_METADATA_SAMPLING_MODE_ITEMS.push_back({"XI_METADATA_SAMPLING_POINT_EXPOSURE_START_END",3, "Selects the sampling point to exposure start and end"});
	XI_METADATA_SAMPLING_MODE_ITEMS.push_back({"XI_METADATA_SAMPLING_POINT_READOUT_END", 4, "Selects the sampling point to readout end"});

// structure containing information about LED functionality
	XI_LED_MODE_ITEMS.push_back({"XI_LED_HEARTBEAT",                       0, "Set led to blink (1 Hz) if link is OK."});
	XI_LED_MODE_ITEMS.push_back({"XI_LED_TRIGGER_ACTIVE",                  1, "Set led to blink if trigger detected."});
	XI_LED_MODE_ITEMS.push_back({"XI_LED_EXT_EVENT_ACTIVE",                2, "Set led to blink if external signal detected."});
	XI_LED_MODE_ITEMS.push_back({"XI_LED_LINK",                            3, "Set led to blink if link is OK."});
	XI_LED_MODE_ITEMS.push_back({"XI_LED_ACQUISITION",                     4, "Set led to blink if data streaming"});
	XI_LED_MODE_ITEMS.push_back({"XI_LED_EXPOSURE_ACTIVE",                 5, "Set led to blink if sensor integration time."});
	XI_LED_MODE_ITEMS.push_back({"XI_LED_FRAME_ACTIVE",                    6, "Set led to blink if device busy/not busy."});
	XI_LED_MODE_ITEMS.push_back({"XI_LED_OFF",                             7, "Set led to off."});
	XI_LED_MODE_ITEMS.push_back({"XI_LED_ON",                              8, "Set led to on."});
	XI_LED_MODE_ITEMS.push_back({"XI_LED_BLINK",                           9, "Blinking (1Hz)."});

// Enumerator for LED selection.
	XI_LED_SELECTOR_ITEMS.push_back({"XI_LED_SEL1",                            1, "LED 1"});
	XI_LED_SELECTOR_ITEMS.push_back({"XI_LED_SEL2",                            2, "LED 2"});
	XI_LED_SELECTOR_ITEMS.push_back({"XI_LED_SEL3",                            3, "LED 3"});
	XI_LED_SELECTOR_ITEMS.push_back({"XI_LED_SEL4",                            4, "LED 4"});
	XI_LED_SELECTOR_ITEMS.push_back({"XI_LED_SEL5",                            5, "LED 5"});

// structure contains frames counter
	XI_COUNTER_SELECTOR_ITEMS.push_back({"XI_CNT_SEL_TRANSPORT_SKIPPED_FRAMES",    0, "Number of skipped frames on transport layer (e.g. when image gets lost while transmission). Occur when capacity of transport channel does not allow to transfer all data."});
	XI_COUNTER_SELECTOR_ITEMS.push_back({"XI_CNT_SEL_API_SKIPPED_FRAMES",          1, "Number of skipped frames on API layer. Occur when application does not process the images as quick as they are received from the camera."});
	XI_COUNTER_SELECTOR_ITEMS.push_back({"XI_CNT_SEL_TRANSPORT_TRANSFERRED_FRAMES",2, "Number of delivered buffers since last acquisition start."});
	XI_COUNTER_SELECTOR_ITEMS.push_back({"XI_CNT_SEL_FRAME_MISSED_TRIGGER_DUETO_OVERLAP",3, "Number of missed triggers overlapped with exposure or read-out stage of previous frame – see XI_PRM_TRG_OVERLAP. (see Note1)"});
	XI_COUNTER_SELECTOR_ITEMS.push_back({"XI_CNT_SEL_FRAME_MISSED_TRIGGER_DUETO_FRAME_BUFFER_OVR",4, "Number of missed triggers due to frame buffer full. (see Note1)"});
	XI_COUNTER_SELECTOR_ITEMS.push_back({"XI_CNT_SEL_FRAME_BUFFER_OVERFLOW",       5, "Internal camera frame buffer memory (RAM) full events counter. It can be incremented multiple times per one frame. (see Note1)"});
	XI_COUNTER_SELECTOR_ITEMS.push_back({"XI_CNT_SEL_TRANSPORT_QUEUE_UNDERRUN",    6, "Incremented when camera starts to transfer new image, however no target buffer is queued in the transport queue. Connected to GenTL.STREAM_INFO_NUM_UNDERRUN. (see Note1)"});
	XI_COUNTER_SELECTOR_ITEMS.push_back({"XI_CNT_SEL_ACQUISITION_AUTO_RESTARTED_ON_FAILURE",7, "Acquisition can be restarted, due to failures on bus"});

// structure containing information about timestamp reset arming
	XI_TS_RST_MODE_ITEMS.push_back({"XI_TS_RST_ARM_ONCE",                     0, "Engine is disabled after TimeStamp has been reset after selected event."});
	XI_TS_RST_MODE_ITEMS.push_back({"XI_TS_RST_ARM_PERSIST",                  1, "Engine is armed permanently so each selected event will trigger TimeStamp reset. "});

// structure containing information about possible timestamp reset sources
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_OFF",                          0, "No source selected TimeStamp reset is not armed."});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_1",                    1, "GPI1 rising edge is active (signal after de-bounce module)"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_2",                    2, "GPI2 rising edge is active"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_3",                    3, "GPI3 rising edge is active"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_4",                    4, "GPI4 rising edge is active"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_1_INV",                5, "GPI1 falling edge is active"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_2_INV",                6, "GPI2 falling edge is active"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_3_INV",                7, "GPI3 falling edge is active"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_4_INV",                8, "GPI4 falling edge is active"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPO_1",                    9, "TimeStamp reset source selected GPO1"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPO_2",                    10, "TimeStamp reset source selected GPO2"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPO_3",                    11, "TimeStamp reset source selected GPO3"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPO_4",                    12, "TimeStamp reset source selected GPO4"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPO_1_INV",                13, "TimeStamp reset source selected GPO1 inverted"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPO_2_INV",                14, "TimeStamp reset source selected GPO2 inverted"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPO_3_INV",                15, "TimeStamp reset source selected GPO3 inverted"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPO_4_INV",                16, "TimeStamp reset source selected GPO4 inverted"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_TRIGGER",                  17, "TRIGGER to sensor rising edge is active"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_TRIGGER_INV",              18, "TRIGGER to sensor rising edge is active"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_SW",                       19, "TRIGGER to sensor rising edge is active. TimeStamp is reset by software take effect imminently."});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_EXPACTIVE",                20, "Exposure Active signal rising edge "});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_EXPACTIVE_INV",            21, "Exposure Active signal falling edge "});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_FVAL",                     22, "Frame valid signal rising edge (internal signal in camera)"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_FVAL_INV",                 23, "Frame valid signal falling edge (internal signal in camera)"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_5",                    24, "GPI5 rising edge is active"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_6",                    25, "GPI6 rising edge is active"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_5_INV",                26, "GPI5 falling edge is active"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_6_INV",                27, "GPI6 falling edge is active"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_7",                    28, "TimeStamp reset source selected GPI7 (after de bounce)"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_8",                    29, "TimeStamp reset source selected GPI8 (after de bounce)"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_9",                    30, "TimeStamp reset source selected GPI9 (after de bounce)"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_10",                   31, "TimeStamp reset source selected GPI10 (after de bounce)"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_11",                   32, "TimeStamp reset source selected GPI11 (after de bounce)"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_7_INV",                33, "TimeStamp reset source selected GPI7 inverted (after de bounce)"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_8_INV",                34, "TimeStamp reset source selected GPI8 inverted (after de bounce)"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_9_INV",                35, "TimeStamp reset source selected GPI9 inverted (after de bounce)"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_10_INV",               36, "TimeStamp reset source selected GPI10 inverted (after de bounce)"});
	XI_TS_RST_SOURCE_ITEMS.push_back({"XI_TS_RST_SRC_GPI_11_INV",               37, "TimeStamp reset source selected GPI11 inverted (after de bounce)"});

// structure containing information about parameters type
	XI_PRM_TYPE_ITEMS.push_back({"xiTypeInteger",                          0, "integer parameter type"});
	XI_PRM_TYPE_ITEMS.push_back({"xiTypeFloat",                            1, "float parameter type"});
	XI_PRM_TYPE_ITEMS.push_back({"xiTypeString",                           2, "string parameter type"});
	XI_PRM_TYPE_ITEMS.push_back({"xiTypeEnum",                             3, "enumerator parameter type"});
	XI_PRM_TYPE_ITEMS.push_back({"xiTypeBoolean",                          4, "boolean parameter type"});
	XI_PRM_TYPE_ITEMS.push_back({"xiTypeCommand",                          5, "command parameter type"});
	XI_PRM_TYPE_ITEMS.push_back({"xiTypeInteger64",                        6, "64bit integer parameter type"});

// Turn parameter On/Off
	XI_SWITCH_ITEMS.push_back({"XI_OFF",                                 0, "Turn parameter off"});
	XI_SWITCH_ITEMS.push_back({"XI_ON",                                  1, "Turn parameter on"});

// Temperature selector
	XI_TEMP_SELECTOR_ITEMS.push_back({"XI_TEMP_IMAGE_SENSOR_DIE_RAW",           0, "Image sensor die (non-calibrated)"});
	XI_TEMP_SELECTOR_ITEMS.push_back({"XI_TEMP_IMAGE_SENSOR_DIE",               1, "Image sensor die (calibrated)"});
	XI_TEMP_SELECTOR_ITEMS.push_back({"XI_TEMP_SENSOR_BOARD",                   2, "Image sensor PCB"});
	XI_TEMP_SELECTOR_ITEMS.push_back({"XI_TEMP_INTERFACE_BOARD",                3, "Data interface PCB"});
	XI_TEMP_SELECTOR_ITEMS.push_back({"XI_TEMP_FRONT_HOUSING",                  4, "Front part of camera housing"});
	XI_TEMP_SELECTOR_ITEMS.push_back({"XI_TEMP_REAR_HOUSING",                   5, "Rear part of camera housing"});
	XI_TEMP_SELECTOR_ITEMS.push_back({"XI_TEMP_TEC1_COLD",                      6, "TEC1 cold side temperature"});
	XI_TEMP_SELECTOR_ITEMS.push_back({"XI_TEMP_TEC1_HOT",                       7, "TEC1 hot side temperature"});
	XI_TEMP_SELECTOR_ITEMS.push_back({"XI_TEMP_VCSEL_BOARD_A",                  8, "VCSEL board temperature "});

// Temperature selector
	XI_TEMP_CTRL_MODE_SELECTOR_ITEMS.push_back({"XI_TEMP_CTRL_MODE_OFF",                  0, "Controlling of elements (TEC/Peltier, Fans) is turned off"});
	XI_TEMP_CTRL_MODE_SELECTOR_ITEMS.push_back({"XI_TEMP_CTRL_MODE_AUTO",                 1, "Controlling of elements is performed automatically by API or camera in order to reach parameter TARGET_TEMP."});
	XI_TEMP_CTRL_MODE_SELECTOR_ITEMS.push_back({"XI_TEMP_CTRL_MODE_MANUAL",               2, "Controlling of elements is done manually by application."});

// Temperature element selector
	XI_TEMP_ELEMENT_SELECTOR_ITEMS.push_back({"XI_TEMP_ELEM_TEC1",                      11, "TEC1 = TEC/Peltier that is closest to the image sensor"});
	XI_TEMP_ELEMENT_SELECTOR_ITEMS.push_back({"XI_TEMP_ELEM_TEC2",                      12, "TEC2 = TEC/Peltier location depends on camera model"});
	XI_TEMP_ELEMENT_SELECTOR_ITEMS.push_back({"XI_TEMP_ELEM_FAN1",                      31, "Temperature element fan current or rotation (FAN1 = Fan)"});
	XI_TEMP_ELEMENT_SELECTOR_ITEMS.push_back({"XI_TEMP_ELEM_FAN1_THRS_TEMP",            32, "Temperature element fan start rotation threshold temperature"});

// Data packing(grouping) types.
	XI_OUTPUT_DATA_PACKING_TYPE_ITEMS.push_back({"XI_DATA_PACK_XI_GROUPING",               0, "Data grouping (10g160, 12g192, 14g224)."});
	XI_OUTPUT_DATA_PACKING_TYPE_ITEMS.push_back({"XI_DATA_PACK_PFNC_LSB_PACKING",          1, "Data packing (10p, 12p)"});

// Downsampling types
	XI_DOWNSAMPLING_TYPE_ITEMS.push_back({"XI_BINNING",                             0, "pixels are interpolated - better image"});
	XI_DOWNSAMPLING_TYPE_ITEMS.push_back({"XI_SKIPPING",                            1, "pixels are skipped - higher frame rate"});

// Exposure time selector
	XI_EXPOSURE_TIME_SELECTOR_TYPE_ITEMS.push_back({"XI_EXPOSURE_TIME_SELECTOR_COMMON",       0, "Selects the common Exposure Time"});
	XI_EXPOSURE_TIME_SELECTOR_TYPE_ITEMS.push_back({"XI_EXPOSURE_TIME_SELECTOR_GROUP1",       1, "Selects the common Exposure Time for pixel group 1 (for InterlineExposureMode)"});
	XI_EXPOSURE_TIME_SELECTOR_TYPE_ITEMS.push_back({"XI_EXPOSURE_TIME_SELECTOR_GROUP2",       2, "Selects the common Exposure Time for pixel group 2 (for InterlineExposureMode)"});
	XI_EXPOSURE_TIME_SELECTOR_TYPE_ITEMS.push_back({"XI_EXPOSURE_TIME_SELECTOR_DUAL_TRG_EXP_ZONE_1",3, "Selects the Exposure Time for Zone 1 (for Dual Trigger Exposure feature)"});
	XI_EXPOSURE_TIME_SELECTOR_TYPE_ITEMS.push_back({"XI_EXPOSURE_TIME_SELECTOR_DUAL_TRG_EXP_ZONE_2",4, "Selects the Exposure Time for Zone 2 (for Dual Trigger Exposure feature)"});

// Interline exposure mode
	XI_INTERLINE_EXPOSURE_MODE_TYPE_ITEMS.push_back({"XI_INTERLINE_EXPOSURE_MODE_OFF",         0, "Disabled"});
	XI_INTERLINE_EXPOSURE_MODE_TYPE_ITEMS.push_back({"XI_INTERLINE_EXPOSURE_MODE_ON",          1, "Enabled"});

// Gain selector
	XI_GAIN_SELECTOR_TYPE_ITEMS.push_back({"XI_GAIN_SELECTOR_ALL",                   0, "Gain selector selects all channels. Implementation of gain type depends on camera."});
	XI_GAIN_SELECTOR_TYPE_ITEMS.push_back({"XI_GAIN_SELECTOR_ANALOG_ALL",            1, "Gain selector selects all analog channels. This is available only on some cameras."});
	XI_GAIN_SELECTOR_TYPE_ITEMS.push_back({"XI_GAIN_SELECTOR_DIGITAL_ALL",           2, "Gain selector selects all digital channels. This is available only on some cameras."});
	XI_GAIN_SELECTOR_TYPE_ITEMS.push_back({"XI_GAIN_SELECTOR_ANALOG_TAP1",           3, "Gain selector selects tap 1. This is available only on some cameras."});
	XI_GAIN_SELECTOR_TYPE_ITEMS.push_back({"XI_GAIN_SELECTOR_ANALOG_TAP2",           4, "Gain selector selects tap 2. This is available only on some cameras."});
	XI_GAIN_SELECTOR_TYPE_ITEMS.push_back({"XI_GAIN_SELECTOR_ANALOG_TAP3",           5, "Gain selector selects tap 3. This is available only on some cameras."});
	XI_GAIN_SELECTOR_TYPE_ITEMS.push_back({"XI_GAIN_SELECTOR_ANALOG_TAP4",           6, "Gain selector selects tap 4. This is available only on some cameras."});
	XI_GAIN_SELECTOR_TYPE_ITEMS.push_back({"XI_GAIN_SELECTOR_ANALOG_N",              7, "First of two channels of programmable gain control (PGC) function - Gain setting of R, B pixels (North column analog gain). This is available only on some cameras."});
	XI_GAIN_SELECTOR_TYPE_ITEMS.push_back({"XI_GAIN_SELECTOR_ANALOG_S",              8, "Second of two channels of programmable gain control (PGC) function - Gain setting of Gr, Gb pixels (South column analog gain). This is available only on some cameras."});

// Shutter mode types
	XI_SHUTTER_TYPE_ITEMS.push_back({"XI_SHUTTER_GLOBAL",                      0, "Sensor Global Shutter(CMOS sensor)"});
	XI_SHUTTER_TYPE_ITEMS.push_back({"XI_SHUTTER_ROLLING",                     1, "Sensor Electronic Rolling Shutter(CMOS sensor)"});
	XI_SHUTTER_TYPE_ITEMS.push_back({"XI_SHUTTER_GLOBAL_RESET_RELEASE",        2, "Sensor Global Reset Release Shutter(CMOS sensor)"});

// structure containing information about CMS functionality
	XI_CMS_MODE_ITEMS.push_back({"XI_CMS_DIS",                             0, "disables color management"});
	XI_CMS_MODE_ITEMS.push_back({"XI_CMS_EN",                              1, "enables color management (high CPU usage)"});
	XI_CMS_MODE_ITEMS.push_back({"XI_CMS_EN_FAST",                         2, "enables fast color management (high RAM usage)"});

// structure containing information about ICC Intents
	XI_CMS_INTENT_ITEMS.push_back({"XI_CMS_INTENT_PERCEPTUAL",               0, "CMS intent perceptual"});
	XI_CMS_INTENT_ITEMS.push_back({"XI_CMS_INTENT_RELATIVE_COLORIMETRIC",    1, "CMS intent relative colorimetry"});
	XI_CMS_INTENT_ITEMS.push_back({"XI_CMS_INTENT_SATURATION",               2, "CMS intent saturation"});
	XI_CMS_INTENT_ITEMS.push_back({"XI_CMS_INTENT_ABSOLUTE_COLORIMETRIC",    3, "CMS intent absolute colorimetry"});

// structure containing information about options for selection of camera before opening
	XI_OPEN_BY_ITEMS.push_back({"XI_OPEN_BY_INST_PATH",                   0, "Open camera by its hardware path"});
	XI_OPEN_BY_ITEMS.push_back({"XI_OPEN_BY_SN",                          1, "Open camera by its serial number"});
	XI_OPEN_BY_ITEMS.push_back({"XI_OPEN_BY_USER_ID",                     2, "open camera by its custom user ID"});
	XI_OPEN_BY_ITEMS.push_back({"XI_OPEN_BY_LOC_PATH",                    3, "Open camera by its hardware location path"});

// Lens feature selector selects which feature will be accessed.
	XI_LENS_FEATURE_ITEMS.push_back({"XI_LENS_FEATURE_MOTORIZED_FOCUS_SWITCH", 1, "Status of lens motorized focus switch"});
	XI_LENS_FEATURE_ITEMS.push_back({"XI_LENS_FEATURE_MOTORIZED_FOCUS_BOUNDED",2, "On read = 1 if motorized focus is on one of limits."});
	XI_LENS_FEATURE_ITEMS.push_back({"XI_LENS_FEATURE_MOTORIZED_FOCUS_CALIBRATION",3, "(planned feature) On read = 1 if motorized focus is calibrated. Write 1 to start calibration."});
	XI_LENS_FEATURE_ITEMS.push_back({"XI_LENS_FEATURE_IMAGE_STABILIZATION_ENABLED",4, "On read = 1 if image stabilization is enabled. Write 1 to enable image stabilization."});
	XI_LENS_FEATURE_ITEMS.push_back({"XI_LENS_FEATURE_IMAGE_STABILIZATION_SWITCH_STATUS",5, "On read = 1 if image stabilization switch is in position On."});
	XI_LENS_FEATURE_ITEMS.push_back({"XI_LENS_FEATURE_IMAGE_ZOOM_SUPPORTED",   6, "On read = 1 if lens supports zoom = are not prime."});

// Sensor feature selector selects which feature will be accessed.
	XI_SENSOR_FEATURE_SELECTOR_ITEMS.push_back({"XI_SENSOR_FEATURE_ZEROROT_ENABLE",       0, "Sensor Zero ROT enable for ONSEMI PYTHON family. For camera model:MQ013xG-ON (on/off)"});
	XI_SENSOR_FEATURE_SELECTOR_ITEMS.push_back({"XI_SENSOR_FEATURE_BLACK_LEVEL_CLAMP",    1, "Black level offset clamping (value). for Camera model:MD"});
	XI_SENSOR_FEATURE_SELECTOR_ITEMS.push_back({"XI_SENSOR_FEATURE_MD_FPGA_DIGITAL_GAIN_DISABLE",2, "Disable digital component of gain for MD family (1=disabled/0=enabled)"});
	XI_SENSOR_FEATURE_SELECTOR_ITEMS.push_back({"XI_SENSOR_FEATURE_ACQUISITION_RUNNING",  3, "Sensor acquisition is running status (0/1). Could be stopped by setting of 0. For camera model:CB,MC,MX,MT"});
	XI_SENSOR_FEATURE_SELECTOR_ITEMS.push_back({"XI_SENSOR_FEATURE_TIMING_MODE",          4, "Sensor timing mode (value depends on sensor)"});
	XI_SENSOR_FEATURE_SELECTOR_ITEMS.push_back({"XI_SENSOR_FEATURE_PARALLEL_ADC",         5, " Enables the parallel ADC readout mode, where all exposed pixels undergo dual sampling, leading to reduced readout noise at the cost of increased readout time "});
	XI_SENSOR_FEATURE_SELECTOR_ITEMS.push_back({"XI_SENSOR_FEATURE_BLACK_LEVEL_OFFSET_RAW",6, "Sensor specific register raw black level offset (value)"});
	XI_SENSOR_FEATURE_SELECTOR_ITEMS.push_back({"XI_SENSOR_FEATURE_SHORT_INTERVAL_SHUTTER",7, "Sensor short Interval Shutter (on/off)"});
	XI_SENSOR_FEATURE_SELECTOR_ITEMS.push_back({"XI_SENSOR_FEATURE_AUTO_LOW_POWER_MODE_AUTO",8, "Sensor low power mode (on/off)"});
	XI_SENSOR_FEATURE_SELECTOR_ITEMS.push_back({"XI_SENSOR_FEATURE_HIGH_CONVERSION_GAIN", 9, "Enables high conversion gain feature which applies additional gain to the signal at the pixel level. This leads to a reduction in read noise and a boost in sensitivity and signal-to-noise ratio, particularly in low-light situations. Consequently, the camera exhibits superior performance in dark environments, capturing images with minimal noise and enhanced detail."});
	XI_SENSOR_FEATURE_SELECTOR_ITEMS.push_back({"XI_SENSOR_FEATURE_DUAL_TRG_EXP_ZONE_DIVIDER_POSITION",10, "Sensor Dual Trigger Exposure Zone Divider Position"});
	XI_SENSOR_FEATURE_SELECTOR_ITEMS.push_back({"XI_SENSOR_FEATURE_TOF_VCSEL_CTRL_VOLTAGE_MV",11, "ToF VCSEL Control Voltage in mV"});
	XI_SENSOR_FEATURE_SELECTOR_ITEMS.push_back({"XI_SENSOR_FEATURE_MULTIPLE_ADC",         12, "Multiple ADC"});
	XI_SENSOR_FEATURE_SELECTOR_ITEMS.push_back({"XI_SENSOR_FEATURE_FAST_TRIGGER_MODE_ENABLE",13, "Enables a sensor trigger mode that starts exposure immediately at the beginning of the trigger signal. When fast trigger mode is disabled, the camera operates in a trigger mode that allows exposure and readout to overlap, resulting in a higher FPS. For camera model: MU051xG. (on/off)"});

// Camera sensor mode enumerator.
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD0",                            0, "Sensor mode number 0"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD1",                            1, "Sensor mode number 1"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD2",                            2, "Sensor mode number 2"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD3",                            3, "Sensor mode number 3"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD4",                            4, "Sensor mode number 4"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD5",                            5, "Sensor mode number 5"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD6",                            6, "Sensor mode number 6"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD7",                            7, "Sensor mode number 7"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD8",                            8, "Sensor mode number 8"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD9",                            9, "Sensor mode number 9"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD10",                           10, "Sensor mode number 10"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD11",                           11, "Sensor mode number 11"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD12",                           12, "Sensor mode number 12"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD13",                           13, "Sensor mode number 13"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD14",                           14, "Sensor mode number 14"});
	XI_SENSOR_MODE_ITEMS.push_back({"XI_SENS_MD15",                           15, "Sensor mode number 15"});

// Defines image sensor area as output.
	XI_IMAGE_AREA_SELECTOR_ITEMS.push_back({"XI_IMAGE_AREA_ACTIVE",                   0, "All light sensitive pixels suggested by image vendor."});
	XI_IMAGE_AREA_SELECTOR_ITEMS.push_back({"XI_IMAGE_AREA_ACTIVE_AND_MASKED",        1, "All Active pixels plus masked pixels surrounding the Active area."});

// Camera channel count enumerator.
	XI_SENSOR_OUTPUT_CHANNEL_COUNT_ITEMS.push_back({"XI_CHANN_CNT2",                          2, "2 sensor readout channels."});
	XI_SENSOR_OUTPUT_CHANNEL_COUNT_ITEMS.push_back({"XI_CHANN_CNT4",                          4, "4 sensor readout channels."});
	XI_SENSOR_OUTPUT_CHANNEL_COUNT_ITEMS.push_back({"XI_CHANN_CNT8",                          8, "8 sensor readout channels."});
	XI_SENSOR_OUTPUT_CHANNEL_COUNT_ITEMS.push_back({"XI_CHANN_CNT16",                         16, "16 sensor readout channels."});
	XI_SENSOR_OUTPUT_CHANNEL_COUNT_ITEMS.push_back({"XI_CHANN_CNT24",                         24, "24 sensor readout channels."});
	XI_SENSOR_OUTPUT_CHANNEL_COUNT_ITEMS.push_back({"XI_CHANN_CNT32",                         32, "32 sensor readout channels."});
	XI_SENSOR_OUTPUT_CHANNEL_COUNT_ITEMS.push_back({"XI_CHANN_CNT48",                         48, "48 sensor readout channels."});

// Sensor defects correction list selector
	XI_SENS_DEFFECTS_CORR_LIST_SELECTOR_ITEMS.push_back({"XI_SENS_DEFFECTS_CORR_LIST_SEL_FACTORY", 0, "Factory defect correction list"});
	XI_SENS_DEFFECTS_CORR_LIST_SELECTOR_ITEMS.push_back({"XI_SENS_DEFFECTS_CORR_LIST_SEL_USER0",   1, "User defect correction list"});
	XI_SENS_DEFFECTS_CORR_LIST_SELECTOR_ITEMS.push_back({"XI_SENS_DEFFECTS_CORR_LIST_SEL_IN_CAMERA",2, "Device specific defect correction list"});

// Acquisition status Selector
	XI_ACQUISITION_STATUS_SELECTOR_ITEMS.push_back({"XI_ACQUISITION_STATUS_ACQ_ACTIVE",       0, " Device is currently doing an acquisition of one or many frames."});

// Select unit where data-pipe is configured
	XI_DP_UNIT_SELECTOR_ITEMS.push_back({"XI_DP_UNIT_SENSOR",                      0, "Selects device image sensor"});
	XI_DP_UNIT_SELECTOR_ITEMS.push_back({"XI_DP_UNIT_FPGA",                        1, "Selects device image FPGA"});

// Select unit processor
	XI_DP_PROC_SELECTOR_ITEMS.push_back({"XI_DP_PROC_NONE",                        0, "Default empty processor"});
	XI_DP_PROC_SELECTOR_ITEMS.push_back({"XI_DP_PROC_CHANNEL_MUXER",               1, "Channel Muxer (selected processor combines multiple input channels)"});
	XI_DP_PROC_SELECTOR_ITEMS.push_back({"XI_DP_PROC_PIXEL_SEQUENCER",             2, "Selects pixel data output sequence"});
	XI_DP_PROC_SELECTOR_ITEMS.push_back({"XI_DP_PROC_CHANNEL_1",                   3, "Selects sensor output channel 1"});
	XI_DP_PROC_SELECTOR_ITEMS.push_back({"XI_DP_PROC_CHANNEL_2",                   4, "Selects sensor output channel 2"});
	XI_DP_PROC_SELECTOR_ITEMS.push_back({"XI_DP_PROC_FRAME_BUFFER",                5, "Selects frame buffer memory"});

// Select processor parameter
	XI_DP_PARAM_SELECTOR_ITEMS.push_back({"XI_DP_PARAM_NONE",                       0, "Empty parameter"});
	XI_DP_PARAM_SELECTOR_ITEMS.push_back({"XI_DP_PARAM_CHMUX_CHANNEL_SELECTOR",     1, "Defines output of Channel Muxer processor"});
	XI_DP_PARAM_SELECTOR_ITEMS.push_back({"XI_DP_PARAM_CHMUX_ALPHA",                2, "Channel merger coefficient Alpha"});
	XI_DP_PARAM_SELECTOR_ITEMS.push_back({"XI_DP_PARAM_CHMUX_BETA",                 3, "Channel merger coefficient Beta"});
	XI_DP_PARAM_SELECTOR_ITEMS.push_back({"XI_DP_PARAM_PIXSEQ_SELECTOR",            4, "PixSeq Selector"});
	XI_DP_PARAM_SELECTOR_ITEMS.push_back({"XI_DP_PARAM_CHANNEL_TIMING",             5, "Selected channel timing"});
	XI_DP_PARAM_SELECTOR_ITEMS.push_back({"XI_DP_PARAM_FRAMEBUF_MODE",              6, "Frame Buffer Mode"});
	XI_DP_PARAM_SELECTOR_ITEMS.push_back({"XI_DP_PARAM_FRAMEBUF_SIZE",              7, "Frame Buffer Size Bytes"});

// Select processor parameter value
	XI_DP_PARAM_VALUE_ITEMS.push_back({"XI_DP_PARAM_VALUE_CHMUX_CHANNEL_1",      0, "Selected source channel 1"});
	XI_DP_PARAM_VALUE_ITEMS.push_back({"XI_DP_PARAM_VALUE_CHMUX_CHANNEL_2",      1, "Selected source channel 2"});
	XI_DP_PARAM_VALUE_ITEMS.push_back({"XI_DP_PARAM_VALUE_CHMUX_CHANNEL_1_2",    2, "Selected source channel 1 and 2"});
	XI_DP_PARAM_VALUE_ITEMS.push_back({"XI_DP_PARAM_VALUE_CHMUX_MERGED",         3, "Merged data of two channels"});
	XI_DP_PARAM_VALUE_ITEMS.push_back({"XI_DP_PARAM_VALUE_CHMUX_CMS_S",          4, "Correlated Multiple Sampling(summing)"});
	XI_DP_PARAM_VALUE_ITEMS.push_back({"XI_DP_PARAM_VALUE_PIXSEQ_ONE_VALUE",     5, "Output is one value per pixel"});
	XI_DP_PARAM_VALUE_ITEMS.push_back({"XI_DP_PARAM_VALUE_PIXSEQ_TWO_VALUES",    6, "Output are two values per pixel"});
	XI_DP_PARAM_VALUE_ITEMS.push_back({"XI_DP_PARAM_VALUE_CHTIM_HG",             7, "High Gain channel timing"});
	XI_DP_PARAM_VALUE_ITEMS.push_back({"XI_DP_PARAM_VALUE_CHTIM_LG",             8, "Low Gain channel timing"});
	XI_DP_PARAM_VALUE_ITEMS.push_back({"XI_DP_PARAM_VALUE_FRAMEBUF_MODE_DISABLED",9, "Frame buffer is disabled"});
	XI_DP_PARAM_VALUE_ITEMS.push_back({"XI_DP_PARAM_VALUE_FRAMEBUF_MODE_ENABLED",10, "Frame buffer is on"});
	XI_DP_PARAM_VALUE_ITEMS.push_back({"XI_DP_PARAM_VALUE_PIXSEQ_FOUR_VALUES",   11, "Output are four values per pixel"});
	XI_DP_PARAM_VALUE_ITEMS.push_back({"XI_DP_PARAM_VALUE_CHMUX_CMS_A",          12, "Correlated Multiple Sampling(averaging)"});

// User Set selector options.
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_12_STD_L",                         10, "12bit per channel STD Low Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_12_STD_H",                         11, "12bit per channel STD High Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_14_STD_L",                         12, "14bit per channel STD Low Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_NONE",                             999, "No preset selected."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_14_STD_H",                         13, "14bit per channel STD High Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_2_12_CMS_S_L",                     14, "12bit per channel, 2 samples,  CMS(summing) Low Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_2_12_CMS_S_H",                     15, "12bit per channel, 2 samples,  CMS(summing) High Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_2_14_CMS_S_L",                     16, "14bit per channel, 2 samples,  CMS(summing) Low Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_2_14_CMS_S_H",                     17, "14bit per channel, 2 samples,  CMS(summing) High Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_4_12_CMS_S_L",                     18, "12bit per channel, 4 samples,  CMS(summing) Low Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_4_12_CMS_S_H",                     19, "12bit per channel, 4 samples,  CMS(summing) High Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_4_14_CMS_S_L",                     20, "14bit per channel, 4 samples,  CMS(summing) Low Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_4_14_CMS_S_H",                     21, "14bit per channel, 4 samples,  CMS(summing) High Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_2_12_HDR_HL",                      22, "12bit per channel, 2 samples,  HDR High Low Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_2_12_HDR_L",                       23, "12bit per channel, 2 samples,  HDR Low Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_2_12_HDR_H",                       24, "12bit per channel, 2 samples,  HDR High Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_4_12_CMS_HDR_HL",                  25, "12bit per channel, 4 samples,  CMS + HDR High Low Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_2_14_HDR_L",                       26, "14bit per channel, 2 samples,  HDR Low Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_2_14_HDR_H",                       27, "14bit per channel, 2 samples,  HDR High Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_2_12_CMS_A_L",                     28, "12bit per channel, 2 samples,  CMS(averaging) Low Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_2_12_CMS_A_H",                     29, "12bit per channel, 2 samples,  CMS(averaging) High Gain mode preset."});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_TOF_DEFAULT",                      30, ""});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_TOF_LONG_RANGE",                   31, ""});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_TOF_SINGLE_READOUT",               32, ""});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_TOF_DEFAULT_EXT_CDS",              33, ""});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_TOF_LONG_RANGE_EXT_CDS",           34, ""});
	XI_USER_SET_SELECTOR_ITEMS.push_back({"XI_US_TOF_SINGLE_READOUT_EXT_CDS",       35, ""});

// Mode of DualADC feature
	XI_DUAL_ADC_MODE_ITEMS.push_back({"XI_DUAL_ADC_MODE_OFF",                   0, "Disable DualADC feature"});
	XI_DUAL_ADC_MODE_ITEMS.push_back({"XI_DUAL_ADC_MODE_COMBINED",              1, "Set Combined mode"});
	XI_DUAL_ADC_MODE_ITEMS.push_back({"XI_DUAL_ADC_MODE_NON_COMBINED",          2, "Set NonCombined mode"});

// Probe Selector
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_CURRENT_MAINBOARD_VCC_IN",0, "Current probe on Main Board at VCC_IN power supply"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_VOLTAGE_MAINBOARD_VCC_IN",1, "Voltage probe on Main Board at VCC_IN power supply"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_CURRENT_MAINBOARD_VCC_ADJ2",2, "Current probe on Main Board at VCC_ADJ2 power supply"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_VOLTAGE_MAINBOARD_VCC_ADJ2",3, "Voltage probe on Main Board at VCC_ADJ2 power supply"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_CURRENT_MAINBOARD_VCC_ADJ1",4, "Current probe on Main Board at VCC_ADJ1 power supply"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_VOLTAGE_MAINBOARD_VCC_ADJ1",5, "Voltage probe on Main Board at VCC_ADJ1 power supply"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_CURRENT_MAINBOARD_VCC_PLT",6, "Current probe on Main Board at VCC_PLT power supply"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_VOLTAGE_MAINBOARD_VCC_PLT",7, "Voltage probe on Main Board at VCC_PLT power supply"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_VOLTAGE_SENSORBOARD_VCC_ADJ1",8, "Voltage probe on Sensor Board at VCC_ADJ1"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_VOLTAGE_SENSORBOARD_VCC_ADJ2",9, "Voltage probe on Sensor Board at VCC_ADJ2"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_VOLTAGE_SENSORBOARD_VCC_5V0",10, "Voltage probe on Sensor Board at VCC_5V0"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_VOLTAGE_SENSORBOARD_VCC_3V3",11, "Voltage probe on Sensor Board at VCC_3V3"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_VOLTAGE_DATA_CON_INPUT",12, "Voltage probe on device input, if device is bus powered"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_VOLTAGE_PELTIER1",     13, "Voltage probe on peltier #1"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_CURRENT_PELTIER1",     14, "Current probe on peltier #1"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_VOLTAGE_PELTIER2",     15, "Voltage probe on peltier #2"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_CURRENT_PELTIER2",     16, "Current probe on peltier #2"});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_VOLTAGE_MAINBOARD_VCC_5V0",17, "Voltage probe on Main Board at VCC_5V0 power supply "});
	XI_PROBE_SELECTOR_ITEMS.push_back({"XI_PROBE_SELECTOR_CURRENT_MAINBOARD_VCC_5V0",18, "Current probe on Main Board at VCC_5V0 power supply"});

// ToF Readout Mode feature
	XI_TOF_READOUT_MODE_ITEMS.push_back({"XI_TOF_READOUT_MODE_A_ONLY",             0, "A Only readout mode"});
	XI_TOF_READOUT_MODE_ITEMS.push_back({"XI_TOF_READOUT_MODE_B_ONLY",             1, "B Only readout mode"});
	XI_TOF_READOUT_MODE_ITEMS.push_back({"XI_TOF_READOUT_MODE_A_MINUS_B",          2, "A Minus B readout mode"});
	XI_TOF_READOUT_MODE_ITEMS.push_back({"XI_TOF_READOUT_MODE_A_PLUS_B",           3, "A Plus B readout mode"});
	XI_TOF_READOUT_MODE_ITEMS.push_back({"XI_TOF_READOUT_MODE_A_AND_B",            4, "A And B readout mode"});

// Image data signedness
	XI_DATA_SM_ITEMS.push_back({"XI_DATA_SM_UNSIGNED",                    0, "Unsigned if it can only represent non-negative numbers (zero or positive numbers)."});
	XI_DATA_SM_ITEMS.push_back({"XI_DATA_SM_SIGNED_2C",                   1, "Signed if it can represent both positive and negative numbers (two's complement)."});
	XI_DATA_SM_ITEMS.push_back({"XI_DATA_SM_SIGNED_FLOATING",             2, "Signed floating point data type."});


//-------------------------------------------------------------------------------------------------------------------
// Error codes xiApi
	XI_RET_ITEMS.push_back({"XI_OK", 0, "Function call succeeded"});
	XI_RET_ITEMS.push_back({"XI_INVALID_HANDLE", 1, "Invalid handle"});
	XI_RET_ITEMS.push_back({"XI_READREG", 2, "Register read error"});
	XI_RET_ITEMS.push_back({"XI_WRITEREG", 3, "Register write error"});
	XI_RET_ITEMS.push_back({"XI_FREE_RESOURCES", 4, "Freeing resources error"});
	XI_RET_ITEMS.push_back({"XI_FREE_CHANNEL", 5, "Freeing channel error"});
	XI_RET_ITEMS.push_back({"XI_FREE_BANDWIDTH", 6, "Freeing bandwith error"});
	XI_RET_ITEMS.push_back({"XI_READBLK", 7, "Read block error"});
	XI_RET_ITEMS.push_back({"XI_WRITEBLK", 8, "Write block error"});
	XI_RET_ITEMS.push_back({"XI_NO_IMAGE", 9, "No image"});
	XI_RET_ITEMS.push_back({"XI_TIMEOUT",10, "Timeout"});
	XI_RET_ITEMS.push_back({"XI_INVALID_ARG",11, "Invalid arguments supplied"});
	XI_RET_ITEMS.push_back({"XI_NOT_SUPPORTED",12, "Not supported"});
	XI_RET_ITEMS.push_back({"XI_ISOCH_ATTACH_BUFFERS",13, "Attach buffers error"});
	XI_RET_ITEMS.push_back({"XI_GET_OVERLAPPED_RESULT",14, "Overlapped result"});
	XI_RET_ITEMS.push_back({"XI_MEMORY_ALLOCATION",15, "Memory allocation error"});
	XI_RET_ITEMS.push_back({"XI_DLLCONTEXTISNULL",16, "DLL context is NULL"});
	XI_RET_ITEMS.push_back({"XI_DLLCONTEXTISNONZERO",17, "DLL context is non zero"});
	XI_RET_ITEMS.push_back({"XI_DLLCONTEXTEXIST",18, "DLL context exists"});
	XI_RET_ITEMS.push_back({"XI_TOOMANYDEVICES",19, "Too many devices connected"});
	XI_RET_ITEMS.push_back({"XI_ERRORCAMCONTEXT",20, "Camera context error"});
	XI_RET_ITEMS.push_back({"XI_UNKNOWN_HARDWARE",21, "Unknown hardware"});
	XI_RET_ITEMS.push_back({"XI_INVALID_TM_FILE",22, "Invalid TM file"});
	XI_RET_ITEMS.push_back({"XI_INVALID_TM_TAG",23, "Invalid TM tag"});
	XI_RET_ITEMS.push_back({"XI_INCOMPLETE_TM",24, "Incomplete TM"});
	XI_RET_ITEMS.push_back({"XI_BUS_RESET_FAILED",25, "Bus reset error"});
	XI_RET_ITEMS.push_back({"XI_NOT_IMPLEMENTED",26, "Not implemented"});
	XI_RET_ITEMS.push_back({"XI_SHADING_TOOBRIGHT",27, "Shading is too bright"});
	XI_RET_ITEMS.push_back({"XI_SHADING_TOODARK",28, "Shading is too dark"});
	XI_RET_ITEMS.push_back({"XI_TOO_LOW_GAIN",29, "Gain is too low"});
	XI_RET_ITEMS.push_back({"XI_INVALID_BPL",30, "Invalid sensor defect correction list"});
	XI_RET_ITEMS.push_back({"XI_BPL_REALLOC",31, "Error while sensor defect correction list reallocation"});
	XI_RET_ITEMS.push_back({"XI_INVALID_PIXEL_LIST",32, "Invalid pixel list"});
	XI_RET_ITEMS.push_back({"XI_INVALID_FFS",33, "Invalid Flash File System"});
	XI_RET_ITEMS.push_back({"XI_INVALID_PROFILE",34, "Invalid profile"});
	XI_RET_ITEMS.push_back({"XI_INVALID_CALIBRATION",35, "Invalid calibration"});
	XI_RET_ITEMS.push_back({"XI_INVALID_BUFFER",36, "Invalid buffer"});
	XI_RET_ITEMS.push_back({"XI_INVALID_DATA",38, "Invalid data"});
	XI_RET_ITEMS.push_back({"XI_TGBUSY",39, "Timing generator is busy"});
	XI_RET_ITEMS.push_back({"XI_IO_WRONG",40, "Wrong operation open/write/read/close"});
	XI_RET_ITEMS.push_back({"XI_ACQUISITION_ALREADY_UP",41, "Acquisition already started"});
	XI_RET_ITEMS.push_back({"XI_OLD_DRIVER_VERSION",42, "Old version of device driver installed to the system."});
	XI_RET_ITEMS.push_back({"XI_GET_LAST_ERROR",43, "To get error code please call GetLastError function."});
	XI_RET_ITEMS.push_back({"XI_CANT_PROCESS",44, "Data cannot be processed"});
	XI_RET_ITEMS.push_back({"XI_ACQUISITION_STOPED",45, "Acquisition is stopped. It needs to be started to perform operation."});
	XI_RET_ITEMS.push_back({"XI_ACQUISITION_STOPED_WERR",46, "Acquisition has been stopped with an error."});
	XI_RET_ITEMS.push_back({"XI_INVALID_INPUT_ICC_PROFILE",47, "Input ICC profile missing or corrupted"});
	XI_RET_ITEMS.push_back({"XI_INVALID_OUTPUT_ICC_PROFILE",48, "Output ICC profile missing or corrupted"});
	XI_RET_ITEMS.push_back({"XI_DEVICE_NOT_READY",49, "Device not ready to operate"});
	XI_RET_ITEMS.push_back({"XI_SHADING_TOOCONTRAST",50, "Shading is too contrast"});
	XI_RET_ITEMS.push_back({"XI_ALREADY_INITIALIZED",51, "Module already initialized"});
	XI_RET_ITEMS.push_back({"XI_NOT_ENOUGH_PRIVILEGES",52, "Application does not have enough privileges (one or more app)"});
	XI_RET_ITEMS.push_back({"XI_NOT_COMPATIBLE_DRIVER",53, "Installed driver is not compatible with current software"});
	XI_RET_ITEMS.push_back({"XI_TM_INVALID_RESOURCE",54, "TM file was not loaded successfully from resources"});
	XI_RET_ITEMS.push_back({"XI_DEVICE_HAS_BEEN_RESETED",55, "Device has been reset, abnormal initial state"});
	XI_RET_ITEMS.push_back({"XI_NO_DEVICES_FOUND",56, "No Devices Found"});
	XI_RET_ITEMS.push_back({"XI_RESOURCE_OR_FUNCTION_LOCKED",57, "Resource (device) or function locked by mutex"});
	XI_RET_ITEMS.push_back({"XI_BUFFER_SIZE_TOO_SMALL",58, "Buffer provided by user is too small"});
	XI_RET_ITEMS.push_back({"XI_COULDNT_INIT_PROCESSOR",59, "Could not initialize processor."});
	XI_RET_ITEMS.push_back({"XI_NOT_INITIALIZED",60, "The object/module/procedure/process being referred to has not been started."});
	XI_RET_ITEMS.push_back({"XI_RESOURCE_NOT_FOUND",61, "Resource not found(could be processor, file, item...)."});
	XI_RET_ITEMS.push_back({"XI_UNKNOWN_PARAM",100, "Unknown parameter"});
	XI_RET_ITEMS.push_back({"XI_WRONG_PARAM_VALUE",101, "Wrong parameter value"});
	XI_RET_ITEMS.push_back({"XI_WRONG_PARAM_TYPE",103, "Wrong parameter type"});
	XI_RET_ITEMS.push_back({"XI_WRONG_PARAM_SIZE",104, "Wrong parameter size"});
	XI_RET_ITEMS.push_back({"XI_BUFFER_TOO_SMALL",105, "Input buffer is too small"});
	XI_RET_ITEMS.push_back({"XI_NOT_SUPPORTED_PARAM",106, "Parameter is not supported"});
	XI_RET_ITEMS.push_back({"XI_NOT_SUPPORTED_PARAM_INFO",107, "Parameter info not supported"});
	XI_RET_ITEMS.push_back({"XI_NOT_SUPPORTED_DATA_FORMAT",108, "Data format is not supported"});
	XI_RET_ITEMS.push_back({"XI_READ_ONLY_PARAM",109, "Read only parameter"});
	XI_RET_ITEMS.push_back({"XI_BANDWIDTH_NOT_SUPPORTED",111, "This camera does not support currently available bandwidth"});
	XI_RET_ITEMS.push_back({"XI_INVALID_FFS_FILE_NAME",112, "FFS file selector is invalid or NULL"});
	XI_RET_ITEMS.push_back({"XI_FFS_FILE_NOT_FOUND",113, "FFS file not found"});
	XI_RET_ITEMS.push_back({"XI_PARAM_NOT_SETTABLE",114, "Parameter value cannot be set (might be out of range or invalid)."});
	XI_RET_ITEMS.push_back({"XI_SAFE_POLICY_NOT_SUPPORTED",115, "Safe buffer policy is not supported. E.g. when transport target is set to GPU (GPUDirect)."});
	XI_RET_ITEMS.push_back({"XI_GPUDIRECT_NOT_AVAILABLE",116, "GPUDirect is not available. E.g. platform isn't supported or CUDA toolkit isn't installed."});
	XI_RET_ITEMS.push_back({"XI_INCORRECT_SENS_ID_CHECK",117, "Incorrect sensor board unique identifier checksum."});
	XI_RET_ITEMS.push_back({"XI_INCORRECT_FPGA_TYPE",118, "Incorrect or unknown FPGA firmware type used for camera."});
	XI_RET_ITEMS.push_back({"XI_PARAM_CONDITIONALLY_NOT_AVAILABLE",119, "Parameter is not available in current context. Available only if another feature is turned on."});
	XI_RET_ITEMS.push_back({"XI_ERR_FRAME_BUFFER_RAM_INIT",120, "Frame buffer RAM initialization error."});
	XI_RET_ITEMS.push_back({"XI_NOT_SUPPORTED_WHILE_ACQUISITION ",121, "Parameter is not supported while acquisition is running."});
	XI_RET_ITEMS.push_back({"XI_PROC_OTHER_ERROR",201, "Processing error - other"});
	XI_RET_ITEMS.push_back({"XI_PROC_PROCESSING_ERROR",202, "Error while image processing."});
	XI_RET_ITEMS.push_back({"XI_PROC_INPUT_FORMAT_UNSUPPORTED",203, "Input format is not supported for processing."});
	XI_RET_ITEMS.push_back({"XI_PROC_OUTPUT_FORMAT_UNSUPPORTED",204, "Output format is not supported for processing."});
	XI_RET_ITEMS.push_back({"XI_OUT_OF_RANGE",205, "Parameter value is out of range"});
	XI_RET_ITEMS.push_back({"XI_INVALID_ARG",1001, "Invalid argument detected (out of range)"});
	XI_RET_ITEMS.push_back({"XI_NOT_IMPLEMENTED",1002, "Implementation is not yet done"});
	XI_RET_ITEMS.push_back({"XI_NOT_SUPPORTED",1003, "This value is not supported"});
	XI_RET_ITEMS.push_back({"XI_UNKNOWN_PARAM",1004, "Using of unknown parameter"});
	XI_RET_ITEMS.push_back({"XI_INPUT_BUFFER_NOT_ALLOCATED",1005, "Input buffer is not allocated"});
	XI_RET_ITEMS.push_back({"XI_OUTPUT_BUFFER_NOT_ALLOCATED",1006, "Output buffer is not allocated"});
	XI_RET_ITEMS.push_back({"XI_OUTPUT_BUFFER_MEMORY_ALLOCATION_ERROR",1007, "Output buffer allocation error"});
	XI_RET_ITEMS.push_back({"XI_NO_PROCESSORS_AVAILABLE",1008, "There are no processors in chain"});
	XI_RET_ITEMS.push_back({"XI_INVALID_REQUIREMENTS",1009, "Some requirement is not met"});
	XI_RET_ITEMS.push_back({"XI_NO_MORE_SPACE_IN_PROCESSORS_CHAIN",1010, "Processors chain is full"});
	XI_RET_ITEMS.push_back({"XI_PROCESSOR_NOT_INITIALIZED",1011, "Processor is not initialized yet"});
	XI_RET_ITEMS.push_back({"XI_PROCESSING_ERROR",1012, "Processor generates error"});
	XI_RET_ITEMS.push_back({"XI_UNKNOWN_DATA_FORMAT",1013, "Unknown data format"});
	XI_RET_ITEMS.push_back({"XI_UNKNOWN_CFA_PATTERN",1014, "Unknown color filter array pattern type"});
	XI_RET_ITEMS.push_back({"XI_UNKNOWN_LOAD_LIBRARY",1020, "Couldn't load library"});
	XI_RET_ITEMS.push_back({"XI_UNKNOWN_LOAD_LIBRARY_FUNC",1021, "Couldn't load function"});
	XI_RET_ITEMS.push_back({"XI_UNKNOWN_OPEN_PROCESS",1022, "Couldn't open process"});
	XI_RET_ITEMS.push_back({"XI_UNKNOWN_BPL_STRING",1023, "Couldn't open process"});
	XI_RET_ITEMS.push_back({"XI_UNSUPPORTED_INPUT_FORMAT",1024, "Unsupported input format"});
	XI_RET_ITEMS.push_back({"XI_UNSUPPORTED_OUTPUT_FORMAT",1025, "Unsupported output format"});
	XI_RET_ITEMS.push_back({"XI_IMAGE_INTENSITY_OUT_OF_RANGE",1026, "Image intensity is our of range"});
	XI_RET_ITEMS.push_back({"XI_TIMEOUT_WAITING_FOR_EVENT",1027, "Timeout waiting for event"});
	XI_RET_ITEMS.push_back({"XI_PROCESSOR_EXECUTION_DISABLED",1028, "Processor execution disabled"});
	XI_RET_ITEMS.push_back({"XI_DEFECT_ALREADY_IN_LIST",1029, "Defect already in list"});
	XI_RET_ITEMS.push_back({"XI_ERROR_OPEN_FILE",1030, "Couldn't open input file"});
	XI_RET_ITEMS.push_back({"XI_ERROR_MEMORY_ALLOCATE",1031, "Couldn't allocate memory"});
	XI_RET_ITEMS.push_back({"XI_ERROR_READ_OPERATION",1032, "Couldn't read"});
	XI_RET_ITEMS.push_back({"XI_ERROR_WRITE_OPERATION",1033, "Couldn't write"});
	XI_RET_ITEMS.push_back({"XI_READ_ONLY_OPERATION",1034, "Operation is read only"});
	XI_RET_ITEMS.push_back({"XI_FFC_INCORRECT_DATA_FORMAT",1035, "Input data not compatible with current FFC processor settings"});
	XI_RET_ITEMS.push_back({"XI_IMAGE_CANT_PROCESSED_DUETO_WIDTH_ALIGNMENT",1036, "Image can't be processed due to image width alignment is not supported by Image Processor."});
	

//-------------------------------------------------------------------------------------------------------------------
// Enumeration of XIMEA ImageProcessing Result Codes
	XI_PROC_RET_ITEMS.push_back({"XIPR_OK",                                        0, "Result OK"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_INVALID_ARG",                            1001, "Invalid argument detected (out of range)"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_NOT_IMPLEMENTED",                        1002, "Implementation is not yet done"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_NOT_SUPPORTED",                          1003, "This value is not supported"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_UNKNOWN_PARAM",                          1004, "Using of unknown parameter"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_INPUT_BUFFER_NOT_ALLOCATED",             1005, "Input buffer is not allocated"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_OUTPUT_BUFFER_NOT_ALLOCATED",            1006, "Output buffer is not allocated"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_OUTPUT_BUFFER_MEMORY_ALLOCATION_ERROR",  1007, "Output buffer allocation error"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_NO_PROCESSORS_AVAILABLE",                1008, "There are no processors in chain"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_INVALID_REQUIREMENTS",                   1009, "Some requirement is not met"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_NO_MORE_SPACE_IN_PROCESSORS_CHAIN",      1010, "Processors chain is full"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_PROCESSOR_NOT_INITIALIZED",              1011, "Processor is not initialized yet"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_PROCESSING_ERROR",                       1012, "Processor generates error"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_UNKNOWN_DATA_FORMAT",                    1013, "Unknown data format"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_UNKNOWN_CFA_PATTERN",                    1014, "Unknown color filter array pattern type"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_UNKNOWN_LOAD_LIBRARY",                   1020, "Couldn't load library"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_UNKNOWN_LOAD_LIBRARY_FUNC",              1021, "Couldn't load function"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_UNKNOWN_OPEN_PROCESS",                   1022, "Couldn't open process"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_UNKNOWN_BPL_STRING",                     1023, "Couldn't open process"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_UNSUPPORTED_INPUT_FORMAT",               1024, "Unsupported input format"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_UNSUPPORTED_OUTPUT_FORMAT",              1025, "Unsupported output format"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_IMAGE_INTENSITY_OUT_OF_RANGE",           1026, "Image intensity is our of range"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_TIMEOUT_WAITING_FOR_EVENT",              1027, "Timeout waiting for event"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_PROCESSOR_EXECUTION_DISABLED",           1028, "Processor execution disabled"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_DEFECT_ALREADY_IN_LIST",                 1029, "Defect already in list"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_ERROR_OPEN_FILE",                        1030, "Couldn't open input file"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_ERROR_MEMORY_ALLOCATE",                  1031, "Couldn't allocate memory"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_ERROR_READ_OPERATION",                   1032, "Couldn't read"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_ERROR_WRITE_OPERATION",                  1033, "Couldn't write"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_READ_ONLY_OPERATION",                    1034, "Operation is read only"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_FFC_INCORRECT_DATA_FORMAT",              1035, "Input data not compatible with current FFC processor settings"});
	XI_PROC_RET_ITEMS.push_back({"XIPR_IMAGE_CANT_PROCESSED_DUETO_WIDTH_ALIGNMENT",1036, "Image can't be processed due to image width alignment is not supported by Image Processor."});
	
};

xi_enum_item_t * FindItemByValue(std::vector <xi_enum_item_t> * collection, int value)
{
	if (collection == NULL)
		return NULL;

	for (std::vector<xi_enum_item_t>::iterator enum_item = collection->begin(); enum_item != collection->end(); ++enum_item)
	{
		if (enum_item->value == value)
		{
			return &(*enum_item);
		}
	}
	return NULL;
}
