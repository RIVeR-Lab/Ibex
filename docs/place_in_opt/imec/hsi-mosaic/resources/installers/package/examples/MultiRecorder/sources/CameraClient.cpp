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
#include "CameraClient.h"
#include "RecorderApi.h"

//-------------------------------------------------------------------------------------------------------------------
CxCameraClient::CxCameraClient(const std::string &sIPAdress, int32_t iPort)
{
    m_pHttpClient = new httplib::Client(sIPAdress, iPort);
    m_pHttpClient->set_keep_alive(true);
}

//-------------------------------------------------------------------------------------------------------------------
CxCameraClient::~CxCameraClient()
{
    if (m_pHttpClient != nullptr)
        delete m_pHttpClient;
}

//-------------------------------------------------------------------------------------------------------------------
bool CxCameraClient::isConnectedToServer(std::string *psErrorCode)
{
    if (m_pHttpClient == nullptr)
        return false;

    auto res = m_pHttpClient->Get(xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_STATUS}}));
    if (res.error() == httplib::Error::Success)
        return true;

    if (psErrorCode != nullptr)
        *psErrorCode = httplib::to_string(res.error());
    return false;
}

//-------------------------------------------------------------------------------------------------------------------
bool CxCameraClient::sendRequest(const std::string &sXmlRequest, std::string *psResponse, std::string *psResponseContentType)
{
    if (m_pHttpClient == nullptr)
        return false;
    auto res = m_pHttpClient->Get(sXmlRequest);
    if (res.error() == httplib::Error::Success)
    {
        if (psResponse != nullptr)
            *psResponse = res->body;
        if (psResponseContentType != nullptr)
            *psResponseContentType = res->get_header_value("Content-type");
        return true;
    }
    else
    {
        xiRecorderApi::outputStringToStdErr(std::string("httplib Error: ") + httplib::to_string(res.error()));
        return false;
    }
}

//-------------------------------------------------------------------------------------------------------------------
bool CxCameraClient::postRawData(const std::string &sData, std::string *psResponse, std::string *psResponseContentType)
{
    if (m_pHttpClient == nullptr)
        return false;
    auto res = m_pHttpClient->Post("/post-data", sData, "text/plain");
    if (res.error() == httplib::Error::Success)
    {
        if (psResponse != nullptr)
            *psResponse = res->body;
        if (psResponseContentType != nullptr)
            *psResponseContentType = res->get_header_value("Content-type");
        return true;
    }
    else
    {
        xiRecorderApi::outputStringToStdErr(std::string("httplib Error: ") + httplib::to_string(res.error()));
        return false;
    }
}
