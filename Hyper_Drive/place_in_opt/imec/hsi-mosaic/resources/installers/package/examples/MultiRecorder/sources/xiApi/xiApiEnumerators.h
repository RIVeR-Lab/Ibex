
			
//-------------------------------------------------------------------------------------------------------------------
// header file
#ifndef __XIAPI_ENUMERATORS_H
#define __XIAPI_ENUMERATORS_H

#include <vector>
#include <string>

// xiApi enumerators

struct xi_enum_item_t
{
	std::string name;
	int value;
	std::string description;
};


//-------------------------------------------------------------------------------------------------------------------
// 
// xiAPI enumerators
extern std::vector<xi_enum_item_t> XI_DOWNSAMPLING_VALUE_ITEMS;                            // Downsampling value enumerator.
extern std::vector<xi_enum_item_t> XI_TEST_PATTERN_GENERATOR_ITEMS;                        // Test Pattern Generator
extern std::vector<xi_enum_item_t> XI_VERSION_ITEMS;                                       // Module/Unit version selector
extern std::vector<xi_enum_item_t> XI_TEST_PATTERN_ITEMS;                                  // Test Pattern Type
extern std::vector<xi_enum_item_t> XI_DEC_PATTERN_ITEMS;                                   // Decimation Pattern Format
extern std::vector<xi_enum_item_t> XI_BIN_PATTERN_ITEMS;                                   // Binning Pattern Format
extern std::vector<xi_enum_item_t> XI_BIN_SELECTOR_ITEMS;                                  // Binning Engine Selector
extern std::vector<xi_enum_item_t> XI_BIN_MODE_ITEMS;                                      // Selects binning mode; to be used with
extern std::vector<xi_enum_item_t> XI_DEC_SELECTOR_ITEMS;                                  // Decimation Engine Selector
extern std::vector<xi_enum_item_t> XI_SENSOR_TAP_CNT_ITEMS;                                // Sensor tap count enumerator.
extern std::vector<xi_enum_item_t> XI_BIT_DEPTH_ITEMS;                                     // Bit depth enumerator.
extern std::vector<xi_enum_item_t> XI_DEBUG_LEVEL_ITEMS;                                   // Debug level enumerator.
extern std::vector<xi_enum_item_t> XI_IMG_FORMAT_ITEMS;                                    // Image output format enumerator.
extern std::vector<xi_enum_item_t> XI_COLOR_FILTER_ARRAY_ITEMS;                            // Bayer color matrix enumerator.
extern std::vector<xi_enum_item_t> XI_IMAGE_SEQUENCE_TYPE_ITEMS;                           // Image sequence type.
extern std::vector<xi_enum_item_t> XI_BP_ITEMS;                                            // structure containing information about buffer policy(can be safe, data will be copied to user/app buffer or unsafe, user will get internally allocated buffer without data copy).
extern std::vector<xi_enum_item_t> XI_TRG_SOURCE_ITEMS;                                    // structure containing information about trigger source
extern std::vector<xi_enum_item_t> XI_TRG_SELECTOR_ITEMS;                                  // structure containing information about trigger functionality
extern std::vector<xi_enum_item_t> XI_TRG_OVERLAP_ITEMS;                                   // Trigger overlap modes
extern std::vector<xi_enum_item_t> XI_ACQ_TIMING_MODE_ITEMS;                               // structure containing information about acquisition timing modes
extern std::vector<xi_enum_item_t> XI_TRANSPORT_DATA_TARGET_MODE_ITEMS;                    // Enumerator for data target modes
extern std::vector<xi_enum_item_t> XI_GPI_SEL_CB_ITEMS;                                    // Enumeration for XI_PRM_GPI_SELECTOR for CB cameras.
extern std::vector<xi_enum_item_t> XI_GPO_SEL_CB_ITEMS;                                    // Enumeration for XI_PRM_GPO_SELECTOR for CB cameras.
extern std::vector<xi_enum_item_t> XI_GPI_MODE_ITEMS;                                      // structure containing information about GPI functionality
extern std::vector<xi_enum_item_t> XI_GPI_SELECTOR_ITEMS;                                  // Enumerator for GPI port selection.
extern std::vector<xi_enum_item_t> XI_GPO_MODE_ITEMS;                                      // structure containing information about GPO functionality
extern std::vector<xi_enum_item_t> XI_GPO_SELECTOR_ITEMS;                                  // Enumerator for GPO port selection.
extern std::vector<xi_enum_item_t> XI_METADATA_SAMPLING_MODE_ITEMS;                        // Metadata sampling mode selector
extern std::vector<xi_enum_item_t> XI_LED_MODE_ITEMS;                                      // structure containing information about LED functionality
extern std::vector<xi_enum_item_t> XI_LED_SELECTOR_ITEMS;                                  // Enumerator for LED selection.
extern std::vector<xi_enum_item_t> XI_COUNTER_SELECTOR_ITEMS;                              // structure contains frames counter
extern std::vector<xi_enum_item_t> XI_TS_RST_MODE_ITEMS;                                   // structure containing information about timestamp reset arming
extern std::vector<xi_enum_item_t> XI_TS_RST_SOURCE_ITEMS;                                 // structure containing information about possible timestamp reset sources
extern std::vector<xi_enum_item_t> XI_PRM_TYPE_ITEMS;                                      // structure containing information about parameters type
extern std::vector<xi_enum_item_t> XI_SWITCH_ITEMS;                                        // Turn parameter On/Off
extern std::vector<xi_enum_item_t> XI_TEMP_SELECTOR_ITEMS;                                 // Temperature selector
extern std::vector<xi_enum_item_t> XI_TEMP_CTRL_MODE_SELECTOR_ITEMS;                       // Temperature selector
extern std::vector<xi_enum_item_t> XI_TEMP_ELEMENT_SELECTOR_ITEMS;                         // Temperature element selector
extern std::vector<xi_enum_item_t> XI_OUTPUT_DATA_PACKING_TYPE_ITEMS;                      // Data packing(grouping) types.
extern std::vector<xi_enum_item_t> XI_DOWNSAMPLING_TYPE_ITEMS;                             // Downsampling types
extern std::vector<xi_enum_item_t> XI_EXPOSURE_TIME_SELECTOR_TYPE_ITEMS;                   // Exposure time selector
extern std::vector<xi_enum_item_t> XI_INTERLINE_EXPOSURE_MODE_TYPE_ITEMS;                  // Interline exposure mode
extern std::vector<xi_enum_item_t> XI_GAIN_SELECTOR_TYPE_ITEMS;                            // Gain selector
extern std::vector<xi_enum_item_t> XI_SHUTTER_TYPE_ITEMS;                                  // Shutter mode types
extern std::vector<xi_enum_item_t> XI_CMS_MODE_ITEMS;                                      // structure containing information about CMS functionality
extern std::vector<xi_enum_item_t> XI_CMS_INTENT_ITEMS;                                    // structure containing information about ICC Intents
extern std::vector<xi_enum_item_t> XI_OPEN_BY_ITEMS;                                       // structure containing information about options for selection of camera before opening
extern std::vector<xi_enum_item_t> XI_LENS_FEATURE_ITEMS;                                  // Lens feature selector selects which feature will be accessed.
extern std::vector<xi_enum_item_t> XI_SENSOR_FEATURE_SELECTOR_ITEMS;                       // Sensor feature selector selects which feature will be accessed.
extern std::vector<xi_enum_item_t> XI_SENSOR_MODE_ITEMS;                                   // Camera sensor mode enumerator.
extern std::vector<xi_enum_item_t> XI_IMAGE_AREA_SELECTOR_ITEMS;                           // Defines image sensor area as output.
extern std::vector<xi_enum_item_t> XI_SENSOR_OUTPUT_CHANNEL_COUNT_ITEMS;                   // Camera channel count enumerator.
extern std::vector<xi_enum_item_t> XI_SENS_DEFFECTS_CORR_LIST_SELECTOR_ITEMS;              // Sensor defects correction list selector
extern std::vector<xi_enum_item_t> XI_ACQUISITION_STATUS_SELECTOR_ITEMS;                   // Acquisition status Selector
extern std::vector<xi_enum_item_t> XI_DP_UNIT_SELECTOR_ITEMS;                              // Select unit where data-pipe is configured
extern std::vector<xi_enum_item_t> XI_DP_PROC_SELECTOR_ITEMS;                              // Select unit processor
extern std::vector<xi_enum_item_t> XI_DP_PARAM_SELECTOR_ITEMS;                             // Select processor parameter
extern std::vector<xi_enum_item_t> XI_DP_PARAM_VALUE_ITEMS;                                // Select processor parameter value
extern std::vector<xi_enum_item_t> XI_USER_SET_SELECTOR_ITEMS;                             // User Set selector options.
extern std::vector<xi_enum_item_t> XI_DUAL_ADC_MODE_ITEMS;                                 // Mode of DualADC feature
extern std::vector<xi_enum_item_t> XI_PROBE_SELECTOR_ITEMS;                                // Probe Selector
extern std::vector<xi_enum_item_t> XI_TOF_READOUT_MODE_ITEMS;                              // ToF Readout Mode feature
extern std::vector<xi_enum_item_t> XI_DATA_SM_ITEMS;                                       // Image data signedness

extern std::vector<xi_enum_item_t>  XI_RET_ITEMS;

extern std::vector<xi_enum_item_t>  XI_PROC_RET_ITEMS;

void xiInitEnumerators();
xi_enum_item_t * FindItemByValue(std::vector <xi_enum_item_t> * collection, int value);

#endif /* __XIAPI_ENUMERATORS_H */
