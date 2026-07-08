
// Custom GenTL XIMEA specific implementation addons
#pragma once

#include <GenTL_v1_5.h>
using namespace GenTL;

// ----------------------------------
// error list

enum GC_ERROR_LIST_XIMEA
{
	GC_XIERR_EVENT_KILLED      = ( GC_ERR_CUSTOM_ID +   1),
	GC_XIERR_GET_LAST_ERR      = ( GC_ERR_CUSTOM_ID +   2),
	GC_XIERR_NOT_SETTABLE      = ( GC_ERR_CUSTOM_ID +   3),  // reported by _Settable handler when not possible to set value
	GC_XIERR_NO_GPUDIRECT      = ( GC_ERR_CUSTOM_ID +   4),  // reported by PCIe_AnnounceBuffer when GPUDirect isn't supported
};

// ----------------------------------
// event types

enum EVENT_TYPE_LIST_XIMEA
{
	// EVENT_XI_SOME_EXAMPLE_EVENT        = (EVENT_CUSTOM_ID +     1),  //!< Description
};

// ----------------------------------
// stream info commands

enum STREAM_INFO_CMD_LIST_XIMEA
{
	STREAM_XI_INFO_BUF_ANNOUNCE_MAX         = (STREAM_INFO_CUSTOM_ID +    1),   //!< UINT32 Max possible number of buffers to annouce
	STREAM_XI_INFO_FRAME_LOST_COUNT         = (STREAM_INFO_CUSTOM_ID +    2),   //!< UINT32 Frame lost (e.g. due to buffer not available)
	STREAM_XI_INFO_BUF_QUEUE_SIZE_MAX       = (STREAM_INFO_CUSTOM_ID +    3),   //!< UINT32 Max possible number of buffers to be queued
	STREAM_XI_INFO_TRANSPORT_IS_IDLE        = (STREAM_INFO_CUSTOM_ID +    4),   //!< UINT32 1 if transport is idle (=no data are waiting in transport (DMA))
    STREAM_XI_INFO_FRAME_MISSED_TRIGGER_DUETO_OVERLAP           = (STREAM_INFO_CUSTOM_ID +  5),     //!< UINT32 Missed triggers due to overlap
    STREAM_XI_INFO_FRAME_MISSED_TRIGGER_DUETO_FRAME_BUFFER_OVR  = (STREAM_INFO_CUSTOM_ID +  6),     //!< UINT32 Missed triggers due to frame buffer overflow
    STREAM_XI_INFO_FRAME_BUFFER_OVERFLOW                        = (STREAM_INFO_CUSTOM_ID +  7),     //!< UINT32 Frame buffer overflow counter
	// If you add new item here - add handlers to all DALs switch cases:
	// DAL_PCIe.cpp, DAL_U3V.cpp, DAL_Simulator_Handlers.cpp, ....
};

// ----------------------------------
// buffer info commands

enum BUFFER_INFO_CMD_LIST_XIMEA
{
	BUFFER_INFO_DEVICE_MODE_ID              = (BUFFER_INFO_CUSTOM_ID + 1),      //!< UINT32 Custom info, defines device features change id (exposure, width...)
	BUFFER_INFO_BLACK_LEVEL		            = (BUFFER_INFO_CUSTOM_ID + 2),      //!< UINT32 Custom info, defines image black level (in transport data bit depth space. e.g. if Pixel Format is Mono10 - valid values 0-1023)
	BUFFER_INFO_EXPOSURE_TIME_US            = (BUFFER_INFO_CUSTOM_ID + 3),      //!< UINT32 Custom info, defines image exposure time in microseconds
	BUFFER_INFO_GAIN_RATIO	                = (BUFFER_INFO_CUSTOM_ID + 4),      //!< UINT32 Custom info, defines image ratio of gains (low gain channel/ high gain channel)
	BUFFER_INFO_SATURATION					= (BUFFER_INFO_CUSTOM_ID + 5),      //!< UINT32 saturation value(maximum possible pixel value) 
	BUFFER_INFO_USER_DATA                   = (BUFFER_INFO_CUSTOM_ID + 6),      //!< UINT32 Custom info, defines incoming image user data set by feature / parameter
	BUFFER_INFO_MULTI_EXPOSURE_TIME1_US     = (BUFFER_INFO_CUSTOM_ID + 30),     //!< UINT32 Custom info, defines image sub exposure 1 in microseconds
	BUFFER_INFO_MULTI_EXPOSURE_TIME2_US     = (BUFFER_INFO_CUSTOM_ID + 31),     //!< UINT32 Custom info, defines image sub exposure 2 in microseconds
	BUFFER_INFO_MULTI_EXPOSURE_TIME3_US     = (BUFFER_INFO_CUSTOM_ID + 32),     //!< UINT32 Custom info, defines image sub exposure 3 in microseconds
	BUFFER_INFO_MULTI_EXPOSURE_TIME4_US     = (BUFFER_INFO_CUSTOM_ID + 33),     //!< UINT32 Custom info, defines image sub exposure 4 in microseconds
	BUFFER_INFO_MULTI_EXPOSURE_TIME5_US     = (BUFFER_INFO_CUSTOM_ID + 34),     //!< UINT32 Custom info, defines image sub exposure 5 in microseconds
	BUFFER_INFO_FRAME_DATA_FINAL_TYPE       = (BUFFER_INFO_CUSTOM_ID + 35),		//!< UINT32 Custom info, defines payload after frame (Can be dummy rows, image correction data) in rows
	BUFFER_INFO_DIGITAL_INPUT_LEVELS		= (BUFFER_INFO_CUSTOM_ID + 36),		//!< UINT32 Custom info, defines status of inputs
	BUFFER_INFO_BLACK_TOP					= (BUFFER_INFO_CUSTOM_ID + 37),		//!< UINT32 Custom info, image has black rows
	BUFFER_INFO_BLACK_BOTTOM				= (BUFFER_INFO_CUSTOM_ID + 38),		//!< UINT32 Custom info, image has black rows
	BUFFER_INFO_BLACK_LEFT					= (BUFFER_INFO_CUSTOM_ID + 39),		//!< UINT32 Custom info, image has black columns
	BUFFER_INFO_BLACK_RIGHT					= (BUFFER_INFO_CUSTOM_ID + 40),		//!< UINT32 Custom info, image has black columns
	BUFFER_INFO_LG_BLACK					= (BUFFER_INFO_CUSTOM_ID + 41),		//!< UINT32 Custom info, black level of LG channel
	BUFFER_INFO_HG_BLACK					= (BUFFER_INFO_CUSTOM_ID + 42),		//!< UINT32 Custom info, black level of HG channel
	BUFFER_INFO_HG_RANGE					= (BUFFER_INFO_CUSTOM_ID + 43),		//!< UINT32 Custom info,  valid range of HG channel <HG_BLACK,HG_BLACK+HG_RANGE>
	BUFFER_INFO_LG_RANGE                    = (BUFFER_INFO_CUSTOM_ID + 44),		//!< UINT32 Custom info,  valid range of LG channel <LG_BLACK,LG_BLACK+LG_RANGE>
	// If you add new item here - add handlers to all DALs switch cases:
	// DAL_PCIe.cpp, DAL_U3V.cpp, DAL_Simulator_Handlers.cpp, ....
};
