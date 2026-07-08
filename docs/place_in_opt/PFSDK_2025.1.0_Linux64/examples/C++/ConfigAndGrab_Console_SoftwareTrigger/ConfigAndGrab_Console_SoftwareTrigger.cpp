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
//  \file ConfigAndGrab_Console_SoftwareTrigger.cpp
//
//  \brief
//  This example is a command line application that shows the generic process to discover the cameras 
//  in the computer or network, connect to a camera, grab images using a software trigger, and disconnect.
//
//  Description: The main functionality of this example is to show all the available cameras in the 
//  computer or network and to connect to a specific one. After that it shows how to configure 
//  some of the camera features and grab images until the 'space' key is pressed.
//  Finally the camera is freezed and disconnected.
//
*/
#include <cstdio>
#include <iostream>
#include <inttypes.h>
#include <atomic>
#include <chrono>
#include <thread>

#include <stdint.h>
#include <inttypes.h>

#include "PFCamera.h"
#include "PFStreamGEV.h"
#include "PFStreamU3V.h"
#include "PFDiscovery.h"
#include "PFImage.h"

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

using namespace std;
using namespace pfcameralib;

int Configure(PFCamera &pfCamera);
int GrabImages(PFCamera &pfCamera, PFStream *pfStream);

int main()
{
    PFDiscovery pfDiscover;
    PFCamera pfCamera;
    PFCameraInfo *pfCameraInfo; 
    PFStream *pfStream;
    PFResult pfResult;
    uint8_t i,camera;
    uint16_t selected;
        
        
    // Discover the cameras available in the computer or network
    pfResult = pfDiscover.DiscoverCameras();
    if (pfResult == PFSDK_ERROR_DISCOVERY_NO_CAMERAS_FOUND)
    {
        std::cout << "No cameras found." << endl;
        _getch();
        return -1;
    }

    // For each discovered camera, print the model name, manufacturer, version, etc
    std::cout << "\nCameras found: \n" << endl;
    for (i = 0; i < pfDiscover.GetCameraCount(); i++)
    {
        pfResult = pfDiscover.GetCameraInfo(pfCameraInfo,i);
        if (pfResult == PFSDK_NOERROR)
        {
            std::cout << i + 1 << "- " << pfCameraInfo->GetModelName() << " Manufacturer info: "  << pfCameraInfo->GetManufacturerInfo() <<  endl;
            pfCameraInfo->printCameraInfo();
        }
    }
    
    // Select one of the following cameras to connect
    if (i > 0)
    {
        std::cout << "Select a camera from the list: ";
        cin >> selected;
        camera = static_cast<uint8_t>(selected)-1;
    }
    else
    {
        cout << "No cameras found." << endl;
        _getch();
        return 0;
    }

    // Get the information of the selected camera and keep it in pfCameraInfo
    pfResult = pfDiscover.GetCameraInfo(pfCameraInfo,camera);
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << endl;
        _getch();
        return -1;
    }

    // Connect the camera using pfCameraInfo
    std::cout << "Connecting camera " << selected << " ..." << endl;
    pfResult = pfCamera.Connect(*pfCameraInfo);
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << endl;
        _getch();
        return -1;
    }           

    // While debugging it is advisable to configure a HeartbeatRate of at least 10 seconds.
#if !defined(NDEBUG)
    pfResult = pfCamera.SetHeartbeatRate(10000);
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << endl;
        _getch();
        return -1;
    }
#endif
    
    // Configure some camera features
    Configure(pfCamera);

    // Set the SCPS PacketSize for a proper streaming
    int packetSize = 8164;
    pfResult = pfCamera.SetFeatureInt("GevSCPSPacketSize", packetSize);
    if (pfResult != PFSDK_NOERROR)
        std::cout << "Error: " << pfResult.GetDescription() << endl;
    else
        std::cout << "GevSCPSPacketSize: " << packetSize << endl;

    // In order to grab images it is necessary to prepare a proper stream.
    if (pfCameraInfo->GetType() == CAMTYPE_GEV)
        pfStream = new PFStreamGEV(true, false, true, true);
    else
        pfStream = new PFStreamU3V();
    
    // Default Ring buffer Count
    pfStream->SetBufferCount(100);
    // It is mandatory to add this stream to the camera before grabbing images.
    pfResult = pfCamera.AddStream(pfStream);
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << endl;
        _getch();
        return -1;
    }

    // Start grabbing images
    pfResult = pfCamera.Grab();

    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << endl;
        // Stop grabbing
        pfCamera.Freeze();
        // Disconnect the Camera
        pfCamera.Disconnect();
        // Delete stream pointer
        if (pfStream != nullptr)
            delete pfStream;

        std::cout << "\r\nPress any key to finish...\r\n" << endl;
        _getch();
        return -2;
    }
    
    GrabImages(pfCamera, pfStream);

    // Stop grabbing
    pfCamera.Freeze();
    // Disconnect the camera
    pfCamera.Disconnect();

    // Finally the stream pointer is deleted
    if (pfStream != nullptr)
        delete pfStream;
    
    std::cout << "\r\nPress any key to finish...\r\n" << endl;
    _getch();
    return 0;
}

int Configure(PFCamera &pfCamera)
{
    PFFeatureParameters pfFeatureParams;
    PFResult pfResult;
    int64_t width, height;
    double double_value;
    char enum_str[64];

    //Enable trigger mode
    pfResult = pfCamera.SetFeatureEnum("TriggerMode", "On");
    if (pfResult != PFSDK_NOERROR)
        std::cout << "Error: " << pfResult.GetDescription() << endl;

    //Select software trigger source
    pfResult = pfCamera.SetFeatureEnum("TriggerSource", "Software");
    if (pfResult != PFSDK_NOERROR)
        std::cout << "Error: " << pfResult.GetDescription() << endl;

    // Check if the value is inside the limits
    pfResult = pfCamera.GetFeatureParams("Width", &pfFeatureParams);
    width = pfFeatureParams.Max;

    // Set the corresponding value
    pfResult = pfCamera.SetFeatureInt("Width", width);
    if (pfResult != PFSDK_NOERROR)
        std::cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back Width value
    pfResult = pfCamera.GetFeatureInt("Width", width);
    if (pfResult != PFSDK_NOERROR)
        std::cout << "Error: " << pfResult.GetDescription() << endl;
    else
        std::cout << "Width: " << (uint16_t)width << endl;

    // Check if the value is inside the limits
    pfResult = pfCamera.GetFeatureParams("Height", &pfFeatureParams);
    height = pfFeatureParams.Max;

    // Set the corresponding value
    pfResult = pfCamera.SetFeatureInt("Height", height);
    if (pfResult != PFSDK_NOERROR)
        std::cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back Height value
    pfResult = pfCamera.GetFeatureInt("Height", height);
    if (pfResult != PFSDK_NOERROR)
        std::cout << "Error: " << pfResult.GetDescription() << endl;
    else
        std::cout << "Height: " << (uint16_t)height << endl;

    // Set pixel format
    pfResult = pfCamera.SetFeatureEnum("PixelFormat", "Mono8");
    if (pfResult != PFSDK_NOERROR)
        std::cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back pixel format
    pfResult = pfCamera.GetFeatureEnum("PixelFormat", enum_str);
    if (pfResult != PFSDK_NOERROR)
        std::cout << "Error: " << pfResult.GetDescription() << endl;
    else
        std::cout << "PixelFormat: " << enum_str << endl;

    // Set Exposure Time value
    double_value = 1000.0;
    // Check if the value is inside the limits
    pfResult = pfCamera.GetFeatureParams("ExposureTime", &pfFeatureParams);
    if (double_value > pfFeatureParams.FloatMax)
        double_value = pfFeatureParams.FloatMax;
    else if (double_value < pfFeatureParams.FloatMin)
        double_value = pfFeatureParams.FloatMin;
    // Set the corresponding value
    pfResult = pfCamera.SetFeatureFloat("ExposureTime", double_value);
    if (pfResult != PFSDK_NOERROR)
        std::cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back Exposure Time value
    pfResult = pfCamera.GetFeatureFloat("ExposureTime", double_value);
    if (pfResult != PFSDK_NOERROR)
        std::cout << "Error: " << pfResult.GetDescription() << endl;
    else
        std::cout << "ExposureTime: " << double_value << endl;

    return 0;
}

int GrabImages(PFCamera& pfCamera, PFStream *pfStream)
{
    PFResult pfResult;
    PFBufferRef pfBuffer;
    int iter = 0;
    int64_t oldFrameCounter = 0;
    StreamStatistics statistics;
    // Grab images
    fflush(stdin);

    std::cout << "\r\nPress SPACE to stop grabbing...\r\n" << endl;
    while (!KeyPressed(' '))
    {
        auto triggerResult = pfCamera.SetFeatureCommand("TriggerSoftware", 1);
        if (triggerResult != PFSDK_NOERROR)
            std::cout << "Error: " << triggerResult.GetDescription() << endl;
        // Get from camera image buffer
        pfResult = pfStream->GetNextBuffer(pfBuffer);
        
        if (pfResult == PFSDK_NOERROR)
        {
        
            /*
            if (iter % 1000 == 0) //Save 1 out of 100 images
            {
                PFImage pfImage;
                // Construct PFImage object
                // The image data is managed inside the class and released in the destructor
                pfBuffer->GetImage(pfImage);
                // Save image to a file
                pfImage.SaveToFile("image.bmp", pfImageFileType::BmpFileType);
                iter = 0;
            }
            */
            statistics = pfStream->GetStreamStatistics();  
            printf("FrameCounter: %" PRId64 " TimeStamp: %" PRIu64 " FPS: %05.3f %05.3f Mbps \r", 
                pfBuffer->GetFrameCounter(), pfBuffer->GetTimestamp(), statistics.m_fpsGrab, statistics.m_networkRate);

            iter++;
        }
        else
        {
            //When there are missing packets, pfbuffer is valid only if stream corrupt frames is enabled.
            if ((pfResult == PFSDK_ERROR_GETIMAGE_MISSING_PACKETS || pfResult == PFSDK_ERROR_GETIMAGE_GRAB_ERROR))
            {
                // If the streamCorruptFrames option of the PFStreamGEV is enabled, pfBuffer will contain the corrupted frame and metadata, including MissingPacketCount.
                printf("FrameCounter: %" PRId64 " TimeStamp: %" PRIu64 " MissingPackets: %" PRIu32 " FrameCorrupted:%u \r\n", pfBuffer->GetFrameCounter(),
                    pfBuffer->GetTimestamp(), pfBuffer->GetMissingPacketCount(), pfBuffer->IsFrameCorrupted());
                PFImage pfImage;
                // Construct PFImage object
                // The image data is managed inside the class and released in the destructor
                pfBuffer->GetImage(pfImage);
                // Save image to a file
                pfImage.SaveToFile("image_error.bmp", image::type_util::BmpFileType);
            }
            else if (pfResult == PFSDK_ERROR_GETIMAGE_TIMEOUT){
                printf("Timeout error!\r\n");
            }
        }

        //Check if any frames were lost. Corrupt frames will count as lost
        //unless the option to stream corrupt frames is enabled.
        int64_t lostFrames;
        if(pfBuffer->GetFrameCounter() > oldFrameCounter)
            lostFrames = pfBuffer->GetFrameCounter() - oldFrameCounter - 1;
        else {
            lostFrames = 65535 - oldFrameCounter - pfBuffer->GetFrameCounter() + 1;
        }

        if (lostFrames)
        {
            printf("\nLost frames: %" PRId64 " - Last(%" PRId64 ") - Current(%" PRId64 ")\n",
                lostFrames, oldFrameCounter, pfBuffer->GetFrameCounter());
        }
        oldFrameCounter = pfBuffer->GetFrameCounter();
      
        // Note: Release the image buffer. It's mandatory to call ReleaseBuffer() 
        fflush(stdout);
        pfStream->ReleaseBuffer(pfBuffer);
    }

    statistics = pfStream->GetStreamStatistics();

    float pct_lost = (statistics.m_lostFrames)*100/(float)statistics.m_totalFrames;
    float pct_error = (statistics.m_errorFrames)*100/(float)statistics.m_totalFrames;
    printf("\n\nTotal frames: %" PRId64 " Lost: %" PRIu64 " (%.3f) Errors: %" PRIu64 " (%.3f)\n",
        statistics.m_totalFrames, statistics.m_lostFrames, pct_lost,
        statistics.m_errorFrames, pct_error);

    std::cout << endl << "\r\nEnd of grabbing process!" << endl;

    return 0;
}
