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

#include "tests.h"
#include "CameraControl.h"
#include "CameraServer.h"
#include "CameraClient.h"
#include "RecorderApi.h"

//----------------------------------------------------------------------------------------
void testFunctionalInterface(const std::vector<std::string> &vecStorageFolders,  uint32_t uiFrameCount, XI_IMG_FORMAT eOutputFormat, const std::string &sOutputFolder)
{
    CxCameraControl aCtl;
    aCtl.setDataStorageFolders(vecStorageFolders);
    aCtl.openCamera(0);
    //aCtl.applyCameraSettingsFromFile("../test-data/test.xicamera");
    aCtl.setCameraParameter(XI_PRM_EXPOSURE, "200000"); // 200 ms
    aCtl.setCameraParameterFloat(XI_PRM_WB_KR, 1.5);
    aCtl.setCameraParameterFloat(XI_PRM_WB_KG, 1.0);
    aCtl.setCameraParameterFloat(XI_PRM_WB_KB, 2.0);
    aCtl.prepareForAcquisition(uiFrameCount);
    aCtl.startAcquisition();
    aCtl.recordData();
    aCtl.stopAcquisition();
    aCtl.exportTiffFiles(sOutputFolder, eOutputFormat);
}

//----------------------------------------------------------------------------------------
void testXmlInterface(const std::vector<std::string> &vecStorageFolders,  uint32_t uiFrameCount, XI_IMG_FORMAT eOutputFormat, const std::string &sOutputFolder)
{
    std::string sFolders;
    for (auto &sPath : vecStorageFolders)
    {
        if (!sFolders.empty())
            sFolders += "|";
        sFolders += sPath;
    }

    CxCameraControl aCtl;
    aCtl.processRemoteRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_DATA_STORAGE}, {"folders", sFolders}}));
    aCtl.processRemoteRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_OPEN_CAMERA}, {"idx", "0"}}));
    aCtl.processRemoteRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_CAMERA_PARAM}, {"name", XI_PRM_EXPOSURE}, {"value", "200000"}}));  // 200 ms
    aCtl.processRemoteRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_CAMERA_PARAM}, {"name", XI_PRM_WB_KR}, {"value", "1.5"}}));
    aCtl.processRemoteRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_CAMERA_PARAM}, {"name", XI_PRM_WB_KG}, {"value", "1.0"}}));
    aCtl.processRemoteRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_CAMERA_PARAM}, {"name", XI_PRM_WB_KB}, {"value", "2.0"}}));
    aCtl.processRemoteRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_GO_READY}, {"frameCount", std::to_string(uiFrameCount)}}));
    aCtl.processRemoteRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_START_ACQ}}));
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        bool bRet = false;
        std::string sDetails;
        std::string sResult, sResponseContentType;
        aCtl.processRemoteRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_STATUS}}), &sResult, &sResponseContentType);
        if (sResponseContentType == "text/xml")
            xiRecorderApi::parseXmlResponse(sResult, bRet, sDetails);
        if (!bRet || sDetails != XI_SRV_STATUS_RECORDING)
            break;
    }
    aCtl.processRemoteRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_STOP_ACQ}}));
    aCtl.processRemoteRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_EXPORT_TIFF_FILES}, {"folder", sOutputFolder}, {"imageFormat", std::to_string((int)eOutputFormat)}}));
    aCtl.processRemoteRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_RELEASE_DATA_BUFFERS}, {"deleteRecorderDataFiles", "true"}}));
}

//----------------------------------------------------------------------------------------
void testCameraServer(const std::vector<std::string> &vecStorageFolders,  uint32_t uiFrameCount, XI_IMG_FORMAT eOutputFormat, const std::string &sOutputFolder)
{
    std::string sFolders;
    for (auto &sPath : vecStorageFolders)
    {
        if (!sFolders.empty())
            sFolders += "|";
        sFolders += sPath;
    }

    CxCameraServer aServer;
    aServer.startAndListenAt("localhost", 8020, true, true);
    CxCameraClient aRecorder("localhost", 8020);

    aRecorder.sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_DATA_STORAGE}, {"folders", sFolders}}));
    aRecorder.sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_OPEN_CAMERA}, {"idx", "0"}}));
    aRecorder.sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_CAMERA_PARAM}, {"name", XI_PRM_EXPOSURE}, {"value", "200000"}}));  // 200 ms
    aRecorder.sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_CAMERA_PARAM}, {"name", XI_PRM_WB_KR}, {"value", "1.5"}}));
    aRecorder.sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_CAMERA_PARAM}, {"name", XI_PRM_WB_KG}, {"value", "1.0"}}));
    aRecorder.sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_SET_CAMERA_PARAM}, {"name", XI_PRM_WB_KB}, {"value", "2.0"}}));
    aRecorder.sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_GO_READY}, {"frameCount", std::to_string(uiFrameCount)}}));
    aRecorder.sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_START_ACQ}}));
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        bool bRet = false;
        std::string sDetails;
        std::string sResult, sResponseContentType;
        aRecorder.sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_STATUS}}), &sResult, &sResponseContentType);
        xiRecorderApi::parseXmlResponse(sResult, bRet, sDetails);
        if (!bRet || sDetails != XI_SRV_STATUS_RECORDING)
            break;
    }
    aRecorder.sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_STOP_ACQ}}));
    aRecorder.sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_EXPORT_TIFF_FILES}, {"folder", sOutputFolder}, {"imageFormat", std::to_string((int)eOutputFormat)}}));
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        bool bRet = false;
        std::string sDetails;
        std::string sResult, sResponseContentType;
        aRecorder.sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_GET_STATUS}}), &sResult, &sResponseContentType);
        xiRecorderApi::parseXmlResponse(sResult, bRet, sDetails);
        if (!bRet || sDetails != XI_SRV_STATUS_EXPORTING)
            break;
    }
    aRecorder.sendRequest(xiRecorderApi::formatRequest({{"cmd", XI_CMD_RELEASE_DATA_BUFFERS}, {"deleteRecorderDataFiles", "true"}}));
}
