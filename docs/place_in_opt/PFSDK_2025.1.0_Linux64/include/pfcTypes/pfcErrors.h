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
/// @file pfcErrors.h
/// @brief Definition of the PF SDK error codes.
///
/// Further Description.
///
/// @note
/// @author TAH 
///
#ifndef PFCERRORS_H
#define PFCERRORS_H

#define GEV_DISCOVERY_STATUS_OK					      0
#define GEV_DISCOVERY_STATUS_ALREADY_OPEN		  1
#define GEV_DISCOVERY_STATUS_NOT_SAME_SUBNET	2
#define GEV_DISCOVERY_STATUS_CONTROL_OPEN		  3

// PFSDK Errors
// Errors are defined as int32_t -> therefore from 0x80000000 to 0xFFFFFFFF

#define PFSDK_NOERROR							                                0x00000000u

// Generic Error Codes
// Note: Max size for signed int int32_t is 2^31 -1. First 4 bytes (16 bits) aure used to codify the error. 
#define PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED					            0x80000000u
#define PFSDK_ERROR_NOT_GEV_CAMERA									              0x80010000u
#define PFSDK_ERROR_NOT_U3V_CAMERA									              0x80020000u
#define PFSDK_ERROR_DISCOVERY_GEV_NO_CAMERAS_FOUND		            0x80030000u
#define PFSDK_ERROR_DISCOVERY_U3V_NO_CAMERAS_FOUND		            0x80040000u
#define PFSDK_ERROR_DISCOVERY_NO_CAMERAS_FOUND				            0x80050000u
#define PFSDK_ERROR_CONNECTION_ERROR								              0x80060000u
#define PFSDK_ERROR_CONNECT_CAMERA_NOT_FOUND					            0x80070000u
#define PFSDK_ERROR_CONNECT_NOT_AVAILABLE_SLOTS				            0x80080000u
#define PFSDK_ERROR_CONNECT_PARAM_WRONG_SYNTAX				            0x80090000u
#define PFSDK_ERROR_CONNECT_PARAM_NOT_VALID						            0x800A0000u
#define PFSDK_ERROR_OPEN_FILE_FAILED								              0x800B0000u
#define PFSDK_ERROR_CAMERA_MISSMATCH_IN_CONFIGFILE		            0x800C0000u
#define PFSDK_ERROR_SAVE_IMAGE_TO_FILE								            0x800D0000u
#define PFSDK_ERROR_LOAD_IMAGE_FROM_FILE							            0x800E0000u
#define PFSDK_ERROR_WRONG_PIXEL_TYPE								              0x800F0000u
#define PFSDK_ERROR_CAMERA_NOT_CONNECTED							            0x80100000u
#define PFSDK_ERROR_CAMERA_NOT_DR									                0x80110000u
#define PFSDK_ERROR_CAMERA_ASSIGNED_IP								            0x80120000u
#define PFSDK_ERROR_CAMERA_UNASSIGNED_IP							            0x80130000u
#define PFSDK_ERROR_STREAM_CLOSED									                0x80140000u
#define PFSDK_ERROR_UNKNOWN_BUFFER_SIZE								            0x80150000u
#define PFSDK_ERROR_INVALID_STREAM_TYPE								            0x80160000u
#define PFSDK_ERROR_STREAM_UNASSIGNED								              0x80170000u
#define PFSDK_ERROR_OPENSTREAMCHANNEL_FAILED					            0x80180000u
#define PFSDK_ERROR_CLOSESTREAMCHANNEL_FAILED					            0x80190000u
#define PFSDK_ERROR_GRAB_READPAYLOADSIZE_FAILED				            0x801A0000u
#define PFSDK_ERROR_BUFFER_ALLOCATED							                0x801B0000u
#define PFSDK_ERROR_UNSUPPORTED_VENDOR								            0x801C0000u
#define PFSDK_ERROR_UNSUPPORTED_PIXEL_TYPE						            0x801D0000u
#define PFSDK_ERROR_QUEUE_EMPTY										                0x801E0000u
#define PFSDK_ERROR_BUFFER_NULL										                0x801F0000u
#define PFSDK_ERROR_STREAM_ALREADY_OPEN						                0x80250000u
#define PFSDK_ERROR_GRAB_STOP_TIMEOUT								              0x80260000u
#define PFSDK_ERROR_BUFFERCOUNT_INVALID								            0x80270000u
#define PFSDK_ERROR_QUEUE_FULL										                0x80280000u
#define PFSDK_ERROR_INVALID_BUFFER_SIZE								            0x80290000u
#define PFSDK_ERROR_TLPARAMSLOCKED				                        0x802A0000u
#define PFSDK_ERROR_RING_BUFFER_INIT_FAILURE                      0x802B0000u
#define PFSDK_ERROR_BUSY_STATUS                                   0x802C0000u
#define PFSDK_ERROR_DRIVER_IOCTL_FAILURE                          0x802D0000u
#define PFSDK_ERROR_QUEUE_NULL						                        0x802E0000u
#define PFSDK_ERROR_CAMERA_UNASSIGNED			                        0x802F0000u


// Feature errors
#define PFSDK_ERROR_GETFEATURE_FAILED	                            0x80300000u
#define PFSDK_ERROR_SETFEATURE_FAILED								              0x80310000u
#define PFSDK_ERROR_SETFEATURE_INT_OUTOFLIMITS				            0x80320000u
#define PFSDK_ERROR_SETFEATURE_INT_WRONGINCREMENT			            0x80330000u
#define PFSDK_ERROR_SETFEATURE_FLOAT_OUTOFLIMITS			            0x80340000u
#define PFSDK_ERROR_GETFEATUREENUMNAME_FAILED					            0x80350000u
#define PFSDK_ERROR_INVALIDATOR_INDEX								              0x80360000u
#define PFSDK_ERROR_GETFEATURE_PARAMETERS_FAILED			            0x80370000u
#define PFSDK_ERROR_GETFEATURE_TOOLTIP_FAILED					            0x80380000u
#define PFSDK_ERROR_FEATURE_INVALID_TYPE							            0x80390000u
#define PFSDK_ERROR_FEATURE_READ_ONLY								              0x803A0000u
#define PFSDK_ERROR_WRONG_PARAMETERS								              0x803B0000u
#define PFSDK_ERROR_ENUM_INVALID_INDEX								            0x803C0000u
#define PFSDK_ERROR_SETFEATURE_STRING_OUTOFLIMITS			            0x803D0000u
#define PFSDK_ERROR_GETFEATURE_ENABLE_FAILED			                0x803E0000u
#define PFSDK_ERROR_GETFEATURE_DISPLAY_NAME                       0x803F0000u
#define PFSDK_ERROR_GETFEATURE_DESCRIPTION_FAILED                 0x80400000u



// Image conversion errors
#define PFSDK_ERROR_CONVERSION_NOT_ALLOWED						            0x80500000u
#define PFSDK_ERROR_DEBAYER_OPERATION_NOT_ALLOWED			            0x80510000u
#define PFSDK_ERROR_DEMODULATE_IMAGE_DR								            0x80520000u
#define PFSDK_ERROR_IMAGE_UNITIALIZED								              0x80530000u
#define PFSDK_ERROR_UNPACK_NOT_PERFORMED							            0x80540000u
#define PFSDK_ERROR_DEMODULATION_NOT_ALLOWED	  			            0x80550000u
#define PFSDK_ERROR_INVALID_IMAGE_TRANSFORM_CONTEXT               0x80560000u

// GEV Error Codes
#define PFSDK_ERROR_DISCOVERY_GEV_FAILED							            0x80600000u
#define PFSDK_ERROR_CONNECT_GEVINIT_FAILED						            0x80610000u
#define PFSDK_ERROR_CONNECT_GEVSETHEARTBEATRATE_FAILED				    0x80620000u
#define PFSDK_ERROR_CONNECT_GEVINITXML_FAILED						          0x80630000u
#define PFSDK_ERROR_CONNECT_GEVOPENSTREAMCHANNEL_FAILED				    0x80640000u
#define PFSDK_ERROR_CONNECT_GEVSETPACKETRESEND_FAILED				      0x80650000u
#define PFSDK_ERROR_CONNECT_GEVSETMAXPACKETSIZE_FAILED				    0x80660000u
#define PFSDK_ERROR_DISCONNECT_FAILED								              0x80670000u
#define PFSDK_ERROR_FORCEIP_FAILED									              0x80680000u
#define PFSDK_ERROR_GETFEATURELIST_GEV_FAILED						          0x80690000u
#define PFSDK_ERROR_GETFEATUREINVALIDATOR_GEV_FAILED				      0x806A0000u
#define PFSDK_ERROR_ADDNETWORKINFO_GEV_FAILED						          0x806B0000u
#define PFSDK_ERROR_GRAB_NOT_STARTED								              0x806C0000u
#define PFSDK_ERROR_GRAB_GEV_ACQUISITIONSTART_FAILED				      0x806D0000u
#define PFSDK_ERROR_GRAB_GEV_TLPARAMSLOCKED_FAILED					      0x806E0000u
#define PFSDK_ERROR_FINDMAX_PACKET_FAILED									        0x806F0000u

#define PFSDK_ERROR_GRAB_ALREADY_STARTED							            0x80700000u
#define PFSDK_ERROR_GRAB_ERROR										                0x80710000u
#define PFSDK_ERROR_SETCHANNEL_FAILED								              0x80720000u
#define PFSDK_ERROR_FREEZE_GEV_ACQUISITIONSTOP_FAILED				      0x80730000u
#define PFSDK_ERROR_FREEZE_GEV_TLPARAMSLOCKED_FAILED				      0x80740000u
#define PFSDK_ERROR_GETXMLFILE_GEV_READXML_FAILED					        0x80750000u
#define PFSDK_ERROR_SETXMLFILE_GEV_LOADXML_FAILED                 0x80760000u
#define PFSDK_ERROR_GETFEATURELIST_UNLOADED							          0x80770000u
#define PFSDK_ERROR_HEARTBEAT_TIMEOUT								              0x80780000u
#define PFSDK_ERROR_CONNECTION_FAILURE								            0x80790000u
#define PFSDK_ERROR_RECONNECTION									                0x807A0000u
#define PFSDK_ERROR_WRITE_REGISTER_FAILURE									      0x807B0000u


// Get Image errors
#define PFSDK_ERROR_GETIMAGE_GEV_FAILED								            0x81000000u
#define	PFSDK_ERROR_GETIMAGE_TIMEOUT								              0x81010000u
#define	PFSDK_ERROR_GETIMAGE_NULL_POINTER							            0x81020000u
#define	PFSDK_ERROR_GETIMAGE_MISSING_PACKETS						          0x81030000u
#define	PFSDK_ERROR_GETIMAGE_GRAB_ERROR								            0x81040000u
#define	PFSDK_ERROR_GETIMAGE_DIFFERENT_IMAGE_HEADER					      0x81050000u

// U3V Error Codes
#define PFSDK_ERROR_DISCOVERY_U3V_FAILED							            0x90000000u
#define PFSDK_ERROR_CONNECT_U3V_OPENDEVICE_FAILED					        0x90010000u
#define PFSDK_ERROR_CONNECT_U3V_OPENCONTROL_FAILED					      0x90020000u
#define PFSDK_ERROR_CONNECT_U3V_READXML_FAILED						        0x90030000u
#define PFSDK_ERROR_CONNECT_U3V_PARSEXML_FAILED						        0x90040000u
#define PFSDK_ERROR_CONNECT_U3V_OPENSTREAM_FAILED					        0x90050000u
#define PFSDK_ERROR_GETFEATURELIST_U3V_FAILED						          0x90060000u
#define PFSDK_ERROR_GETFEATUREINVALIDATOR_U3V_FAILED				      0x90070000u
#define PFSDK_ERROR_GRAB_U3V_SETBUFFERCOUNT_FAILED					      0x90080000u
#define PFSDK_ERROR_GRAB_U3V_SETTRANSFERSIZE_FAILED					      0x90090000u
#define PFSDK_ERROR_GRAB_U3V_ACQUISITIONSTART_FAILED				      0x90100000u
#define PFSDK_ERROR_GRAB_U3V_TLPARAMSLOCKED_FAILED					      0x90110000u
#define PFSDK_ERROR_GETIMAGE_U3V_FAILED								            0x90120000u
#define PFSDK_ERROR_FREEZE_U3V_ACQUISITIONSTOP_FAILED				      0x90130000u
#define PFSDK_ERROR_FREEZE_U3V_TLPARAMSLOCKED_FAILED				      0x90140000u
#define PFSDK_ERROR_GETXMLFILE_U3V_READXML_FAILED					        0x90150000u
#define PFSDK_ERROR_UNKNOWN											                  0xFFFF0000u

//
#define PFSDK_ERROR_SHARED_MEMORY_OPEN_FAILED				              0xA0000000u
#define PFSDK_ERROR_NETWORK_STARTUP									              0xA1000000u

#define PFSDK_ERROR_FILE_EMPTY						                        0xB0000000u

#define PFSDK_ERROR_DRIVER_INIT_FAILED								            0xD0000000u
#define PFSDK_ERROR_DRIVER_GET_VERSION_FAILED						          0xD0010000u

//User errors
#define PFSDK_ERROR_EVENT_HANDLER_ALREADY_EXISTS                  0xC0000000u
#define PFSDK_ERROR_EVENT_HANDLER_NOT_FOUND                       0xC0010000u

#endif //PFCERRORS_H