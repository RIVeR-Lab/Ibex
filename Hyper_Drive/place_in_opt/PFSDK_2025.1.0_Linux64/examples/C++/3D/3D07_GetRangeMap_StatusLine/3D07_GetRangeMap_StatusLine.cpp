/*
******************************************************************************
* @attention
*
*<h2><center>&copy; COPYRIGHT(c) 2021 Photonfocus AG</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
* 3. Neither the name of Photonfocus nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/
/**
//  \file 3D07_GetRangeMap_StatusLine.cpp
//
//  \brief
//  This example is a command line application that shows how to configure the camera
//  with a set of properties, grab some frames, create a range map and store it as a PLY file.
//
//  Description: The main functionality of this example is to show all the available cameras
//  in the network and to connect to a specific one. After that it shows how to configure
//  some of the camera features, grab some images, extract the 3D information and create a range map.
//  The StatusLine is extracted and processed accordingly MV4 Luxima 3D07 Specification.
//  Finally the camera is freezed and disconnected.
//
//  The PFBufferReader3D class is used to extract the 3D information (i.e. laser line peak positions)
//  from the raw image buffer received from the camera. For every frame, this class will create a profile.
//  Take into account that the camera just delivers the peak positions in pixel units, thus uncalibrated units.
//
//  The laser profiles can be stored using the PFProfileMap class in order to create a range map.
//
//  The range map can be later stored as PLY file.
//
*/
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <iomanip>
#include <inttypes.h>

#include "PFCamera.h"
#include "PFCameraConfig.h"
#include "PFStreamGEV.h"
#include "PFDiscovery.h"
#include "PFImage.h"
#include "PFBufferReader3D.h"
#include "PFProfileMap.h"

// Stream options
#define ENABLE_FILTER_DRIVER   false
#define ENABLE_PACKET_RESEND   true
#define ENABLE_FIND_MAX_PACKET false
#define STREAM_CORRUPT_FRAMES  true


#ifdef WIN32
#include <Windows.h>
#include <conio.h>
#else
#include <unistd.h>  /* only for sleep() */
#include <sys/select.h>
#include <sys/ioctl.h>
#include <termios.h>
#if HAVE_STROPTS_H
    #include <stropts.h>
#endif

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

char _getch(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
 }
#endif

int KeyPressed(char key)
{
    return (_kbhit() != 0) && (_getch() == key);
}

using namespace PFCameraDLL;
using namespace pf3DLib;

int Configure_MVx_D1280_L01_3D07(PFCamera &pfCamera);

int GrabImages(PFCamera &pfCamera, PFStream *pfStream, uint32_t numProfilesToCapture);
int processStatusLine(PFBufferRef pfBuffer, uint32_t width, uint32_t height);

void checkFilterDriverVersion(PFStream* pfStream);

// The minimum profiles per rangemap
// This is set so performance while capturing is acceptable, sice capturing and setting the buffers/saving on the components is single threaded on this example,
// It's not a sdk or component limitation.
const int minProfiles=10;

int main()
{
    PFDiscovery pfDiscover;
    PFCamera pfCamera;
    PFCameraInfo *pfCameraInfo;
    PFStream *pfStream;
    PFResult pfResult;
    uint8_t i, cameraIndex;
    uint16_t selected;
    uint32_t numProfilesToCapture;

    // Discover the cameras available in the computer or network
    pfResult = pfDiscover.DiscoverCameras();
    if (pfResult == PFSDK_ERROR_DISCOVERY_NO_CAMERAS_FOUND)
    {
        std::cout << "No cameras found." << std::endl;
        _getch();
        return -1;
    }

    // For each discovered camera, print the model name, manufacturer, version, etc
    std::cout << "\nCameras found: \n" << std::endl;
    for (i = 0; i < pfDiscover.GetCameraCount(); i++)
    {
        pfResult = pfDiscover.GetCameraInfo(pfCameraInfo,i);
        if (pfResult == PFSDK_NOERROR)
        {
            std::cout << i + 1 << "- " << pfCameraInfo->GetModelName() << std::endl;
            pfCameraInfo->printCameraInfo();
        }
    }

    cameraIndex = 0;
    selected = 1;
    // Select one of the following cameras to connect
    if (i > 0)
    {
        std::cout << "Select a camera from the list: ";
        std::cin >> selected;
        cameraIndex = selected-1;
    }

    // Get the information of the selected camera and keep it in pfCameraInfo
    pfResult = pfDiscover.GetCameraInfo(pfCameraInfo, cameraIndex);
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << std::endl;
        _getch();
        return -1;
    }

    do{
        // Get the number of profiles to capture:
        std::cout << "Get the number of profiles per RangeMap to capture: ";
        std::cin >> numProfilesToCapture;
        if(numProfilesToCapture<minProfiles){
            std::cout << std::endl;
            std::cout << "Enter a number greater than " << minProfiles << std::endl;
        }
    }while(numProfilesToCapture<minProfiles);

    // Connect the camera using pfCameraInfo
    std::cout << "Connecting camera " << selected << " ..." << std::endl;
    pfResult = pfCamera.Connect(*pfCameraInfo);
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << std::endl;
        _getch();
        return -1;
    }

    // While debugging it is advisable to configure a HeartbeatRate of at least 10 seconds.
#ifdef _DEBUG
    pfResult = pfCamera.SetHeartbeatRate(10000);
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << std::endl;
        _getch();
        return -1;
    }
#endif

    // Configure some camera features
    std::string model(pfCameraInfo->GetModelName());
    if(model.find_first_of("3D07")){
        Configure_MVx_D1280_L01_3D07(pfCamera);
    }else{
        std::cout << "Camera no Supported!" << std::endl;
        _getch();
        return -1;
    }

    // Set the SCPS PacketSize for a proper streaming
    uint16_t packetSize = 8228;
    // Check if the value is inside the limits
    PFFeatureParameters pfFeatureParams;
    pfResult = pfCamera.GetFeatureParams("GevSCPSPacketSize", &pfFeatureParams);
    if (packetSize > pfFeatureParams.Max)
        packetSize = (uint16_t)pfFeatureParams.Max;
    else if (packetSize < pfFeatureParams.Min)
        packetSize = (uint16_t)pfFeatureParams.Min;
    pfResult = pfCamera.SetFeatureInt("GevSCPSPacketSize", packetSize);
    if (pfResult != PFSDK_NOERROR)
        std::cout << "Error: " << pfResult.GetDescription() << std::endl;
    else
        std::cout << "GevSCPSPacketSize: " << packetSize << std::endl;

    // In order to grab images it is necessary to prepare a proper stream.
    pfStream = new PFStreamGEV(ENABLE_FILTER_DRIVER, ENABLE_PACKET_RESEND, ENABLE_FIND_MAX_PACKET, STREAM_CORRUPT_FRAMES);

    // Default Ring buffer Count
    pfStream->SetBufferCount(1000);
    // It is mandatory to add this stream to the camera before grabbing images.
    pfResult = pfCamera.AddStream(pfStream);
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << std::endl;
        _getch();
        return -1;
    }

    checkFilterDriverVersion(pfStream);

    GrabImages(pfCamera, pfStream, numProfilesToCapture);

    // Disconnect the camera
    pfCamera.Disconnect();

    // Finally the stream pointer is deleted
    if (pfStream != nullptr)
        delete pfStream;

    std::cout << "\r\nPress any key to finish...\r\n" << std::endl;
    _getch();
    return 0;
}

void checkFilterDriverVersion(PFStream* pfStream)
{
  // Get filter driver msg
  if (ENABLE_FILTER_DRIVER) {
    char msg_version[256];

    PFResult pfResult = pfStream->GetFilterDriverVersion(msg_version);
    if (pfResult != PFSDK_NOERROR) {
      // Report error and continue (filter driver might be unistalled or not available!)
      std::cout << "Not using filter driver! error: " << pfResult.GetDescription() << std::endl;
    }
    else {
      std::cout << "Filter driver version: " << msg_version << std::endl;
    }
  }
  else {
    std::cout << "Not using filter driver! " << std::endl;
  }
}

int GrabImages(PFCamera &pfCamera, PFStream *pfStream, uint32_t numProfilesToCapture)
{
    PFResult pfResult;
    PFBufferRef pfBuffer;
    char ch = 0;
    double fps;
    double networkRate;

    int64_t int_value;
    uint32_t width, height;
    size_t maxProfiles = numProfilesToCapture;
    uint8_t numberOfPeaks = 1;
   

    // The class PFBufferReader3D extracts the 3D information from the grabbed frames. 
    // To work properly it has to be configured setting values to certain tags, such as:
    // "3D07#Width=1280;LineFinder_Mode=2;LineFinder_NumberOfPeaks=4;LineFinder_PeakHeight_Enable=false;LineFinder_PeakWidth_Enable=true;FrameCombine_NrOfFrames=1;"
    // The method GetConfigString() does this job for you.
    char configString[1000];
    PFCameraConfig::GetConfigString(pfCamera, configString, PFCameraConfig::BUFFERREADER_3D);
    // Initialize PFBufferReader3D with the proper configuration string
    PFBufferReader3D pfBufferReader3D(configString);

    // Get Width value
    pfResult = pfCamera.GetFeatureInt("Width", int_value);
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << std::endl;
        _getch();
        return -1;
    }
    width = (uint32_t)int_value;


    // Get Height value
    pfResult = pfCamera.GetFeatureInt("Height", int_value);
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << std::endl;
        _getch();
        return -1;
    }
    height = (uint32_t)int_value;


    pfResult = pfCamera.GetFeatureInt("LineFinder_NumberOfPeaks", int_value);
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << std::endl;
        _getch();
        return -1;
    }
    numberOfPeaks = (uint8_t)int_value;
    
    // Prepare the Range Maps
    std::vector<PFProfileMap> rangeMapArray;
    rangeMapArray.reserve(numberOfPeaks);
    for (int i = 0; i < numberOfPeaks; i++)
        rangeMapArray.emplace_back(width, maxProfiles);

    // Prepare the first Qualifier Maps
    std::vector<PFProfileMap> qualifier0MapArray;
    qualifier0MapArray.reserve(numberOfPeaks);
    for (int i = 0; i < numberOfPeaks; i++)
        qualifier0MapArray.emplace_back(width, maxProfiles);

    // Prepare the second Qualifier Maps
    std::vector<PFProfileMap> qualifier1MapArray;
    qualifier1MapArray.reserve(numberOfPeaks);
    for (int i = 0; i < numberOfPeaks; i++)
        qualifier1MapArray.emplace_back(width, maxProfiles);

    // Start grabbing images
    pfResult = pfCamera.Grab();
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << std::endl;
        // Stop grabbing
        pfCamera.Freeze();
        return -2;
    }

    // Grab images
    fflush(stdin);

    std::cout << "\r\nPress SPACE to stop grabbing...\r\n" << std::endl;

    PFImage pfImage;

    // Add profiles to the rangemap until MAXPROFILES reached or SPACE is pressed
    while (ch != ' ')
    {
        while (!rangeMapArray[0].isFull())
        {
            // Get from camera image buffer
            pfResult = pfStream->GetNextBuffer(pfBuffer);

            if (pfResult == PFSDK_NOERROR)
            {
                // Construct PFImage object
                // The image data is managed inside the class and released in the destructor
                pfBuffer->GetImage(pfImage);

                pfBufferReader3D.notify(pfBuffer->GetRawData(), pfBuffer->GetSizeX(), pfBuffer->GetSizeY());

                size_t numFrames = pfBufferReader3D.getAvailable3DFrames();
                for (size_t frameIterator = 0; frameIterator < numFrames; frameIterator++)
                {
                    if(rangeMapArray[0].isFull())
                        break;

                    size_t  numPeaks = pfBufferReader3D.getAvailablePeaks();
                    for (size_t peakIterator = 0; peakIterator < numPeaks; peakIterator++)
                    {
                        // Get laser line peak position (profile)
                        float *profile = rangeMapArray[peakIterator].getNewProfile();
                        pfBufferReader3D.getProfile(profile, frameIterator, peakIterator);

                        // Get the first qualifier. If both qualifiers are enabled this will be the laser line width.
                        float *profileWidth = qualifier0MapArray[peakIterator].getNewProfile();
                        pfBufferReader3D.getQualifier(0, profileWidth, frameIterator, peakIterator);

                        // Get the second qualifier. If both qualifiers are enabled this will be the laser line intensity (or height). 
                        float *profileHeight = qualifier1MapArray[peakIterator].getNewProfile();
                        pfBufferReader3D.getQualifier(1, profileHeight, frameIterator, peakIterator);
                    }
                }

                fps = pfStream->GetStreamStatistics().m_fpsGrab;
                networkRate = pfStream->GetStreamStatistics().m_networkRate;
                std::cout << "FrameCounter: " << pfBuffer->GetFrameCounter() << " TimeStamp: " << pfBuffer->GetTimestamp() << 
                std::fixed << std::setw(5) << std::setprecision(3) << std::setfill('0') << " FPS: " << fps << " " << networkRate << " Mbps \r";
                
                // Note: Release the image buffer. It's mandatory to call ReleaseBuffer() after each iteration.
                pfStream->ReleaseBuffer(pfBuffer);
            }
            else
            {
                std::cout << "\nError: " << pfResult.GetDescription() << "\r\n";
                if (pfResult == PFSDK_ERROR_GETIMAGE_MISSING_PACKETS || pfResult == PFSDK_ERROR_GETIMAGE_GRAB_ERROR)
                {
                    // If the streamCorruptFrames option of the PFStreamGEV is enabled, pfBuffer will contain the corrupted frame and metadata, including MissingPacketCount.
                    std::cout << "FrameCounter: " <<  pfBuffer->GetFrameCounter() << " TimeStamp: " << pfBuffer->GetTimestamp() << " MissingPackets: " <<  
                    pfBuffer->GetMissingPacketCount() << " FrameCorrupted: " << pfBuffer->IsFrameCorrupted() << "\r\n";        
                           
                    // Construct PFImage object
                    // The image data is managed inside the class and released in the destructor
                    pfBuffer->GetImage(pfImage);
                    // Save image to a file
                    pfImage.SaveToFile("image_error.bmp", PFCameraDLL::image::type_util::BmpFileType);

                    // Note: Release the image buffer. It's mandatory to call ReleaseBuffer() after each iteration.
                    pfStream->ReleaseBuffer(pfBuffer);
                }
                else if (pfResult == PFSDK_ERROR_GETIMAGE_TIMEOUT){
                    std::cout << "Timeout error!\r\n";
                        // If there are some profiles in the range map already, then break the current scan and allow the viewers to show the grabbed profiles.
                        if (rangeMapArray[0].getProfilesCount() > 0)
                            break;
                }
            }
        }

        // If there are at least 10 profiles in the range map, save it PLY file
        if (rangeMapArray[0].getProfilesCount() >= minProfiles)
        {
            std::cout << "\r\nSaving range map as PLY file...\r\n" << std::endl;

            for (int peakIterator = 0; peakIterator < numberOfPeaks; peakIterator++)
            {
                std::ostringstream fileName;
                fileName << "pointcloud_" << peakIterator << ".ply";
                rangeMapArray[peakIterator].saveAsPLYFile(fileName.str().c_str());
            }
        }

        // Clear all the maps
        for (int i = 0; i < numberOfPeaks; i++)
        {
            rangeMapArray[i].reset();
            qualifier0MapArray[i].reset();
            qualifier1MapArray[i].reset();
        }

        // Extract and process status liene
        processStatusLine(pfBuffer, width, height);

       
        if (_kbhit())
            ch = _getch();
    }

    // Stop grabbing
    pfCamera.Freeze();

    std::cout << std::endl << "\r\nAcqusition has stopped." << std::endl;
    std::cout << std::endl << "\r\nClose the window to finish." << std::endl;

    return 0;
}


int processStatusLine(PFBufferRef pfBuffer, uint32_t width, uint32_t height)
{
    // Obtain status line and parse each item
    uint8_t* pDataPointer = pfBuffer->GetRawData();

    // Select the last row of the image to get the Status Line
    uint8_t* pStatusLine = &(pDataPointer[width * (height - 1)]);

    // Counter 0 Value_0 (4 Bytes). 
    uint32_t Counter0 = *(uint32_t*)(pStatusLine);
    // Get the Counter_1 (4 Bytes). 
    uint32_t Counter1 = *(uint32_t*)(pStatusLine + 4);
    // Get the Counter_2 (4 Bytes). 
    uint32_t Counter2 = *(uint32_t*)(pStatusLine + 8);
    // Get the Counter_3 (4 Bytes). 
    uint32_t Counter3 = *(uint32_t*)(pStatusLine + 12);
    // Get the Encoder Position (4 Bytes). 
    uint32_t EncPos = *(uint32_t*)(pStatusLine + 16);
    // Line input level
    uint8_t TrigLevel = *(uint8_t*)(pStatusLine + 20);
    // Roi 
    uint32_t RoiParams = *(uint32_t*)(pStatusLine + 24);
    // Roi Col Warn  
    uint8_t RoiColWarn = RoiParams & 1;
    // Roi Border
    uint8_t RoiBorder = RoiParams & 2;
    // Roi Laser Finder
    uint8_t RoiLaserFinder = RoiParams & 4;
    // Roi Y
    uint16_t RoiY = (RoiParams & 0x3FF0) >> 4;
    // Roi MinMax params
    uint16_t RoiMinPos = *(uint16_t*)(pStatusLine + 28);
    // Roi Min position (10 lower bits)
    RoiMinPos = RoiMinPos & 0x3FF;
    // Roi Max position 
    uint16_t RoiMaxPos = *(uint16_t*)(pStatusLine + 30);
    RoiMaxPos = RoiMaxPos & 0x3FF;
    // Algorithm
    uint8_t Alg3D = *(uint8_t*)(pStatusLine + 32);
    Alg3D = Alg3D & 3;
    // TypeCode
    uint16_t TypeCode = *(uint16_t*)(pStatusLine + 36);
    TypeCode = TypeCode & 0x7FFF;
    // Serial number
    uint32_t SerialNumber = *(uint32_t*)(pStatusLine + 40);
    // Custom Value0
    uint32_t Custom0 = *(uint32_t*)(pStatusLine + 44);
    // Custom Value1
    uint32_t Custom1 = *(uint32_t*)(pStatusLine + 48);

    printf("FrameCounter: %6" PRId64 " | TimeStamp: %12" PRId64 "| Counter1: %6d | Counter2: %6d | Counter3: %6d | Counter4: %6d\r\n", pfBuffer->GetFrameCounter(), pfBuffer->GetTimestamp(),
     Counter0, Counter1, Counter2, Counter3);
    
    printf("EncPos: %6" PRId32 "| LineInputLevel: %2d \r\n", EncPos, TrigLevel);
    printf("RoiColWarn: %2d | RoiBorder: %2d |RoiLaserFinder: %2d | RoiY: %4d |RoiMinPos: %4d | RoiMaxPos: %4d \r\n", RoiColWarn, RoiBorder, RoiLaserFinder, RoiY, RoiMinPos, RoiMaxPos);
    printf("Algorithm 3D: %d |TypeCode: %04X | Serial: %6d | Custom0: %6d | Custom1: %6d \r\n", Alg3D, TypeCode, SerialNumber, Custom0, Custom1);

    return 0;
}
