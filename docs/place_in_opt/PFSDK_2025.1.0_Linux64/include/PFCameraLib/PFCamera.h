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
/// @file PFCamera.h
/// @brief Class containing the structures and functions to handle the communication with the camera.
///

#ifndef PFCAMERA_H
#define PFCAMERA_H

#include "PFBuffer.h"
#include "PFResult.h"
#include "IPFEventError.h"
#include "IPFEventFeatureChanged.h"
#include "IPFMessageChannelEvent.h"
#include "PFStream.h"
#include "PFCameraLib.h"
#include "PFCameraInfo.h"
#include "pfcImageTypes.h"
#include "pfcFeatureTypes.h"
#include "pfcCameraTypes.h"

#include <map>

/// 
/// @namespace pfcameralib  
///
/// @brief Main PFSDK Namespace
///
namespace pfcameralib
{
  ///
  /// @brief Set error receiver. This receiver is global for all the library and therefore you can only use 1 receiver for all the PFCamera objects used.
  ///
  void PFCAMERALIB_API SetEventErrorReceiver(IPFEventError* pEventErrorReceiver);

  ///
  /// @class PFCamera PFCamera.h
  ///
  /// This class contains the structures and functions to handle the communication with the camera.
  ///
  class PFCAMERALIB_API PFCamera
  {
  public:
    ///
    /// @struct Control
    /// @brief PFCamera control parameters
    /// 
    struct Control {
      ///
      /// @struct Acquisition
      /// @brief Acquisition configuration parameters
      /// 
      struct Acquisition {
        bool manage_streams = true;            // if true the stream can be Open/Closed after grab starts/stops
        bool reset_timestamp_counter = true;   // if true the timestamp can be resetted before grab starts
      } acquisition;
    };

    ///
    /// @brief Public class constructor
    ///
    PFCamera();

    ///
    /// @brief Public class destructor
    ///
    ~PFCamera();

    ///
    /// @brief Copy constructor
    ///
    PFCamera(const PFCamera&);

    ///
    /// @brief Move constructor
    ///
    PFCamera(PFCamera&&);

    ///
    /// @brief Copy assignment operator
    ///
    PFCamera& operator=(const PFCamera&);

    ///
    /// @brief Move assignment operator
    ///
    PFCamera& operator=(PFCamera&&);

    ///
    /// @brief Add Stream management to Camera. Note that this operation is mandatory before start to grab images.
    ///
    /// @param[in] pStream      Pointer to the Stream class.
    ///
    /// @return PFResult
    /// Error Codes:
    ///      PFSDK_ERROR_INVALID_STREAM_TYPE
    ///      PFSDK_NOERROR;
    PFResult AddStream(PFStream* pStream);

    ///
    /// @brief Connect to a GEV or U3V camera.
    ///
    /// @param[in] pfCameraInfo     Pointer with the struct of the selected camera info.
    ///
    /// @return PFResult
    /// Error Codes:
    ///      PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///      PFSDK_ERROR_CONNECT_NOT_AVALIBLE_SLOTS
    ///      PFSDK_ERROR_CONNECT_GEVINIT_FAILED
    ///      PFSDK_ERROR_CONNECT_GEVSETHEARTBEATRATE_FAILED
    ///      PFSDK_ERROR_CONNECT_GEVINIT_FAILED
    ///      PFSDK_ERROR_CONNECT_GEVINITXML_FAILED
    ///      PFSDK_ERROR_CONNECT_GEVOPENSTREAMCHANNEL_FAILED
    ///      PFSDK_ERROR_CONNECT_GEVSETPACKETRESEND_FAILED
    ///      PFSDK_ERROR_CONNECT_GEVSETMAXPACKETSIZE_FAILED
    ///      PFSDK_ERROR_CONNECT_U3V_OPENDEVICE_FAILED
    ///      PFSDK_ERROR_CONNECT_U3V_OPENCONTROL_FAILED
    ///      PFSDK_ERROR_CONNECT_U3V_READXML_FAILED
    ///      PFSDK_ERROR_CONNECT_U3V_PARSEXML_FAILED
    ///      PFSDK_ERROR_CONNECT_U3V_OPENSTREAM_FAILED
    ///      PFSDK_ERROR_CONNECT_CAMERA_NOT_FOUND
    ///      PFSDK_NOERROR
    ///
    PFResult Connect(PFCameraInfo& pfCameraInfo);

    ///
    /// @brief  Connect to a GEV camera giving the MAC or/and IP addresses.
    ///
    /// This function search for all connected  GEV devices and connects the camera that matches the input parameters.
    /// Variable parameters: (3 optional) TAG{"ip", "mac", "file", "deviceid"}, TAG_VALUE{"192.168.1.1","00:11:1C:F5:AE:9A", char *FileName, char *deviceID}
    ///
    /// @param[in] num      Number of input arguments: TAG and TAG_VALUE.
    /// @param[in] ...      Variable number of parameters "tag1, value1, tag2, value2, ..."
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_CONNECT_PARAM_NOT_VALID
    ///     PFSDK_ERROR_CONNECTION_ERROR
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_ERROR_CONNECT_NOT_AVALIBLE_SLOTS
    ///     PFSDK_ERROR_CONNECT_GEVINIT_FAILED
    ///     PFSDK_ERROR_CONNECT_GEVSETHEARTBEATRATE_FAILED
    ///     PFSDK_ERROR_CONNECT_GEVINIT_FAILED
    ///     PFSDK_ERROR_CONNECT_GEVINITXML_FAILED
    ///     PFSDK_ERROR_CONNECT_GEVOPENSTREAMCHANNEL_FAILED
    ///     PFSDK_ERROR_CONNECT_GEVSETPACKETRESEND_FAILED
    ///     PFSDK_ERROR_CONNECT_GEVSETMAXPACKETSIZE_FAILED
    ///     PFSDK_ERROR_CONNECT_U3V_OPENDEVICE_FAILED
    ///     PFSDK_ERROR_CONNECT_U3V_OPENCONTROL_FAILED
    ///     PFSDK_ERROR_CONNECT_U3V_READXML_FAILED
    ///     PFSDK_ERROR_CONNECT_U3V_PARSEXML_FAILED
    ///     PFSDK_ERROR_CONNECT_U3V_OPENSTREAM_FAILED
    ///     PFSDK_ERROR_CONNECT_CAMERA_NOT_FOUND
    ///     PFSDK_ERROR_OPEN_FILE_FAILED
    ///     PFSDK_ERROR_CAMERA_MISSMATCH_IN_CONFIGFILE
    ///     PFSDK_NOERROR
    ///
    PFResult Connect(uint8_t num, ...);

    ///
    /// @brief  Connect to a GEV camera giving the MAC or/and IP addresses.
    ///
    /// This function search for all connected  GEV devices and connects the camera that matches the input parameters.
    /// Variable parameters: (3 optional) TAG{"ip", "mac", "file", "deviceid"}, TAG_VALUE{"192.168.1.1","00:11:1C:F5:AE:9A", char *FileName, char *deviceID}
    ///
    /// @param[in] mapOfParameters A map of key - values representing the desired connection configuration.
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_CONNECT_PARAM_NOT_VALID
    ///     PFSDK_ERROR_CONNECTION_ERROR
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_ERROR_CONNECT_NOT_AVALIBLE_SLOTS
    ///     PFSDK_ERROR_CONNECT_GEVINIT_FAILED
    ///     PFSDK_ERROR_CONNECT_GEVSETHEARTBEATRATE_FAILED
    ///     PFSDK_ERROR_CONNECT_GEVINIT_FAILED
    ///     PFSDK_ERROR_CONNECT_GEVINITXML_FAILED
    ///     PFSDK_ERROR_CONNECT_GEVOPENSTREAMCHANNEL_FAILED
    ///     PFSDK_ERROR_CONNECT_GEVSETPACKETRESEND_FAILED
    ///     PFSDK_ERROR_CONNECT_GEVSETMAXPACKETSIZE_FAILED
    ///     PFSDK_ERROR_CONNECT_U3V_OPENDEVICE_FAILED
    ///     PFSDK_ERROR_CONNECT_U3V_OPENCONTROL_FAILED
    ///     PFSDK_ERROR_CONNECT_U3V_READXML_FAILED
    ///     PFSDK_ERROR_CONNECT_U3V_PARSEXML_FAILED
    ///     PFSDK_ERROR_CONNECT_U3V_OPENSTREAM_FAILED
    ///     PFSDK_ERROR_CONNECT_CAMERA_NOT_FOUND
    ///     PFSDK_ERROR_OPEN_FILE_FAILED
    ///     PFSDK_ERROR_CAMERA_MISSMATCH_IN_CONFIGFILE
    ///     PFSDK_NOERROR
    ///
    PFResult Connect(const std::map<std::string, std::string>& mapOfParameters);

    ///
    /// @brief  Connect to a GEV camera giving the MAC or/and IP addresses.
    ///
    /// This function searches for all connected  GEV devices and connects the camera that matches the input parameters.
    /// Parameters: (4 options)  Names: "ip", "mac", "deviceid", "sn", "file";  Values: "192.168.1.1", "00:11:1C:F5:AE:9A"
    ///
    /// @param[in] connectionString Names and values in the following format: name1,value1|name2,value2|...|nameN,valueN|
    ///                             For example: "ip,192.168.1.1|mac,00:11:1C:F5:AE:9A|"
    /// @return PFSDK_ERROR_CONNECT_PARAM_NOT_VALID
    /// @return PFSDK_ERROR_CONNECTION_ERROR
    /// @return PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    /// @return PFSDK_ERROR_CONNECT_NOT_AVALIBLE_SLOTS
    /// @return PFSDK_ERROR_CONNECT_GEVINIT_FAILED
    /// @return PFSDK_ERROR_CONNECT_GEVSETHEARTBEATRATE_FAILED
    /// @return PFSDK_ERROR_CONNECT_GEVINIT_FAILED
    /// @return PFSDK_ERROR_CONNECT_GEVINITXML_FAILED
    /// @return PFSDK_ERROR_CONNECT_GEVOPENSTREAMCHANNEL_FAILED
    /// @return PFSDK_ERROR_CONNECT_GEVSETPACKETRESEND_FAILED
    /// @return PFSDK_ERROR_CONNECT_GEVSETMAXPACKETSIZE_FAILED
    /// @return PFSDK_ERROR_CONNECT_U3V_OPENDEVICE_FAILED
    /// @return PFSDK_ERROR_CONNECT_U3V_OPENCONTROL_FAILED
    /// @return PFSDK_ERROR_CONNECT_U3V_READXML_FAILED
    /// @return PFSDK_ERROR_CONNECT_U3V_PARSEXML_FAILED
    /// @return PFSDK_ERROR_CONNECT_U3V_OPENSTREAM_FAILED
    /// @return PFSDK_ERROR_CONNECT_CAMERA_NOT_FOUND
    /// @return PFSDK_ERROR_OPEN_FILE_FAILED
    /// @return PFSDK_ERROR_CAMERA_MISSMATCH_IN_CONFIGFILE
    /// @return PFSDK_NOERROR
    ///
    PFResult ConnectWithString(const char* const connectionString);

    ///
    /// @brief Disconnect from a GEV or U3V camera.
    ///
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_ERROR_DISCONNECT_FAILED
    ///     PFSDK_NOERROR
    ///
    PFResult Disconnect();

    ///
    /// @brief Returns wether the camera is connected or not.
    ///
    ///
    /// @return  0                  Camera not connected
    /// @return  1                  Camera connected.
    ///
    uint8_t IsConnected();

    ///
    /// @brief Get information of the camera currently connected.
    ///
    /// @param[in,out] cameraInfo PFCameraInfo class.
    ///
    /// @return PFResult
    /// Error Codes:
    ///      PFSDK_ERROR_CAMERA_NOT_CONNECTED
    ///      PFSDK_NOERROR
    ///
    PFResult GetCameraInfo(PFCameraInfo*& cameraInfo);

    ///
    /// @brief Set Channel parameter values
    ///
    /// This function set the channel parameters for the camera.
    ///
    /// @param[in]      cparam          Channel parameter structure.
    ///                 HeartbeatTimeout: control channel heartbeat timeout counter in milliseconds
    ///                 ChannelTimeout: control channel timeout in milliseconds
    ///                 ChannelRetry : control channel retry count
    //                  StreamTimeout : stream channel timeout in milliseconds
    ///                 StreamPacketResend : stream channel packet resend count
    ///                 StreamImageWaitTimeout : stream channel wait of an image timeout in milliseconds
    ///
    /// @return PFSDK_ERROR_SETCHANNEL_FAILED
    /// @return PFSDK_NOERROR
    /// @return PFSDK_ERROR_NOT_GEV_CAMERA
    /// @return PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///
    PFResult SetChannelParameter(PFChannelParameter cparam);

    ///
    /// @brief Get Channel parameter values
    ///
    /// This function get the channel parameters for the camera.
    ///
    /// @param[in]      cparam      Channel parameter structure.
    ///                 HeartbeatTimeout: control channel heartbeat timeout counter in milliseconds
    ///                 ChannelTimeout: control channel timeout in milliseconds
    ///                 ChannelRetry : control channel retry count
    //                  StreamTimeout : stream channel timeout in milliseconds
    ///                 StreamPacketResend : stream channel packet resend count
    ///                 StreamImageWaitTimeout : stream channel wait of an image timeout in milliseconds
    ///
    /// @return PFSDK_ERROR_GETCHANNEL_FAILED
    /// @return PFSDK_NOERROR
    /// @return PFSDK_ERROR_NOT_GEV_CAMERA
    /// @return PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///
    PFResult GetChannelParameter(PFChannelParameter& cparam);

    ///
    /// @brief Set Heartbeat rate of the selected GEV camera.
    ///
    /// This function sets the heartbeat rate of the camera.
    /// It also changes the heartbeat timeout to a third of the heartbeate rate.
    ///
    /// @param[in]      heartBeatRate       Heartbeat Rate in milliseconds.
    ///
    /// @return PFResult
    /// Error Codes:
    ///      PFSDK_ERROR_CONNECT_GEVSETHEARTBEATRATE_FAILED
    ///      PFSDK_ERROR_NOT_GEV_CAMERA
    ///      PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///      PFSDK_NOERROR
    ///
    PFResult SetHeartbeatRate(uint32_t heartBeatRate);

    ///
    /// @brief Get Heartbeat rate of the selected GEV camera.
    ///
    /// This function sets the heartbeat rate of the camera.
    /// It also changes the heartbeat timeout to a third of the heartbeate rate.
    ///
    /// @param[in,out]      heartBeatRate       Heartbeat Rate.
    ///
    /// @return PFResult
    /// Error Codes:
    ///      PFSDK_ERROR_CONNECT_GEVSETHEARTBEATRATE_FAILED
    ///      PFSDK_ERROR_NOT_GEV_CAMERA
    ///      PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///      PFSDK_NOERROR
    ///
    PFResult GetHeartbeatRate(uint32_t& heartBeatRate);

    ///
    /// @brief Get the list of features available in the selected camera.
    ///
    /// @param[in, out] **pfFeatureItemInfo     Pointer to the feature list info.
    /// @param[in,out] *itemListSize            Size of the feature list.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_GETFEATURELIST_GEV_FAILED
    ///     PFSDK_ERROR_GETFEATURELIST_U3V_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult GetFeatureList(PFFeatureItemInfo** pfFeatureItemInfo, uint32_t* itemListSize);

    ///
    /// @brief Get the feature parameters.
    ///
    /// This function gets the selected feature parameters and it stores them into a struct.
    ///
    /// @param[in] *feature_name        Pointer to a string with the name of the feature.
    /// @param[in,out] *f_param         Pointer to a struct with the selected feature parameters.
    ///
    /// @return PFResult
    /// Error Codes:
    ///      PFSDK_ERROR_GETFEATURE_PARAMETERS_FAILED
    ///      PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///      PFSDK_ERROR_WRONG_PARAMETERS
    ///      PFSDK_NOERROR
    ///
    PFResult GetFeatureParams(const char* feature_name, PFFeatureParameters* f_param);

    ///
    /// @brief Get the feature's type.
    ///
    /// This function gets the selected feature's type.
    ///
    /// @param[in] *feature_name        Pointer to a string with the name of the feature.
    /// @param[in,out] type             Type of the feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///      PFSDK_ERROR_GETFEATURE_PARAMETERS_FAILED
    ///      PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///      PFSDK_ERROR_WRONG_PARAMETERS
    ///      PFSDK_NOERROR
    ///
    PFResult GetFeatureType(const char* feature_name, uint8_t& type);

    /// @brief Get feature invalidators.
    ///
    /// This function gets the invalidator parameters from a feature inside the XML file.
    ///
    /// @param[in] *feature_name            Pointer to the name of the selected feature.
    /// @param[in] index                    Invalidator index.
    /// @param[in,out] *invalidator_name    Pointer to the name with the feature invalidator.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_GETFEATUREINVALIDATOR_U3V_FAILED
    ///     PFSDK_ERROR_GETFEATUREINVALIDATOR_GEV_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult GetFeatureInvalidator(const char* feature_name, const uint8_t index, char* invalidator_name);

    ///
    /// @brief Get boolean value of a feature.
    ///
    /// This function returns the boolean value of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name    Pointer to the name of the selected feature.
    /// @param[in,out] bool_value   Pointer to the boolean value of the feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_GETFEATURE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult GetFeatureBool(const char* feature_name, bool& bool_value);

    ///
    /// @brief Set boolean value of a feature.
    ///
    /// This function sets the boolean value of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name    Pointer to the name of the selected feature.
    /// @param[in,out] bool_value   Boolean value of the feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_SETFEATURE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult SetFeatureBool(const char* feature_name, const bool bool_value);

    ///
    /// @brief Get command value of a feature.
    ///
    /// This function returns the command value of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name    Pointer to the name of the selected feature.
    /// @param[in,out] cmd_value    Reference to the command value of the feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_GETFEATURE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult GetFeatureCommand(const char* feature_name, uint32_t& cmd_value);

    ///
    /// @brief Set command value of a feature.
    ///
    /// This function sets the command value of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name    Pointer to the name of the selected feature.
    /// @param[in,out] cmd_value    Command value of the feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_SETFEATURE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult SetFeatureCommand(const char* feature_name, uint32_t cmd_value);

    ///
    /// @brief Get enumeration name of a feature.
    ///
    /// This function returns the enumeration name of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name    Pointer to the name of the selected feature.
    /// @param[in,out] *str_value   Pointer to the enumeration name of the feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_GETFEATURE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult GetFeatureEnum(const char* feature_name, char* str_value);

    ///
    /// @brief Set enumeration name of a feature.
    ///
    /// This function sets the enumeration name of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name    Pointer to the name of the selected feature.
    /// @param[in,out] *str_value   Pointer to the enumeration name of the feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_SETFEATURE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult SetFeatureEnum(const char* feature_name, const char* str_value);

    ///
    /// @brief Get name of enumeration name of a feature.
    ///
    /// This function returns name of enumeration name of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name    Pointer to the name of the selected feature.
    /// @param[in] enum_index       Enumeration index.
    /// @param[in,out] *enum_name   Pointer to the name of enumeration name of the feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_GETFEATURE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult GetFeatureEnumName(const char* feature_name, uint8_t enum_index, char* enum_name);

    ///
    /// @brief Get float value of a feature.
    ///
    /// This function returns the float value of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name        Pointer to the name of the selected feature.
    /// @param[in,out] **double_value   Pointer to the float value of the feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_GETFEATURE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult GetFeatureFloat(const char* feature_name, double& double_value);

    ///
    /// @brief Set float value of a feature.
    ///
    /// This function sets the float value of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name        Pointer to the name of the selected feature.
    /// @param[in,out] double_value     Float value of the feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_SETFEATURE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult SetFeatureFloat(const char* feature_name, const double double_value);

    ///
    /// @brief Get int value of a feature.
    ///
    /// This function returns the int value of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name    Pointer to the name of the selected feature.
    /// @param[in,out] *int_value   Pointer to the int value of the feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_GETFEATURE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult GetFeatureInt(const char* feature_name, int64_t& int_value);

    ///
    /// @brief Set int value of a feature.
    ///
    /// This function sets the int value of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name    Pointer to the name of the selected feature.
    /// @param[in] int_value        Int value of the feature.
    /// @param[in] bCheckRange      Check if value is inside min,max range values. By default is true.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_SETFEATURE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult SetFeatureInt(const char* feature_name, const int64_t int_value, const bool bCheckRange = true);

    ///
    /// @brief Get string value of a feature.
    ///
    /// This function returns the string value of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name    Pointer to the name of the selected feature.
    /// @param[in,out] *str_value   Pointer to the string value of the feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_GETFEATURE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult GetFeatureString(const char* feature_name, char* str_value);

    /// @brief Set string value of a feature.
    ///
    /// This function sets the string value of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name    Pointer to the name of the selected feature.
    /// @param[in,out] *str_value   Pointer to the string value of the feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_SETFEATURE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult SetFeatureString(const char* feature_name, const char* str_value);

    /// @brief SetS the feature's value from a string regardless of the feature type.
    ///
    /// This function sets the value of the given feature from a string regardless of the feature type.
    ///
    /// @param[in] *feature_name    Pointer to the name of the selected feature.
    /// @param[in,out] *str_value   Pointer to the string value of the feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_SETFEATURE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult SetFeatureValueFromString(const char* feature_name, const char* str_value);

    ///
    /// @brief Get tooltip value of a feature.
    ///
    /// This function returns the tooltip value of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name            Pointer to the name of the selected feature.
    /// @param[in,out] *tooltip_name        Pointer to the tooltip value of the feature.
    /// @param[in,out] tooltip_name_length  Integer to the tooltip value length.
    ///
    /// @return PFResult
    /// Error Codes:
    ///      PFSDK_ERROR_GETFEATURE_TOOLTIP_FAILED
    ///      PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///      PFSDK_NOERROR
    ///
    PFResult GetFeatureToolTip(const char* feature_name, char* tooltip_name, int tooltip_name_length);
    
    ///
    /// @brief Get Description value of a feature. 
    ///
    /// This function returns the Description value of a dedicated feature, descripted in XML file.
    ///
    /// @param[in] *feature_name            Pointer to the name of the selected feature.
    /// @param[in,out] *description         Pointer to the description value of the feature.
    /// @param[in,out] description_length   Integer to the description value length.
    ///
    /// @return PFResult
    /// Error Codes:
    ///      PFSDK_ERROR_GETFEATURE_TOOLTIP_FAILED
    ///      PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///      PFSDK_NOERROR
    ///
    PFResult GetFeatureDescription(const char* feature_name, char* description, int description_length);
    
    ///
    /// @brief Get Feature Enable Status
    ///
    /// This function reads enable status for specific feature
    /// 
    /// @param[in] *feature_name        Name of the feature
    /// @param[in] *enable              Enable status for feature: 0 = disable 1 = enable
    /// 
    /// Return    : PFSDK_NOERROR
    ///             PFSDK_ERROR_GETFEATURE_ENABLE_FAILED
    ///
    PFResult GetFeatureEnableStatus(const char* feature_name, uint8_t* enable);


    /// 
    /// @brief Get Feature Display Name
    ///
    /// This function gets the feature display name 
    /// 
    /// @param[in] *feature_name        Name of the feature
    /// @param[in] *display_name        Display name
    /// @param[in] display_name_length  Length of display name
    ///
    /// Return    : PFSDK_NOERROR
    ///             PFSDK_ERROR_GETFEATURE_DISPLAY_NAME
    ///
    PFResult GetFeatureDisplayName(const char* feature_name, char* display_name, int display_name_length);

    ///
    /// @brief Save current GEV/U3V camera configuration to a file.
    ///
    /// This function reads all the features values and it stores them in a text file.
    ///
    /// @param[in] *FileName            Pointer to a path to store the configuration file.
    ///
    /// @return PFResult
    /// Error Codes:
    ///      PFSDK_ERROR_OPEN_FILE_FAILED
    ///      PFSDK_ERROR_GETFEATURE_FAILED
    ///      PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///      PFSDK_NOERROR
    ///
    PFResult SaveCurrentConfigToFile(const char* FileName);

    ///
    /// @brief Load to a GEV/U3V camera  a configuration from a file.
    ///
    /// To use this function you must be connected to the camera.
    /// This function reads all (see exceptions bellow) the features values described in the configuration file and
    /// it loads them to the connected camera.
    /// 
    /// Most Gev**** parameters are ignored to avoid changing the network configuration of the camera.
    /// List of GEV parameters that DO take effect:
    /// - 	"GevSCPD","GevMCRC","GevMCTT","GevHeartbeatTimeout","GevSCPSPacketSize","GevSCPSDoNotFragment","GevSCPSFireTestPacket"
    ///
    /// @param[in] *FileName            Pointer to the path of the configuration file.
    ///
    /// @return PFResult
    /// Error Codes:
    ///      PFSDK_ERROR_CAMERA_NOT_CONNECTED
    ///      PFSDK_ERROR_OPEN_FILE_FAILED
    ///      PFSDK_ERROR_CAMERA_MISSMATCH_IN_CONFIGFILE
    ///      PFSDK_ERROR_SETFEATURE_FAILED
    ///      PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///      PFSDK_NOERROR
    ///
    PFResult LoadConfigFromFile(const char* FileName);

    ///
    /// @brief Get current GEV/U3V camera GenICam XML file.
    ///
    /// This function gets the xml file inside the camera and it stores int in a text file.
    ///
    /// @param[in] *FileName            Pointer to a path to store the xml file.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_GETXMLFILE_U3V_READXML_FAILED
    ///     PFSDK_ERROR_GETXMLFILE_GEV_READXML_FAILED
    ///     PFSDK_ERROR_OPEN_FILE_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult GetXMLFile(const char* FileName);
        
    ///
    /// @brief Set current GEV camera GenICam XML file.
    ///
    /// This function configures xml file inside the camera.
    ///
    /// @param[in] *FileName   Pointer to a path to load the xml file.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_SETXMLFILE_GEV_LOADXML_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    /// 
    PFResult SetXMLFile(const char* FileName);

    ///
    /// @brief Start image acquisition from GEV/U3V camera.
    ///
    /// By default, this function locks stream parameters (TLParamsLocked), opens the stream channel, 
    /// resets timestamp, and starts acquisition.
    /// 
    /// To override this behaviour, use the SetControl method.
    /// See PFCameraLib_ConfigAndGrabConsole_Advanced_Asynchronous sample.
    /// 
    /// The grab mode is determined by the "AcquisitionMode" feature.
    ///
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_GRAB_U3V_SETBUFFERCOUNT_FAILED
    ///     PFSDK_ERROR_GRAB_U3V_SETTRANSFERSIZE_FAILED
    ///     PFSDK_ERROR_GRAB_U3V_ACQUISITIONSTART_FAILED
    ///     PFSDK_ERROR_GRAB_U3V_TLPARAMSLOCKED_FAILED
    ///     PFSDK_ERROR_GRAB_GEV_ACQUISITIONSTART_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult Grab();

    ///
    /// @brief Stop image acquisition from GEV/U3V camera.
    ///
    /// By default, this function stops image acquisition from the camera, closes the stream channel, 
    /// and unlocks the stream parameters (TLParamsLocked).
    ///
    /// To override this behaviour, use the SetControl method.
    /// See PFCameraLib_ConfigAndGrabConsole_Advanced_Asynchronous sample.
    /// 
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_FREEZE_U3V_ACQUISITIONSTOP_FAILED
    ///     PFSDK_ERROR_FREEZE_U3V_TLPARAMSLOCKED_FAILED
    ///     PFSDK_ERROR_FREEZE_GEV_ACQUISITIONSTOP_FAILED
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult Freeze();

    ///
    /// @brief Reset the timestamp counter from which the timestamp of the acquired images is fetched.
    /// 
    /// @return PFResult
    /// Error Codes:
    ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
    ///     PFSDK_NOERROR
    ///
    PFResult ResetTimestampCounter();

    ///
    /// @brief Check if the camera is grabbing.
    ///
    /// This function returns the camera grab status.
    ///
    /// @return  0              Camera is not grabbing images.
    /// @return  1              Camera is grabbing images.
    ///
    uint8_t IsGrabbing();

    ///
    /// @brief Check if the camera images are in color
    ///
    /// This function returns if the camera images are in color
    ///
    /// @return  false          Camera has not color support
    /// @return  true           Camera has color support
    ///
    bool isColorCamera();

    ///
    /// @brief Check if the camera images are in double rate format
    ///
    /// This function returns if the camera images are in double rate format
    ///
    /// @return  false              Camera images are not in double rate format
    /// @return  true               Camera images are in double rate format
    ///
    bool isDRCamera();

    ///
    /// @brief Set PFCamera to disconnected state and initialize internal data
    ///
    /// Call this function perform a logical disconnection when Disconnect method returns an error.
    /// This can be the case when the camera has been physically disconnected and no communication is possible.
    ///
    void SetDisconnected();

    ///
    /// @brief SetActionCommand
    ///
    /// Set action command
    ///
    /// @param[in] *device_key        Device Key    - specifies the device key to authorize the action on this device.
    /// @param[in] *group_key         Group Key     - specifies the group key to define a group of devices on which the actions have to be executed.
    /// @param[in] *group_mask        Group Mask    - specifies the group mask to be used to filter out some of these devices from the group.
    /// @param[in] *action_time       Action Time   - time offset in seconds
    ///
    /// @return  PFResult
    ///
    PFResult SetActionCommand(uint32_t device_key, uint32_t group_key, uint32_t group_mask, uint32_t action_time);

    ///
    /// @brief AddEventFeatureChangedSink
    ///
    /// Add a sink which will receive a notification when the specified feature changes.
    ///
    /// @param[in] *feature_name        Feature name            -   specifies the name of the feature that will trigger the callback
    /// @param[in] *pSink               Sink object pointer     -   pointer to object implementing the IPFEventFeatureChanged interface.
    ///
    /// @return  PFResult
    ///
    PFResult AddEventFeatureChangedSink(const char* feature_name, IPFEventFeatureChanged* pSink);

    ///
    /// @brief RemoveEventFeatureChangedSink
    ///
    /// Remove an event sink from the specified feature. This object will no longer receive notifications when the feature changes.
    ///
    /// @param[in] *feature_name        Feature name            -   specifies the name of the feature
    /// @param[in] *pSink               Sink object pointer     -   pointer to object to be removed
    ///
    /// @return  PFResult
    ///
    PFResult RemoveEventFeatureChangedSink(const char* feature_name, IPFEventFeatureChanged* pSink);

    ///
    /// @brief RemoveEventFeatureChangedSink
    ///
    /// Remove an event sink from all features. This object will no longer receive notifications.
    ///
    /// @param[in] *pSink               Sink object pointer     -   pointer to object to be removed
    ///
    /// @return  PFResult
    ///
    PFResult RemoveEventFeatureChangedSink(IPFEventFeatureChanged* pSink);

    ///
    /// @brief AddEventFeatureChangedSink
    ///
    /// Same as the above, but adds a C style callback.
    ///
    /// void callback(const char*, uint8_t, PFFeatureValueUnion, void*)
    ///
    /// @param[in] *feature_name            Feature name to which the callback is added.
    /// @param[in] *callback                Pointer to callback function.
    /// @param[in] *user_data               User data pased through to the callback.
    ///
    /// @return  PFResult
    ///
    PFResult AddEventFeatureChangedSink(const char* feature_name, PFFeatureCallback callback, void* user_data = nullptr);

    ///
    /// @brief RemoveEventFeatureChangedSink
    ///
    /// Same as the above, but removes a C style callback.
    ///
    /// @param[in] *feature_name            Feature name to which from which the callback is removed.
    /// @param[in] *callback                Pointer to callback function.
    ///
    /// @return  PFResult
    ///
    PFResult RemoveEventFeatureChangedSink(const char* feature_name, PFFeatureCallback callback);

    ///
    /// @brief SetMessageChannelSink
    ///
    /// @param[in] *pEventMessageSink        Pointer to IPFMessageChannelEvent class implementation
    ///
    /// @return  PFResult
    PFResult SetMessageChannelSink(IPFMessageChannelEvent* pEventMessageSink);

    ///
    /// @brief Get Acquisition Frame Count.
    ///
    /// Number of frames for to capture depending on Acquisition Mode.
    ///
    ///    0               --> Continuous mode
    ///    1               --> Single frame
    ///    > 1             --> Multi frames
    ///
    /// @param[out] acquisition_frame_count   Variable to which the result is stored
    ///
    /// @return PFResult
    ///
    PFResult GetAcquisitionFrameCount(uint64_t& acquisition_frame_count);

    /// 
    /// @brief Define the behaviour of the camera object.
    /// 
    /// Provides a means of overriding default behaviour. 
    /// Intended for use with advanced grabbing workflows.
    /// 
    /// i.e Keep the stream channel open and only issue AcquisitionStart and AcquisitionStop commands, reusing resources.
    ///
    /// @param[in] control            New configuration.
    ///
    /// @return  PFResult   
    /// 
    PFResult SetControl(const PFCamera::Control& control);

    /// 
    /// @brief Get current control parameters.
    
    /// @return  PFCamera::Control
    /// 
    PFCamera::Control GetControl();

  private:
    friend class PFStream;
    class PFCameraImpl;
    PFCameraImpl* m_pPfCameraImpl;
  };
}




#endif //PFCAMERA_H