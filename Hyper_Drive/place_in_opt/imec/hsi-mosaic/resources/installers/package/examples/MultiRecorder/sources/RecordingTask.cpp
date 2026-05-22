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

#include "RecordingTask.h"
#include "CameraServer.h"
#include "CameraClient.h"
#include "RecorderApi.h"

#include <tinyxml.h>

CxRecordingTask::CxRecordingTask(int32_t iAutoStartServerPort, bool bVerboseLog)
    : m_iAutoStartServerPort(iAutoStartServerPort), m_bPrintServerLogVerbose(bVerboseLog)
{
    m_iFrameCount = 10;
    m_sExportImageFormat = "XI_RGB24";
    m_pServer = nullptr;
    m_pClient = nullptr;
    m_bCameraConnected = false;
    m_bTiffEnableStoring16bit = true;
    m_bTiffScaleDataToFull16bitRange = false;

    m_bTestStorageSpeedOnly = false;
    m_iSpeedTestDurationS = 0;
    m_iSpeedTestBufferSize = 0;
}

//----------------------------------------------------------------------------------------
CxRecordingTask::~CxRecordingTask()
{
    if (m_pClient != nullptr)
        delete m_pClient;
    if (m_pServer != nullptr)
        delete m_pServer;
}

//----------------------------------------------------------------------------------------
void CxRecordingTask::printLogMessage(const std::string &sMessage, bool bVerbose)
{
    if (!bVerbose || m_bPrintServerLogVerbose)
        xiRecorderApi::outputStringToStdErr(sMessage);
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::sendRequestToServer(const std::string &sRequest, std::string *psResponseDetails, XI_RETURN *pxiRet)
{
    std::string sXmlResponse, sResponseContentType;
    if (!sendRequestToServerResGeneric(sRequest, &sXmlResponse, &sResponseContentType))
        return false;

    if (sResponseContentType != "text/xml")
        return false;

    bool bResult = false;
    std::string sDetails;
    xiRecorderApi::parseXmlResponse(sXmlResponse, bResult, sDetails, pxiRet);
    if (psResponseDetails != nullptr)
        *psResponseDetails = sDetails;

    return bResult;
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::sendRequestToServerResXml(const std::string &sRequest, TiXmlDocument *pResponseToXmlDoc, TiXmlElement *&pRootResponseElement, XI_RETURN *pxiRet)
{
    if (pResponseToXmlDoc == nullptr)
        return false;

    std::string sXmlResponse, sResponseContentType;
    if (!sendRequestToServerResGeneric(sRequest, &sXmlResponse, &sResponseContentType))
        return false;

    if (sResponseContentType != "text/xml")
        return false;

    bool bResult = false;
    xiRecorderApi::parseXmlResponseToXml(sXmlResponse, bResult, pResponseToXmlDoc, pRootResponseElement, pxiRet);
    if (!sXmlResponse.empty() && !bResult)
    {
        std::string sErrorMsg;
        if (pRootResponseElement != nullptr)
        {
            const char *szDetailsText = pRootResponseElement->GetText();
            if (szDetailsText != nullptr)
                sErrorMsg = szDetailsText;
        }

        printLogMessage(std::string("RecordingTask: server returned error \"") + sErrorMsg
                               + "\" to request: " + sRequest, false);
    }
    return bResult;
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::sendRequestToServerResGeneric(const std::string &sRequest, std::string *psResponse, std::string *psResponseContentType)
{
    if (m_pClient == nullptr)
    {
        printLogMessage("RecordingTask error: http client not initialized.", false);
        return false; // no setup done
    }

    if (m_bPrintServerLogVerbose)
        printLogMessage("RecordingTask sending request: " + sRequest, true);

    bool bRet = m_pClient->sendRequest(sRequest, psResponse, psResponseContentType);

    if (m_bPrintServerLogVerbose && bRet && psResponse != nullptr)
    {
        if (psResponseContentType != nullptr && *psResponseContentType == "text/xml")
            printLogMessage("RecordingTask received response: " + *psResponse, true);
        else
            printLogMessage("RecordingTask received response with data size: " + std::to_string(psResponse->length()), true);
    }
    return bRet;
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::readFromRecordingSetupXmlTag(TiXmlElement *pElTask)
{
    if (pElTask == nullptr)
    {
        printLogMessage("RecordingTask error: invalid XML script, <cameraTask> not present.", false);
        return false;
    }

    // reqiured options to define task
    const char *szAttrFrameCount    = pElTask->Attribute("frameCount");
    if (szAttrFrameCount != nullptr)
        m_iFrameCount = atoi(szAttrFrameCount);
    const char *szAttrStorageName   = pElTask->Attribute("recordingStorageName");
    if (szAttrStorageName != nullptr)
        m_sStorageName = szAttrStorageName;
    const char *szAttrImageFormat   = pElTask->Attribute("exportImageFormat");
    if (szAttrImageFormat != nullptr)
        m_sExportImageFormat = szAttrImageFormat;
    const char *szAttrOutputFolder  = pElTask->Attribute("exportFolder");
    if (szAttrOutputFolder != nullptr)
        m_sExportFolder = szAttrOutputFolder;
    const char *szAttrTiffEnable16bit  = pElTask->Attribute("tiffEnableStoring16bit");
    if (szAttrTiffEnable16bit != nullptr)
        m_bTiffEnableStoring16bit = (strcmp(szAttrTiffEnable16bit, "false") != 0);
    const char *szAttrTiffScale16bit  = pElTask->Attribute("tiffScaleDataToFull16bitRange");
    if (szAttrTiffScale16bit != nullptr)
        m_bTiffScaleDataToFull16bitRange = (strcmp(szAttrTiffScale16bit, "true") == 0);
    const char *szAttrSaveSessionFile  = pElTask->Attribute("saveSession");
    if (szAttrSaveSessionFile != nullptr)
        m_sSaveSessionFilename = szAttrSaveSessionFile;

    std::string sRemoteHost;
    int iRemotePort = 0;
    const char *szAttrConnectToServer   = pElTask->Attribute("connectToServer");
    if (szAttrConnectToServer != nullptr)
    {
        sRemoteHost = szAttrConnectToServer;
        const char *szPort = strchr(szAttrConnectToServer, ':');
        if (szPort != nullptr && strlen(szPort) > 1)
        {
            sRemoteHost = std::string(szAttrConnectToServer, szPort-szAttrConnectToServer);
            iRemotePort = atoi(szPort + 1);
        }
    }

    // stop old client and server
    if (m_pServer != nullptr)
    {
        delete m_pServer;
        m_pServer = nullptr;
    }
    if (m_pClient != nullptr)
    {
        delete m_pClient;
        m_pClient = nullptr;
    }

    // auto-run server and connect to it
    if (sRemoteHost.empty() || iRemotePort <= 0)
    {
        startOwnServer();
    }
    else
    {
        // connect to already started server
        if (!connectToRemoteServer(sRemoteHost, iRemotePort))
            return false;
    }

    if (m_bTestStorageSpeedOnly && m_iSpeedTestBufferSize > 0)
    {
        // when testing speed (and having buffer size), do not open camera
        return true;
    }

    // options to define camera
    const char *szAttrCameraIdx     = pElTask->Attribute("cameraIdx");
    const char *szAttrCameraInstPath = pElTask->Attribute("cameraInstPath");
    const char *szAttrCameraSn      = pElTask->Attribute("cameraSn");
    const char *szAttrCameraUserID  = pElTask->Attribute("cameraUserID");
    const char *szAttrCameraLocPath = pElTask->Attribute("cameraLocPath");

    bool bOK;
    std::string sErrorMsg;
    if (szAttrCameraIdx != nullptr)
        bOK = openCamera(szAttrCameraIdx, &sErrorMsg);
    else if (szAttrCameraInstPath != nullptr)
        bOK = openCameraBy(XI_OPEN_BY_INST_PATH, szAttrCameraInstPath, &sErrorMsg);
    else if (szAttrCameraSn != nullptr)
        bOK = openCameraBy(XI_OPEN_BY_SN, szAttrCameraSn, &sErrorMsg);
    else if (szAttrCameraUserID != nullptr)
        bOK = openCameraBy(XI_OPEN_BY_USER_ID, szAttrCameraUserID, &sErrorMsg);
    else if (szAttrCameraLocPath != nullptr)
        bOK = openCameraBy(XI_OPEN_BY_LOC_PATH, szAttrCameraLocPath, &sErrorMsg);
    else
    {
        // when no camera defined, open the first camera
        bOK = openCamera("0", &sErrorMsg);
    }
    m_bCameraConnected = bOK;
    if (!m_bCameraConnected)
    {
        if (m_bTestStorageSpeedOnly)
            return true;    // no problem we will use some generic buffer size

        if (sErrorMsg.empty())
            sErrorMsg = "Error when opening camera.";
        xiRecorderApi::outputStringToConsole(sErrorMsg);
        return false;
    }

    // output camera name
    xiRecorderApi::outputStringToConsole(std::string("Connected to camera: ") + m_sCameraName);

    // read camera parameters and other stuff
    TiXmlElement *pSubEl = pElTask->FirstChildElement();
    while (pSubEl != nullptr)
    {
        std::string sElName = pSubEl->ValueStr();
        if (sElName == "applyXiCameraFile")
        {
            const char *pAttrName = pSubEl->Attribute("name");
            if (pAttrName != nullptr)
            {
                if (!sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_APPLY_XICAMERA_FILE}, {"name", pAttrName}}), &sErrorMsg))
                    xiRecorderApi::outputStringToConsole(std::string("applyXiCameraFile error: ") + sErrorMsg);
            }
        }
        else if (sElName == "setCameraParam")
        {
            const char *pElCameraParName  = pSubEl->Attribute("name");
            const char *pElCameraParValue = pSubEl->Attribute("value");
            if (pElCameraParName != nullptr && pElCameraParValue != nullptr)
            {
                if (!setCameraParam(pElCameraParName, pElCameraParValue, &sErrorMsg))
                    xiRecorderApi::outputStringToConsole(std::string("setCameraParam(") + pElCameraParName + ") error: " + sErrorMsg);
            }
        }
        pSubEl = pSubEl->NextSiblingElement();
    }

    const char *szAttrConcatImgCount = pElTask->Attribute("concatImgCount");
    if (szAttrConcatImgCount != nullptr)
    {
        int iConcatImgCount = atoi(szAttrConcatImgCount);
        if (iConcatImgCount > 1)
        {
            if (!setCameraParam(XI_PRM_CONCAT_IMG_MODE, "1", &sErrorMsg))
                xiRecorderApi::outputStringToConsole(std::string("setCameraParam(XI_PRM_CONCAT_IMG_MODE) error: ") + sErrorMsg);
            if (!setCameraParam(XI_PRM_CONCAT_IMG_COUNT, szAttrConcatImgCount, &sErrorMsg))
                xiRecorderApi::outputStringToConsole(std::string("setCameraParam(XI_PRM_CONCAT_IMG_COUNT) error: ") + sErrorMsg);
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------
void CxRecordingTask::startOwnServer()
{
    int iServerPort;
    m_pServer = new CxCameraServer;
    m_pServer->startAndListenAt("localhost", m_iAutoStartServerPort, m_bPrintServerLogVerbose, true, &iServerPort);
    m_pClient = new CxCameraClient("localhost", iServerPort);
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::connectToRemoteServer(const std::string &sRemoteHost, int32_t iRemotePort)
{
    m_pClient = new CxCameraClient(sRemoteHost, iRemotePort);

    // print status
    std::string sFinalServer = sRemoteHost + ":" + std::to_string(iRemotePort);
    std::string sHttpStatus;
    bool bConnected = m_pClient->isConnectedToServer(&sHttpStatus);
    if (bConnected)
        xiRecorderApi::outputStringToConsole(std::string("Connected to server ") + sFinalServer);
    else
    {
        xiRecorderApi::outputStringToConsole(std::string("Error connecting to server ") + sFinalServer + ", error: " + sHttpStatus);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
void CxRecordingTask::setStorageSpeedTest(int iDurationS, int iBufferSize)
{
    m_bTestStorageSpeedOnly = (iDurationS > 0);
    m_iSpeedTestDurationS = iDurationS;
    m_iSpeedTestBufferSize = iBufferSize;
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::openCamera(const std::string &sDeviceIndex, std::string *psErrorMsg)
{
    m_bCameraConnected = sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_OPEN_CAMERA}, {"idx", sDeviceIndex}}), psErrorMsg);
    if (m_bCameraConnected)
        readCameraName();
    return m_bCameraConnected;
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::openCameraBy(XI_OPEN_BY eMethod, const std::string &sValue, std::string *psErrorMsg)
{
    bool bOK = false;
    switch (eMethod)
    {
    case XI_OPEN_BY_INST_PATH:
        bOK = sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_OPEN_CAMERA}, {"instPath", sValue}}), psErrorMsg); break;
    case XI_OPEN_BY_SN:
        bOK = sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_OPEN_CAMERA}, {"sn", sValue}}), psErrorMsg); break;
    case XI_OPEN_BY_USER_ID:
        bOK = sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_OPEN_CAMERA}, {"userID", sValue}}), psErrorMsg); break;
    case XI_OPEN_BY_LOC_PATH:
        bOK = sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_OPEN_CAMERA}, {"locPath", sValue}}), psErrorMsg); break;
    }
    m_bCameraConnected = bOK;
    if (m_bCameraConnected)
        readCameraName();
    return bOK;
}

//----------------------------------------------------------------------------------------
void CxRecordingTask::readCameraName()
{
    std::string sCameraUserId;
    if (getCameraParam(XI_PRM_DEVICE_NAME, m_sCameraName)
        && getCameraParam(XI_PRM_DEVICE_USER_ID, sCameraUserId)
        && !sCameraUserId.empty())
    {
        m_sCameraName += std::string(" (") + sCameraUserId + ")";
    }
}

//----------------------------------------------------------------------------------------
std::string CxRecordingTask::cameraName()
{
    return m_sCameraName;
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::setCameraParam(const std::string &sParamName, const std::string &sValue, std::string *psErrorMsg)
{
    if (!m_bCameraConnected)
        return false;
    return sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_CAMERA_PARAM}, {"name", sParamName}, {"value", sValue}}), psErrorMsg);
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::getCameraParam(const std::string &sParamName, std::string &sValue, std::string *psErrorMsg)
{
    if (!m_bCameraConnected)
        return false;
    std::string sResponse;
    if (sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_CAMERA_PARAM}, {"name", sParamName}}), &sResponse))
    {
        sValue = sResponse;
        return true;
    }
    else if (psErrorMsg != nullptr)
        *psErrorMsg = sResponse;
    return false;
}

//----------------------------------------------------------------------------------------
void CxRecordingTask::setRecordingFrameCount(int32_t iFrameCount)
{
    m_iFrameCount = iFrameCount;
}

//----------------------------------------------------------------------------------------
std::string CxRecordingTask::storageName()
{
    return m_sStorageName;
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::goReady(const std::vector<std::string> &vecStorageFolders, bool bShowLiveImagePreview)
{
    if (!vecStorageFolders.empty() && m_sStorageName.empty())
        m_sStorageName = "internal";    // task defined without storage name, only folders

    std::string sFolders;
    for (auto &sPath : vecStorageFolders)
    {
        if (!sFolders.empty())
            sFolders += "|";
        sFolders += sPath;
    }

    std::string sErrorMsg;
    if (!sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_DATA_STORAGE}, {"folders", sFolders}}), &sErrorMsg))
    {
        xiRecorderApi::outputStringToConsole(std::string("Initializing recording storage error: ") + sErrorMsg);
        return false;
    }

    if (bShowLiveImagePreview && !sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_LIVE_IMG_PREVIEW}, {"set", bShowLiveImagePreview?"true":"false"}}), &sErrorMsg))
    {
        xiRecorderApi::outputStringToConsole(std::string("Initializing live image preview error: ") + sErrorMsg);
    }

    if (m_bTestStorageSpeedOnly)
    {
        if (m_iSpeedTestBufferSize == 0)    // read the size from camera
        {
            std::string sPayloadSize;
            if (m_bCameraConnected && getCameraParam(XI_PRM_IMAGE_PAYLOAD_SIZE, sPayloadSize))
                m_iSpeedTestBufferSize = atoi(sPayloadSize.c_str());
            if (m_iSpeedTestBufferSize <= 0)
                m_iSpeedTestBufferSize = (1<<20);   // use 1MB as default
            xiRecorderApi::outputStringToConsole(std::string("Storage speed test buffer size (KB): ") + std::to_string(m_iSpeedTestBufferSize >> 10));
        }
        return true;
    }

    if (!m_bCameraConnected)
        return false;

    if (!sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_GO_READY}, {"frameCount", std::to_string(m_iFrameCount)}}), &sErrorMsg))
    {
        xiRecorderApi::outputStringToConsole(std::string("Initializing recording error: ") + sErrorMsg);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::start()
{
    if (m_bTestStorageSpeedOnly)
    {
        return sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_START_STORAGE_SPEED_TEST},
                                                                 {"duration", std::to_string(m_iSpeedTestDurationS)},
                                                                 {"bufferSize", std::to_string(m_iSpeedTestBufferSize)},
                                                                 {"frameLoopCount", std::to_string(m_iFrameCount)}}));
    }
    else
    {
        if (!m_bCameraConnected)
            return false;
        else
            return sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_START_ACQ}}));
    }
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::isRecording()
{
    std::string sResponseDetails;
    bool bRet = sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_STATUS}}), &sResponseDetails);
    return bRet && (sResponseDetails == XI_SRV_STATUS_RECORDING || sResponseDetails == XI_SRV_STATUS_SPEEDTESTING);
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::stop()
{
    if (m_bTestStorageSpeedOnly)
    {
        return sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_STOP_STORAGE_SPEED_TEST}}));
    }
    else
    {
        if (!m_bCameraConnected)
            return false;
        else
            return sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_STOP_ACQ}}));
    }
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::willSaveSession()
{
    return !m_sSaveSessionFilename.empty() && !m_bTestStorageSpeedOnly;
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::saveSessionToFile()
{
    if (!willSaveSession())
        return true;

    if (!m_bCameraConnected)
        return false;

    std::string sErrorMsg;
    if (!sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_SAVE_SESSION}, {"file", m_sSaveSessionFilename}, {"imageFormat", m_sExportImageFormat}}), &sErrorMsg))
    {
        xiRecorderApi::outputStringToConsole(std::string("Save session error: ") + sErrorMsg);
        return false;
    }
    xiRecorderApi::outputStringToConsole(m_sCameraName + ": " + m_sSaveSessionFilename);
    return true;
}

//----------------------------------------------------------------------------------------
void CxRecordingTask::setExportOptions(const std::string &sExportFolder, const std::string &sExportImageFormat, bool bTiffEnableStoring16bit, bool bTiffScaleDataToFull16bitRange)
{
    m_sExportFolder = sExportFolder;
    m_sExportImageFormat = sExportImageFormat;
    m_bTiffEnableStoring16bit = bTiffEnableStoring16bit;
    m_bTiffScaleDataToFull16bitRange = bTiffScaleDataToFull16bitRange;
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::willExportTiffs()
{
    return !m_sExportFolder.empty() && !m_bTestStorageSpeedOnly;
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::exportData()
{
    if (!willExportTiffs())
        return true;

    if (!m_bCameraConnected)
        return false;

    std::string sErrorMsg;
    if (!sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_EXPORT_TIFF_FILES}, {"folder", m_sExportFolder}, {"imageFormat", m_sExportImageFormat},
           {"tiffEnableStoring16bit", m_bTiffEnableStoring16bit?"true":"false"}, {"tiffScaleDataToFull16bitRange", m_bTiffScaleDataToFull16bitRange?"true":"false"}}), &sErrorMsg))
    {
        xiRecorderApi::outputStringToConsole(std::string("Export data error: ") + sErrorMsg);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::isExporting()
{
    if (!m_bCameraConnected || m_bTestStorageSpeedOnly)
        return false;

    std::string sResponseDetails;
    bool bRet = sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_STATUS}}), &sResponseDetails);
    return bRet && sResponseDetails == XI_SRV_STATUS_EXPORTING;
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::outputCounters()
{
    if (!m_bCameraConnected && !m_bTestStorageSpeedOnly)
        return false;

    TiXmlDocument aResponseDoc;
    TiXmlElement* pElResponseRoot = nullptr;
    bool bRet = sendRequestToServerResXml(xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_IMAGES_COUNT}}), &aResponseDoc, pElResponseRoot);
    if (!bRet || pElResponseRoot == nullptr)
        return false;

    std::string sOutput = m_sCameraName + "  ";

    TiXmlElement* pElCounter = pElResponseRoot->FirstChildElement("counter");
    while (pElCounter != nullptr)
    {
        const char *szAttrName = pElCounter->Attribute("name");
        const char *szTextContents = pElCounter->GetText();
        if (szAttrName != nullptr && szTextContents != nullptr)
        {
            sOutput += std::string(szAttrName) + ": " + szTextContents + "  ";
        }
        pElCounter = pElCounter->NextSiblingElement("counter");
    }
    xiRecorderApi::outputStringToConsole(sOutput);

    return true;
}

//----------------------------------------------------------------------------------------
void CxRecordingTask::outputExportFolder()
{
    if (!m_bCameraConnected)
        return;
    xiRecorderApi::outputStringToConsole(m_sCameraName + ": " + m_sExportFolder);
}

//----------------------------------------------------------------------------------------
bool CxRecordingTask::finalize()
{
    // delete data files only we don't need the data later (for restoring the session)
    //   - willSaveSessiong is set when user explicity wants to keep he data
    //   - m_sStorageName is empty when no recording was done (probably only export from savedSession)
    std::string sDeleteDataFiles(willSaveSession() || m_sStorageName.empty() ? "false" : "true");

    sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_RELEASE_DATA_BUFFERS}, {"deleteRecorderDataFiles", sDeleteDataFiles}}));
    sendRequestToServer(xiRecorderApi::formatRequest({{"cmd", XI_CMD_CLOSE_CAMERA}}));
    return true;
}
