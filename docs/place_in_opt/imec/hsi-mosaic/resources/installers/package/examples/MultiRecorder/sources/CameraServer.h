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

#ifndef CXCAMERASERVER_H
#define CXCAMERASERVER_H

#include "CameraControl.h"

namespace httplib
{
class Server;
}

/**
 * \brief Helper class that runs http server, capable of controlling one XIMEA camera
 *
 * Uses cpp-httplib library.
 */
class CxCameraServer
{
public:
    CxCameraServer();
    ~CxCameraServer();

    void setUserImageProcessor(CxUserImageProcessor *pProcessor);

    /**
    \brief Starts new server at hostname (this computer)

    @param[in] sIPAdress                IP address or hostname of this computer
    @param[in] iPort                    port to bind to, and listen at. When 0 passed as iPort, the server selects some free port, and returns its number in piBoundToPort
    @param[in] bVerboseServerLog        log all communication with all clients
    @param[in] bStartAsync              use false to start this class as server (execution of the main application thread is blocked here).
    @param[out] piBoundToPort           optional pointer to int variable that is filled with the actual port number the server is bound to. May be helful when passing 0 for iPort (automatic)
    @return true when the request was processed on server and responce received
    */
    bool startAndListenAt(const std::string &sIPAdress, int32_t iPort, bool bVerboseServerLog, bool bStartAsync, int *piBoundToPort = nullptr); // fills piBoundToPort with the actual port number the server is bound to. May be helful when passing 0 for iPort (automatic)
    void stopServer();

protected:
    CxCameraControl m_aCamera;      //!< Main class controlling the camera, executing the commands
    httplib::Server *m_pHttpServer;
    std::thread *m_pServerThread;
};

#endif // CXCAMERASERVER_H
