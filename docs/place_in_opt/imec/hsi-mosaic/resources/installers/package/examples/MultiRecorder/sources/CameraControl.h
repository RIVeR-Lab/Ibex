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

#ifndef CXCAMERACONTROL_H
#define CXCAMERACONTROL_H

#include <ctime>
#include <chrono>
#include <map>
#include <string>
#include <thread>
#include <vector>
#include <xiApi.h>

#ifdef MULTIRECORDER_FIS_ENABLE
#include <FIS_image_metadata.h>
#endif

class IxFisRecorder;
class CxUserImageProcessor;

#define DX_IMG_BINARY_MIME "application/octet-stream"

//----------------------------------------------------------------------------------------
/**
 * \brief Structure used to describe camera parameter. Used as std::list<SxCameraParamDesc>.
 *
 * Is neeeded to translate constant names to their values (e.g. "XI_PRM_EXPOSURE" to "exposure"),
 * as we support using constants for parameter names.
 *
 * Is implemented by including xiApiParams.h.
 *
 * We use similar technique for parameter values by including xiApiEnumerators.h.
 */
struct SxCameraParamDesc
{
    std::string name;           //!< e.g. "exposure"
    std::string defConstName;   //!< e.g. "XI_PRM_EXPOSURE"
    XI_PRM_TYPE type;           //!< xiTypeString, xiTypeInteger, xiTypeFloat, ....
    std::string access;
    int custom;
};

//----------------------------------------------------------------------------------------
/**
 * \brief Structure used to describe connected camera identification.
 *
 * Is returned in CxCameraControl::listConnectedDevices.
 * Values are obtained using xiGetDeviceInfoString from xiApi.
 */
struct SxCameraDeviceInfo
{
    std::string sSN;            //!< XI_PRM_DEVICE_SN
    std::string sInstPath;      //!< XI_PRM_DEVICE_INSTANCE_PATH
    std::string sDeviceName;    //!< XI_PRM_DEVICE_NAME
    std::string sDeviceType;    //!< XI_PRM_DEVICE_TYPE
};

//----------------------------------------------------------------------------------------
/**
 * \brief Main class that controls one camera, keeps recorded data and performs all other actions.
 *
 * It receives the XML commands via CxCameraServer, parses them and provides responses back.
 * Uses FIS_Lib to store recorded image data, implements their export to TIFF files.
 *
 * Can be also used as a implementation of remote camera for CamTool or other projects
 */
class CxCameraControl
{
public:
    CxCameraControl();
    ~CxCameraControl();

    bool setDataStorageFolders(const std::vector<std::string> &vecStoragePath, std::string *psErrorMsg = nullptr);  //!< sets storage folders to FIS_Lib. Pass "RAM" to record to RAM, and not to HDD. Should be called before setting other recording parameters.
    bool numberOfDevices(uint32_t &uiCount, std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);        //!< calls xiGetNumberDevices
    bool getDeviceInfoString(DWORD DevId, const std::string &sParamName, std::string &sValue, std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);  //!< calls xiGetDeviceInfoString
    bool listConnectedDevices(std::vector<SxCameraDeviceInfo> &vecDeviceInfo, std::string *psErrorMsg = nullptr);   //!< lists all cameras connected to this computer (passed to client, to choose one)
    bool openCamera(DWORD DevId, std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);                   //!< calls xiOpenDevice
    bool openCameraBy(XI_OPEN_BY eMethod, const char *szParameterValue, std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);    //!< calls xiOpenDeviceBy
    bool closeOpenedCamera();                                                                                       //!< calls xiCloseDevice
    bool applyCameraSettingsFromFile(const std::string &sXicameraFile, std::string *psErrorMsg = nullptr);          //!< opens file in xiCamera format (saved in CamTool), and sets the camera parameters as described in there
    bool getCameraParameter(const std::string &sParamName, std::string &sValue, std::string *psValueType = nullptr, std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);    //!< determines the parameter type automatically, returning value converted to string sValue
    bool getCameraParameterInt(const std::string &sParamName, int32_t &iValue, std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);         //!< calls xiGetParamInt
    bool getCameraParameterInt64(const std::string &sParamName, uint64_t &uiValue, std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);     //!< calls xiGetParamInt64
    bool getCameraParameterFloat(const std::string &sParamName, float &fValue, std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);         //!< calls xiGetParamFloat
    bool getCameraParameterString(const std::string &sParamName, std::string &sValue, std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);  //!< calls xiGetParamString
    bool setCameraParameter(const std::string &sParamName, const std::string &sValue, std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);  //!< determines the parameter type automatically, supports named constants as sValue (e.g. "XI_RGB24")
    bool setCameraParameterInt(const std::string &sParamName, int32_t iValue, std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);          //!< calls xiSetParamInt
    bool setCameraParameterInt64(const std::string &sParamName, uint64_t uiValue, std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);      //!< calls xiSetParamInt64
    bool setCameraParameterFloat(const std::string &sParamName, float fValue, std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);          //!< calls xiSetParamFloat
    bool setCameraParameterString(const std::string &sParamName, const std::string &sValue, std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);    //!< calls xiSetParamString

    /**
    \brief Calls xiGetImage ad saves the XI_IMG to byte array (std::string).

    When the image data is returned in application/octet-stream content type, the response starts
    with XI_IMG.size bytes with XI_IMG structure, followed by XI_IMG.bp_size of image data in XI_IMG.frm format.

    @param[in] uiTimeout                for xiGetImage
    @param[in] sEncoding                requested encoding the the image data (ignored, may be used later to compress the image data with PNG, JPEG, etc)
    @param[out] sResponse               data buffer, or error message
    @param[out] sResponseContentType    encoding used to compile the response data. Is usually "application/octet-stream" for images, or "text/plain" for errors
    @param[out] pxiRet                  return value of xiGetImage
    @return true when the image received and saved to sResponse
    */
    bool getCameraImageWithEncoding(uint32_t uiTimeout, const std::string &sEncoding, std::string &sResponse, std::string &sResponseContentType, XI_RETURN *pxiRet = nullptr);
    bool cameraCounter(XI_COUNTER_SELECTOR eCounter, int &iValue);      //!< reads the camera counter (XI_PRM_COUNTER_SELECTOR, XI_PRM_COUNTER_VALUE), returns false the counter not supported by camera
    bool recordingCounter(const std::string &sCounter, int &iValue);    //!< fills the counter used in recording (sCounter is one of XI_SRV_COUNTER_ constants)
    bool prepareForAcquisition(uint32_t uiFrameCount, std::string *psErrorMsg = nullptr);   //!< prepare to capture N frames. When camera supports pausing acq, it also starts acq and pauses it
    bool startAcquisition(std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);  //!< calls xiStartAcquisition, or unpauses acq when paused by prepareForAcquisition.
    bool recordData();                  //!< main loop for recording all requests frames
    bool recordDataAsync();             //!< calls recordData() in separate thread
    bool cancelRecording();             //!< cancel recording when run using async function recordDataAsync
    bool stopAcquisition(std::string *psErrorMsg = nullptr, XI_RETURN *pxiRet = nullptr);               //!< calls xiStopAcquisition (and cancelRecording)
    bool getRecordedFrameData(int iIdx, std::string &sResponse, std::string &sResponseContentType);     //!< returns recorded image data for frame in FIS_Lib. Response is similar to getCameraImageWithEncoding.
    bool exportTiffFiles(const std::string &sOutputFolder, int iFmtOutput = -1, bool bTiffEnable16bit = true, bool bTiffRescale16bit = false, int iOnlyFrame = -1);     //!< exports all image frames to TIFF files in XI_IMG_FORMAT, with bits setting in optional parameters (\ref CxImageExporter::saveImageToFile)
    bool exportTiffFilesAsync(const std::string &sOutputFolder, int iFmtOutput, bool bTiffEnable16bit = true, bool bTiffRescale16bit = false, int iOnlyFrame = -1, std::string *psErrorMsg = nullptr);  //!< calls exportTiffFiles() in separate thread. Process can be cancelled using cancelExporting
    bool cancelExporting();             //!< cancel exporting when run using async function exportTiffFilesAsync
    bool releaseDataBuffers(bool bRemoveDataFiles);     //!< releases data in FIS_Lib

    bool saveSession(const std::string &sFilename, XI_IMG_FORMAT iFmtOutput, std::string *psErrorMsg = nullptr);    //!< Saves the recorded image data in xiRecSession file that can be later opened in CamTool or exported by MultiRecorder
    bool loadSession(const std::string &sFilename, std::string *psErrorMsg = nullptr);                              //!< Loads the recorded image data from xiRecSession file, that can be later exported to TIFF files

    std::string exportedFilename(size_t idx);       //!< returns TIFF file name for the image frame with index (after it was exported)

public:
    enum ExStatus
    {
        exsInitialization,          //!< XI_SRV_STATUS_INIT, startup, prepared to open camera and set parameters
        exsInitializationError,     //!< XI_SRV_STATUS_INIT_ERROR, failed to init storage or open camera
        exsPreparedForAcq,          //!< XI_SRV_STATUS_PREPARED_FOR_ACQ, after prepareForAcquisition
        exsRecording,               //!< XI_SRV_STATUS_RECORDING, after stopAcquisition, while server is recording images
        exsRecordingFinished,       //!< XI_SRV_STATUS_RECORDING_FINISHED, after finished recording images
        exsExporting,               //!< XI_SRV_STATUS_EXPORTING, during exporting files
        exsExportingFinished,       //!< XI_SRV_STATUS_EXPORTING_FINISHED, after images saved to their files
        exsSpeedTesting,            //!< XI_SRV_STATUS_SPEEDTESTING, after stopAcquisition, while server is recording images
        exsSpeedTestFinished,       //!< XI_SRV_STATUS_SPEEDTEST_FINISHED, after finished recording images
    };

    ExStatus status();              //!< returns the object status when used for recording as one of ExStatus constants
    std::string statusString();     //!< returns the object status when used for recording as one of XI_SRV_STATUS_ constants

    // IPC
public:
    bool processRemoteRequest(const std::string &sXmlString, std::string *psResponse = nullptr, std::string *psResponseContentType = nullptr);        //!< accepts commands in xml form <request cmd=setparam name=exp value=123 />, <request cmd=startAcq />. One command per request.
    void setVerboseOutputLog(bool bVerose);
    void printOutputMessage(const std::string &sMessage, bool bVerbose);    //!< write string to console
    static void makeXiImgResponse(XI_IMG *pXiImg, std::string &sResponse, std::string &sResponseContentType);      //!< puts contents of XI_IMG to server response
    void processRawData(const std::string& sData, std::string* psResponse = nullptr, std::string* psResponseContentType = nullptr);

protected:
    bool m_bPrintServerLogVerbose;
#ifdef MULTIRECORDER_FIS_ENABLE
    IxFisRecorder *m_pRecorder;
    CxFisImageMetadata m_aImageMetadata;
#endif
    std::thread* m_pRecordingThread; // our own thread that captures images and sends them to FIS (when run async)
    std::thread* m_pExportingThread;
    HANDLE m_hCamera;
    int m_iLoadedExportImageFormat;
    bool m_bAcquisitionPaused;
    bool m_bCancelRecording, m_bCancelExporting;
    ExStatus m_eStatus;
    int m_iTransportBufferSizeDefault;
    time_t m_timeAcqStart;
    int m_iTiffExportCounter;
    std::map<XI_COUNTER_SELECTOR, bool> m_mapCounterSupported;

    // recording speed calculation
    int m_iLastRecordedFrames;
    std::chrono::system_clock::time_point m_timeLastSpeedCheck;
    double m_dLastSpeedValue;

    // image format
    int m_iConcatImgCount;          //!< number of images inside one buffer (see XI_PRM_CONCAT_IMG_COUNT)
    int m_iConcatImgNextOffset;     //!< offset to next image inside one concatenated buffer (see XI_PRM_CONCAT_IMG_TRANSPORT_IMG_OFFSET)
    std::vector<uint32_t> m_vecFrameNo;         //!< frame numbers of catpured frames (image data stored in FIS_Lib, metadata is kept in this class)
    std::vector<uint64_t> m_vecTimeStamps;      //!< time stamps of catpured frames in microseconds (image data stored in FIS_Lib, metadata is kept in this class)
    std::vector<std::string> m_vecExportedFilenames;    //!< list of names of TIFF files, used to write xiSeq index file later

    // image preview
    bool m_bShowLiveImagePreview;
    double m_dLiveImagePreviewFps;
    std::chrono::system_clock::time_point m_timeLastLiveImagePreview;

    uint64_t storageBytesAvailable();
    bool startStorageSpeedTestAsync(int iDuration, int iBufferSize, int iFrameLoopCount);
    void storageSpeedTest(int iDuration, int iBufferSize, int iFrameLoopCount);
    void postOpenCameraInitialization();
    void writeSequenceIndexFile(const std::string &sFilename);
    void showLiveImagePreview(XI_IMG *pXiImg);

    // user-provided image processing
public:
    CxUserImageProcessor *m_pUserImageProcessor;

    // static helpers
public:
    static SxCameraParamDesc* findParam(const std::string &sParamName);                     //!< Returns the parameter description (uses xiApiParams.h)
    static bool findXiApiEnumValue(const std::string &sConstantName, int &iValue);          //!< Translates the xiApi constant in string to its value, e.g. "XI_RGB24" to XI_RGB24 (uses xiApiEnumerator.h)
    static std::string xiApiErrorString(XI_RETURN ret);                                     //!< returns human-readable error message from XI_RETURN (uses xiApiEnumerator.h)
    static std::string counterResponseString(const std::string &sCounterName, int iValue);  //!< outputs XML tag with counter name and its values
    static std::string counterResponseStringDbl(const std::string &sCounterName, double dValue, const std::string &sUnit = ""); //!< outputs XML tag with counter name and its values
};

#endif // CXCAMERACONTROL_H
