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

#ifndef RECORDERAPI_H
#define RECORDERAPI_H

//-------------------------------------------------------------------------------------------------------------------
// basic tags
#define  XI_SRV_TAG_REQUEST             "request"               // XML tag name for request, i.e. <request cmd... />
#define  XI_SRV_TAG_RESPONSE            "response"              // XML tag name for response to request sent to client, i.e. <response code=ok>...</response>

//-------------------------------------------------------------------------------------------------------------------
// request commands, (use XI_SRV_TAG_REQUEST, XI_SRV_TAG_RESPONSE for XML element names)

#define  XI_CMD_OPEN_CAMERA             "openCamera"            // <request cmd=openCamera idx=0 /> or <request cmd=openCamera instPath="PATH" /> or <request cmd=openCamera sn="SN1234" /> or <request cmd=openCamera userID="USER_ID" /> or <request cmd=openCamera locPath="PATH" />
#define  XI_CMD_CLOSE_CAMERA            "closeCamera"           // <request cmd=closeCamera />
#define  XI_CMD_GET_CONNECTED_DEVICES   "getConnectedDevices"   // <request cmd=getConnectedDevices />  -> returns <response code=ok><device idx=0 sn="SN1234" instPath="PATH" deviceName="NAME" deviceType="type" /><device idx=1 .../><device idx=2 .../></response>
#define  XI_CMD_GET_NUMBER_DEVICES      "getNumberDevices"      // <request cmd=getNumberDevices />     -> returns <response code=ok>2</response>  (number of connected devices to server computer)
#define  XI_CMD_GET_DEVICE_INFO         "getDeviceInfo"         // <request cmd=getDeviceInfo idx=0 name=XI_PRM_DEVICE_SN />  -> returns <response code=ok>SN1234</response>
#define  XI_CMD_APPLY_XICAMERA_FILE     "applyXiCameraFile"     // <request cmd=applyXiCameraFile name="c:/test/settings.xicamera" />
#define  XI_CMD_SET_CAMERA_PARAM        "setCameraParam"        // <request cmd=setCameraParam name=exp value=123 /> (value type is determined automatically)
#define  XI_CMD_SET_CAMERA_PARAM_INT    "setCameraParamInt"     // <request cmd=setCameraParamInt name=exp value=123 />     -> returns <response code=ok xiret=0></response>
#define  XI_CMD_SET_CAMERA_PARAM_INT64  "setCameraParamInt64"   // <request cmd=setCameraParamInt64 name=exp value=123 />   -> returns <response code=ok xiret=0></response>
#define  XI_CMD_SET_CAMERA_PARAM_FLOAT  "setCameraParamFloat"   // <request cmd=setCameraParamFloat name=exp value=123.0 /> -> returns <response code=ok xiret=0></response>
#define  XI_CMD_SET_CAMERA_PARAM_STRING "setCameraParamString"  // <request cmd=setCameraParamString name=exp value=text /> -> returns <response code=ok xiret=0></response>
#define  XI_CMD_GET_CAMERA_PARAM        "getCameraParam"        // <request cmd=getCameraParam name=exp />       -> returns <response code=ok xiret=0 valuetype=xiTypeInteger>123</response> (value type is determined automatically and is returned in valuetype attribute)
#define  XI_CMD_GET_CAMERA_PARAM_INT    "getCameraParamInt"     // <request cmd=getCameraParamInt name=exp />    -> returns <response code=ok xiret=0>123</response>
#define  XI_CMD_GET_CAMERA_PARAM_INT64  "getCameraParamInt64"   // <request cmd=getCameraParamInt64 name=exp />  -> returns <response code=ok xiret=0>123</response>
#define  XI_CMD_GET_CAMERA_PARAM_FLOAT  "getCameraParamFloat"   // <request cmd=getCameraParamFloat name=exp />  -> returns <response code=ok xiret=0>123.0</response>
#define  XI_CMD_GET_CAMERA_PARAM_STRING "getCameraParamString"  // <request cmd=getCameraParamString name=exp /> -> returns <response code=ok xiret=0>text</response>
#define  XI_CMD_GET_CAMERA_IMAGE        "getCameraImage"        // <request cmd=getCameraImage timeout=2000 encoding=application/octet-stream /> -> returns data with encoding in http header content-type, or error with usual <response ... > text/xml content-type (calls xiGetImage)
#define  XI_CMD_SET_DATA_STORAGE        "setDataStorage"        // <request cmd=setDataStorage folders="c:/test/f1|d:/test/f2" />
#define  XI_CMD_SET_LIVE_IMG_PREVIEW    "setLiveImagePreview"   // <request cmd=setLiveImagePreview set="true/false" />
#define  XI_CMD_GO_READY                "goReady"               // <request cmd=goReady frameCount=10 />
#define  XI_CMD_START_ACQ               "startAcq"              // <request cmd=startAcq />
#define  XI_CMD_CANCEL_RECORDING        "cancelRecording"       // <request cmd=cancelRecording />
#define  XI_CMD_STOP_ACQ                "stopAcq"               // <request cmd=stopAcq />
#define  XI_CMD_GET_STATUS              "getStatus"             // <request cmd=getStatus />            -> returns <response code=ok>XI_SRV_STATUS_X</response>
#define  XI_CMD_GET_IMAGES_COUNT        "getImagesCount"        // <request cmd=getImagesCount />       -> returns <response code=ok><counter name="done">8</counter><counter name="total">10</counter>...</response>  (number of captured images at the moment, counter names are XI_SRV_COUNTER_xxx)
#define  XI_CMD_GET_STORAGE_CAPACITY    "getStorageCapacity"    // <request cmd=getStorageCapacity />   -> returns <response code=ok>1200000000</response>  (size in bytes, uint64)
#define  XI_CMD_GET_RECORDED_FRAMEDATA  "getRecordedFrameData"  // <request cmd=getRecordedFrameData idx=0 />   -> returns recorded image in transport format with application/octet-stream encoding
#define  XI_CMD_SAVE_SESSION            "saveSession"           // <request cmd=saveSession file="c:/test/sessionname" imageFormat="one of XI_IMG_FORMAT"/>
#define  XI_CMD_EXPORT_TIFF_FILES       "exportTiffFiles"       // <request cmd=exportTiffFiles folder="c:/test/recorded" imageFormat="one of XI_IMG_FORMAT" [tiffEnableStoring16bit="true" tiffScaleDataToFull16bitRange="false"]/>
#define  XI_CMD_CANCEL_EXPORTING        "cancelExporting"       // <request cmd=cancelExporting />
#define  XI_CMD_RELEASE_DATA_BUFFERS    "releaseDataBuffers"    // <request cmd=releaseDataBuffers deleteRecorderDataFiles="true/false" />

#define  XI_CMD_START_STORAGE_SPEED_TEST "startStorageSpeedTest"// <request cmd=startStorageSpeedTest duration=10000 bufferSize=0 frameLoopCount=1000 />    (creates file with 'frameLoopCount'*'bufferSize' bytes, and starts writing buffers with 'bufferSize' bytes in loop for 'duration' seconds)
#define  XI_CMD_STOP_STORAGE_SPEED_TEST  "stopStorageSpeedTest" // <request cmd=stopStorageSpeedTest />  (cancel test even before duration elapsed)

#define  XI_CMD_USER_IMG_PROCESSING_CMD "uipCommand"            // <request cmd=uipCommand ... some attributes...>..some XML content</request> -> returns some response

//-------------------------------------------------------------------------------------------------------------------
// server response codes returned by all commands, <response code=xxx>details</response> (use XI_SRV_TAG_RESPONSE)

#define  XI_SRV_RESPONSE_CODE_OK        "ok"
#define  XI_SRV_RESPONSE_CODE_ERROR     "error"

//-------------------------------------------------------------------------------------------------------------------
// server status codes returned by XI_CMD_GET_STATUS

#define  XI_SRV_STATUS_INIT                 "Initialization"        // startup, prepared to open camera and set parameters
#define  XI_SRV_STATUS_INIT_ERROR           "InitializationError"   // failed to init storage or open camera
#define  XI_SRV_STATUS_PREPARED_FOR_ACQ     "PreparedForAcq"        // after XI_CMD_GOREADY
#define  XI_SRV_STATUS_RECORDING            "Recording"             // after XI_CMD_STARTACQ, while server is recording images
#define  XI_SRV_STATUS_RECORDING_FINISHED   "RecordingFinished"     // after finished recording images
#define  XI_SRV_STATUS_EXPORTING            "ExportingFiles"        // during exporting files
#define  XI_SRV_STATUS_EXPORTING_FINISHED   "ExportingFilesFinished" // after images saved to their files
#define  XI_SRV_STATUS_SPEEDTESTING         "SpeedTesting"          // during storage write speed test
#define  XI_SRV_STATUS_SPEEDTEST_FINISHED   "SpeedTestFinished"     // after storage write speed test

//-------------------------------------------------------------------------------------------------------------------
// counter names returned by XI_CMD_GET_IMAGES_COUNT

#define  XI_SRV_COUNTER_DONE                 "done"
#define  XI_SRV_COUNTER_TOTAL                "total"
#define  XI_SRV_COUNTER_SPEED                "speed"
#define  XI_SRV_COUNTER_STORAGE_ERRORS       "storage errors"
#define  XI_SRV_COUNTER_TRANSPORT_SKIPPED    "skipped"
#define  XI_SRV_COUNTER_MISSED_TRIGGER_DUETO_OVERLAP            "trigger overlap"
#define  XI_SRV_COUNTER_MISSED_TRIGGER_DUETO_FRAME_BUFFER_OVR   "buffer overlap"
#define  XI_SRV_COUNTER_BUFFER_OVERFLOW      "buffer overflow"

//-------------------------------------------------------------------------------------------------------------------
// helper functions for communication
#include <map>
#include <string>
class TiXmlDocument;
class TiXmlElement;

namespace xiRecorderApi
{
    std::string formatRequest(const std::map<std::string, std::string> &dictParameters);    //!< outputs "<request name1=value1 name2=value2 ... />"
    std::string formatResponse(bool bResult, const std::string &sDetails, int *pxiReturnCode = nullptr, const std::string *psValueType = nullptr);   //!< outputs "<response code=xxx [xiret=yyy] [valuetype=ttt]>details</response>"
    void parseXmlResponse(const std::string &sXMLResponse, bool &bResult, std::string &sDetails, int *pxiReturnCode = nullptr);  //!< parses the server <response...> back to variables
    void parseXmlResponseToXml(const std::string &sXMLResponse, bool &bResult,
                               TiXmlDocument *pResponseToXmlDoc, TiXmlElement *&pRootResponseElement,
                               int *pxiReturnCode = nullptr);   //!< parses the server <response...> back to variables

    void setLogFile(const std::string &sLogFilename);           //!< enables logging and sets log file
    bool hasLogFile();                                          //!< returns true when logging enabled
    void outputStringToStdErr(const std::string &sMessage);     //!< outputs message in error output, appends newline (std::cerr, possibly log)
    void outputStringToConsole(const std::string &sMessage);    //!< shows message in console window, appends newline (std::cout, visible to user)
}

#endif // RECORDERAPI_H
