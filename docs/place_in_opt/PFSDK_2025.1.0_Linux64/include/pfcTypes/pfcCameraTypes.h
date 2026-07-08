/*
******************************************************************************
* @attention
*
*<h2><center>&copy; COPYRIGHT(c) 2021 Photonfocus AG</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
* 3. Neither the name of Photonfocus nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

///
/// @file pfcCameraTypes.h
/// @brief Definition of different data structs to get a list of the available cameras, their interface type and other information.
///
/// Further Description.
///
/// @note
/// @author TAH 
/// @version 0.1 
/// @date 2016/11/09
///
#ifndef PFCCAMERATYPES_H
#define PFCCAMERATYPES_H

#include <stdint.h>

/// 
/// @typedef CameraType
/// @struct camera_info_types
///
/// @brief Communication protocols supported currently by the SDK
///
typedef enum camera_info_types
{
	CAMTYPE_DEFAULT = 0,			/// Default value  
	CAMTYPE_GEV = 1,				/// GigE Camera protocol
	CAMTYPE_U3V = 2					/// USB3 Camera protocol
} CameraType;

///
///	@struct GEVCameraInfo
///
/// @brief GigE Camera information structure
///
typedef struct gev_camera_info_t
{
	char ModelName[64];				///< Camera model name
	char DeviceVersion[64];			///< Camera version
	char ManufacturerInfo[64];		///< Camera manufacturer information
	char Serial[32];				///< Camera serial number
	char UserDefinedName[32];		///< Camera user defined name
	uint8_t Status;					///< Camera access status
	char CamMAC[20];				///< Camera MAC address
	char CamIP[20];					///< Camera IP address
	char CamSubnet[20];				///< Camera subnet
	char CamGateway[20];			///< Camera gateway
	char AdapterIP[20];				///< Camera adapter IP address
	char AdapterMask[20];			///< Camera adapter mask
	char AdapterName[256 + 4];		///< Camera adapter name
	char AdapterDescription[256];   ///< Camera adapter description
	char AdapterMAC[64];            ///< Camera adapter MAC address
} GEVCameraInfo;

///
///	@struct U3VCameraInfo
///
/// @brief USB3 Camera information
///
typedef struct u3v_camera_info_t
{
	uint32_t    GenCPVersion;          ///< GenCP version
	uint32_t    U3VVersion;            ///< USB3 Vision version
	char   DeviceGUID[13];        ///< Index to USB3 Vision GUID string.
	char   VendorName[64];        ///< Index to Vendor Name string.
	char   ModelName[64];         ///< Index to Model Name string
	char   FamilyName[64];        ///< Index to Family Name string
	char   DeviceVersion[64];     ///< Index to Device Version string
	char   ManufacturerInfo[64];  ///< Index to Manufacturer Info string
	char   SerialNumber[64];      ///< Index to Serial Number string
	char   UserDefinedName[64];   ///< Index to Defined Name string
	uint8_t     SpeedSupport;          ///< Bitmask indicating the bus speeds supported
	uint8_t     OpenStatus;            ///< Status if device is already opened
} U3VCameraInfo;

///
///	@struct pfcamera_information_t
///
/// @brief Camera info structure
typedef struct pfcamera_information_t
{
	CameraType type;                        ///< Camera type: CAMTYPE_GEV, CAMTYPE_U3V
	GEVCameraInfo *GEVInfo;				    ///< Camera GEV information
	U3VCameraInfo *U3VInfo;				    ///< Camera U3V information
}pfCameraItem;

///
///	@struct pfcamera_list_t
///
/// @brief List of available Cameras. The maximum available is 40.
typedef struct pfcamera_list_t
{
	pfCameraItem Camera[40];		///< Available cameras array
	uint8_t GEVCount;					///< Number of GEV cameras
	uint8_t U3VCount;					///< Number of U3V cameras
} PFCameraList;

///
///	@struct pfcamera_channel_t
///
/// @brief Network configuration parameters
///
typedef struct pfcamera_channel_t
{
	uint32_t HeartbeatTimeout;			/// Control channel heartbeat timeout counter in milliseconds
	uint32_t ChannelTimeout;			/// Control channel timeout in milliseconds
	uint8_t ChannelRetry;				/// Control channel retry count
	uint32_t StreamTimeout;			/// Stream channel timeout in milliseconds
	uint16_t StreamPacketResend;		/// Stream channel packet resend count
	uint32_t StreamImageWaitTimeout;	/// Stream channel wait of an image timeout in milliseconds
} PFChannelParameter;

///
///	@struct pfcamera_msgchannel_t
///
/// @brief Event Control parameters for Message Channel according to GigE
///
typedef struct pfcamera_msgchannel_t
{
	uint16_t  EventID;						/// Identifies asyncronous event
	uint16_t  StreamChannelIndex;	/// Index of stream channel
	uint64_t  BlockID;					  /// Block identifier 
	uint64_t  TimeStamp;					/// Timestamp for the event
	uint8_t* Data;								/// Additional Data
	uint16_t DataLength;					/// Length of data	
} PFMSGChannelParameter;

#endif //PFCCAMERATYPES_H