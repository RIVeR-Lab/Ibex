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

#include "code_samples.h"
#include "RecordingTask.h"
#include "RecorderApi.h"

#include <iostream>
#include <list>
#include <thread>

//----------------------------------------------------------------------------------------
// This is an alternative how to make tasks from code when not using XML
// You can run this when you pass --test-sample-task-code on command line
int runSample1TaskFromCode()
{
    CxRecordingTask aTask(0, false);
    aTask.startOwnServer();     // start own server, do not connect to remote one
    if (!aTask.openCamera("0"))
        return 0;

    xiRecorderApi::outputStringToConsole(std::string("Connected to camera: ") + aTask.cameraName());

    aTask.setCameraParam(XI_PRM_EXPOSURE, "200000");   // set exposure to 200ms
    aTask.setCameraParam(XI_PRM_WB_KR, "1.5");
    aTask.setCameraParam(XI_PRM_WB_KG, "1.0");
    aTask.setCameraParam(XI_PRM_WB_KB, "2.0");
    aTask.setCameraParam(XI_PRM_DOWNSAMPLING, "XI_DWN_1x1");

    aTask.setRecordingFrameCount(10);

    // record
    std::vector<std::string> vecStorages;
    vecStorages.push_back("code_data");
    if (!aTask.goReady(vecStorages))
        return 0;

    std::cout << std::endl << "Recording data" << std::endl;
    aTask.start();

    bool bTaskRunning = true;
    while (bTaskRunning)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        aTask.outputCounters();
        bTaskRunning = aTask.isRecording();
    }

    aTask.stop();

    // export data
    std::cout << std::endl << "Exporting to TIFF" << std::endl;
    aTask.setExportOptions("code_output", "XI_RGB24");
    aTask.exportData();

    bool bExportRunning = true;
    while (bExportRunning)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        aTask.outputCounters();
        bExportRunning = aTask.isExporting();
    }

    aTask.finalize();
    std::cout << std::endl;
    return 1;
}

//----------------------------------------------------------------------------------------
// Now the one with 3 cameras
int runSample3TaskFromCode()
{
    const int iNumCameras = 3;
    std::list<CxRecordingTask*> lstTasks;

    for (int i = 0; i < iNumCameras; i++)
    {
        CxRecordingTask* pTask = new CxRecordingTask(i, false);
        pTask->startOwnServer();
        if (pTask->openCamera(std::to_string(i)))
        {
            pTask->setCameraParam(XI_PRM_EXPOSURE, "10000");
            pTask->setRecordingFrameCount(10);
        }
        lstTasks.push_back(pTask);
    }

    // record
    bool bAnyReady = false;
    std::vector<std::string> vecStorages;
    vecStorages.push_back("code_data3");
    for (auto pTask : lstTasks)
    {
        if (pTask->goReady(vecStorages))
            bAnyReady = true;
    }
    if (!bAnyReady)
    {
        xiRecorderApi::outputStringToConsole("All tasks in error state, recording aborted.");
        for (auto pTask : lstTasks)
        {
            pTask->finalize();
            delete pTask;
        }
        return 0;
    }
    std::cout << std::endl << "Recording data" << std::endl;
    // start tasks
    for (auto &pTask : lstTasks)
        pTask->start();

    // wait until finished
    bool bAnyTaskRunning = true;
    while (bAnyTaskRunning)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        bAnyTaskRunning = false;
        for (auto &pTask : lstTasks)
        {
            pTask->outputCounters();
            bool bStillRunning = pTask->isRecording();
            if (bStillRunning)
                bAnyTaskRunning = true;
        }
    }

    // stop tasks
    for (auto &pTask : lstTasks)
        pTask->stop();

    // export data
    std::cout << std::endl << "Exporting to TIFF" << std::endl;
    for (auto &pTask : lstTasks)
    {
        pTask->setExportOptions("code_output3", "XI_RGB24");  // all cameras output to same folder and format for code simplicity
        pTask->exportData();
    }

    // wait until exporting finished
    bool bAnyTaskRExporting = true;
    while (bAnyTaskRExporting)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        bAnyTaskRExporting = false;
        for (auto &pTask : lstTasks)
        {
            pTask->outputCounters();
            bool bStillRunning = pTask->isExporting();
            if (bStillRunning)
                bAnyTaskRExporting = true;
        }
    }
    std::cout << std::endl << "Done. TIFF files exported to the following folders (as defined in XML):" << std::endl;

    // finalize tasks
    for (auto pTask : lstTasks)
    {
        pTask->finalize();
        delete pTask;
    }
    lstTasks.clear();
    std::cout << std::endl;
    return 1;
}
