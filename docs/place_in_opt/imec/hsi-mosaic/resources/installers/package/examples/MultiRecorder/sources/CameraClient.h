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

#ifndef CXCAMERACLIENT_H
#define CXCAMERACLIENT_H

#include <string>

namespace httplib
{
class Client;
}

/**
 * \brief Helper class that keeps one http connection (socket) with the CxCameraServer on remote computer.
 *
 * Uses cpp-httplib library.
 */

class CxCameraClient
{
public:
    CxCameraClient(const std::string &sIPAdress, int32_t iPort);
    ~CxCameraClient();

    bool isConnectedToServer(std::string *psErrorCode = nullptr);

    /**
    \brief Low-level function to send request to server and receive the response contents

    This function is used by other functions in other classes (CxRemoteCameraClient, CxRecordingTask, ...)

    @param[in] sXmlRequest              XML string with request commad. You may use xiRecorderApi::formatRequest to get this string.
    @param[out] psResponse              string data (byte buffer) with the server response, optional
    @param[out] psResponseContentType   content-type of the response. Usually "text/xml" for common functions or "application/octet-stream" for images
    @return true when the request was processed on server and responce received
    */
    bool sendRequest(const std::string &sXmlRequest, std::string *psResponse = nullptr, std::string *psResponseContentType = nullptr);    // returns the response from server
    bool postRawData(const std::string &sData, std::string *psResponse, std::string *psResponseContentType); // for sending raw data using POST method
protected:
    httplib::Client *m_pHttpClient;
};

#endif // CXCAMERACLIENT_H
