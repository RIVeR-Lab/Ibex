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

#include "RecordingTask.h"
#include "CameraServer.h"
#include "RecorderApi.h"
#include "LiveImagePreview.h"
#include "tests.h"
#include "code_samples.h"
#include <tinyxml.h>

#include <algorithm>
#include <list>
#include <map>
#include <thread>
#include <filesystem>

#define XI_UNUSED(x) (void)x;

void runTest();
int runFromRecordingSetup(const char *szFilename, bool bVerboseLog, bool bShowLiveImagePreview, int iOnlyTestStorageSpeedWithDuration = 0, int iSpeedTestBufferSize = 0);
int exportSequenceFromSession(const char *szFilename, const char *szOutputFolder, const char *szImgFormat,
                              const char *szTiffEnable16bit, const char *szTiffRescale16bit, const char *szOnlyFrame, bool bVerboseLog);

//----------------------------------------------------------------------------------------
void printCommandLineHelp()
{
    std::cout << "Command line arguments (either run a script, or just start the server, or only export tiffs)" << std::endl
              << " -h or --help : show this command line arguments help" << std::endl
              << " -r or --run filename : run the recording XML script" << std::endl
              << " -s or --start-server hostname[:port] : start camera server listening at specified address and (optionally) port" << std::endl
              << " -v or --verbose : verbose log output" << std::endl
              << " -l or --log logfilename : output log to a file, not to console" << std::endl
              << " --live-preview : show live image preview during recording" << std::endl
              << std::endl
              << " -e or --export-tiffs filename : load the recorded session with data and export it to TIFF sequence" << std::endl
              << " --export-tiffs-folder folder : export tiff sequence to this folder (optional, session name is used by default)" << std::endl
              << " --export-tiffs-frame index : export only one frame (optional, exports whole sequence when not set)" << std::endl
              << " --export-tiffs-enable16bitdata true : export TIFFs with 16bit data, else rescale to 8bit (optional, true by default)" << std::endl
              << " --export-tiffs-rescaleto16bitrange false : rescale 16bit data in TIFFs to full 16bit range to increase compatibility with other apps (optional, false by default)" << std::endl
              //<< " --export-tiffs-format fmt : use given image format when exporting the sequence (optional, XI_RGB24 is default, possible values are in XI_IMG_FORMAT)" << std::endl
              << std::endl
              << " --test-storage-speed-duration duration_s : performs writing speed test, with time duration in seconds (must be combined with --run)" << std::endl
              << " --test-storage-speed-size buffer_size : use buffer of given size in bytes for speed test, do not open camera (optional)" << std::endl
        ;
}

//----------------------------------------------------------------------------------------
// helpers for parsing command line
char* getCmdOption(char** begin, char** end, const std::string& option, const std::string& shortOption = "")
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    if (!shortOption.empty())
        return getCmdOption(begin, end, shortOption);
    else
        return nullptr;
}

//----------------------------------------------------------------------------------------
bool cmdOptionExists(char** begin, char** end, const std::string& option, const std::string& shortOption = "")
{
    if (std::find(begin, end, option) != end)
        return true;

    return (!shortOption.empty() && std::find(begin, end, shortOption) != end);
}

//----------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    xiRecorderApi::outputStringToConsole("XIMEA MultiRecorder");

    if (cmdOptionExists(argv, argv + argc, "--help", "-h") || cmdOptionExists(argv, argv + argc, "/?"))
    {
        printCommandLineHelp();
        return 1;
    }

    bool bVerboseLog = cmdOptionExists(argv, argv + argc, "--verbose", "-v");

    char *szLogFile = getCmdOption(argv, argv + argc, "--log", "-l");
    if (szLogFile != nullptr)
        xiRecorderApi::setLogFile(szLogFile);

    bool bShowLiveImagePreview = cmdOptionExists(argv, argv + argc, "--live-preview");
    if (bShowLiveImagePreview)
    {
        // preload the DLLs
        if (!CxLiveImagePreview::isLivePreviewSupported())
        {
            std::string sMsg = "Warning: OpenCV libraries not found. ";
#ifdef WIN32
            sMsg += "Please install OpenCV and copy opencv_worldNNN.dll next to MultiRecorder.exe.";
#else
            sMsg += "OpenCV libraries are expected in /usr/local/lib.";
#endif
            xiRecorderApi::outputStringToConsole(sMsg);
        }
    }

    char *szStartServer = getCmdOption(argv, argv + argc, "--start-server", "-s");
    if (szStartServer != nullptr)
    {
        std::string sHost(szStartServer);
        int iPort = 0;  // auto assign port

        const char *szPort = strchr(szStartServer, ':');
        if (szPort != nullptr && strlen(szPort) > 1)
        {
            sHost = std::string(szStartServer, szPort - szStartServer);
            iPort = atoi(szPort + 1);
        }
        if (sHost.empty())
        {
            xiRecorderApi::outputStringToConsole("Error: cannot start server host name (or IP address) not specified.");
            return 0;
        }

        CxCameraServer aServer;
        aServer.startAndListenAt(sHost, iPort, bVerboseLog, false);
        return 1;
    }

    // create logfile automatically for client app, not to mix stdout and stderr messages in console
//#ifndef WIN32
    if (szLogFile == nullptr)
    {
        std::string sImplicitLogFile("MultiRecorder-log.txt");
        xiRecorderApi::outputStringToConsole("Log file not specified, using file MultiRecorder-log.txt.");
        xiRecorderApi::setLogFile("MultiRecorder-log.txt");
    }
//#endif

    // parameters for testing the storage write speed
    int iTestSpeedDurationS = 0;
    char *szTestSpeedDuration = getCmdOption(argv, argv + argc, "--test-storage-speed-duration");
    if (szTestSpeedDuration != nullptr)
        iTestSpeedDurationS = atoi(szTestSpeedDuration);
    int iTestSpeedBufferSize = 0;
    char *szTestSpeedBufferSize = getCmdOption(argv, argv + argc, "--test-storage-speed-size");
    if (szTestSpeedBufferSize != nullptr)
        iTestSpeedBufferSize = atoi(szTestSpeedBufferSize);

    // run the script
    char *szRecordingSetupFile = getCmdOption(argv, argv + argc, "--run", "-r");
    if (szRecordingSetupFile != nullptr)
    {
        return runFromRecordingSetup(szRecordingSetupFile, bVerboseLog, bShowLiveImagePreview, iTestSpeedDurationS, iTestSpeedBufferSize);
    }

    // export images from saved session
    char *szSessionFile = getCmdOption(argv, argv + argc, "--export-tiffs", "-e");
    if (szSessionFile != nullptr)
    {
        char *szOuputFolder = getCmdOption(argv, argv + argc, "--export-tiffs-folder");
        char *szImgFormat = getCmdOption(argv, argv + argc, "--export-tiffs-format");
        char *szOnlyFrame = getCmdOption(argv, argv + argc, "--export-tiffs-frame");
        char *szTiffEnable16bit = getCmdOption(argv, argv + argc, "--export-tiffs-enable16bitdata");
        char *szTiffRescale16bit = getCmdOption(argv, argv + argc, "--export-tiffs-rescaleto16bitrange");
        return exportSequenceFromSession(szSessionFile, szOuputFolder, szImgFormat, szTiffEnable16bit, szTiffRescale16bit, szOnlyFrame, bVerboseLog);
    }

    if (cmdOptionExists(argv, argv + argc, "--test"))
    {
        //runTest();
        return runFromRecordingSetup("samples/s1-capture-10-images/sample1.xml", bVerboseLog, bShowLiveImagePreview);
    }
    if (cmdOptionExists(argv, argv + argc, "--test-sample-task-code"))
    {
        return runSample1TaskFromCode();
    }
    xiRecorderApi::outputStringToConsole("Error: command line arguments don't specify the task, use -h for help.");
    return 0;
}

//----------------------------------------------------------------------------------------
int runFromRecordingSetup(const char *szFilename, bool bVerboseLog, bool bShowLiveImagePreview,
            int iOnlyTestStorageSpeedWithDuration, int iSpeedTestBufferSize)
{
    TiXmlDocument doc(szFilename);
    if (!doc.LoadFile())
    {
        xiRecorderApi::outputStringToConsole(std::string("Error: failed to open file ") + szFilename);
        return 0;
    }
    TiXmlElement *pElRoot = doc.FirstChildElement("RecordingSetup");
    if (pElRoot == nullptr)
    {
        xiRecorderApi::outputStringToConsole("Error: the XML file does not start with <RecordingSetup> tag");
        return 0;
    }

    // read storages
    std::map<std::string, std::vector<std::string>> mapStorages;

    TiXmlElement *pElStorages = pElRoot->FirstChildElement("recordingStorages");
    if (pElStorages == nullptr)
        return 0;

    TiXmlElement *pElStorage = pElStorages->FirstChildElement("storage");
    while (pElStorage != nullptr)
    {
        const char *szStorageName = pElStorage->Attribute("name");
        std::vector<std::string> vecFolders;
        TiXmlElement *pElFolder = pElStorage->FirstChildElement("folder");
        while (pElFolder != nullptr)
        {
            const char *szFolder = pElFolder->GetText();
            if (szFolder != nullptr && strlen(szFolder) > 0)
                vecFolders.push_back(szFolder);

            pElFolder = pElFolder->NextSiblingElement("folder");
        }

        if (szStorageName != nullptr && strlen(szStorageName) > 0)
            mapStorages[szStorageName] = vecFolders;

        pElStorage = pElStorage->NextSiblingElement("storage");
    }

    TiXmlElement *pElTasks = pElRoot->FirstChildElement("tasks");
    if (pElTasks == nullptr)
        return 0;

    bool bTestingStorageSpeed = (iOnlyTestStorageSpeedWithDuration > 0);

    xiRecorderApi::outputStringToConsole("Initializing cameras...");

    // read tasks
    //int iNewTaskServerPort = 8030;
    std::list<CxRecordingTask*> lstTasks;
    TiXmlElement *pElTask = pElTasks->FirstChildElement("cameraTask");
    while (pElTask != nullptr)
    {
        CxRecordingTask *pNewTask = new CxRecordingTask(0 /*iNewTaskServerPort++*/, bVerboseLog);

        if (bTestingStorageSpeed)
            pNewTask->setStorageSpeedTest(iOnlyTestStorageSpeedWithDuration, iSpeedTestBufferSize);

        pNewTask->readFromRecordingSetupXmlTag(pElTask);
        lstTasks.push_back(pNewTask);

        pElTask = pElTask->NextSiblingElement("cameraTask");
    }

    // prepare tasks
    bool bAnyReady = false;
    for (auto &pTask : lstTasks)
    {
        std::string sTaskStorageName = pTask->storageName();
        auto itStorage = mapStorages.find(sTaskStorageName);
        if (itStorage != mapStorages.end())
        {
            if (pTask->goReady(itStorage->second, bShowLiveImagePreview))
                bAnyReady = true;
        }
        else
            xiRecorderApi::outputStringToConsole(std::string("Error: XML does not define recording storage named \"") + sTaskStorageName + "\"");
    }

    if (!bAnyReady)
    {
        xiRecorderApi::outputStringToConsole("All tasks in error state, recording aborted.");
        for (auto &pTask : lstTasks)
        {
            pTask->finalize();
            delete pTask;
        }
        lstTasks.clear();
        return 0;
    }

    std::cout << std::endl << "Recording data" << std::endl << "--------------------------" << std::endl;
    {
        CxLiveImagePreview aPreviewDisplayLooper;

        // start tasks
        for (auto &pTask : lstTasks)
            pTask->start();

        // wait until finished
        // output conter every 0.5s, and 1s when testing storage write speed
        int iSleepTime = (bTestingStorageSpeed ? 1000 : 500);
        bool bAnyTaskRunning = true;
        while (bAnyTaskRunning)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(iSleepTime));

            bAnyTaskRunning = false;
            for (auto &pTask : lstTasks)
            {
                pTask->outputCounters();
                bool bStillRunning = pTask->isRecording();
                if (bStillRunning)
                    bAnyTaskRunning = true;
            }
            aPreviewDisplayLooper.processPreviewTasks();
        }

        // stop tasks
        for (auto &pTask : lstTasks)
            pTask->stop();

        // preview windows are closed here (at destructor)
    }

    if (bTestingStorageSpeed)
        std::cout << std::endl << "Speed values were also written to server log file as CSV." << std::endl;

    // prepare data output
    bool bSavingSessionNeeded = false;
    bool bExportingTiffNeeded = false;
    for (auto &pTask : lstTasks)
    {
        if (pTask->willSaveSession()) bSavingSessionNeeded = true;
        if (pTask->willExportTiffs()) bExportingTiffNeeded = true;
    }

    // save session
    if (bSavingSessionNeeded)
    {
        std::cout << std::endl << "Saving recording session" << std::endl << "--------------------------" << std::endl;
        for (auto &pTask : lstTasks)
            pTask->saveSessionToFile();
    }

    // export data
    if (bExportingTiffNeeded)
    {
        std::cout << std::endl << "Exporting to TIFF" << std::endl << "--------------------------" << std::endl;
        for (auto &pTask : lstTasks)
            pTask->exportData();

        // wait until exporting finished
        bool bAnyTaskRunning = true;
        while (bAnyTaskRunning)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            bAnyTaskRunning = false;
            for (auto &pTask : lstTasks)
            {
                pTask->outputCounters();
                bool bStillRunning = pTask->isExporting();
                if (bStillRunning)
                    bAnyTaskRunning = true;
            }
        }
        std::cout << std::endl << "Done. TIFF files exported to the following folders (as defined in XML):" << std::endl;
        for (auto &pTask : lstTasks)
            pTask->outputExportFolder();
    }

    // finalize tasks
    for (auto &pTask : lstTasks)
    {
        pTask->finalize();
        delete pTask;
    }
    lstTasks.clear();

    std::cout << std::endl;
    return 1;
}

//----------------------------------------------------------------------------------------
void runTest()
{
    std::vector<std::string> vecStorageFolders;
    vecStorageFolders.push_back("dataTest");

    uint32_t uiFrameCount = 10;
    XI_IMG_FORMAT eOutputFormat = XI_RGB32;

    //testFunctionalInterface(vecStorageFolders, uiFrameCount, eOutputFormat, "exported");
    //testXmlInterface(vecStorageFolders, uiFrameCount, eOutputFormat, "exportedXml");
    testCameraServer(vecStorageFolders, uiFrameCount, eOutputFormat, "exportedHtml");
}

//----------------------------------------------------------------------------------------
int exportSequenceFromSession(const char *szFilename, const char *szOutputFolder, const char *szImgFormat,
                              const char *szTiffEnable16bit, const char *szTiffRescale16bit, const char *szOnlyFrame, bool bVerboseLog)
{
    CxCameraControl aCtl;
    if (szFilename == nullptr || !aCtl.loadSession(szFilename))
    {
        xiRecorderApi::outputStringToConsole(std::string("Error: failed to open session file ") + szFilename);
        return 0;
    }

    std::string sOutputFolder;
    if (szOutputFolder != nullptr)
        sOutputFolder = szOutputFolder;
    else
    {
        // default value - use file title part of filename
        try {
            std::filesystem::path p(szFilename);
            sOutputFolder = p.stem().string();
        }
        catch (...) {
            std::cout << std::endl << "Exception: cannot get file title from session filename" << std::endl;
            return 0;
        }
    }

    std::cout << std::endl << "Exporting to TIFF" << std::endl << "--------------------------" << std::endl;

    int iOutputFmt = -1;
    if (szImgFormat != nullptr)
    {
        if (!CxCameraControl::findXiApiEnumValue(szImgFormat, iOutputFmt))
            iOutputFmt = atoi(szImgFormat);
    }
    bool bTiffEnable16bit = true;
    bool bTiffRescale16bit = false;
    if (szTiffEnable16bit != nullptr)
        bTiffEnable16bit = (strcmp(szTiffEnable16bit, "false") != 0);
    if (szTiffRescale16bit != nullptr)
        bTiffRescale16bit = (strcmp(szTiffRescale16bit, "true") == 0);

    int iOnlyFrame = -1;
    if (szOnlyFrame != nullptr)
        iOnlyFrame = atoi(szOnlyFrame)-1;

    aCtl.exportTiffFilesAsync(sOutputFolder, iOutputFmt, bTiffEnable16bit, bTiffRescale16bit, iOnlyFrame, nullptr);

    // wait until exporting finished
    bool bTaskRunning = true;
    while (bTaskRunning)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // output progress
        int iCounterValue;
        if (aCtl.recordingCounter(XI_SRV_COUNTER_DONE, iCounterValue))
            std::cout << "  done: " << iCounterValue;
        if (aCtl.recordingCounter(XI_SRV_COUNTER_TOTAL, iCounterValue))
            std::cout << "  total: " << iCounterValue;
        std::cout << std::endl;

        bTaskRunning = (aCtl.status() == CxCameraControl::exsExporting);
    }
    if (iOnlyFrame >= 0)
        std::cout << std::endl << "Done. TIFF file exported to " << sOutputFolder << "/" << aCtl.exportedFilename(0) << std::endl;
    else
        std::cout << std::endl << "Done. TIFF files exported to folder " << sOutputFolder << std::endl;

    aCtl.releaseDataBuffers(false);

    std::cout << std::endl;
    return 1;
}
