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

#ifndef USERIMAGEPROCESSOR_H
#define USERIMAGEPROCESSOR_H

#include <string>
#include <xiApi.h>

class TiXmlElement;

/**
 * \brief Base class for User Image Processing Applications
 *
 * Create your derived class and set it to CxCameraControl instance to process the images on server
 * computer, before sending them to client computer.
 */
class CxUserImageProcessor
{
public:
    CxUserImageProcessor();
    virtual ~CxUserImageProcessor();

    /**
    \brief Some custom command (maybe to set some internal parameter value) passed from the client application

    The command is passed using cmd=XI_CMD_USER_IMG_PROCESSING_CMD in server request. This entire
    request XML element is passed to this function, so XML attributes or child elements can be used
    to transfer additional command parameters.

    When sResponseContentType is "text/plain" or "text/xml", the final server response is put inside <response> tag.

    @param[in] pElRequst                some content received from client app
    @param[out] sResponse               anything the client app expects (data buffer, text, or error message)
    @param[out] sResponseContentType    encoding used to compile the response data
    @return true when the parameter processed
    */
    virtual bool command(TiXmlElement *pElRequest, std::string &sResponse, std::string &sResponseContentType);

    /**
    \brief Processes the new image from camera producing some response.

    The response can be either image (use same response as CxCameraControl::makeXiImgResponse to encode XI_IMG),
    or some text or XML string. Be sure to set the sResponseContentType.

    @param[in] pXiImg                   image got from camera via xiGetImage
    @param[out] sResponse               anything the client app expects (data buffer, text, or error message)
    @param[out] sResponseContentType    encoding used to compile the response data. Is usually "application/octet-stream" for images, or "text/plain" for errors
    @return true when the image processed
    */
    virtual bool processImage(XI_IMG *pXiImg, std::string &sResponse, std::string &sResponseContentType);

    /**
    \brief Server passes handle to opened camera to CxUserImageProcessor class.

    When processing is done on each xiGetImage, it is not needed to control the camera from this class.
    But sometimes you may want to control the camera after receiving some special command. So there
    is an alternative way, and there is a camera handle for you to e.g. start the acquisition,
    implement the image loop, etc. without letting this flow through CxCameraControl.

    @param[in] hDevice                  handle to device
    */
    virtual void setCameraHandle(HANDLE hDevice);

    /**
    \brief Callback for processing raw data.

    Used to accept large amount of data sent via POST method. Doesn't use XML format and expects raw bytes.

    @param[in] sData                  data
    */
    virtual void processRawData(const std::string &sData, std::string *psResponse, std::string *psResponseContentType);
};

#endif // USERIMAGEPROCESSOR_H
