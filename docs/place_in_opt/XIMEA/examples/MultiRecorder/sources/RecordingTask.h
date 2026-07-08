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

#ifndef CXRECORDINGTASK_H
#define CXRECORDINGTASK_H

#include <string>
#include <vector>

#include <xiApi.h>

class CxCameraServer;
class CxCameraClient;
class TiXmlDocument;
class TiXmlElement;

/**
 * \brief Class representing one recording task defined in provided RecordingSetup.XML.
 *
 * It contains the http client, and may start its own http server when recording should be run on local computer.
 */
class CxRecordingTask
{
public:
    CxRecordingTask(int32_t iAutoStartServerPort, bool bVerboseLog);    //!< pass 0 for iAutoStartServerPort to start at any port at localhost
    ~CxRecordingTask();

    bool readFromRecordingSetupXmlTag(TiXmlElement *pElTask);   //!< read task definition from XML and make initialization
    std::string storageName();  //!< storage name defined in XML file for this task
    bool goReady(const std::vector<std::string> &vecStorageFolders, bool bShowLiveImagePreview = false);    //!< set everything up for recording
    bool start();               //!< start recording
    bool isRecording();         //!< recording still running
    bool stop();                //!< stop recording
    bool willSaveSession();     //!< task will need saving session step
    bool saveSessionToFile();   //!< keep recorded data and add xirecsession description
    bool willExportTiffs();     //!< task will need TIF export step
    bool exportData();          //!< write data to TIFF image files
    bool isExporting();         //!< export is still running
    bool outputCounters();      //!< write current task countes to console
    bool finalize();            //!< cleanup
    void outputExportFolder();  //!< write export folder to console, with same style as outputCounters

    // direct communication functions (high level) as seen in XML
public:
    void startOwnServer();      //!< do not connect to remote server, use own server to open a camera on this computes
    bool connectToRemoteServer(const std::string &sRemoteHost, int32_t iRemotePort);        //!< use this or startOwnServer before opening camera
    void setStorageSpeedTest(int iDurationS, int iBufferSize);
    bool openCamera(const std::string &sDeviceIndex, std::string *psErrorMsg = nullptr);
    bool openCameraBy(XI_OPEN_BY eMethod, const std::string &sValue, std::string *psErrorMsg = nullptr);
    std::string cameraName();
    bool setCameraParam(const std::string &sParamName, const std::string &sValue, std::string *psErrorMsg = nullptr);
    bool getCameraParam(const std::string &sParamName, std::string &sValue, std::string *psErrorMsg = nullptr);
    void setRecordingFrameCount(int32_t iFrameCount);
    void setExportOptions(const std::string &sExportFolder, const std::string &sExportImageFormat, bool bTiffEnableStoring16bit = true, bool bTiffScaleDataToFull16bitRange = false);

    // communication functions (low level)
public:
    bool sendRequestToServer(const std::string &sRequest, std::string *psResponseDetails = nullptr, XI_RETURN *pxiRet = nullptr);
    bool sendRequestToServerResXml(const std::string &sRequest, TiXmlDocument *pResponseToXmlDoc, TiXmlElement *&pRootResponseElement, XI_RETURN *pxiRet = nullptr);
    bool sendRequestToServerResGeneric(const std::string &sRequest, std::string *psResponse = nullptr, std::string *psResponseContentType = nullptr);
    void printLogMessage(const std::string &sMessage, bool bVerbose);    // write string to cerr or logfile

protected:
    CxCameraServer *m_pServer;              //!< instance of own local server started instead of connecting to remote one. Is null when connected to remote server.
    CxCameraClient* m_pClient;              //!< http client

    int32_t m_iAutoStartServerPort;
    bool m_bPrintServerLogVerbose;          //!< output all messages into logfiles
    int32_t m_iFrameCount;                  //!< number of frames that should be rocorded in this task
    std::string m_sExportImageFormat;       //!< e.g. XI_RGB24, or some values found in XML
    std::string m_sExportFolder;            //!< folder for TIFFs
    std::string m_sSaveSessionFilename;     //!< file name to save session instead of exporting (may be empty)
    std::string m_sStorageName;             //!< storage identification
    std::string m_sCameraName;              //!< device name
    bool m_bCameraConnected;
    bool m_bTiffEnableStoring16bit, m_bTiffScaleDataToFull16bitRange;   //!< options for TIFF export, see CxImageExporter::saveImageToFile

    void readCameraName();

protected:
    bool m_bTestStorageSpeedOnly;           //!< do not record data, only test writing speed
    int m_iSpeedTestDurationS;              //!< duration of the test in seconds (may last a few hours too)
    int m_iSpeedTestBufferSize;             //!< use artificial buffer size, so it can be run without camera
};

#endif // CXRECORDINGTASK_H
