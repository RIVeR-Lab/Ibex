

//-------------------------------------------------------------------------------------------------------------------
// xiApi Extension header file

// Generated for xiAPI version 4.21.06

#ifndef __XIAPIEXT_H
#define __XIAPIEXT_H

//-------------------------------------------------------------------------------------------------------------------
// xiApiExtended parameters

#define  XI_PRM_SENSOR_LINE_PERIOD                         "sensor_line_period"                     // Image sensor line period in us 
#define  XI_PRM_LENS_COMM_DATA                             "lens_comm_data"                         // Write/Read data sequences to/from lens 
#define  XI_PRM_DEVICE_ID                                  "device_id"                              // Return unique device ID 
#define  XI_PRM_ACQ_TRANSPORT_DATA_COMMIT_TOTAL_SIZE       "acq_transport_data_commit_total_size"   // Total number of bytes to be commit in one time on transport (all transport buffers together). Increasing can enhance transport capacity. E.g. on USB 
#define  XI_PRM_IMAGE_CORRECTION_SELECTOR                  "image_correction_selector"              // Select image correction function XI_IMAGE_CORRECTION_SELECTOR
#define  XI_PRM_IMAGE_CORRECTION_VALUE                     "image_correction_value"                 // Select image correction selected function value 
#define  XI_PRM_TRANS_DATA_BLACK_LEVEL_OVR                 "trans_data_black_level_ovr"             // Overwrites black level comming from transport data. 
#define  XI_PRM_TRANS_DATA_BLACK_LEVEL_OVR_EN              "trans_data_black_level_ovr_en"          // Enables/disables black level overwrite. 
#define  XI_PRM_CAM_ENUM_GOLDEN_ENABLED                    "cam_enum_golden_enabled"                // Enable enumeration of golden devices 
#define  XI_PRM_RESET_USB_IF_BOOTLOADER                    "reset_usb_if_bootloader"                // Resets USB device if started as bootloader 
#define  XI_PRM_CAM_SIMULATORS_COUNT                       "cam_simulators_count"                   // Number of camera simulators to be available. 
#define  XI_PRM_CAM_SENSOR_INIT_DISABLED                   "cam_sensor_init_disabled"               // Camera sensor will not be initialized when 1=XI_ON is set. 
#define  XI_PRM_PROC_ENGINE                                "proc_engine"                            // Set processing engine XI_PROC_ENGINE
#define  XI_PRM_FFS_FILE_OFFSET                            "ffs_file_offset"                        // Offset of data in file. 
#define  XI_PRM_EXTENDED_FEATURE_SELECTOR                  "ext_feature_selector"                   // Selection of extended feature. XI_EXT_FEATURE_SELECTOR
#define  XI_PRM_EXTENDED_FEATURE                           "ext_feature"                            // Extended feature value. 
#define  XI_PRM_DEVICE_UNIT_SELECTOR                       "device_unit_selector"                   // Selects device unit. XI_DEVICE_UNIT_SELECTOR
#define  XI_PRM_DEVICE_UNIT_REGISTER_SELECTOR              "device_unit_register_selector"          // Selects register of selected device unit(XI_PRM_DEVICE_UNIT_SELECTOR). 
#define  XI_PRM_DEVICE_UNIT_REGISTER_VALUE                 "device_unit_register_value"             // Sets/gets register value of selected device unit(XI_PRM_DEVICE_UNIT_SELECTOR). 
#define  XI_PRM_API_PROGRESS_CALLBACK                      "api_progress_callback"                  // Callback address of pointer that is called upon long tasks (e.g. XI_PRM_WRITE_FILE_FFS). 


//-------------------------------------------------------------------------------------------------------------------
// defines
#define XI_LIST_STATUS_GENERATED        0x0001  //Sensor defects list was generated
#define XI_LIST_STATUS_UPTODATE         0x0002  //Sensor defects list is uptodate
#define XI_LIST_STATUS_CUSTOM_LIST      0x0004  //Sensor defects list is should not be saved to camera
//-------------------------------------------------------------------------------------------------------------------
/**
   \brief Initialize device
   
   This function prepares the camera's software for work.
   It populates structures, runs initializing procedures, allocates resources - prepares the camera for work.

	\note Function creates and returns handle of the specified device. To de-initialize the camera and destroy the handler xiCloseDevice should be called.	

   @param[in] DevId						index of the device
   @param[out] hDevice					handle to device
   @return XI_OK on success, error value otherwise.
 */
//-------------------------------------------------------------------------------------------------------------------
//Set device info
XIAPI XI_RETURN __cdecl xiSetDeviceInfoString(IN DWORD DevId, const char* prm, char* value, DWORD value_size);
//-------------------------------------------------------------------------------------------------------------------
// xiAPI enumerators
// Selector of processing engine(instructions set)
typedef enum
{
	XI_PE_ALL                    =0, // Use all available instructions
	XI_PE_C                      =1, // Use C(C++) code
	XI_PE_SSE2                   =3, // Use SSE2 instructions
	XI_PE_AVX                    =4, // Use AVX instructions
	XI_PE_AVX2                   =5, // Use AVX2 instructions
	
} XI_PROC_ENGINE;

// Image correction function
typedef enum
{
	XI_CORRECTION_TYPE_SELECTOR  =0, // Correction Type selected see XI_TYPE_CORRECTION_SELECTOR
	XI_DEFECT_ID                 =1, // Select defect id
	XI_DEFECTS_COUNT_BY_TYPE     =2, // Count of defects selected by current XI_DEFECT_TYPE
	XI_DEFECT_TYPE               =3, // Type of defect see XI_IMAGE_DEFECT_TYPE
	XI_DEFECT_SUB_TYPE           =4, // Defect sub type see XI_IMAGE_DEFECT_SUB_TYPE
	XI_DEFECT_POS_X              =5, // Defect position x
	XI_DEFECT_POS_Y              =6, // Defect position y
	XI_DEFECT_CMD_ADD            =7, // Write cached defect to the list
	XI_DEFECT_CMD_DELETE         =8, // Delete defect to the list
	XI_DEFECT_CMD_APPLY_CHANGES  =9, // Apply changes
	XI_DEFECT_CMD_LIST_CLEAR     =10, // Clear list
	XI_DEFECT_CMD_LISTS_CLEAR    =11, // Clear lists
	XI_DEFECT_CMD_SAVE           =12, // Save list to device
	XI_CORRECTION_TYPE_ENABLED   =13, // Enable or disable correction type
	XI_DEFECT_ID_BY_TYPE         =14, // Select defect id by type
	XI_LIST_ID                   =15, // Select list id
	XI_DEFECT_CMD_APPLY_CHANGES_ALL=16, // Apply changes to all lists
	XI_LIST_STATUS               =17, // Current list status (Read-only). Result is mask of bits XI_LIST_STATUS_GENERATED, XI_LIST_STATUS_...
	XI_IMG_COR_TAP_SELECTOR      =64, // Selected tap id (0-N) for image correction
	XI_IMG_COR_GAIN_TUNE         =65, // Adjustment of gain in dB. For multitap sensors, active tap is selected by XI_IMG_COR_TAP_SELECTOR.
	XI_IMG_COR_OFFSET_TUNE       =66, // Adjustment of pixel values offset. For multitap sensors, active tap is selected by XI_IMG_COR_TAP_SELECTOR.
	
} XI_IMAGE_CORRECTION_SELECTOR;

// Define image  correction type
typedef enum
{
	XI_CORR_TYPE_SENSOR_DEFECTS_FACTORY=0, // Factory defect list
	XI_CORR_TYPE_SENSOR_COLUMN_FPN=1, // Select Fixed Pattern Noise Correction for Columns
	XI_CORR_TYPE_SENSOR_ADC_BLO  =2, // ADC gain and black level offset sensor register correction
	XI_CORR_TYPE_SENSOR_ROW_FPN  =3, // Select Fixed Pattern Noise Correction for Rows
	XI_CORR_TYPE_SENSOR_DEFECTS_USER0=4, // User defect list
	XI_CORR_TYPE_SENSOR_CHANNELS_TUNE=5, // Image channel/tap intensity correction
	XI_CORR_TYPE_SENSOR_COLUMN_BLACK_OFFSET=6, // Select image black offset Correction for Columns
	XI_CORR_TYPE_SENSOR_ROW_BLACK_OFFSET=7, // Select image black offset Correction for Rows
	
} XI_TYPE_CORRECTION_SELECTOR;

// Define image defect types
typedef enum
{
	XI_IMAGE_DEFECT_TYPE_PIXEL   =0, // Defect is pixel
	XI_IMAGE_DEFECT_TYPE_COLUMN  =1, // Defect is column
	XI_IMAGE_DEFECT_TYPE_ROW     =2, // Defect is row
	
} XI_IMAGE_DEFECT_TYPE;

// Define image defect sub types
typedef enum
{
	XI_IMAGE_DEFECT_SUB_TYPE_DARK=0, // Defect pixel(s) is(are) too dark
	XI_IMAGE_DEFECT_SUB_TYPE_BRIGHT=1, // Defect pixel(s) is(are) out of range
	XI_IMAGE_DEFECT_SUB_TYPE_HOT =2, // Defect pixel(s) is(are) too bright
	
} XI_IMAGE_DEFECT_SUB_TYPE;

// Extended feature selector.
typedef enum
{
	XI_EXT_FEATURE_SEL_SIMULATOR_GENERATOR_FRAME_LOST_PERIOD_MIN=1, // Camera simulator lost frame generation minimum period (in frames).
	XI_EXT_FEATURE_SEL_SIMULATOR_GENERATOR_FRAME_LOST_PERIOD_MAX=2, // Camera simulator lost frame generation random period (in frames).
	XI_EXT_FEATURE_SEL_SIMULATOR_IMAGE_DATA_FORMAT=3, // Camera simulator image data format.
	XI_EXT_FEATURE_SEL_BANDWIDTH_MEASUREMENT_TIME_SECONDS=4, // Number of seconds for bandwidth measurement. Default = 1.
	XI_EXT_FEATURE_SEL_IMAGE_INTENSIFIER_VOLTAGE=5, // Input voltage for image intensifier. Default = 0.
	XI_EXT_FEATURE_SEL_TRIG_FRAME=6, // Triggers frame(s) on internal event. Default = 0.
	XI_EXT_FEATURE_SEL_IMAGE_OVERSAMPLING=7, // Enable/disable image pixels oversampling. Default = 0.
	XI_EXT_FEATURE_SEL_APPLY_DATA_FINAL=8, // Enable/disable applying data final. Default = 1.
	XI_EXT_FEATURE_SEL_FAN_RPM   =9, // Sets camera cooling fan rpm (% from max). Default = 100.
	XI_EXT_FEATURE_SEL_DITHERING_HOST=10, // Enables/Disables shifted(left/up) image data dithering on HOST side. Default = 0(off).
	XI_EXT_FEATURE_SEL_DITHERING_DEVICE=11, // Enables/Disables shifted(left/up) image data dithering on DEVICE side. Default = 0(off).
	XI_EXT_FEATURE_SEL_FAN_THR_TEMP=12, // Sets camera fan/back side threshold temperature. Default = 35.
	XI_EXT_FEATURE_PCIE_IOCTL_GLOBAL_LOCK_ENABLED=13, // Controls if PCIe IOCTL global locking is enabled. If disabled, concurrent operation (e.g. using filesystem is running faster in multiple threads)
	XI_EXT_FEATURE_SEL_EXTERNAL_POWER_SOURCE_VOLTAGE=14, // Input voltage from external power source.
	
} XI_EXT_FEATURE_SELECTOR;

// Device unit selector
typedef enum
{
	XI_DEVICE_UNIT_SENSOR1       =0, // Selects first sensor on device
	XI_DEVICE_UNIT_FPGA1         =1, // Selects first FPGA on device
	XI_DEVICE_UNIT_SAL           =2, // Selects sensor abstraction layer
	XI_DEVICE_UNIT_DAL           =3, // Selects driver abstraction layer
	XI_DEVICE_UNIT_SCM           =4, // Selects sensor correction module
	XI_DEVICE_UNIT_FGENTL        =5, // Selects register in underlying GenTL layer
	XI_DEVICE_UNIT_MCU1          =6, // Selects first MCU on device
	XI_DEVICE_UNIT_MCU2          =7, // Selects second MCU on device
	XI_DEVICE_UNIT_CHF           =8, // Selects Camera High Features Model
	
} XI_DEVICE_UNIT_SELECTOR;

	
#endif /* __XIAPIEXT_H */
