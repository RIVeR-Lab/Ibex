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

#include "httplib/httplib.h"
#include "CameraServer.h"
#include "RecorderApi.h"

#include <thread>

//-------------------------------------------------------------------------------------------------------------------
CxCameraServer::CxCameraServer()
{
    m_pHttpServer = new httplib::Server;
    m_pServerThread = nullptr;
}

//-------------------------------------------------------------------------------------------------------------------
CxCameraServer::~CxCameraServer()
{
    stopServer();

    if (m_pHttpServer != nullptr)
        delete m_pHttpServer;
}

//-------------------------------------------------------------------------------------------------------------------
void CxCameraServer::setUserImageProcessor(CxUserImageProcessor *pProcessor)
{
    m_aCamera.m_pUserImageProcessor = pProcessor;
}

//-------------------------------------------------------------------------------------------------------------------
bool CxCameraServer::startAndListenAt(const std::string &sIPAdress, int32_t iPort, bool bVerboseServerLog, bool bStartAsync, int *piBoundToPort)
{
    if (m_pHttpServer == nullptr)
        return false;

    m_aCamera.setVerboseOutputLog(bVerboseServerLog);

    m_pHttpServer->set_pre_routing_handler([&](const auto& req, auto& res){
        try {
            m_aCamera.printOutputMessage(std::string("CameraServer received: ") + req.path, true);
            if (req.path.find("/post-data") != std::string::npos)
                return httplib::Server::HandlerResponse::Unhandled;
            std::string sCameraResponse, sResponseContentType;
            m_aCamera.processRemoteRequest(req.path, &sCameraResponse, &sResponseContentType);
            if (!sCameraResponse.empty())
            {
                if (sResponseContentType == DX_IMG_BINARY_MIME)
                    m_aCamera.printOutputMessage(std::string("CameraServer responding with data of size: ") + std::to_string(sCameraResponse.length()) + "; status: " + m_aCamera.statusString(), true);
                else
                    m_aCamera.printOutputMessage(std::string("CameraServer responding: ") + sCameraResponse + "; status: " + m_aCamera.statusString(), true);
                res.set_content(std::move(sCameraResponse), sResponseContentType);
                res.status = 200;
                return httplib::Server::HandlerResponse::Handled;
            }
        } catch (...) {
        }
        return httplib::Server::HandlerResponse::Unhandled;
    });

    m_pHttpServer->Post("/post-data", [&](const auto& req, auto& res) {
        try {
            m_aCamera.printOutputMessage(std::string("CameraServer received raw data of size: ") + std::to_string(req.body.length()), true);
            std::string sResponse, sResponseContentType;
            m_aCamera.processRawData(req.body, &sResponse, &sResponseContentType);
            res.set_content(std::move(sResponse), sResponseContentType);
            res.status = 200;
            return httplib::Server::HandlerResponse::Handled;
        }
        catch (...) {
        }
        return httplib::Server::HandlerResponse::Unhandled;
        });

    bool bRet;
    if (iPort > 0)
        bRet = m_pHttpServer->bind_to_port(sIPAdress, iPort);
    else
    {
        iPort = m_pHttpServer->bind_to_any_port(sIPAdress);
        bRet = (iPort > 0);
    }

    if (piBoundToPort != nullptr)
        *piBoundToPort = iPort;

    if (bRet)
    {
        std::string sMsg = std::string("CameraServer started, listening at ") + sIPAdress + ":" + std::to_string(iPort);
        m_aCamera.printOutputMessage(sMsg, false);
        if (!bStartAsync && xiRecorderApi::hasLogFile())
            xiRecorderApi::outputStringToConsole(sMsg); // when running server only, output the hostname and port to console too
    }
    else
    {
        std::string sMsg = std::string("CameraServer not started, requested to run at ") + sIPAdress + ":" + std::to_string(iPort);
        m_aCamera.printOutputMessage(sMsg, false);
        if (!bStartAsync && xiRecorderApi::hasLogFile())
            xiRecorderApi::outputStringToConsole(sMsg);
        return false;
    }
    if (bVerboseServerLog || !bStartAsync)
    {
        std::string eError;
        std::vector<SxCameraDeviceInfo> vecDeviceInfo;
        if (!m_aCamera.listConnectedDevices(vecDeviceInfo, &eError))
            m_aCamera.printOutputMessage(eError, false);
        else
        {
            m_aCamera.printOutputMessage("List of connected devices:", false);
            if (vecDeviceInfo.empty())
                m_aCamera.printOutputMessage("none", false);
            for (size_t idx = 0; idx < vecDeviceInfo.size(); idx++)
            {
                const auto &aInfo = vecDeviceInfo[idx];
                m_aCamera.printOutputMessage(std::string("idx: ") + std::to_string(idx)
                    + ", name: " + aInfo.sDeviceName + ", type: " + aInfo.sDeviceType
                    + ", sn: " + aInfo.sSN + ", instPath: " + aInfo.sInstPath, false);
            }
        }
    }

    if (bStartAsync)
        //m_pServerThread = new std::thread(&httplib::Server::listen, m_pHttpServer, sIPAdress, iPort, 0);
        m_pServerThread = new std::thread(&httplib::Server::listen_after_bind, m_pHttpServer);
    else
        m_pHttpServer->listen_after_bind();     // blocking call
    return true;
}

//-------------------------------------------------------------------------------------------------------------------
void CxCameraServer::stopServer()
{
    if (m_pHttpServer != nullptr)
    {
        m_aCamera.printOutputMessage("CameraServer stopping", true);
        m_pHttpServer->stop();

        if (m_pServerThread != nullptr)
        {
            m_pServerThread->join();
            delete m_pServerThread;
            m_pServerThread = nullptr;
        }
        m_aCamera.printOutputMessage("CameraServer stopped", true);
    }
}
