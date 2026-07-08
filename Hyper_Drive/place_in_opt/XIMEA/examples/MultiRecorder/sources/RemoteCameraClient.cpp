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

#include "RemoteCameraClient.h"
#include "CameraClient.h"
#include "RecorderApi.h"

#include <tinyxml.h>
#include <xiApi.h>

CxRemoteCameraClient::CxRemoteCameraClient()
{
    m_pHttpClient = nullptr;
    m_pHttpClientForImageStream = nullptr;
    m_bLogAllErrors = false;
}

//----------------------------------------------------------------------------------------
CxRemoteCameraClient::~CxRemoteCameraClient()
{
    if (m_pHttpClient != nullptr)
        delete m_pHttpClient;
    if (m_pHttpClientForImageStream != nullptr)
        delete m_pHttpClientForImageStream;
}

//----------------------------------------------------------------------------------------
TxRemoteCameraLogCallback g_fnCameraLogCallback = nullptr;
void CxRemoteCameraClient::setErrorLogFunction(TxRemoteCameraLogCallback callback)
{
    g_fnCameraLogCallback = callback;
}

//----------------------------------------------------------------------------------------
void CxRemoteCameraClient::logError(const std::string &msg)
{
    if (g_fnCameraLogCallback != nullptr)
        g_fnCameraLogCallback(msg);
}

//----------------------------------------------------------------------------------------
bool CxRemoteCameraClient::sendRequestToServer(CxCameraClient *pHttpClient, const std::string &sRequest, std::string *psResponseDetails, XI_RETURN *pxiRet)
{
    if (pxiRet != nullptr) *pxiRet = -1;
    if (pHttpClient == nullptr)
        return false; // no setup done

    std::string sXmlResponse, sResponseContentType;
    if (!pHttpClient->sendRequest(sRequest, &sXmlResponse, &sResponseContentType))
    {
        if (pxiRet != nullptr) *pxiRet = XI_DEVICE_NOT_READY;
        return false;
    }

    if (sResponseContentType != "text/xml")
        return false;

    bool bResponseOk;
    std::string sDetails;
    xiRecorderApi::parseXmlResponse(sXmlResponse, bResponseOk, sDetails, pxiRet);
    if (psResponseDetails != nullptr)
        *psResponseDetails = sDetails;
    return bResponseOk;
}

//----------------------------------------------------------------------------------------
bool CxRemoteCameraClient::sendRequestToServerResXml(CxCameraClient *pHttpClient, const std::string &sRequest, TiXmlDocument *pResponseToXmlDoc, TiXmlElement *&pRootResponseElement, XI_RETURN *pxiRet)
{
    if (pxiRet != nullptr) *pxiRet = -1;
    if (pHttpClient == nullptr)
        return false; // no setup done

    std::string sXmlResponse, sResponseContentType;
    if (!pHttpClient->sendRequest(sRequest, &sXmlResponse, &sResponseContentType))
    {
        if (pxiRet != nullptr) *pxiRet = XI_DEVICE_NOT_READY;
        return false;
    }

    if (sResponseContentType != "text/xml")
        return false;

    bool bResponseOk;
    xiRecorderApi::parseXmlResponseToXml(sXmlResponse, bResponseOk, pResponseToXmlDoc, pRootResponseElement, pxiRet);
    return bResponseOk;
}

//---------------------------------------------------------
std::list<CxRemoteCameraClient::SxRemoteCameraDesc> CxRemoteCameraClient::listCamerasAtAddress(const std::string &sIPAddress, int iPort, std::string &sErrorCode)
{
    std::list<CxRemoteCameraClient::SxRemoteCameraDesc> lstRes;
    CxCameraClient *pHttpClient = new CxCameraClient(sIPAddress, iPort);
    if (pHttpClient == nullptr)
        return lstRes;

    if (pHttpClient->isConnectedToServer(&sErrorCode))
    {
        // get list of cameras
        TiXmlDocument aResponseDoc;
        TiXmlElement *pRootResponseElement = nullptr;

        if (CxRemoteCameraClient::sendRequestToServerResXml(pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_CONNECTED_DEVICES}}), &aResponseDoc, pRootResponseElement)
            && pRootResponseElement != nullptr)
        {
            // parse the output
            TiXmlElement* pElDevice = pRootResponseElement->FirstChildElement("device");
            while (pElDevice != nullptr)
            {
                const char *szAttrSerial = pElDevice->Attribute("sn");
                const char *szAttrDeviceName = pElDevice->Attribute("deviceName");
                if (szAttrSerial != nullptr && szAttrDeviceName != nullptr)
                {
                    CxRemoteCameraClient::SxRemoteCameraDesc aItem;
                    aItem.m_sIPAddress = sIPAddress;
                    aItem.m_iPort = iPort;
                    aItem.m_sSerial = std::string(szAttrSerial);
                    aItem.m_sName = std::string(szAttrDeviceName);
                    lstRes.push_back(aItem);
                }
                pElDevice = pElDevice->NextSiblingElement("device");
            }
        }
    }
    delete pHttpClient;
    return lstRes;
}

//---------------------------------------------------------
CxRemoteCameraClient* CxRemoteCameraClient::openCamera(const std::string &sIPAddress, int iPort, const std::string &sDeviceName, const std::string &sSerial, bool bLogErrors)
{
    CxCameraClient *pHttpClient = new CxCameraClient(sIPAddress, iPort);
    if (pHttpClient == nullptr)
        return nullptr;

    std::string sErrorMsg;
    if (!pHttpClient->isConnectedToServer(&sErrorMsg))
    {
        if (bLogErrors)
            CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::openCamera() ") + sErrorMsg);
        delete pHttpClient;
        return nullptr;
    }

    if (!CxRemoteCameraClient::sendRequestToServer(pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_OPEN_CAMERA}, {"sn", sSerial}}), &sErrorMsg))
    {
        if (bLogErrors)
            CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::openCamera() ") + sErrorMsg);
        delete pHttpClient;
        return nullptr;
    }

    CxRemoteCameraClient *pRemoteCamera = new CxRemoteCameraClient;
    pRemoteCamera->m_pHttpClient = pHttpClient;
    pRemoteCamera->m_pHttpClientForImageStream = new CxCameraClient(sIPAddress, iPort);
    pRemoteCamera->m_aDesc.m_sIPAddress = sIPAddress;
    pRemoteCamera->m_aDesc.m_iPort = iPort;
    pRemoteCamera->m_aDesc.m_sName = sDeviceName;
    pRemoteCamera->m_aDesc.m_sSerial = sSerial;
    return pRemoteCamera;
}

//----------------------------------------------------------------------------------------
void CxRemoteCameraClient::remoteConnectionParameters(std::string &sHostName, int &iPort) const
{
    sHostName = m_aDesc.m_sIPAddress;
    iPort = m_aDesc.m_iPort;
}

//---------------------------------------------------------
int CxRemoteCameraClient::closeDevice()
{
    XI_RETURN xiRet = XI_INVALID_HANDLE;
    std::string sErrorMsg;
    if (!sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_CLOSE_CAMERA}}), &sErrorMsg, &xiRet))
    {
        CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::closeDevice() ") + sErrorMsg);
        return xiRet;
    }
    return xiRet;
}

//---------------------------------------------------------
int CxRemoteCameraClient::startAcquisition()
{
    XI_RETURN xiRet = XI_INVALID_HANDLE;
    std::string sErrorMsg;
    if (!sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_START_ACQ}}), &sErrorMsg, &xiRet))
    {
        CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::startAcquisition() ") + sErrorMsg);
        return xiRet;
    }
    return xiRet;
}

//---------------------------------------------------------
int CxRemoteCameraClient::stopAcquisition()
{
    XI_RETURN xiRet = XI_INVALID_HANDLE;
    std::string sErrorMsg;
    if (!sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_STOP_ACQ}}), &sErrorMsg, &xiRet))
    {
        CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::stopAcquisition() ") + sErrorMsg);
        return xiRet;
    }
    return xiRet;
}

//---------------------------------------------------------
int CxRemoteCameraClient::getImage(unsigned uiTimeout, XI_IMG *pXiImg)
{
    XI_RETURN xiRet = XI_INVALID_HANDLE;
    std::string sResponse, sResponseContentType;
    if (!getImageForResponse(uiTimeout, sResponse, sResponseContentType))
        return xiRet;

    if (sResponseContentType == "text/xml")
    {
        // is error
        bool bResponseOk;
        std::string sErrorMsg;
        xiRecorderApi::parseXmlResponse(sResponse, bResponseOk, sErrorMsg, &xiRet);
        CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::getImage() ") + sErrorMsg);
        return xiRet;
    }
    if (sResponseContentType == "application/octet-stream")
    {
        xiRet = CxRemoteCameraClient::fillXiImgFromResponse(pXiImg, sResponse);
        //CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::getImage() ") + std::string("First pixel got: %1").arg((int)(*(char*)pXiImg->bp))));
        return xiRet;
    }
    return XI_NOT_SUPPORTED;
}

//---------------------------------------------------------
bool CxRemoteCameraClient::getImageForResponse(unsigned uiTimeout, std::string &sResponse, std::string &sResponseContentType)
{
    CxCameraClient *pHttpClient = m_pHttpClientForImageStream;
    if (pHttpClient == nullptr)
        pHttpClient = m_pHttpClient;

    if (pHttpClient == nullptr)
        return false;
    if (!pHttpClient->sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_CAMERA_IMAGE}, {"timeout", std::to_string(uiTimeout)}}), &sResponse, &sResponseContentType))
    {
        CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::getImage() ") + "Request failed");
        return false;
    }
    return true;
}

//---------------------------------------------------------
int CxRemoteCameraClient::fillXiImgFromResponse(XI_IMG *pXiImg, const std::string &sResponse)
{
    return fillXiImgFromResponse(pXiImg, sResponse.data(), sResponse.length());
}

//---------------------------------------------------------
int CxRemoteCameraClient::fillXiImgFromResponse(XI_IMG *pXiImg, const void* pResponseBuffer, uint64_t uiResponseSize)
{
    if (uiResponseSize < SIZE_XI_IMG_V1)
        return XI_INVALID_HANDLE;

    if (pXiImg == nullptr)
        return XI_INVALID_ARG;

    // get the XI_IMG contents
    XI_IMG xiImg;
    memcpy(&xiImg, pResponseBuffer, SIZE_XI_IMG_V1);
    if (xiImg.size > SIZE_XI_IMG_V1)
        memcpy(&xiImg, pResponseBuffer, xiImg.size);

    if (uiResponseSize < xiImg.size + xiImg.bp_size)
        return XI_IO_WRONG;     // response should contain header and data

    // get the data itself
    void *pOrigBpPointer = pXiImg->bp;
    DWORD uiOrigBpSize = pXiImg->bp_size;
    *pXiImg = xiImg;
    pXiImg->bp = pOrigBpPointer;

    // we always have BP_SAFE buffer policy. So it means the pXiImg count have valid data pointer
    if (pXiImg->bp == nullptr || uiOrigBpSize < xiImg.bp_size)
        return XI_WRONG_PARAM_SIZE;

    memcpy(pXiImg->bp, (const char*)pResponseBuffer + xiImg.size, xiImg.bp_size);
    return XI_OK;
}

//---------------------------------------------------------
int CxRemoteCameraClient::setParamInt(const char *szParam, const int iVal)
{
    if (strcmp(szParam, XI_PRM_BUFFER_POLICY) == 0 && iVal != XI_BP_SAFE)
        return XI_NOT_SUPPORTED;

    XI_RETURN xiRet = XI_INVALID_HANDLE;
    std::string sErrorMsg;
    if (!sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_CAMERA_PARAM_INT}, {"name", szParam}, {"value", std::to_string(iVal)}}), &sErrorMsg, &xiRet))
    {
        if (m_bLogAllErrors)
            CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::setParamInt() ") + sErrorMsg);
        return xiRet;
    }
    return xiRet;
}

//---------------------------------------------------------
int CxRemoteCameraClient::setParamInt64(const char *szParam, const uint64_t uiVal)
{
    XI_RETURN xiRet = XI_INVALID_HANDLE;
    std::string sErrorMsg;
    if (!sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_CAMERA_PARAM_INT64}, {"name", szParam}, {"value", std::to_string(uiVal)}}), &sErrorMsg, &xiRet))
    {
        if (m_bLogAllErrors)
            CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::setParamInt64() ") + sErrorMsg);
        return xiRet;
    }
    return xiRet;
}

//---------------------------------------------------------
int CxRemoteCameraClient::setParamFloat(const char *szParam, const float fVal)
{
    XI_RETURN xiRet = XI_INVALID_HANDLE;
    std::string sErrorMsg;
    if (!sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_CAMERA_PARAM_FLOAT}, {"name", szParam}, {"value", std::to_string(fVal)}}), &sErrorMsg, &xiRet))
    {
        if (m_bLogAllErrors)
            CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::setParamFloat() ") + sErrorMsg);
        return xiRet;
    }
    return xiRet;
}

//---------------------------------------------------------
int CxRemoteCameraClient::setParamString(const char *szParam, const char *szVal, uint32_t uiValSize)
{
    XI_RETURN xiRet = XI_INVALID_HANDLE;
    std::string sErrorMsg;
    if (!sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_CAMERA_PARAM_STRING}, {"name", szParam}, {"value", std::string(szVal, uiValSize)}}), &sErrorMsg, &xiRet))
    {
        if (m_bLogAllErrors)
            CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::setParamString() ") + sErrorMsg);
        return xiRet;
    }
    return xiRet;
}

//---------------------------------------------------------
int CxRemoteCameraClient::getParamInt(const char *szParam, int &iVal)
{
    if (strcmp(szParam, XI_PRM_BUFFER_POLICY) == 0)
    {
        iVal = XI_BP_SAFE;
        return XI_OK;
    }

    XI_RETURN xiRet = XI_INVALID_HANDLE;
    std::string sErrorMsg;
    if (!sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_CAMERA_PARAM_INT}, {"name", szParam}}), &sErrorMsg, &xiRet))
    {
        if (m_bLogAllErrors)
            CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::getParamInt() ") + sErrorMsg);
        return xiRet;
    }
    try {
        iVal = std::stoi(sErrorMsg);
    } catch (...) {
        xiRet = XI_INVALID_ARG;
    }
    return xiRet;
}

//---------------------------------------------------------
int CxRemoteCameraClient::getParamInt64(const char *szParam, uint64_t &uiVal)
{
    XI_RETURN xiRet = XI_INVALID_HANDLE;
    std::string sErrorMsg;
    if (!sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_CAMERA_PARAM_INT64}, {"name", szParam}}), &sErrorMsg, &xiRet))
    {
        if (m_bLogAllErrors)
            CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::getParamInt64() ") + sErrorMsg);
        return xiRet;
    }
    try {
        uiVal = std::stoull(sErrorMsg);
    } catch (...) {
        xiRet = XI_INVALID_ARG;
    }
    return xiRet;
}

//---------------------------------------------------------
int CxRemoteCameraClient::getParamFloat(const char *szParam, float &fVal)
{
    XI_RETURN xiRet = XI_INVALID_HANDLE;
    std::string sErrorMsg;
    if (!sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_CAMERA_PARAM_FLOAT}, {"name", szParam}}), &sErrorMsg, &xiRet))
    {
        if (m_bLogAllErrors)
            CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::getParamFloat() ") + sErrorMsg);
        return xiRet;
    }
    try {
        fVal = std::stof(sErrorMsg);
    } catch (...) {
        xiRet = XI_INVALID_ARG;
    }
    return xiRet;
}

//---------------------------------------------------------
int CxRemoteCameraClient::getParamString(const char *szParam, std::string &sVal)
{
    XI_RETURN xiRet = XI_INVALID_HANDLE;
    std::string sErrorMsg;
    if (!sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_CAMERA_PARAM_STRING}, {"name", szParam}}), &sErrorMsg, &xiRet))
    {
        if (m_bLogAllErrors)
            CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::getParamString() ") + sErrorMsg);
        return xiRet;
    }
    sVal = sErrorMsg;
    return xiRet;
}

//---------------------------------------------------------
bool CxRemoteCameraClient::setLoopRecorderStorage(const std::string &sFilePaths)
{
    std::string sErrorMsg;
    if (!sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_DATA_STORAGE}, {"folders", sFilePaths}}), &sErrorMsg))
    {
        CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::setLoopRecorderStorage() ") + sErrorMsg);
        return false;
    }
    return true;
}

//---------------------------------------------------------
bool CxRemoteCameraClient::loopRecordStorageCapacity(uint64_t &uiSpace)
{
    XI_RETURN xiRet = XI_INVALID_HANDLE;
    std::string sErrorMsg;
    if (!sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_STORAGE_CAPACITY}}), &sErrorMsg))
    {
        CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::loopRecordStorageCapacity() ") + sErrorMsg);
        return false;
    }
    try {
        uiSpace = std::stoull(sErrorMsg);
    } catch (...) {
        return false;
    }
    return true;
}

//---------------------------------------------------------
bool CxRemoteCameraClient::loopRecordGoReady(int iFrameCount)
{
    std::string sErrorMsg;
    if (!sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_GO_READY}, {"frameCount", std::to_string(iFrameCount)}}), &sErrorMsg))
    {
        CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::prepareForLoopRecordStart() ") + sErrorMsg);
        return false;
    }
    return true;
}

//---------------------------------------------------------
bool CxRemoteCameraClient::loopRecordCancel()
{
    return sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_CANCEL_RECORDING}}));
}

//---------------------------------------------------------
bool CxRemoteCameraClient::loopRecordedFrameData(int iIdx, void *pDstBuffer, uint64_t uiDstBufferSize)
{
    XI_RETURN xiRet = XI_INVALID_HANDLE;
    CxCameraClient *pHttpClient = m_pHttpClientForImageStream;
    if (pHttpClient == nullptr)
        pHttpClient = m_pHttpClient;

    if (pHttpClient == nullptr || pDstBuffer == nullptr)
        return xiRet;
    std::string sResponse, sResponseContentType;
    if (!pHttpClient->sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_RECORDED_FRAMEDATA}, {"idx", std::to_string(iIdx)}}), &sResponse, &sResponseContentType))
    {
        CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::loopRecordedFrameData() ") + "Request failed");
        return xiRet;
    }

    if (sResponseContentType == "text/xml")
    {
        // is error
        bool bResponseOk;
        std::string sErrorMsg;
        xiRecorderApi::parseXmlResponse(sResponse, bResponseOk, sErrorMsg, &xiRet);
        CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::loopRecordedFrameData() ") + sErrorMsg);
        return xiRet;
    }
    if (sResponseContentType == "application/octet-stream")
    {
        if (sResponse.length() > uiDstBufferSize)
        {
            CxRemoteCameraClient::logError(std::string("CxRemoteCameraClient::loopRecordedFrameData() ") + "insufficient memory in dst buffer of " + std::to_string(uiDstBufferSize) + " bytes, got " + std::to_string(sResponse.length()));
            return false;
        }
        memcpy(pDstBuffer, sResponse.data(), sResponse.length());
        return true;
    }
    return false;
}

//---------------------------------------------------------
bool CxRemoteCameraClient::loopRecordReleaseDataBuffers(bool bRemoveDataFiles)
{
    return sendRequestToServer(m_pHttpClient, xiRecorderApi::formatRequest({{"cmd", XI_CMD_RELEASE_DATA_BUFFERS}, {"deleteRecorderDataFiles", bRemoveDataFiles ? "true" : "false"}}));
}

//---------------------------------------------------------
bool CxRemoteCameraClient::userImageProcessorCommand(TiXmlElement *pRequest, std::string *psResponse, std::string *psResponseContentType)
{
    if (m_pHttpClient == nullptr)
        return false;

    pRequest->SetValue(XI_SRV_TAG_REQUEST);
    pRequest->SetAttribute("cmd", XI_CMD_USER_IMG_PROCESSING_CMD);
    std::ostringstream xmlStream;
    xmlStream << *pRequest;
    return m_pHttpClient->sendRequest(xmlStream.str(), psResponse, psResponseContentType);
}

//---------------------------------------------------------
bool CxRemoteCameraClient::sendRawData(const std::string& sData, std::string* psResponse, std::string* psResponseContentType)
{
    if (m_pHttpClient == nullptr)
        return false;

    return m_pHttpClient->postRawData(sData, psResponse, psResponseContentType);
}
