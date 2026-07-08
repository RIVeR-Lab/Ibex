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

#ifndef REMOTECAMERACLIENT_H
#define REMOTECAMERACLIENT_H

#include <list>
#include <string>
#include <xiApi.h>

class CxCameraClient;
class TiXmlDocument;
class TiXmlElement;

typedef void (*TxRemoteCameraLogCallback)(const std::string &);

/**
 * \brief Class representing the remotely connected camera for the client application
 *
 * This class is not used by MultiRecorder.exe, but implements the communication with the camera on
 * server via MultiRecorder API. It has the interface similar to xiApi with functions having
 * blocking remote execution: i.e. calling CxRemoteCameraClient::getParamInt really returns
 * the value and return code from the camera on remote computer.
 */
class CxRemoteCameraClient
{
public:
    struct SxRemoteCameraDesc
    {
        std::string m_sIPAddress;
        int m_iPort;
        std::string m_sName;
        std::string m_sSerial;
    };

    /**
    \brief Low-level function to send request to server and receive the response contents

    This function is used by other higher-level functions in this class (openCamera, getParam, setParam, startAcquisition, ...)

    @param[in] pHttpClient              connection to use (control, image stream)
    @param[in] sRequest                 XML string with request commad. You may use xiRecorderApi::formatRequest to get this string.
    @param[out] psResponseDetails       string with the response contents (without XML tags), e.g. value of xiGetParam
    @param[out] pxiRet                  optional parameter that receives parsed XI_RESULT from xiApi functions
    @return true when the request was processed on server and responce received
    */
    static bool sendRequestToServer(CxCameraClient *pHttpClient, const std::string &sRequest, std::string *psResponseDetails = nullptr, XI_RETURN *pxiRet = nullptr);

    /**
    \brief Low-level function to send request to server and receive the response parsed into XML structures in TinyXml library

    This function is used by other higher-level functions in this class (listCamerasAtAddress, ...)

    @param[in] pHttpClient              connection to use (control, image stream)
    @param[in] sRequest                 XML string with request commad. You may use xiRecorderApi::formatRequest to get this string.
    @param[in] pResponseToXmlDoc        pre-created XML document object that is filled inside this function
    @param[int,out] pRootResponseElement  root XML element of the response, filled as a result
    @param[out] pxiRet                  optional parameter that receives parsed XI_RESULT from xiApi functions
    @return true when the request was processed on server and responce received
    */
    static bool sendRequestToServerResXml(CxCameraClient *pHttpClient, const std::string &sRequest, TiXmlDocument *pResponseToXmlDoc, TiXmlElement *&pRootResponseElement, XI_RETURN *pxiRet = nullptr);

    /**
    \brief Returns the list of cameras connected to computer where CameraServer (MultiRecorder) is running.

    @param[in] sIPAddress   server hostname or IP address (e.g. "localhost")
    @param[in] iPort        server port number
    @param[out] sErrorCode  error message returned from the server
    @return list of cameras, with model names and serial numbers
    */
    static std::list<SxRemoteCameraDesc> listCamerasAtAddress(const std::string &sIPAddress, int iPort, std::string &sErrorCode);

    /**
    \brief Establishing connection to camera identified with model name and serial number connected to server computer at specified address.

    MultiRecorder server has to run at the target computer, listening on given port number

    @param[in] sIPAddress   server hostname or IP address (e.g. "localhost")
    @param[in] iPort        server port number
    @param[in] szDeviceName camera device name to open
    @param[in] szSerial     camera serial number to open
    @return pointer to remote camera object
    */
    static CxRemoteCameraClient* openCamera(const std::string &sIPAddress, int iPort, const std::string &sDeviceName, const std::string &sSerial, bool bLogErrors);

    // instances
protected:
    bool m_bLogAllErrors;       //!< log all errors, even in all xiSetParam (default false)

    CxRemoteCameraClient();     //!< protected constructor: create new class instance using CxRemoteCameraClient::openCamera()

    // communication
public:
    SxRemoteCameraDesc m_aDesc;                     //!< address of the currently opened camera server (and camera)
    CxCameraClient *m_pHttpClient;                  //!< http client with its socket to control the camera, set parameters, etc
    CxCameraClient *m_pHttpClientForImageStream;    //!< http client with its socket just for xiGetImage, not to block the other communication

public:
    ~CxRemoteCameraClient();
    void remoteConnectionParameters(std::string &sHostName, int &iPort) const;  //!< returns address (hostname) and port this client is connected to

    // function to mimic classic camera / xiApi commands implementation
public:
    int closeDevice();          //!< alternative to xiCloseDevice in xiApi, returns XI_RETURN
    int startAcquisition();     //!< alternative to xiStartAcquisition in xiApi, returns XI_RETURN
    int stopAcquisition();      //!< alternative to xiStopAcquisition in xiApi, returns XI_RETURN
    int getImage(unsigned uiTimeout, XI_IMG *pXiImg);       //!< alternative to xiGetImage in xiApi, returns XI_RETURN
    int setParamInt(const char *szParam, const int iVal);   //!< alternative to xiSetParamInt in xiApi, returns XI_RETURN
    int setParamInt64(const char *szParam, const uint64_t uiVal);   //!< alternative to xiSetParamInt64 in xiApi, returns XI_RETURN
    int setParamFloat(const char *szParam, const float fVal);       //!< alternative to xiSetParamFloat in xiApi, returns XI_RETURN
    int setParamString(const char *szParam, const char *szVal, uint32_t uiValSize);   //!< alternative to xiSetParamString in xiApi, returns XI_RETURN
    int getParamInt(const char *szParam, int &iVal);            //!< alternative to xiGetParamInt in xiApi, returns XI_RETURN
    int getParamInt64(const char *szParam, uint64_t &uiVal);    //!< alternative to xiGetParamInt64 in xiApi, returns XI_RETURN
    int getParamFloat(const char *szParam, float &fVal);        //!< alternative to xiGetParamFloat in xiApi, returns XI_RETURN
    int getParamString(const char *szParam, std::string &sVal); //!< alternative to xiGetParamString in xiApi, returns XI_RETURN

    // a bit lower level processing
public:
    bool getImageForResponse(unsigned uiTimeout, std::string &sResponse, std::string &sResponseContentType); //!< calls xiGetImage in xiApi and outputs the server response, returns XI_RETURN
    bool userImageProcessorCommand(TiXmlElement *pRequest, std::string *psResponse = nullptr, std::string *psResponseContentType = nullptr);  //!< to send commands to CxUserImageProcessor, and outputs the server response
    static int fillXiImgFromResponse(XI_IMG *pXiImg, const std::string &sResponse);        //!< fill pre-allocated XI_IMG with response in application/octet-stream content type
    static int fillXiImgFromResponse(XI_IMG *pXiImg, const void* pResponseBuffer, uint64_t uiResponseSize);
    bool sendRawData(const std::string& sData, std::string* psResponse, std::string* psResponseContentType);

    // functions for LoopRecording
public:
    bool setLoopRecorderStorage(const std::string &sFilePaths);
    bool loopRecordStorageCapacity(uint64_t &uiSpace);
    bool loopRecordGoReady(int iFrameCount);
    bool loopRecordCancel();
    bool loopRecordedFrameData(int iIdx, void *pDstBuffer, uint64_t uiDstBufferSize);   // just data in transport format
    bool loopRecordReleaseDataBuffers(bool bRemoveDataFiles);

    // error logging
public:
    static void setErrorLogFunction(TxRemoteCameraLogCallback callback);    //!< sets capplication allback to let log errors in client-server communication
    static void logError(const std::string &msg);                           //!< sends error message to log (used internally in CxRemoteCameraClient)
};

#endif // REMOTECAMERACLIENT_H
