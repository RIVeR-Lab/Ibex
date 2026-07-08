/*
MultiRecorder - communication with XIMEA cameras over http, fast recording, remote camera control, multiplatform

Copyright (c) 2023-2025 XIMEA s.r.o.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*/

#include "CameraControl.h"
#include "ImageExporter.h"
#include "LiveImagePreview.h"
#include "RecorderApi.h"
#include "UserImageProcessor.h"

#include <cmath>
#include <chrono>
#include <filesystem>
#include <list>
#include <set>

#ifdef MULTIRECORDER_FIS_ENABLE
#include <FIS_lib.h>
#include <FIS_camera_io.h>
#endif

#include <tinyxml.h>

std::list<SxCameraParamDesc> g_lstCameraParamDesc = {
#include <xiApiParams.h>
};

#include <xiApiEnumerators.h>

//----------------------------------------------------------------------------------------
CxCameraControl::CxCameraControl()
{
    m_hCamera = nullptr;
    m_bAcquisitionPaused = false;
    m_eStatus = exsInitialization;
    m_pRecordingThread = nullptr;
    m_pExportingThread = nullptr;
    m_bCancelRecording = false;
    m_bCancelExporting = false;
    m_bPrintServerLogVerbose = true;
    m_iTransportBufferSizeDefault = 0;
    m_timeAcqStart = 0;
    m_iLastRecordedFrames = 0;
    m_dLastSpeedValue = 0.0;
    m_iTiffExportCounter = 0;
    m_iLoadedExportImageFormat = -1;
    m_iConcatImgCount = 0;
    m_iConcatImgNextOffset = 0;
    m_bShowLiveImagePreview = false;
    m_dLiveImagePreviewFps = 1.0;
    m_pUserImageProcessor = nullptr;

    xiSetParamInt(NULL, XI_PRM_NEW_PROCESS_CHAIN_ENABLE, XI_ON);

#ifdef MULTIRECORDER_FIS_ENABLE
    FIS_setErrorLogFunction(xiRecorderApi::outputStringToStdErr);

    if (FIS_isHDDLowLevelRecorderAvailable())
        m_pRecorder = FIS_createHDDLowLevelRecorder();
    else
        m_pRecorder = FIS_createHDDRecorder();
#endif
}

//----------------------------------------------------------------------------------------
CxCameraControl::~CxCameraControl()
{
    if (m_pRecordingThread != nullptr)
    {
        m_bCancelRecording = true;
        m_pRecordingThread->join();
        delete m_pRecordingThread;
    }
    if (m_pExportingThread != nullptr)
    {
        m_bCancelExporting = true;
        m_pExportingThread->join();
        delete m_pExportingThread;
    }

    // delete data files
    releaseDataBuffers(true);

    // delete rocorder
#ifdef MULTIRECORDER_FIS_ENABLE
    if (m_pRecorder != nullptr)
        delete m_pRecorder;
#endif

    if (m_pUserImageProcessor != nullptr)
        delete m_pUserImageProcessor;

    // close camera
    closeOpenedCamera();
}

//----------------------------------------------------------------------------------------
void CxCameraControl::setVerboseOutputLog(bool bVerose)
{
    m_bPrintServerLogVerbose = bVerose;
}

//----------------------------------------------------------------------------------------
void CxCameraControl::printOutputMessage(const std::string &sMessage, bool bVerbose)
{
    if (!bVerbose || m_bPrintServerLogVerbose)
        xiRecorderApi::outputStringToStdErr(sMessage);
}

//----------------------------------------------------------------------------------------
CxCameraControl::ExStatus CxCameraControl::status()
{
    return m_eStatus;
}

//----------------------------------------------------------------------------------------
std::string CxCameraControl::statusString()
{
    std::string sStatus;
    switch (m_eStatus)
    {
    case exsInitialization:     sStatus = XI_SRV_STATUS_INIT; break;
    case exsInitializationError: sStatus = XI_SRV_STATUS_INIT_ERROR; break;
    case exsPreparedForAcq:     sStatus = XI_SRV_STATUS_PREPARED_FOR_ACQ; break;
    case exsRecording:          sStatus = XI_SRV_STATUS_RECORDING; break;
    case exsRecordingFinished:  sStatus = XI_SRV_STATUS_RECORDING_FINISHED; break;
    case exsExporting:          sStatus = XI_SRV_STATUS_EXPORTING; break;
    case exsExportingFinished:  sStatus = XI_SRV_STATUS_EXPORTING_FINISHED; break;
    case exsSpeedTesting:       sStatus = XI_SRV_STATUS_SPEEDTESTING; break;
    case exsSpeedTestFinished:  sStatus = XI_SRV_STATUS_SPEEDTEST_FINISHED; break;
    }
    return sStatus;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::setDataStorageFolders(const std::vector<std::string> &vecStoragePath, std::string *psErrorMsg)
{
#ifdef MULTIRECORDER_FIS_ENABLE
    if (m_pRecorder != nullptr)
    {
        try {
            // check for special case user want to store to RAM
            if (vecStoragePath.size()==1 && vecStoragePath[0] == "RAM")
            {
                delete m_pRecorder;
                m_pRecorder = FIS_createRAMRecorder();
                return true;
            }
            else if (m_pRecorder->className() == "CxFisRecorderRAM")
            {
                // switch back to recording to HDD
                delete m_pRecorder;
                if (FIS_isHDDLowLevelRecorderAvailable())
                    m_pRecorder = FIS_createHDDLowLevelRecorder();
                else
                    m_pRecorder = FIS_createHDDRecorder();
                if (m_pRecorder == nullptr)
                    return false;
            }

            // FIS_lib expect absolute, existing paths
            std::vector<std::string> vecPathAbsolute;
            for (auto &sPath : vecStoragePath)
            {
                std::string sAbsolutePath = CxImageExporter::absolutePath(sPath);
                if (!CxImageExporter::createPath(sAbsolutePath))
                {
                    std::string sError = std::string("Error: cannot create folder ") + sPath;
                    printOutputMessage(sError, false);
                    if (psErrorMsg != nullptr) *psErrorMsg = sError;
                    return false;
                }
                vecPathAbsolute.push_back(sAbsolutePath);
            }

            m_pRecorder->setStoragePaths(vecPathAbsolute);
        }
        catch (FIS_lib_Exception const &e)
        {
            printOutputMessage(e.what(), false);
            if (psErrorMsg != nullptr) *psErrorMsg = e.what();
            return false;
        }
        catch (std::exception const &e)
        {
            printOutputMessage(e.what(), false);
            if (psErrorMsg != nullptr) *psErrorMsg = e.what();
            return false;
        }
    }
    return true;
#else
    return false;
#endif //MULTIRECORDER_FIS_ENABLE
}

//----------------------------------------------------------------------------------------
SxCameraParamDesc* CxCameraControl::findParam(const std::string &sParamName)
{
    for (auto &it: g_lstCameraParamDesc)
    {
        if (it.name == sParamName || it.defConstName == sParamName)
            return &it;
    }
    return nullptr;
}

//----------------------------------------------------------------------------------------
std::map<std::string, int> g_mapEnumVals;
void process_xi_enum(const std::vector<xi_enum_item_t> &vecItems)
{
    for (const auto &it: vecItems)
        g_mapEnumVals[it.name] = it.value;
}

bool CxCameraControl::findXiApiEnumValue(const std::string &sConstantName, int &iValue)
{
    if (g_mapEnumVals.empty())
    {
        xiInitEnumerators();
        process_xi_enum(XI_DOWNSAMPLING_VALUE_ITEMS);
        process_xi_enum(XI_TEST_PATTERN_GENERATOR_ITEMS);
        process_xi_enum(XI_VERSION_ITEMS);
        process_xi_enum(XI_TEST_PATTERN_ITEMS);
        process_xi_enum(XI_DEC_PATTERN_ITEMS);
        process_xi_enum(XI_BIN_PATTERN_ITEMS);
        process_xi_enum(XI_BIN_SELECTOR_ITEMS);
        process_xi_enum(XI_BIN_MODE_ITEMS);
        process_xi_enum(XI_DEC_SELECTOR_ITEMS);
        process_xi_enum(XI_SENSOR_TAP_CNT_ITEMS);
        process_xi_enum(XI_BIT_DEPTH_ITEMS);
        process_xi_enum(XI_DEBUG_LEVEL_ITEMS);
        process_xi_enum(XI_IMG_FORMAT_ITEMS);
        process_xi_enum(XI_COLOR_FILTER_ARRAY_ITEMS);
        process_xi_enum(XI_IMAGE_SEQUENCE_TYPE_ITEMS);
        process_xi_enum(XI_BP_ITEMS);
        process_xi_enum(XI_TRG_SOURCE_ITEMS);
        process_xi_enum(XI_TRG_SELECTOR_ITEMS);
        process_xi_enum(XI_TRG_OVERLAP_ITEMS);
        process_xi_enum(XI_ACQ_TIMING_MODE_ITEMS);
        process_xi_enum(XI_TRANSPORT_DATA_TARGET_MODE_ITEMS);
        process_xi_enum(XI_GPI_SEL_CB_ITEMS);
        process_xi_enum(XI_GPO_SEL_CB_ITEMS);
        process_xi_enum(XI_GPI_MODE_ITEMS);
        process_xi_enum(XI_GPI_SELECTOR_ITEMS);
        process_xi_enum(XI_GPO_MODE_ITEMS);
        process_xi_enum(XI_GPO_SELECTOR_ITEMS);
        process_xi_enum(XI_LED_MODE_ITEMS);
        process_xi_enum(XI_LED_SELECTOR_ITEMS);
        process_xi_enum(XI_COUNTER_SELECTOR_ITEMS);
        process_xi_enum(XI_TS_RST_MODE_ITEMS);
        process_xi_enum(XI_TS_RST_SOURCE_ITEMS);
        process_xi_enum(XI_PRM_TYPE_ITEMS);
        process_xi_enum(XI_SWITCH_ITEMS);
        process_xi_enum(XI_TEMP_SELECTOR_ITEMS);
        process_xi_enum(XI_TEMP_CTRL_MODE_SELECTOR_ITEMS);
        process_xi_enum(XI_TEMP_ELEMENT_SELECTOR_ITEMS);
        process_xi_enum(XI_OUTPUT_DATA_PACKING_TYPE_ITEMS);
        process_xi_enum(XI_DOWNSAMPLING_TYPE_ITEMS);
        process_xi_enum(XI_EXPOSURE_TIME_SELECTOR_TYPE_ITEMS);
        process_xi_enum(XI_INTERLINE_EXPOSURE_MODE_TYPE_ITEMS);
        process_xi_enum(XI_GAIN_SELECTOR_TYPE_ITEMS);
        process_xi_enum(XI_SHUTTER_TYPE_ITEMS);
        process_xi_enum(XI_CMS_MODE_ITEMS);
        process_xi_enum(XI_CMS_INTENT_ITEMS);
        process_xi_enum(XI_OPEN_BY_ITEMS);
        process_xi_enum(XI_LENS_FEATURE_ITEMS);
        process_xi_enum(XI_SENSOR_FEATURE_SELECTOR_ITEMS);
        process_xi_enum(XI_SENSOR_MODE_ITEMS);
        process_xi_enum(XI_IMAGE_AREA_SELECTOR_ITEMS);
        process_xi_enum(XI_SENSOR_OUTPUT_CHANNEL_COUNT_ITEMS);
        process_xi_enum(XI_SENS_DEFFECTS_CORR_LIST_SELECTOR_ITEMS);
        process_xi_enum(XI_ACQUISITION_STATUS_SELECTOR_ITEMS);
        process_xi_enum(XI_DP_UNIT_SELECTOR_ITEMS);
        process_xi_enum(XI_DP_PROC_SELECTOR_ITEMS);
        process_xi_enum(XI_DP_PARAM_SELECTOR_ITEMS);
        process_xi_enum(XI_DP_PARAM_VALUE_ITEMS);
        process_xi_enum(XI_USER_SET_SELECTOR_ITEMS);
        process_xi_enum(XI_DUAL_ADC_MODE_ITEMS);
        process_xi_enum(XI_PROBE_SELECTOR_ITEMS);
        process_xi_enum(XI_TOF_READOUT_MODE_ITEMS);
        process_xi_enum(XI_DATA_SM_ITEMS);
    }
    // now find that value!
    auto it = g_mapEnumVals.find(sConstantName);
    if (it != g_mapEnumVals.end())
    {
        iValue = it->second;
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------
std::string CxCameraControl::xiApiErrorString(XI_RETURN ret)
{
    // be sure the enumerators are initialized
    if (g_mapEnumVals.empty())
    {
        int dummy;
        CxCameraControl::findXiApiEnumValue("dummy", dummy);
    }

    std::string sRetValue = std::to_string(ret);

    auto pEnumItem = FindItemByValue(&XI_RET_ITEMS, (int)ret);
    if (pEnumItem != nullptr)
        return std::string(pEnumItem->name) + " (" + sRetValue + "," + pEnumItem->description + ")";

    // not found, return just error code
    return sRetValue;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::numberOfDevices(uint32_t &uiCount, std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    DWORD uiXiCount = 0;
    XI_RETURN ret = xiGetNumberDevices(&uiXiCount);
    uiCount = (uint32_t)uiXiCount;
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("xiGetNumberDevices returned error ") + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::getDeviceInfoString(DWORD DevId, const std::string &sParamName, std::string &sValue, std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    char szPath[512] = {0,};
    XI_RETURN ret = xiGetDeviceInfoString(DevId, sParamName.c_str(), szPath, (DWORD)sizeof(szPath));
    if (pxiRet != nullptr) *pxiRet = ret;
    if(ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("xiGetDeviceInfoString returned error ") + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    sValue = szPath;
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::listConnectedDevices(std::vector<SxCameraDeviceInfo> &vecDeviceInfo, std::string *psErrorMsg)
{
    DWORD uiCount;
    XI_RETURN ret = xiGetNumberDevices(&uiCount);
    if (ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("xiGetNumberDevices returned error  ") + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    vecDeviceInfo.clear();
    for (DWORD uiDevId = 0; uiDevId < uiCount; uiDevId++)
    {
        SxCameraDeviceInfo aInfo;
        getDeviceInfoString(uiDevId, XI_PRM_DEVICE_SN, aInfo.sSN);
        getDeviceInfoString(uiDevId, XI_PRM_DEVICE_INSTANCE_PATH, aInfo.sInstPath);
        getDeviceInfoString(uiDevId, XI_PRM_DEVICE_NAME, aInfo.sDeviceName);
        getDeviceInfoString(uiDevId, XI_PRM_DEVICE_TYPE, aInfo.sDeviceType);
        vecDeviceInfo.push_back(aInfo);
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::openCamera(DWORD DevId, std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    closeOpenedCamera();

    DWORD uiNumDevices = 0;
    if (xiGetNumberDevices(&uiNumDevices) != XI_OK || uiNumDevices == 0)  // call this to re-detect newly connected cameras during MultiRecorder lifetime
    {
        if (pxiRet != nullptr) *pxiRet = XI_DEVICE_NOT_READY;
        return false;
    }

    XI_RETURN ret = xiOpenDevice(DevId, &m_hCamera);
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK)
    {
        m_eStatus = exsInitializationError;
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("Cannot open camera, xiOpenDevice returned error ") + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    postOpenCameraInitialization();
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::openCameraBy(XI_OPEN_BY eMethod, const char *szParameterValue, std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    closeOpenedCamera();

    DWORD uiNumDevices = 0;
    if (xiGetNumberDevices(&uiNumDevices) != XI_OK || uiNumDevices == 0)  // call this to re-detect newly connected cameras during MultiRecorder lifetime
    {
        if (pxiRet != nullptr) *pxiRet = XI_DEVICE_NOT_READY;
        return false;
    }

    XI_RETURN ret = xiOpenDeviceBy(eMethod, szParameterValue, &m_hCamera);
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK)
    {
        m_eStatus = exsInitializationError;
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("Cannot open camera, xiOpenDeviceBy returned error ") + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    postOpenCameraInitialization();
    return true;
}

//----------------------------------------------------------------------------------------
#ifndef XI_PRM_EXTENDED_FEATURE_SELECTOR
  #define XI_PRM_EXTENDED_FEATURE_SELECTOR  "ext_feature_selector"
  #define XI_PRM_EXTENDED_FEATURE           "ext_feature"
  #define XI_EXT_FEATURE_SEL_TRANSPARENT_RETURN_CODES_ENABLED 17
#endif

void CxCameraControl::postOpenCameraInitialization()
{
    xiSetParamInt(m_hCamera, XI_PRM_BUFFER_POLICY, XI_BP_UNSAFE);

    //Enable transparent error codes for MC cameras.
    if (xiSetParamInt(m_hCamera, XI_PRM_EXTENDED_FEATURE_SELECTOR, XI_EXT_FEATURE_SEL_TRANSPARENT_RETURN_CODES_ENABLED) == XI_OK)
        xiSetParamInt(m_hCamera, XI_PRM_EXTENDED_FEATURE, XI_ON);
    xiGetParamInt(m_hCamera, XI_PRM_ACQ_TRANSPORT_BUFFER_SIZE, &m_iTransportBufferSizeDefault);

    if (m_pUserImageProcessor != nullptr)
        m_pUserImageProcessor->setCameraHandle(m_hCamera);
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::closeOpenedCamera()
{
    if (m_hCamera != nullptr)
    {
        xiCloseDevice(m_hCamera);
        m_hCamera = nullptr;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::applyCameraSettingsFromFile(const std::string &sXicameraFile, std::string *psErrorMsg)
{
    if (m_hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera not opened";
        return false;
    }

    TiXmlDocument doc(sXicameraFile);
    if (!doc.LoadFile())
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("Failed to open file ") + sXicameraFile;
        return false;
    }

    //TiXmlHandle hDoc(&doc);
    TiXmlElement *pElRoot = doc.FirstChildElement("CameraParameterValues");
    if (pElRoot == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Corrupted xicamera file, tag CameraParameterValues not found";
        return false;
    }

    TiXmlElement *pElValues = pElRoot->FirstChildElement("Values");
    if (pElValues == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Corrupted xicamera file, tag Values not found";
        return false;
    }

    TiXmlElement *pElParam = pElValues->FirstChildElement();
    while (pElParam != nullptr)
    {
        const char *szParamName = pElParam->Value();
        const char *szType = pElParam->Attribute("type");
        const char *szTextValue = pElParam->GetText();

        if (szParamName != nullptr && szType != nullptr && szTextValue != nullptr)
        {
            // apply this parameter
            if (strcmp(szType, "int") == 0)
                xiSetParamInt(m_hCamera, szParamName, atoi(szTextValue));
            else if (strcmp(szType, "string") == 0)
                xiSetParamString(m_hCamera, szParamName, (void*)szTextValue, (DWORD)strlen(szTextValue));
            else if (strcmp(szType, "float") == 0)
                xiSetParamFloat(m_hCamera, szParamName, (float)atof(szTextValue));
            else if (strcmp(szType, "bool") == 0)
                xiSetParamInt(m_hCamera, szParamName, strcmp(szTextValue, "1")==0 ? 1 : 0);
        }
        pElParam = pElParam->NextSiblingElement();
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::getCameraParameter(const std::string &sParamName, std::string &sValue, std::string *psValueType, std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    if (m_hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera not opened";
        return false;
    }

    SxCameraParamDesc *pParamDesc = CxCameraControl::findParam(sParamName);
    if (pParamDesc == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera parameter not found";
        return false;
    }

    XI_RETURN ret = XI_UNKNOWN_PARAM;
    switch (pParamDesc->type)
    {
    case xiTypeCommand:
    case xiTypeEnum:
    case xiTypeBoolean:
    case xiTypeInteger:
        {
            int iVal;
            ret = xiGetParamInt(m_hCamera, pParamDesc->name.c_str(), &iVal);
            if (ret == XI_OK)
            {
                sValue = std::to_string(iVal);
                if (psValueType != nullptr)
                {
                    if (pParamDesc->type == xiTypeBoolean)
                        *psValueType = "xiTypeBoolean";
                    else
                        *psValueType = "xiTypeInteger";
                }
            }
            break;
        }
    case xiTypeInteger64:
        {
            uint64_t uiVal;
            ret = xiGetParamInt64(m_hCamera, pParamDesc->name.c_str(), &uiVal);
            if (ret == XI_OK)
            {
                sValue = std::to_string(uiVal);
                if (psValueType != nullptr)
                    *psValueType = "xiTypeInteger64";
            }
            break;
        }
    case xiTypeString:
        {
            // first get the buffer size
            int iReqBuffSize = 0;
            std::string sParamReqSize = pParamDesc->name + XI_PRMM_REQ_VAL_BUFFER_SIZE;
            ret = xiGetParamInt(m_hCamera, sParamReqSize.c_str(), &iReqBuffSize);
            if (ret != XI_OK)
                iReqBuffSize = 2048;

            char *szBuf = (char*)calloc(iReqBuffSize, sizeof(char));
            if (szBuf != nullptr)
            {
                ret = xiGetParamString(m_hCamera, pParamDesc->name.c_str(), szBuf, (DWORD)iReqBuffSize);
                if (ret == XI_OK)
                {
                    sValue = szBuf;
                    if (psValueType != nullptr)
                        *psValueType = "xiTypeString";
                }
                free(szBuf);
            }
            else
                ret = XI_MEMORY_ALLOCATION;
            break;
        }
    case xiTypeFloat:
        {
            float fVal;
            ret = xiGetParamFloat(m_hCamera, pParamDesc->name.c_str(), &fVal);
            if (ret == XI_OK)
            {
                sValue = std::to_string(fVal);
                if (psValueType != nullptr)
                    *psValueType = "xiTypeFloat";
            }
            break;
        }
    }
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("Error getting parameter ") + pParamDesc->name + ", xiApi returned error " + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::getCameraParameterInt(const std::string &sParamName, int32_t &iValue, std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    if (m_hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera not opened";
        return false;
    }
    XI_RETURN ret = xiGetParamInt(m_hCamera, sParamName.c_str(), &iValue);
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("Error getting parameter ") + sParamName + ", xiApi returned error " + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::getCameraParameterInt64(const std::string &sParamName, uint64_t &uiValue, std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    if (m_hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera not opened";
        return false;
    }
    XI_RETURN ret = xiGetParamInt64(m_hCamera, sParamName.c_str(), &uiValue);
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("Error getting parameter ") + sParamName + ", xiApi returned error " + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::getCameraParameterFloat(const std::string &sParamName, float &fValue, std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    if (m_hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera not opened";
        return false;
    }
    XI_RETURN ret = xiGetParamFloat(m_hCamera, sParamName.c_str(), &fValue);
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("Error getting parameter ") + sParamName + ", xiApi returned error " + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::getCameraParameterString(const std::string &sParamName, std::string &sValue, std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    if (m_hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera not opened";
        return false;
    }
    // first get the buffer size
    int iReqBuffSize = 0;
    std::string sParamReqSize = sParamName + XI_PRMM_REQ_VAL_BUFFER_SIZE;
    XI_RETURN ret = xiGetParamInt(m_hCamera, sParamReqSize.c_str(), &iReqBuffSize);
    if (ret != XI_OK)
        iReqBuffSize = 2048;

    char *szBuf = (char*)calloc(iReqBuffSize, sizeof(char));
    if (szBuf != nullptr)
    {
        ret = xiGetParamString(m_hCamera, sParamName.c_str(), szBuf, (DWORD)iReqBuffSize);
        if (ret == XI_OK)
            sValue = szBuf;
        free(szBuf);
    }
    else
        ret = XI_MEMORY_ALLOCATION;
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("Error getting parameter ") + sParamName + ", xiApi returned error " + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::setCameraParameter(const std::string &sParamName, const std::string &sValue, std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    if (m_hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera not opened";
        return false;
    }

    SxCameraParamDesc *pParamDesc = CxCameraControl::findParam(sParamName);
    if (pParamDesc == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera parameter not found";
        return false;
    }

    XI_RETURN ret = XI_UNKNOWN_PARAM;
    try {
        switch (pParamDesc->type)
        {
        case xiTypeCommand:
        case xiTypeEnum:
        case xiTypeBoolean:
        case xiTypeInteger:
        {
            // try applying the xiApi constants for values
            int iIntValue = 0;
            if (!findXiApiEnumValue(sValue, iIntValue))
                iIntValue = std::stoi(sValue);
            ret = xiSetParamInt(m_hCamera, pParamDesc->name.c_str(), iIntValue);
            break;
        }
        case xiTypeInteger64: ret = xiSetParamInt64(m_hCamera, pParamDesc->name.c_str(), std::stoull(sValue)); break;
        case xiTypeString:  ret = xiSetParamString(m_hCamera, pParamDesc->name.c_str(), (void*)sValue.c_str(), (DWORD)sValue.length()); break;
        case xiTypeFloat: ret = xiSetParamFloat(m_hCamera, pParamDesc->name.c_str(), std::stof(sValue)); break;
        }
    } catch (...) {
    }
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("Error setting parameter ") + pParamDesc->name + ", xiApi returned error " + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::setCameraParameterInt(const std::string &sParamName, int32_t iValue, std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    if (m_hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera not opened";
        return false;
    }
    XI_RETURN ret = xiSetParamInt(m_hCamera, sParamName.c_str(), iValue);
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("Error setting parameter ") + sParamName + ", xiApi returned error " + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::setCameraParameterInt64(const std::string &sParamName, uint64_t uiValue, std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    if (m_hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera not opened";
        return false;
    }
    XI_RETURN ret = xiSetParamInt64(m_hCamera, sParamName.c_str(), uiValue);
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("Error setting parameter ") + sParamName + ", xiApi returned error " + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::setCameraParameterFloat(const std::string &sParamName, float fValue, std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    if (m_hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera not opened";
        return false;
    }
    XI_RETURN ret = xiSetParamFloat(m_hCamera, sParamName.c_str(), fValue);
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("Error setting parameter ") + sParamName + ", xiApi returned error " + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::setCameraParameterString(const std::string &sParamName, const std::string &sValue, std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    if (m_hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera not opened";
        return false;
    }
    XI_RETURN ret = xiSetParamString(m_hCamera, sParamName.c_str(), (void*)sValue.c_str(), (DWORD)sValue.length());
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("Error setting parameter ") + sParamName + ", xiApi returned error " + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::getCameraImageWithEncoding(uint32_t uiTimeout, const std::string &sEncoding, std::string &sResponse, std::string &sResponseContentType, XI_RETURN *pxiRet)
{
    sResponseContentType = "text/plain";
    if (m_hCamera == nullptr)
    {
        sResponse = "Camera not opened";
        return false;
    }
    XI_IMG img;
    memset(&img, 0, sizeof(XI_IMG));
    img.size = sizeof(XI_IMG);
    // Note: the camera buffer policy should be always in BP_UNSAFE, i.e. xiGetImage fills img with data pointer from xiApi
    XI_RETURN ret = xiGetImage(m_hCamera, uiTimeout, &img);
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK || img.bp == nullptr)
    {
        sResponse = std::string("Error getting the image, xiApi returned error ") + CxCameraControl::xiApiErrorString(ret);
        return false;
    }

    // custom image processing
    if (m_pUserImageProcessor != nullptr)
    {
        return m_pUserImageProcessor->processImage(&img, sResponse, sResponseContentType);
    }

    CxCameraControl::makeXiImgResponse(&img, sResponse, sResponseContentType);

    //printOutputMessage(std::string("First pixel: ") + std::to_string((int)(*(char*)img.bp)), false);
    return true;
}

//----------------------------------------------------------------------------------------
void CxCameraControl::makeXiImgResponse(XI_IMG *pXiImg, std::string &sResponse, std::string &sResponseContentType)
{
    sResponseContentType = DX_IMG_BINARY_MIME;
    sResponse = std::string((char*)pXiImg, pXiImg->size);
    sResponse.append((char*)pXiImg->bp, pXiImg->bp_size);
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::getRecordedFrameData(int iIdx, std::string &sResponse, std::string &sResponseContentType)
{
#ifdef MULTIRECORDER_FIS_ENABLE
    sResponseContentType = "text/plain";
    if (m_pRecorder == nullptr)
    {
        sResponse = "No recorder";
        return false;
    }
    if (iIdx == -1) // get the last frame
    {
        if (m_pRecorder->framesCount() >= m_pRecorder->circleSize())
            iIdx = m_pRecorder->circleSize() - 1;
        else
            iIdx = m_pRecorder->lastFrame();
    }
    if (iIdx < 0 || iIdx >= (int)m_pRecorder->framesCount())
    {
        sResponse = "Index out of range";
        return false;
    }
    uint64_t uiFrameSize = m_pRecorder->frameSize();
    sResponse.resize(uiFrameSize);

    sResponseContentType = "application/octet-stream";
    m_pRecorder->frame(iIdx, sResponse.data(), uiFrameSize);
    return true;
#else
    return false;
#endif //MULTIRECORDER_FIS_ENABLE
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::cameraCounter(XI_COUNTER_SELECTOR eCounter, int &iValue)
{
    iValue = 0;
    if (m_hCamera == nullptr)
        return false;

    // do not spam error log releatedly when counter not supported by camera
    bool bCounterSupported;
    auto it = m_mapCounterSupported.find(eCounter);
    if (it != m_mapCounterSupported.end())
        bCounterSupported = it->second;
    else
    {
        bCounterSupported = (xiSetParamInt(m_hCamera, XI_PRM_COUNTER_SELECTOR XI_PRM_INFO_SETTABLE, eCounter) == XI_OK);
        m_mapCounterSupported[eCounter] = bCounterSupported;
    }
    if (!bCounterSupported)
        return false;

    // get the counter value
    if (xiSetParamInt(m_hCamera, XI_PRM_COUNTER_SELECTOR, eCounter) == XI_OK)
         return (xiGetParamInt(m_hCamera, XI_PRM_COUNTER_VALUE, &iValue) == XI_OK);
    return false;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::recordingCounter(const std::string &sCounter, int &iValue)
{
    if (m_eStatus == exsExporting || m_eStatus == exsExportingFinished)
    {
        if (sCounter == XI_SRV_COUNTER_DONE)
        {
            iValue = m_iTiffExportCounter;
            return true;
        }
#ifdef MULTIRECORDER_FIS_ENABLE
        if (sCounter == XI_SRV_COUNTER_TOTAL && m_pRecorder != nullptr)
        {
            try {
                iValue = (int)m_pRecorder->framesCount();

                if (m_iConcatImgCount > 1)
                    iValue *= m_iConcatImgCount;
            }
            catch (FIS_lib_Exception const &e)
            {
                printOutputMessage(e.what(), false);
                return false;
            }
            return true;
        }
#endif
    }
    return false;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::prepareForAcquisition(uint32_t uiFrameCount, std::string *psErrorMsg)
{
#ifdef MULTIRECORDER_FIS_ENABLE
    if (m_hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera not opened";
        return false;
    }
    m_aImageMetadata.readFromCamera(m_hCamera);

    xiSetParamInt(m_hCamera, XI_PRM_IMAGE_DATA_FORMAT, XI_FRM_TRANSPORT_DATA);

    int32_t iDataAlign = 0;
    std::string sFisErrorMsg;
    if (!FIS_setBufferAligningAndFrameSize(m_pRecorder, m_hCamera, &sFisErrorMsg, &iDataAlign))
    {
        printOutputMessage(sFisErrorMsg, false);
        if (psErrorMsg != nullptr) *psErrorMsg = sFisErrorMsg;
        return false;
    }

    if (!sFisErrorMsg.empty())
        printOutputMessage(sFisErrorMsg, false);        // treat it as a non-fatal warning
    printOutputMessage(std::string("FIS_Lib data align: ") + std::to_string(iDataAlign), true);

    int iConcatOn = 0;
    if (xiGetParamInt(m_hCamera, XI_PRM_CONCAT_IMG_MODE, &iConcatOn) == XI_OK && iConcatOn > 0)
    {
        if (xiGetParamInt(m_hCamera, XI_PRM_CONCAT_IMG_COUNT, &m_iConcatImgCount) != XI_OK)
            m_iConcatImgCount = 0;
        if (xiGetParamInt(m_hCamera, XI_PRM_CONCAT_IMG_TRANSPORT_IMG_OFFSET, &m_iConcatImgNextOffset) != XI_OK)
            m_iConcatImgNextOffset = 0;
    }
    else
    {
        m_iConcatImgCount = 0;
        m_iConcatImgNextOffset = 0;
    }

    if (m_iConcatImgCount > 1)
        uiFrameCount = (uiFrameCount + m_iConcatImgCount - 1) / m_iConcatImgCount;  // round up

    int iFrameSize = 1;
    if (m_pRecorder != nullptr)
    {
        try
        {
            iFrameSize = (int)m_pRecorder->frameSize();
            m_pRecorder->setCircleSize(uiFrameCount);
            m_pRecorder->initializeBuffer();
        }
        catch (FIS_lib_Exception const &e)
        {
            printOutputMessage(e.what(), false);
            if (psErrorMsg != nullptr) *psErrorMsg = e.what();
            return false;
        }
    }

    // optimize performance according to https://www.ximea.com/support/projects/usb3/wiki/How_to_optimize_software_performance_on_high_frame_rates
    // i.e. increate queue size and set optimized transport buffer size as nearest increment to payload
    int iTransportIncrement = 1;
    xiGetParamInt(m_hCamera, XI_PRM_ACQ_TRANSPORT_BUFFER_SIZE XI_PRM_INFO_INCREMENT, &iTransportIncrement);
    int iTransportMinimum = 0;
    xiGetParamInt(m_hCamera, XI_PRM_ACQ_TRANSPORT_BUFFER_SIZE XI_PRM_INFO_MIN, &iTransportMinimum);

    if (iFrameSize < m_iTransportBufferSizeDefault + iTransportIncrement)
    {
        //use optimized transport buffer size as nearest increment to payload
        int iTransportSize = iFrameSize;
        if (iTransportIncrement > 0)
        {
            int iRemainder = iTransportSize % iTransportIncrement;
            if (iRemainder > 0)
                iTransportSize += iTransportIncrement - iRemainder;
        }
        //check minimum
        if (iTransportSize < iTransportMinimum)
            iTransportSize = iTransportMinimum;

        xiSetParamInt(m_hCamera, XI_PRM_ACQ_TRANSPORT_BUFFER_SIZE, iTransportSize);
    }
    else if (m_iTransportBufferSizeDefault > 0)
    {
        // restoring the transport buffer size that was valid after camera open
        xiSetParamInt(m_hCamera, XI_PRM_ACQ_TRANSPORT_BUFFER_SIZE, m_iTransportBufferSizeDefault);
    }

    int iQueueSize = 0;
    int iQueueSizeMax = 0;
    xiGetParamInt(m_hCamera, XI_PRM_BUFFERS_QUEUE_SIZE, &iQueueSize);
    xiGetParamInt(m_hCamera, XI_PRM_BUFFERS_QUEUE_SIZE XI_PRM_INFO_MAX, &iQueueSizeMax);
    if (iQueueSize < iQueueSizeMax)
        xiSetParamInt(m_hCamera, XI_PRM_BUFFERS_QUEUE_SIZE, iQueueSizeMax);

    // do not skip frames, reset timestamps, etc
    xiSetParamInt(m_hCamera, XI_PRM_RECENT_FRAME, XI_OFF);
    xiSetParamInt(m_hCamera, XI_PRM_BUFFER_POLICY, XI_BP_UNSAFE);
    if (xiSetParamInt(m_hCamera, XI_PRM_TS_RST_MODE, XI_TS_RST_ARM_ONCE) == XI_OK)
        xiSetParamInt(m_hCamera, XI_PRM_TS_RST_SOURCE, XI_TS_RST_SRC_SW);

    m_aImageMetadata.m_sApiContextList.clear();
    m_bAcquisitionPaused = false;
    m_vecTimeStamps.clear();
    m_vecFrameNo.clear();

    bool bSupportsAcqPause = (xiSetParamInt(m_hCamera, XI_PRM_SENSOR_FEATURE_SELECTOR, XI_SENSOR_FEATURE_ACQUISITION_RUNNING) == XI_OK);
    if (bSupportsAcqPause)
    {
        // start the acq now and let it pause
        // start acq
        XI_RETURN ret = xiStartAcquisition(m_hCamera);
        if (ret != XI_OK)
        {
            if (psErrorMsg != nullptr) *psErrorMsg = std::string("Cannot start the acquisition, xiStartAcquisition returned error ") + CxCameraControl::xiApiErrorString(ret) ;
            return false;
        }

        xiSetParamInt(m_hCamera, XI_PRM_SENSOR_FEATURE_SELECTOR, XI_SENSOR_FEATURE_ACQUISITION_RUNNING);
        ret = xiSetParamInt(m_hCamera, XI_PRM_SENSOR_FEATURE_VALUE, XI_OFF);
        if (ret == XI_OK)
        {
            m_bAcquisitionPaused = true;

            // wait for [exposure time] before reading the queue
            int iWaitTimeMs = 1;
            int iExposureUs;
            if (xiGetParamInt(m_hCamera, XI_PRM_EXPOSURE, &iExposureUs) == XI_OK)
                iWaitTimeMs = iExposureUs / 1000;
            std::this_thread::sleep_for(std::chrono::milliseconds(iWaitTimeMs));

            // read buffers already queued in xiApi
            while (true)
            {
                XI_IMG xiImg;
                memset(&xiImg, 0, sizeof(xiImg));
                xiImg.size = sizeof(XI_IMG);
                ret = xiGetImage(m_hCamera, 0, &xiImg);
                if (ret == XI_TIMEOUT)
                    break;
            }
        }
    }
    m_eStatus = exsPreparedForAcq;
    m_bCancelRecording = false;
    m_bCancelExporting = false;
    return true;
#else
    return false;
#endif //MULTIRECORDER_FIS_ENABLE
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::startAcquisition(std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    if (m_hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera not opened";
        return false;
    }

    if (m_bAcquisitionPaused)
    {
        // unpause
        m_bAcquisitionPaused = false;
        xiSetParamInt(m_hCamera, XI_PRM_SENSOR_FEATURE_SELECTOR, XI_SENSOR_FEATURE_ACQUISITION_RUNNING);
        xiSetParamInt(m_hCamera, XI_PRM_SENSOR_FEATURE_VALUE, XI_ON);
        if (pxiRet != nullptr) *pxiRet = XI_OK;
    }
    else
    {
        XI_RETURN ret = xiStartAcquisition(m_hCamera);
        if (pxiRet != nullptr) *pxiRet = ret;
        if (ret != XI_OK)
        {
            if (psErrorMsg != nullptr) *psErrorMsg = std::string("Cannot start the acquisition, xiStartAcquisition returned error ") + CxCameraControl::xiApiErrorString(ret);
            return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::recordData()
{
#ifdef MULTIRECORDER_FIS_ENABLE
    m_eStatus = exsRecording;
    m_iLastRecordedFrames = 0;
    if (m_hCamera == nullptr || m_pRecorder == nullptr)
    {
        m_eStatus = exsRecordingFinished;
        return false;
    }
    if (m_timeAcqStart == 0)
        m_timeAcqStart = std::time(nullptr);
    m_timeLastSpeedCheck = std::chrono::system_clock::now();
    m_dLastSpeedValue = 0.0;
    m_aImageMetadata.m_timeAcqDateTime = m_timeAcqStart;

    m_timeLastLiveImagePreview = std::chrono::system_clock::now();

    uint64_t uiFisDataSize = m_pRecorder->frameSize();
    uint32_t uiFramesCaptured = 0;
    uint32_t uiFramesNeeded = m_pRecorder->circleSize();
    while (uiFramesCaptured < uiFramesNeeded && !m_bCancelRecording)
    {
        XI_IMG xiImg;
        memset(&xiImg, 0, sizeof(xiImg));
        xiImg.size = sizeof(XI_IMG);
        XI_RETURN ret = xiGetImage(m_hCamera, 2000, &xiImg);
        if (ret != XI_OK)
            printOutputMessage(std::string("Recording error: xiGetImage returned error ") + xiApiErrorString(ret), false);
        else
        {
            if (xiImg.bp_size > uiFisDataSize)
            {
                printOutputMessage(std::string("Recording warning: xiGetImage returned image with data size ")
                                       + std::to_string(xiImg.bp_size) + ", expected " + std::to_string(uiFisDataSize), false);
            }
            try
            {
                m_pRecorder->pushFrame(xiImg.bp);

                if (m_aImageMetadata.m_sApiContextList.empty())
                {
                    // save current context list for unpacking and debayering
                    char szContextList[10000];
                    memset(szContextList, 0, 10000);
                    if (xiGetParamString(m_hCamera, XI_PRM_API_CONTEXT_LIST, szContextList, 10000) == XI_OK)
                        m_aImageMetadata.m_sApiContextList = szContextList;
                    m_aImageMetadata.m_uTransportDataFormat = xiImg.transport_frm;
                }

                uint64_t uiTimeStamp = ((uint64_t)xiImg.tsSec)*(uint64_t)1000000 + ((uint64_t)xiImg.tsUSec);
                m_vecTimeStamps.push_back(uiTimeStamp);
                m_vecFrameNo.push_back(xiImg.acq_nframe);

                uiFramesCaptured++;

                if (m_bShowLiveImagePreview && m_dLiveImagePreviewFps > 0.0)
                {
                    auto timeNow = std::chrono::system_clock::now();
                    double dDiff = std::chrono::duration<double>(timeNow - m_timeLastLiveImagePreview).count();
                    if (dDiff > 1.0 / m_dLiveImagePreviewFps)
                    {
                        // show preview now
                        showLiveImagePreview(&xiImg);
                        m_timeLastLiveImagePreview = timeNow;
                    }
                }
            }
            catch (FIS_lib_Exception const &e)
            {
                printOutputMessage(e.what(), false);
            }
        }
    }

    m_pRecorder->finishWriting();
    m_eStatus = exsRecordingFinished;
    return true;
#else
    m_eStatus = exsRecordingFinished;
    return false;
#endif //MULTIRECORDER_FIS_ENABLE
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::recordDataAsync()
{
    if (m_pRecordingThread != nullptr)
    {
        m_bCancelRecording = true;
        m_pRecordingThread->join();
        delete m_pRecordingThread;
    }

    m_timeAcqStart = std::time(nullptr);
    m_dLastSpeedValue = 0.0;
    m_eStatus = exsRecording;
    m_bCancelRecording = false;
    m_pRecordingThread = new std::thread(&CxCameraControl::recordData, this);
    return (m_pRecordingThread != nullptr);
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::cancelRecording()
{
    if (m_pRecordingThread != nullptr)
    {
        m_bCancelRecording = true;

        m_pRecordingThread->join();

        delete m_pRecordingThread;
        m_pRecordingThread = nullptr;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::startStorageSpeedTestAsync(int iDuration, int iBufferSize, int iFrameLoopCount)
{
    if (m_pRecordingThread != nullptr)
    {
        m_bCancelRecording = true;
        m_pRecordingThread->join();
        delete m_pRecordingThread;
    }

    // output to server log too in a format suitable for CSV (headers now
    printOutputMessage("time,speed,storage errors", false);

    m_timeAcqStart = std::time(nullptr);
    m_dLastSpeedValue = 0.0;
    m_eStatus = exsSpeedTesting;
    m_bCancelRecording = false;
    m_pRecordingThread = new std::thread(&CxCameraControl::storageSpeedTest, this,
                                         iDuration, iBufferSize, iFrameLoopCount);
    return (m_pRecordingThread != nullptr);
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::stopAcquisition(std::string *psErrorMsg, XI_RETURN *pxiRet)
{
    if (m_hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera not opened";
        return false;
    }

    cancelRecording();

    XI_RETURN ret = xiStopAcquisition(m_hCamera);
    if (pxiRet != nullptr) *pxiRet = ret;
    if (ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("xiStopAcquisition returned error ") + CxCameraControl::xiApiErrorString(ret);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::exportTiffFiles(const std::string &sOutputFolder, int iFmtOutput, bool bTiffEnable16bit, bool bTiffRescale16bit, int iOnlyFrame)
{
#ifdef MULTIRECORDER_FIS_ENABLE
    m_eStatus = exsExporting;
    m_iTiffExportCounter = 0;

    if (iFmtOutput == -1)
        iFmtOutput = m_iLoadedExportImageFormat;
    if (iFmtOutput == -1 || iFmtOutput == XI_FRM_TRANSPORT_DATA)
        iFmtOutput = XI_RGB24;

    if (m_pRecorder == nullptr)
    {
        m_eStatus = exsExportingFinished;
        return false;
    }

    std::string sFilePrefix = "frame";
    if (m_timeAcqStart > 0)
    {
        // prefix with acq time start
        std::tm const* pTInfo = std::localtime(&m_timeAcqStart);
        std::stringstream ssDateTime;
        ssDateTime << (pTInfo->tm_year+1900) << std::setw(2) << std::setfill('0') << (pTInfo->tm_mon+1)
                   << std::setw(2) << std::setfill('0') << pTInfo->tm_mday << "-"
                   << std::setw(2) << std::setfill('0') << pTInfo->tm_hour
                   << std::setw(2) << std::setfill('0') << pTInfo->tm_min
                   << std::setw(2) << std::setfill('0') << pTInfo->tm_sec;
        sFilePrefix = ssDateTime.str();

        // and also prefix with camera serial number
        if (!m_aImageMetadata.m_sCameraSerialNumber.empty())
            sFilePrefix += std::string("_") + m_aImageMetadata.m_sCameraSerialNumber;
    }
    if (!CxImageExporter::createPath(sOutputFolder))
    {
        xiRecorderApi::outputStringToStdErr(std::string("exportTiffFiles: cannot create export folder ") + sOutputFolder);
        return false;
    }

    if (m_bPrintServerLogVerbose)
        xiRecorderApi::outputStringToStdErr(std::string("exportTiffFiles to \"") + sOutputFolder + "\", with format " + std::to_string(iFmtOutput));

    uint64_t uiDataSize = m_pRecorder->frameSize();
    void *pImgData = malloc(uiDataSize);
    if (pImgData == nullptr)
    {
        xiRecorderApi::outputStringToStdErr("exportTiffFiles: cannot allocate export buffer.");
        m_eStatus = exsExportingFinished;
        return false;
    }

    xiProcessingHandle_t hProc;
    XI_RETURN ret = xiProcOpen(&hProc);
    if (ret != XI_OK)
    {
        xiRecorderApi::outputStringToStdErr(std::string("exportTiffFiles: xiProcOpen failed with error ") + CxCameraControl::xiApiErrorString(ret));
        free(pImgData);
        m_eStatus = exsExportingFinished;
        return false;
    }

    ret = xiProcSetParam(hProc, XI_PRM_API_CONTEXT_LIST, (void*)m_aImageMetadata.m_sApiContextList.c_str(), (DWORD)m_aImageMetadata.m_sApiContextList.length(), xiTypeString);
    if (ret != XI_OK)
        xiRecorderApi::outputStringToStdErr(std::string("exportTiffFiles: xiProcSetParam(apiContextList) failed with error ") + CxCameraControl::xiApiErrorString(ret));

    ret = xiProcSetParam(hProc, XI_PRM_IMAGE_DATA_FORMAT, &iFmtOutput, sizeof(iFmtOutput), xiTypeInteger);
    if (ret != XI_OK)
        xiRecorderApi::outputStringToStdErr(std::string("exportTiffFiles: xiProcSetParam(imageDataFormat) failed with error ") + CxCameraControl::xiApiErrorString(ret));

    m_vecExportedFilenames.clear();
    bool bOK = true;
    uint32_t uiFramesCaptured = m_pRecorder->framesCount();
    int iImagesPerBuffer = m_iConcatImgCount > 1 ? m_iConcatImgCount : 1;
    int iFramesCounterPlaces = (int)log10(uiFramesCaptured * iImagesPerBuffer) + 1;
    for (uint32_t i = 0; i < uiFramesCaptured; i++)
    {
        if (m_bCancelExporting)
            break;  // cancel

        if (iOnlyFrame >= 0  && (int)i != iOnlyFrame)
            continue;

        if (m_bPrintServerLogVerbose)
            xiRecorderApi::outputStringToStdErr(std::string("exportTiffFiles: saving frame ") + std::to_string(i+1));

        try {
            m_pRecorder->frame(i, pImgData, uiDataSize);
            for (int iSubImg = 0; iSubImg < iImagesPerBuffer; iSubImg++)
            {
                bool bDone = false;
                ret = xiProcPushImage(hProc, (uint8_t*)pImgData + iSubImg * m_iConcatImgNextOffset);
                if (ret != XI_OK)
                    xiRecorderApi::outputStringToStdErr(std::string("exportTiffFiles: xiProcPushImage failed with error ") + CxCameraControl::xiApiErrorString(ret));
                else
                {
                    XI_IMG xiImgOutput;
                    memset(&xiImgOutput, 0, sizeof(XI_IMG));
                    xiImgOutput.size = sizeof(XI_IMG);

                    ret = xiProcPullImage(hProc, 2000, &xiImgOutput);
                    if (ret != XI_OK)
                        xiRecorderApi::outputStringToStdErr(std::string("exportTiffFiles: xiProcPullImage failed with error ") + CxCameraControl::xiApiErrorString(ret));
                    else
                    {
                        std::stringstream ssNumber;
                        ssNumber << std::setw(iFramesCounterPlaces) << std::setfill('0') << (i*iImagesPerBuffer + iSubImg + 1);        // format to 4 width with leading zeros
                        std::string sFile = sFilePrefix + "_f" + ssNumber.str() + ".tif";

                        bDone = CxImageExporter::saveImageToFile(sOutputFolder + "/" + sFile, &xiImgOutput, bTiffEnable16bit, bTiffRescale16bit);
                        if (bDone)
                        {
                            m_iTiffExportCounter++;
                            m_vecExportedFilenames.push_back(sFile);
                        }
                        else
                            xiRecorderApi::outputStringToStdErr("exportTiffFiles: save image to TIFF file failed");
                    }
                }
                if (!bDone)
                {
                    bOK = false;
                    m_vecExportedFilenames.push_back("");
                }
            }
        }
        catch (FIS_lib_Exception const &e) {
            xiRecorderApi::outputStringToStdErr(std::string("exportTiffFiles: FIS_Lib exception: ") + e.what());
            bOK = false;
            continue;
        }
        catch (std::exception const &e) {
            xiRecorderApi::outputStringToStdErr(std::string("exportTiffFiles: exception: ") + e.what());
            bOK = false;
            continue;
        }
    }

    free(pImgData);
    xiProcClose(hProc);

    if (iOnlyFrame < 0)
        writeSequenceIndexFile(sOutputFolder + "/" + sFilePrefix + ".xiseq");

    m_eStatus = exsExportingFinished;
    return bOK;
#else
    m_eStatus = exsExportingFinished;
    return false;
#endif //MULTIRECORDER_FIS_ENABLE
}

//----------------------------------------------------------------------------------------
void CxCameraControl::writeSequenceIndexFile(const std::string &sFilename)
{
#ifdef MULTIRECORDER_FIS_ENABLE
    if (m_vecExportedFilenames.empty())
        return;

    size_t uiFileCount = m_vecExportedFilenames.size();

    TiXmlDocument doc;
    TiXmlElement *pElRoot = new TiXmlElement("ImageSequence");
    //pElRoot->SetAttribute("version", xiapiversion);
    doc.LinkEndChild(pElRoot);

    // image metadata
    TiXmlElement *pEl;
    TiXmlElement *pElMeta;
    pElMeta = new TiXmlElement("imageMetadata");
    m_aImageMetadata.storeToXml(pElMeta);
    pElRoot->LinkEndChild(pElMeta);

    // start time as ISO date
    pEl = new TiXmlElement("baseTime");
    std::stringstream ssAcqStart;
    ssAcqStart << std::put_time(std::localtime(&m_timeAcqStart), "%Y-%m-%dT%H:%M:%S");
    pEl->LinkEndChild(new TiXmlText(ssAcqStart.str()));
    pElRoot->LinkEndChild(pEl);

    // file sequence
    for (size_t uiFrame = 0; uiFrame < uiFileCount; uiFrame++)
    {
        if (m_vecExportedFilenames[uiFrame].empty())    // error storing this frame
            continue;

        pEl = new TiXmlElement("file");

        if (m_iConcatImgCount <= 1)
        {
            // no concatenation
            if (m_vecTimeStamps.size() > uiFrame)
                pEl->SetAttribute("timestamp", std::to_string(m_vecTimeStamps[uiFrame]));
            if (m_vecFrameNo.size() > uiFrame)
                pEl->SetAttribute("frame", std::to_string(m_vecFrameNo[uiFrame]));
        }
        else
        {
            int iConcatFrame = (int)uiFrame / m_iConcatImgCount;
            int iSubImg = (int)uiFrame - iConcatFrame*m_iConcatImgCount;

            if ((int)m_vecTimeStamps.size() > iConcatFrame)
            {
                // interpolate timestamp from this and next concatenated buffer
                uint64_t uiInterTs;
                if (iConcatFrame+1 < (int)m_vecTimeStamps.size())
                {
                    uiInterTs = (m_vecTimeStamps[iConcatFrame] * (m_iConcatImgCount-iSubImg) + m_vecTimeStamps[iConcatFrame+1] * (iSubImg)) / m_iConcatImgCount;
                }
                else if (iConcatFrame > 0)
                {
                    // we are at the end, cannot interpolate: lets extrapolate timestamps from the previous concatenated image
                    uiInterTs = (m_vecTimeStamps[iConcatFrame-1] * (m_iConcatImgCount-iSubImg) + m_vecTimeStamps[iConcatFrame] * (iSubImg)) / m_iConcatImgCount;
                }
                else
                {
                    // no other data to calc sub-image timestamp, use exposure value to get a different value
                    uiInterTs = m_vecTimeStamps[iConcatFrame] + m_aImageMetadata.m_iExposure * iSubImg;
                }
                pEl->SetAttribute("timestamp", std::to_string(uiInterTs));
            }
            if ((int)m_vecFrameNo.size() > iConcatFrame)
                pEl->SetAttribute("frame", std::to_string(m_vecFrameNo[iConcatFrame] + iSubImg));
        }

        pEl->LinkEndChild(new TiXmlText(m_vecExportedFilenames[uiFrame]));
        pElRoot->LinkEndChild(pEl);
    }

    doc.SaveFile(sFilename);
#endif //MULTIRECORDER_FIS_ENABLE
}

//----------------------------------------------------------------------------------------
std::string CxCameraControl::exportedFilename(size_t idx)
{
    if (idx < m_vecExportedFilenames.size())
        return m_vecExportedFilenames[idx];
    else
        return std::string();
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::exportTiffFilesAsync(const std::string &sOutputFolder, int iFmtOutput,
                                           bool bTiffEnable16bit, bool bTiffRescale16bit,
                                           int iOnlyFrame, std::string *psErrorMsg)
{
    if (m_pExportingThread != nullptr)
    {
        m_bCancelExporting = true;
        m_pExportingThread->join();
        delete m_pExportingThread;
    }

    if (!CxImageExporter::createPath(sOutputFolder))
    {
        std::string sError = std::string("exportTiffFiles: cannot create export folder ") + sOutputFolder;
        xiRecorderApi::outputStringToStdErr(sError);
        if (psErrorMsg != nullptr) *psErrorMsg = sError;
        m_eStatus = exsExportingFinished;
        return false;
    }

    m_eStatus = exsExporting;
    m_iTiffExportCounter = 0;
    m_bCancelExporting = false;
    m_pExportingThread = new std::thread(&CxCameraControl::exportTiffFiles, this, sOutputFolder,
                                         iFmtOutput, bTiffEnable16bit, bTiffRescale16bit, iOnlyFrame);
    return (m_pExportingThread != nullptr);
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::cancelExporting()
{
    if (m_pExportingThread != nullptr)
    {
        m_bCancelExporting = true;

        m_pExportingThread->join();

        delete m_pExportingThread;
        m_pExportingThread = nullptr;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::releaseDataBuffers(bool bRemoveDataFiles)
{
#ifdef MULTIRECORDER_FIS_ENABLE
    if (m_pRecorder == nullptr)
        return false;
    try {
        m_pRecorder->finishBuffer(bRemoveDataFiles);
    }
    catch (FIS_lib_Exception const &e) {
        printOutputMessage(e.what(), false);
        return false;
    }
    if (bRemoveDataFiles)
        m_eStatus = exsInitialization;
    return true;
#else
    return false;
#endif //MULTIRECORDER_FIS_ENABLE
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::processRemoteRequest(const std::string &sXmlString, std::string *psResponse, std::string *psResponseContentType)
{
    bool bRet = false;
    XI_RETURN iXiReturnCode = -1;
    std::string sResponseDetail;
    std::string sResponseDetailContentType = "text/plain";  // it gets encoded to text/xml
    std::string sResponseValueType;
    if (psResponse != nullptr)
        psResponse->clear();
    bool bResponseAlreadySet = false;
    if (psResponseContentType != nullptr)
        *psResponseContentType = "text/xml";

    TiXmlDocument doc;
    doc.Parse(sXmlString.c_str(), nullptr, TIXML_ENCODING_UTF8);
    TiXmlElement* pElCmd = doc.FirstChildElement(XI_SRV_TAG_REQUEST);
    if (pElCmd == nullptr)
        return false;      // not a XML request

    std::string sCmdName;
    const char *szAttrCmd = pElCmd->Attribute("cmd");
    if (szAttrCmd != nullptr)
        sCmdName = szAttrCmd;

    if (sCmdName == XI_CMD_GET_NUMBER_DEVICES)
    {
        uint32_t uiCount;
        bRet = numberOfDevices(uiCount, &sResponseDetail, &iXiReturnCode);
        if (bRet)
            sResponseDetail = std::to_string(uiCount);
    }
    else if (sCmdName == XI_CMD_GET_DEVICE_INFO)
    {
        const char *szAttrIndex = pElCmd->Attribute("idx");
        const char *szAttrName = pElCmd->Attribute("name");
        if (szAttrIndex == nullptr || szAttrName == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            std::string sValue;
            bRet = getDeviceInfoString(atoi(szAttrIndex), szAttrName, sValue, &sResponseDetail, &iXiReturnCode);
            if (bRet)
                sResponseDetail = sValue;
        }
    }
    else if (sCmdName == XI_CMD_GET_CONNECTED_DEVICES)
    {
        std::vector<SxCameraDeviceInfo> vecDeviceInfo;
        bRet = listConnectedDevices(vecDeviceInfo, &sResponseDetail);
        if (bRet)
        {
            std::string sOut;
            for (size_t idx = 0; idx < vecDeviceInfo.size(); idx++)
            {
                const SxCameraDeviceInfo &aInfo = vecDeviceInfo[idx];
                // <device idx=0 sn="SN1234" instPath="PATH" deviceName="NAME" deviceType="type" />
                sOut += std::string("<device idx=") + std::to_string(idx) + " sn=\"" + aInfo.sSN + "\" instPath=\"" + aInfo.sInstPath
                        + "\" deviceName=\"" + aInfo.sDeviceName + "\" deviceType=\"" + aInfo.sDeviceType + "\" />";
            }
            sResponseDetail = sOut;
        }
    }
    else if (sCmdName == XI_CMD_OPEN_CAMERA)
    {
        const char *szAttrInstPath = pElCmd->Attribute("instPath");
        const char *szAttrSerial = pElCmd->Attribute("sn");
        const char *szAttrUserId = pElCmd->Attribute("userID");
        const char *szAttrLocPath = pElCmd->Attribute("locPath");
        const char *szAttrIndex = pElCmd->Attribute("idx");
        if (szAttrInstPath != nullptr)
            bRet = openCameraBy(XI_OPEN_BY_INST_PATH, szAttrSerial, &sResponseDetail, &iXiReturnCode);
        else if (szAttrSerial != nullptr)
            bRet = openCameraBy(XI_OPEN_BY_SN, szAttrSerial, &sResponseDetail, &iXiReturnCode);
        else if (szAttrUserId != nullptr)
            bRet = openCameraBy(XI_OPEN_BY_USER_ID, szAttrSerial, &sResponseDetail, &iXiReturnCode);
        else if (szAttrLocPath != nullptr)
            bRet = openCameraBy(XI_OPEN_BY_LOC_PATH, szAttrSerial, &sResponseDetail, &iXiReturnCode);
        else if (szAttrIndex != nullptr)
            bRet = openCamera((DWORD)atoi(szAttrIndex), &sResponseDetail, &iXiReturnCode);
        else
            sResponseDetail = "Missing argument";
    }
    else if (sCmdName == XI_CMD_CLOSE_CAMERA)
    {
        bRet = closeOpenedCamera();
    }
    else if (sCmdName == XI_CMD_APPLY_XICAMERA_FILE)
    {
        const char *szAttrName = pElCmd->Attribute("name");
        if (szAttrName == nullptr)
            sResponseDetail = "Missing argument";
        else
            bRet = applyCameraSettingsFromFile(szAttrName, &sResponseDetail);
    }
    else if (sCmdName == XI_CMD_GET_CAMERA_PARAM)
    {
        const char *szAttrName = pElCmd->Attribute("name");
        if (szAttrName == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            std::string sValue;
            bRet = getCameraParameter(szAttrName, sValue, &sResponseValueType, &sResponseDetail, &iXiReturnCode);
            if (bRet)
                sResponseDetail = sValue;
        }
    }
    else if (sCmdName == XI_CMD_GET_CAMERA_PARAM_INT)
    {
        const char *szAttrName = pElCmd->Attribute("name");
        if (szAttrName == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            int iValue;
            bRet = getCameraParameterInt(szAttrName, iValue, &sResponseDetail, &iXiReturnCode);
            if (bRet)
            {
                sResponseDetail = std::to_string(iValue);
                sResponseValueType = "xiTypeInteger";
            }
        }
    }
    else if (sCmdName == XI_CMD_GET_CAMERA_PARAM_INT64)
    {
        const char *szAttrName = pElCmd->Attribute("name");
        if (szAttrName == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            uint64_t uiValue;
            bRet = getCameraParameterInt64(szAttrName, uiValue, &sResponseDetail, &iXiReturnCode);
            if (bRet)
            {
                sResponseDetail = std::to_string(uiValue);
                sResponseValueType = "xiTypeInteger64";
            }
        }
    }
    else if (sCmdName == XI_CMD_GET_CAMERA_PARAM_FLOAT)
    {
        const char *szAttrName = pElCmd->Attribute("name");
        if (szAttrName == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            float fValue;
            bRet = getCameraParameterFloat(szAttrName, fValue, &sResponseDetail, &iXiReturnCode);
            if (bRet)
            {
                sResponseDetail = std::to_string(fValue);
                sResponseValueType = "xiTypeFloat";
            }
        }
    }
    else if (sCmdName == XI_CMD_GET_CAMERA_PARAM_STRING)
    {
        const char *szAttrName = pElCmd->Attribute("name");
        if (szAttrName == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            std::string sValue;
            bRet = getCameraParameterString(szAttrName, sValue, &sResponseDetail, &iXiReturnCode);
            if (bRet)
            {
                sResponseDetail = sValue;
                sResponseValueType = "xiTypeString";
            }
        }
    }
    else if (sCmdName == XI_CMD_SET_CAMERA_PARAM)
    {
        const char *szAttrName = pElCmd->Attribute("name");
        const char *szAttrValue = pElCmd->Attribute("value");
        if (szAttrName == nullptr || szAttrValue == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            bRet = setCameraParameter(szAttrName, szAttrValue, &sResponseDetail, &iXiReturnCode);
        }
    }
    else if (sCmdName == XI_CMD_SET_CAMERA_PARAM_INT)
    {
        const char *szAttrName = pElCmd->Attribute("name");
        const char *szAttrValue = pElCmd->Attribute("value");
        if (szAttrName == nullptr || szAttrValue == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            bRet = setCameraParameterInt(szAttrName, atoi(szAttrValue), &sResponseDetail, &iXiReturnCode);
        }
    }
    else if (sCmdName == XI_CMD_SET_CAMERA_PARAM_INT64)
    {
        const char *szAttrName = pElCmd->Attribute("name");
        const char *szAttrValue = pElCmd->Attribute("value");
        if (szAttrName == nullptr || szAttrValue == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            bRet = setCameraParameterInt64(szAttrName, atoi(szAttrValue), &sResponseDetail, &iXiReturnCode);
        }
    }
    else if (sCmdName == XI_CMD_SET_CAMERA_PARAM_FLOAT)
    {
        const char *szAttrName = pElCmd->Attribute("name");
        const char *szAttrValue = pElCmd->Attribute("value");
        if (szAttrName == nullptr || szAttrValue == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            bRet = setCameraParameterFloat(szAttrName, (float)atof(szAttrValue), &sResponseDetail, &iXiReturnCode);
        }
    }
    else if (sCmdName == XI_CMD_SET_CAMERA_PARAM_STRING)
    {
        const char *szAttrName = pElCmd->Attribute("name");
        const char *szAttrValue = pElCmd->Attribute("value");
        if (szAttrName == nullptr || szAttrValue == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            bRet = setCameraParameterString(szAttrName, szAttrValue, &sResponseDetail, &iXiReturnCode);
        }
    }
    else if (sCmdName == XI_CMD_GET_CAMERA_IMAGE)
    {
        const char *szAttrTimeout = pElCmd->Attribute("timeout");
        const char *szAttrEncoding = pElCmd->Attribute("encoding");
        uint32_t uiTimeout = 2000;
        std::string sEncoding(DX_IMG_BINARY_MIME);
        if (szAttrTimeout != nullptr)
            uiTimeout = atoi(szAttrTimeout);
        if (szAttrEncoding != nullptr)
            sEncoding = szAttrEncoding;

        if (psResponse != nullptr)
        {
            bRet = getCameraImageWithEncoding(uiTimeout, sEncoding, *psResponse, sResponseDetailContentType, &iXiReturnCode);
            bResponseAlreadySet = (sResponseDetailContentType != "text/plain");
            if (!bResponseAlreadySet)
                sResponseDetail = *psResponse;
        }
        else
            bRet = getCameraImageWithEncoding(uiTimeout, sEncoding, sResponseDetail, sResponseDetailContentType, &iXiReturnCode);
    }
    else if (sCmdName == XI_CMD_SET_DATA_STORAGE)
    {
        const char *szAttrFolders = pElCmd->Attribute("folders");
        if (szAttrFolders == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            // split by '|'
            std::stringstream ssFolders(szAttrFolders);
            std::string segment;
            std::vector<std::string> vecFolders;

            while (std::getline(ssFolders, segment, '|'))
                vecFolders.push_back(segment);

            bRet = setDataStorageFolders(vecFolders, &sResponseDetail);
        }
    }
    else if (sCmdName == XI_CMD_SET_LIVE_IMG_PREVIEW)
    {
        const char *szAttrSet = pElCmd->Attribute("set");
        if (szAttrSet == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            m_bShowLiveImagePreview = (strcmp(szAttrSet, "true") == 0);
            if (CxLiveImagePreview::isLivePreviewSupported())
                bRet = true;
            else
                sResponseDetail = "Live image preview not available. Try placing the opencv_worldNNN.dll on PATH or next to MultiRecorder.exe";
        }
    }
    else if (sCmdName == XI_CMD_GO_READY)
    {
        const char *szAttrFrameCount = pElCmd->Attribute("frameCount");
        if (szAttrFrameCount == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            int32_t iFrameCount = std::atoi(szAttrFrameCount);
            if (iFrameCount <= 0)
                sResponseDetail = "Invalid argument value";
            else
            {
                bRet = prepareForAcquisition((uint32_t)iFrameCount, &sResponseDetail);
            }
        }
    }
    else if (sCmdName == XI_CMD_START_ACQ)
    {
        if (startAcquisition(&sResponseDetail, &iXiReturnCode))
        {
            // In case MultiRecorder is used for LoopRecording (we are after XI_CMD_GO_READY),
            // start the recording. Else just start the acquisition.
            if (m_eStatus == exsPreparedForAcq)
                bRet = recordDataAsync();
            else
                bRet = true;
        }
    }
    else if (sCmdName == XI_CMD_CANCEL_RECORDING)
    {
        bRet = cancelRecording();
    }
    else if (sCmdName == XI_CMD_STOP_ACQ)
    {
        bRet = stopAcquisition(&sResponseDetail, &iXiReturnCode);
    }
    else if (sCmdName == XI_CMD_GET_STATUS)
    {
        bRet = true;
        sResponseDetail = statusString();
    }
    else if (sCmdName == XI_CMD_GET_IMAGES_COUNT)
    {
        if (m_eStatus == exsRecording || m_eStatus == exsRecordingFinished)
        {
#ifdef MULTIRECORDER_FIS_ENABLE
            if (m_pRecorder != nullptr)
            {
                try {
                    int iImagesPerBuffer = m_iConcatImgCount > 1 ? m_iConcatImgCount : 1;
                    int iFramesDone = (int)m_pRecorder->framesCount() * iImagesPerBuffer;
                    sResponseDetail += counterResponseString(XI_SRV_COUNTER_DONE, iFramesDone);
                    sResponseDetail += counterResponseString(XI_SRV_COUNTER_TOTAL, (int)m_pRecorder->circleSize() * iImagesPerBuffer);

                    if (iFramesDone > m_iLastRecordedFrames && iFramesDone > 0)
                    {
                        auto timeNow = std::chrono::system_clock::now();
                        double dTimeDiffSec = std::chrono::duration<double>(timeNow - m_timeLastSpeedCheck).count();
                        if (dTimeDiffSec > 0.0)
                        {
                            m_dLastSpeedValue = ((iFramesDone-m_iLastRecordedFrames) * m_pRecorder->frameSize()/1000000) / dTimeDiffSec; // MBps
                            m_timeLastSpeedCheck = timeNow;
                            m_iLastRecordedFrames = iFramesDone;
                        }
                    }
                    sResponseDetail += counterResponseStringDbl(XI_SRV_COUNTER_SPEED, m_dLastSpeedValue, "MBps");
                    sResponseDetail += counterResponseString(XI_SRV_COUNTER_STORAGE_ERRORS, (int)m_pRecorder->storageErrors());
                }
                catch (FIS_lib_Exception const &e)
                {
                    printOutputMessage(e.what(), false);
                }
            }
#endif
            if (m_hCamera != nullptr)
            {
                int iCounter;
                if (cameraCounter(XI_CNT_SEL_TRANSPORT_SKIPPED_FRAMES, iCounter))
                    sResponseDetail += counterResponseString(XI_SRV_COUNTER_TRANSPORT_SKIPPED, iCounter);
                if (cameraCounter(XI_CNT_SEL_FRAME_MISSED_TRIGGER_DUETO_OVERLAP, iCounter))
                    sResponseDetail += counterResponseString(XI_SRV_COUNTER_MISSED_TRIGGER_DUETO_OVERLAP, iCounter);
                if (cameraCounter(XI_CNT_SEL_FRAME_MISSED_TRIGGER_DUETO_FRAME_BUFFER_OVR, iCounter))
                    sResponseDetail += counterResponseString(XI_SRV_COUNTER_MISSED_TRIGGER_DUETO_FRAME_BUFFER_OVR, iCounter);
                if (cameraCounter(XI_CNT_SEL_FRAME_BUFFER_OVERFLOW, iCounter))
                    sResponseDetail += counterResponseString(XI_SRV_COUNTER_BUFFER_OVERFLOW, iCounter);
            }
        }
        else if (m_eStatus == exsSpeedTesting || m_eStatus == exsSpeedTestFinished)
        {
#ifdef MULTIRECORDER_FIS_ENABLE
            if (m_pRecorder != nullptr)
            {
                try {
                    int iStorageErrors = (int)m_pRecorder->storageErrors();
                    if (m_iLastRecordedFrames > 0)
                    {
                        auto timeNow = std::chrono::system_clock::now();
                        double dTimeDiffSec = std::chrono::duration<double>(timeNow - m_timeLastSpeedCheck).count();
                        if (dTimeDiffSec > 0.0)
                        {
                            m_dLastSpeedValue = (m_iLastRecordedFrames * m_pRecorder->frameSize()/1000000) / dTimeDiffSec; // MBps
                            m_timeLastSpeedCheck = timeNow;
                            m_iLastRecordedFrames = 0;

                            // output to server log too in a format suitable for CSV
                            if (xiRecorderApi::hasLogFile())
                            {
                                time_t timeTestTime = std::chrono::system_clock::to_time_t(timeNow);
                                std::stringstream ssTestTime;
                                ssTestTime << std::put_time(std::localtime(&timeTestTime), "%Y-%m-%dT%H:%M:%S");
                                printOutputMessage(ssTestTime.str() + "," + std::to_string(m_dLastSpeedValue) + "," + std::to_string(iStorageErrors), false);
                            }
                        }
                    }
                    sResponseDetail += counterResponseStringDbl(XI_SRV_COUNTER_SPEED, m_dLastSpeedValue, "MBps");
                    sResponseDetail += counterResponseString(XI_SRV_COUNTER_STORAGE_ERRORS, iStorageErrors);
                }
                catch (FIS_lib_Exception const &e)
                {
                    printOutputMessage(e.what(), false);
                }
            }
#endif
        }
        else if (m_eStatus == exsExporting || m_eStatus == exsExportingFinished)
        {
            int iCounterValue = 0;
            if (recordingCounter(XI_SRV_COUNTER_DONE, iCounterValue))
                sResponseDetail += counterResponseString(XI_SRV_COUNTER_DONE, iCounterValue);
            if (recordingCounter(XI_SRV_COUNTER_TOTAL, iCounterValue))
                sResponseDetail += counterResponseString(XI_SRV_COUNTER_TOTAL, iCounterValue);
        }
        bRet = !sResponseDetail.empty();
    }
    else if (sCmdName == XI_CMD_GET_STORAGE_CAPACITY)
    {
#ifdef MULTIRECORDER_FIS_ENABLE
        if (m_pRecorder != nullptr)
        {
            try {
                uint64_t uiSpace = storageBytesAvailable();
                bRet = uiSpace > 0;
                sResponseDetail = std::to_string(uiSpace);
            }
            catch (FIS_lib_Exception const &e)
            {
                printOutputMessage(e.what(), false);
                sResponseDetail = e.what();
            }
        }
#endif
    }
    else if (sCmdName == XI_CMD_START_STORAGE_SPEED_TEST)
    {
        const char *szAttrDuration = pElCmd->Attribute("duration");
        const char *szAttrBufferSize = pElCmd->Attribute("bufferSize");
        const char *szAttrBufferLoopCount = pElCmd->Attribute("frameLoopCount");
        if (szAttrDuration == nullptr || szAttrBufferSize == nullptr || szAttrBufferLoopCount == nullptr)
            sResponseDetail = "Missing argument";
        else
            startStorageSpeedTestAsync(atoi(szAttrDuration), atoi(szAttrBufferSize), atoi(szAttrBufferLoopCount));
    }
    else if (sCmdName == XI_CMD_STOP_STORAGE_SPEED_TEST)
    {
        if (m_eStatus == exsSpeedTesting)
            cancelRecording();
        m_eStatus = exsSpeedTestFinished;
    }
    else if (sCmdName == XI_CMD_GET_RECORDED_FRAMEDATA)
    {
        const char *szAttrIdx = pElCmd->Attribute("idx");
        if (szAttrIdx == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            int iIdx = atoi(szAttrIdx);
            if (psResponse != nullptr)
            {
                bRet = getRecordedFrameData(iIdx, *psResponse, sResponseDetailContentType);
                bResponseAlreadySet = (sResponseDetailContentType == DX_IMG_BINARY_MIME);
                if (!bResponseAlreadySet)
                    sResponseDetail = *psResponse;
            }
            else
                bRet = getRecordedFrameData(iIdx, sResponseDetail, sResponseDetailContentType);
        }
    }
    else if (sCmdName == XI_CMD_SAVE_SESSION)
    {
        const char *szAttrFilename = pElCmd->Attribute("file");
        const char *szAttrImageFormat = pElCmd->Attribute("imageFormat");
        if (szAttrFilename == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            int iOutputFmt = XI_RGB24;
            if (szAttrImageFormat != nullptr && strlen(szAttrImageFormat) > 0)
            {
                if (!findXiApiEnumValue(szAttrImageFormat, iOutputFmt))
                    iOutputFmt = atoi(szAttrImageFormat);
            }
            bRet = saveSession(szAttrFilename, (XI_IMG_FORMAT)iOutputFmt, &sResponseDetail);
        }
    }
    else if (sCmdName == XI_CMD_EXPORT_TIFF_FILES)
    {
        const char *szAttrFolder = pElCmd->Attribute("folder");
        const char *szAttrImageFormat = pElCmd->Attribute("imageFormat");
        const char *szAttrTiffEnable16bit  = pElCmd->Attribute("tiffEnableStoring16bit");
        const char *szAttrTiffScale16bit  = pElCmd->Attribute("tiffScaleDataToFull16bitRange");
        if (szAttrFolder == nullptr || szAttrImageFormat == nullptr)
            sResponseDetail = "Missing argument";
        else
        {
            bool bTiffEnableStoring16bit = true;
            bool bTiffScaleDataToFull16bitRange = false;
            if (szAttrTiffEnable16bit != nullptr)
                bTiffEnableStoring16bit = (strcmp(szAttrTiffEnable16bit, "false") != 0);
            if (szAttrTiffScale16bit != nullptr)
                bTiffScaleDataToFull16bitRange = (strcmp(szAttrTiffScale16bit, "true") == 0);

            int iOutputFmt = XI_RGB24;
            if (!findXiApiEnumValue(szAttrImageFormat, iOutputFmt))
                iOutputFmt = atoi(szAttrImageFormat);
            bRet = exportTiffFilesAsync(szAttrFolder, iOutputFmt, bTiffEnableStoring16bit, bTiffScaleDataToFull16bitRange, -1, &sResponseDetail);
        }
    }
    else if (sCmdName == XI_CMD_CANCEL_EXPORTING)
    {
        bRet = cancelExporting();
    }
    else if (sCmdName == XI_CMD_RELEASE_DATA_BUFFERS)
    {
        cancelExporting();      // delete thread

        bool bDelete = true;
        const char *szAttrDelete = pElCmd->Attribute("deleteRecorderDataFiles");
        if (szAttrDelete != nullptr)
            bDelete = (strcmp(szAttrDelete, "true") == 0);

        bRet = releaseDataBuffers(bDelete);
    }
    else if (sCmdName == XI_CMD_USER_IMG_PROCESSING_CMD)
    {
        if (m_pUserImageProcessor != nullptr)
        {
            if (psResponse != nullptr)
            {
                bRet = m_pUserImageProcessor->command(pElCmd, *psResponse, sResponseValueType);
                bResponseAlreadySet = (sResponseDetailContentType != "text/plain");
                if (!bResponseAlreadySet)
                    sResponseDetail = *psResponse;
            }
            else
                bRet = m_pUserImageProcessor->command(pElCmd, sResponseDetail, sResponseValueType);
        }
        else
            sResponseDetail = "Unknown command";
    }
    else if (!sCmdName.empty())
        sResponseDetail = "Unknown command";
    else
        sResponseDetail = "Unknown request";

    if (psResponse != nullptr && !bResponseAlreadySet)
    {
        if (sResponseDetailContentType == "text/plain" || sResponseDetailContentType == "text/xml")
        {
            if (iXiReturnCode != -1)
                *psResponse = xiRecorderApi::formatResponse(bRet, sResponseDetail, &iXiReturnCode, &sResponseValueType);
            else
                *psResponse = xiRecorderApi::formatResponse(bRet, sResponseDetail);
            sResponseDetailContentType = "text/xml";
        }
        else
            *psResponse = sResponseDetail;
    }
    if (psResponseContentType != nullptr)
        *psResponseContentType = sResponseDetailContentType;
    return bRet;
}

//----------------------------------------------------------------------------------------
std::string CxCameraControl::counterResponseString(const std::string &sCounterName, int iValue)
{
    return std::string("<counter name=\"") + sCounterName + "\">" + std::to_string(iValue) + "</counter>";
}

//----------------------------------------------------------------------------------------
std::string CxCameraControl::counterResponseStringDbl(const std::string &sCounterName, double dValue, const std::string &sUnit)
{
    std::stringstream ssCounter;
    ssCounter << "<counter name=\"" << sCounterName << "\">" << std::fixed << std::setprecision(2) << dValue;
    if (!sUnit.empty())
        ssCounter << " " << sUnit;
    ssCounter << "</counter>";
    return ssCounter.str();
}

//----------------------------------------------------------------------------------------
uint64_t CxCameraControl::storageBytesAvailable()
{
#ifdef MULTIRECORDER_FIS_ENABLE
    if (m_pRecorder == nullptr)
        return 0;

    //TODO: use this code in IxFisRecorder::storageBytesAvailable() after all build servers support std::filesystem (failed on Linux ARM and macOS)
    std::vector<std::string> vecStoragePaths = m_pRecorder->storagePaths();
    std::set<std::string> setCheckedPaths;
    uint64_t uiTotalFreeSpace = 0;
    for (std::string &sPath : vecStoragePaths)
    {
        // path destinations can be checked only once
        if (setCheckedPaths.find(sPath) != setCheckedPaths.end())   // found
            continue;

        // get the free space
        std::error_code err;
        const std::filesystem::space_info si = std::filesystem::space(std::filesystem::path(sPath), err);
        if (err) continue;

        // remove 128 MBs from free space
        uint64_t u128M = 128 * 1000 * 1000;
        if (si.available > u128M)
            uiTotalFreeSpace += si.available - u128M;
        setCheckedPaths.insert(sPath);
    }
    return uiTotalFreeSpace;
#else
    return 0;
#endif //MULTIRECORDER_FIS_ENABLE
}

//----------------------------------------------------------------------------------------
void CxCameraControl::storageSpeedTest(int iDuration, int iBufferSize, int iFrameLoopCount)
{
#ifdef MULTIRECORDER_FIS_ENABLE
    if (m_pRecorder == nullptr)
        return;

    // first align buffer size to FIS
    int iAlign = (int)m_pRecorder->requiredFrameSizeByteAligning();
    if (iAlign > 0)
        iBufferSize = ((iBufferSize + iAlign-1) / iAlign) * iAlign;

    void *pBuffer = malloc(iBufferSize);
    if (pBuffer == nullptr)
    {
        m_eStatus = exsSpeedTestFinished;
        return;
    }

    try
    {
        m_pRecorder->setFrameSize(iBufferSize);
        m_pRecorder->setCircleSize(iFrameLoopCount);
        m_pRecorder->initializeBuffer();
    }
    catch (FIS_lib_Exception const &e)
    {
        printOutputMessage(e.what(), false);
        return;
    }

    std::chrono::system_clock::time_point timeTestStart = std::chrono::system_clock::now();
    m_timeLastSpeedCheck = timeTestStart;
    m_dLastSpeedValue = 0.0;
    m_iLastRecordedFrames = 0;

    while (!m_bCancelRecording)
    {
        try {
            m_pRecorder->pushFrame(pBuffer);
        }
        catch (FIS_lib_Exception const &e)
        {
            printOutputMessage(e.what(), false);
        }
        m_iLastRecordedFrames++;

        double dDiff = std::chrono::duration<double>(std::chrono::system_clock::now() - timeTestStart).count();
        if (dDiff > iDuration)
            break;
    }
    m_pRecorder->finishWriting();
    m_eStatus = exsSpeedTestFinished;
    free(pBuffer);
#endif
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::saveSession(const std::string &sFilename, XI_IMG_FORMAT iFmtOutput, std::string *psErrorMsg)
{
#ifdef MULTIRECORDER_FIS_ENABLE
    if (m_pRecorder == nullptr)
        return false;

    std::string sSessionFilename(sFilename);
    std::string sFileExt;
    try {
        std::filesystem::path p(sSessionFilename);
        sFileExt = p.extension().string();
    } catch (...) {
    }

    if (sFileExt != ".xirecsession")
        sSessionFilename += ".xirecsession";

    SxFisAdditionalPersistanceData aFisAdditionalData;
    aFisAdditionalData.m_iOutputFmt = iFmtOutput;
    aFisAdditionalData.m_iConcatImgCount = m_iConcatImgCount;
    aFisAdditionalData.m_iConcatImgNextOffset = m_iConcatImgNextOffset;
    aFisAdditionalData.m_aMetadata = m_aImageMetadata;
    aFisAdditionalData.m_vecTimeStamps = m_vecTimeStamps;
    aFisAdditionalData.m_vecFrameNo = m_vecFrameNo;
    return m_pRecorder->saveConfiguration(sSessionFilename, &aFisAdditionalData);
#else
    return false;
#endif //MULTIRECORDER_FIS_ENABLE
}

//----------------------------------------------------------------------------------------
bool CxCameraControl::loadSession(const std::string &sFilename, std::string *psErrorMsg)
{
#ifdef MULTIRECORDER_FIS_ENABLE
    if (m_pRecorder != nullptr)
    {
        delete m_pRecorder;
        m_pRecorder = nullptr;
    }

    SxFisAdditionalPersistanceData aFisAdditionalData;
    m_pRecorder = IxFisRecorder::loadConfiguration(sFilename, &aFisAdditionalData);
    if (m_pRecorder == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Corrupted xirecsession file, tag RecSession not found";
        return false;
    }

    // load our data
    m_iLoadedExportImageFormat = aFisAdditionalData.m_iOutputFmt;
    m_iConcatImgCount = aFisAdditionalData.m_iConcatImgCount;
    m_iConcatImgNextOffset = aFisAdditionalData.m_iConcatImgNextOffset;
    m_aImageMetadata = aFisAdditionalData.m_aMetadata;
    m_vecTimeStamps = aFisAdditionalData.m_vecTimeStamps;
    m_vecFrameNo = aFisAdditionalData.m_vecFrameNo;
    m_timeAcqStart = m_aImageMetadata.m_timeAcqDateTime;
    return true;
#else
    return false;
#endif //MULTIRECORDER_FIS_ENABLE
}

//----------------------------------------------------------------------------------------
void CxCameraControl::showLiveImagePreview(XI_IMG *pXiImg)
{
#ifdef MULTIRECORDER_FIS_ENABLE
    if (pXiImg != nullptr)
    {
        // convert to RGB
        int iFmtOutput = XI_RGB24;
        XI_IMG xiImgOutput;
        memset(&xiImgOutput, 0, sizeof(XI_IMG));
        xiImgOutput.size = sizeof(XI_IMG);

        xiProcessingHandle_t hProc;
        XI_RETURN ret = xiProcOpen(&hProc);
        if (ret != XI_OK)
        {
            xiRecorderApi::outputStringToStdErr(std::string("showLiveImagePreview: xiProcOpen failed with error ") + CxCameraControl::xiApiErrorString(ret));
            return;
        }

        ret = xiProcSetParam(hProc, XI_PRM_API_CONTEXT_LIST, (void*)m_aImageMetadata.m_sApiContextList.c_str(), (DWORD)m_aImageMetadata.m_sApiContextList.length(), xiTypeString);
        if (ret != XI_OK)
            xiRecorderApi::outputStringToStdErr(std::string("showLiveImagePreview: xiProcSetParam(apiContextList) failed with error ") + CxCameraControl::xiApiErrorString(ret));

        ret = xiProcSetParam(hProc, XI_PRM_IMAGE_DATA_FORMAT, &iFmtOutput, sizeof(iFmtOutput), xiTypeInteger);
        if (ret != XI_OK)
            xiRecorderApi::outputStringToStdErr(std::string("showLiveImagePreview: xiProcSetParam(imageDataFormat) failed with error ") + CxCameraControl::xiApiErrorString(ret));

        bool bOK = false;
        ret = xiProcPushImage(hProc, (unsigned char*)pXiImg->bp);
        if (ret == XI_OK)
        {
            ret = xiProcPullImage(hProc, 2000, &xiImgOutput);
            if (ret != XI_OK)
                xiRecorderApi::outputStringToStdErr(std::string("showLiveImagePreview: xiProcPullImage failed with error ") + CxCameraControl::xiApiErrorString(ret));
            else
                bOK = true;
        }
        if (!bOK)
        {
            xiProcClose(hProc);
            return;
        }

        // pass to main thread, OpenCV GUI needs to run there
        std::string sWndName = std::string("MultiRecorder Live Preview") + " - " + m_aImageMetadata.cameraDisplayName();
        CxLiveImagePreview::showPreviewForXiImg(sWndName, &xiImgOutput, true);
        xiProcClose(hProc);
    }
#endif //MULTIRECORDER_FIS_ENABLE
}

//----------------------------------------------------------------------------------------
void CxCameraControl::processRawData(const std::string& sData, std::string* psResponse, std::string* psResponseContentType)
{
    if (m_pUserImageProcessor != nullptr)
        m_pUserImageProcessor->processRawData(sData, psResponse, psResponseContentType);
}
