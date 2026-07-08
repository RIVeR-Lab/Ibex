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
//  \file DiscoverConfigAndGrab_Console.cpp
//
//  \brief
//  This example is a command line application that shows the generic process to discover the cameras 
//  in the computer or network, connect to a camera, grab images and disconnect.
//
//  Description: The main functionality of this example is to show all the available cameras in the 
//  computer or network and to connect to a specific one. After that it shows how to configure 
//  some of the camera features and grab images until the 'space' key is pressed.
//  Finally the camera is freezed and disconnected.
//
*/
#include <cstdio>
#include <iostream>
#include <cinttypes>

#include "PFCamera.h"
#include "PFStreamGEV.h"
#include "PFDiscovery.h"
#include "PFImage.h"
#include "PFBufferRef.h"

// Stream options
#define ENABLE_FILTER_DRIVER   true
#define ENABLE_PACKET_RESEND   true
#define ENABLE_FIND_MAX_PACKET true
#define STREAM_CORRUPT_FRAMES  true

#ifdef WIN32
#include <Windows.h>
#include <conio.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <termios.h>
#if HAVE_STROPTS_H
    #include <stropts.h>
#endif

#define _getcwd getcwd
#define MAX_PATH  4096

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (!initialized) {
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
    struct termios old = { 0 };
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}
#endif

int KeyPressed(char key)
{
    return (_kbhit() != 0) && (tolower(_getch()) == tolower(key));
}


using namespace std;
using namespace pfcameralib;
using namespace pfcameralib::image::type_util;


int Configure(PFCamera &pfCamera);
int GrabImages(PFStream *pfStream);
void checkFilterDriverVersion(PFStream* pfStream);

int main()
{
    PFDiscovery pfDiscover;
    PFCamera pfCamera;
    PFCameraInfo *pfCameraInfo;
    PFStream *pfStream;
    PFResult pfResult;
    uint8_t i, camera;
    uint16_t selected;
    
    // Discover the cameras available in the computer or network
    pfResult = pfDiscover.DiscoverCameras();
    if (pfResult == PFSDK_ERROR_DISCOVERY_NO_CAMERAS_FOUND)
    {
        cout << "No cameras found." << endl;
        _getch();
        return -1;
    }

    // For each discovered camera, print the model name, manufacturer, version, etc
    cout << "\nCameras found: \n" << endl;
    for (i = 0; i < pfDiscover.GetCameraCount(); i++)
    {
        pfResult = pfDiscover.GetCameraInfo(pfCameraInfo,i);
        if (pfResult == PFSDK_NOERROR)
        {
            cout << i + 1 << "- " << pfCameraInfo->GetModelName() << endl;
            pfCameraInfo->printCameraInfo();
        }
    }
        
    // Select one of the following cameras to connect
    if (i > 0)
    {
        cout << "Select a camera from the list: ";
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
        cout << "Error: " << pfResult.GetDescription() << endl;
        _getch();
        return -1;
    }
    
    // Connect the camera using pfCameraInfo
    cout << "Connecting camera " << selected << " ..." << endl;
    pfResult = pfCamera.Connect(*pfCameraInfo);
    if (pfResult != PFSDK_NOERROR)
    {
        cout << "Error: " << pfResult.GetDescription() << endl;
        _getch();
        return -1;
    }           

    // While debugging it is advisable to configure a HeartbeatRate of at least 10 seconds.
    #if !defined(NDEBUG)
    pfResult = pfCamera.SetHeartbeatRate(10000);
    if (pfResult != PFSDK_NOERROR)
    {
        cout << "Error: " << pfResult.GetDescription() << endl;
        _getch();
        return -1;
    }
    #endif
    
    // Configure some camera features
    Configure(pfCamera);

    // In order to grab images it is necessary to prepare a proper stream.
    if (pfCameraInfo->GetType() == CAMTYPE_GEV)
        pfStream = new PFStreamGEV(ENABLE_FILTER_DRIVER, ENABLE_PACKET_RESEND, ENABLE_FIND_MAX_PACKET, STREAM_CORRUPT_FRAMES);
    else{
        cout << "Camera type not supported" << endl;
        _getch();
        return -1;
    }

    // It is mandatory to add this stream to the camera before grabbing images.
    pfResult = pfCamera.AddStream(pfStream);
    if (pfResult != PFSDK_NOERROR)
    {
        cout << "Error: " << pfResult.GetDescription() << endl;
        _getch();
        return -1;
    }

    pfStream->SetBufferCount(100);

    checkFilterDriverVersion(pfStream);

    // Start grabbing images
    pfResult = pfCamera.Grab();
    if (pfResult != PFSDK_NOERROR)
    {
        cout << "Error: " << pfResult.GetDescription() << endl;
        // Stop grabbing
        pfCamera.Freeze();
        // Disconnect the Camera
        pfCamera.Disconnect();
        // Delete stream pointer
        if (pfStream != nullptr)
            delete pfStream;

        cout << "\r\nPress any key to finish...\r\n" << endl;
        _getch();
        return -2;
    }

    GrabImages(pfStream);
    
    // Stop grabbing
    pfCamera.Freeze();
          
    // Disconnect the camera
    pfCamera.Disconnect();

    // Finally the stream pointer is deleted
    if (pfStream != nullptr)
        delete pfStream;
    
    cout << "\r\nPress any key to finish...\r\n" << endl;
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

    // Set Width value
    width = 1280;
    // Check if the value is inside the limits
    pfResult = pfCamera.GetFeatureParams("Width", &pfFeatureParams);
    if (width > pfFeatureParams.Max)
        width = pfFeatureParams.Max;
    else if (width < pfFeatureParams.Min)
        width = pfFeatureParams.Min;
    // Set the corresponding value
    pfResult = pfCamera.SetFeatureInt("Width", width);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back Width value
    pfResult = pfCamera.GetFeatureInt("Width", width);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "Width: " << (uint16_t)width << endl;

    // Set Height value
    height = 1024;
    // Check if the value is inside the limits
    pfResult = pfCamera.GetFeatureParams("Height", &pfFeatureParams);
    if (height > pfFeatureParams.Max)
        height = pfFeatureParams.Max;
    else if (height < pfFeatureParams.Min)
        height = pfFeatureParams.Min;
    // Set the corresponding value
    pfResult = pfCamera.SetFeatureInt("Height", height);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back Height value
    pfResult = pfCamera.GetFeatureInt("Height", height);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "Height: " << (uint16_t)height << endl;
    
    // Set pixel format
    pfResult = pfCamera.SetFeatureEnum("PixelFormat", "Mono8");
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back pixel format
    pfResult = pfCamera.GetFeatureEnum("PixelFormat", enum_str);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "PixelFormat: " << enum_str << endl;

    // Set Exposure Time value
    double_value = 3000.0;
    // Check if the value is inside the limits
    pfResult = pfCamera.GetFeatureParams("ExposureTime", &pfFeatureParams);
    if (double_value > pfFeatureParams.FloatMax)
        double_value = pfFeatureParams.FloatMax;
    else if (double_value < pfFeatureParams.FloatMin)
        double_value = pfFeatureParams.FloatMin;
    // Set the corresponding value
    pfResult = pfCamera.SetFeatureFloat("ExposureTime", double_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back Exposure Time value
    pfResult = pfCamera.GetFeatureFloat("ExposureTime", double_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "ExposureTime: " << double_value << endl;

    // Set the SCPS PacketSize for a proper streaming
    pfResult = pfCamera.GetFeatureParams("GevSCPSPacketSize", &pfFeatureParams);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else{
        pfResult = pfCamera.SetFeatureInt("GevSCPSPacketSize", pfFeatureParams.Max);
        if (pfResult != PFSDK_NOERROR)
            cout << "Error: " << pfResult.GetDescription() << endl;
        else
            cout << "GevSCPSPacketSize: " << pfFeatureParams.Max << endl;
    }

    int64_t iValue;
    pfResult = pfCamera.SetFeatureInt("DigitalOffset", -100);
    if (pfResult != PFSDK_NOERROR)
      cout << "Error: " << pfResult.GetDescription() << endl;
    else {
      pfResult = pfCamera.GetFeatureInt("DigitalOffset", iValue);
      if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
      else
        cout << "Digital Offset is " << iValue << endl;
    }
    
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
      std::cout << "Not using filter driver! error: " << pfResult.GetDescription() << endl;
    }
    else {
      std::cout << "Filter driver version: " << msg_version << std::endl;
    }
  }
  else {
    std::cout << "Not using filter driver! " << std::endl;
  }
}

int GrabImages(PFStream *pfStream)
{
    PFResult pfResult;
    PFBufferRef pfBuffer;
    int iter = 0;

    fflush(stdin);

    // Grab images
    cout << "\r\nPress SPACE to stop grabbing...\r\n" << endl;
    while (!KeyPressed(' '))
    {
        // Get from camera image buffer
        pfResult = pfStream->GetNextBuffer(pfBuffer);

        if (pfResult == PFSDK_NOERROR)
        {
            if (iter % 100 == 0) //Save 1 out of 100 images
            {
                PFImage pfImage;
                // Construct PFImage object
                // The image data is managed inside the class and released in the destructor
                pfBuffer->GetImage(pfImage);
                // Save image to a file
                pfImage.SaveToFile("image.bmp", pfImageFileType::BmpFileType);
                iter = 0;
            }
            StreamStatistics statistics = pfStream->GetStreamStatistics();
    
            printf("FrameCounter: %" PRId64 " TimeStamp: %" PRIu64 " FPS: %05.3f %05.3f Mbps \r", pfBuffer->GetFrameCounter(), pfBuffer->GetTimestamp(), statistics.m_fpsGrab, statistics.m_networkRate);
            // Note: Release the image buffer. It's mandatory to call ReleaseBuffer() after each iteration.
            pfStream->ReleaseBuffer(pfBuffer);
            iter++;
        }
        else
        {
            cout << "\nError: " << pfResult.GetDescription() << "\r\n";
            if (pfResult == PFSDK_ERROR_GETIMAGE_MISSING_PACKETS || pfResult == PFSDK_ERROR_GETIMAGE_GRAB_ERROR)
            {
                // If the streamCorruptFrames option of the PFStreamGEV is enabled, pfBuffer will contain the corrupted frame and metadata, including MissingPacketCount.
                printf("FrameCounter: %" PRId64 " TimeStamp: %" PRIu64 " MissingPackets: %" PRIu32 " FrameCorrupted:%u \r\n", pfBuffer->GetFrameCounter(),
                        pfBuffer->GetTimestamp(), pfBuffer->GetMissingPacketCount(), pfBuffer->IsFrameCorrupted());
                PFImage pfImage;
                // Construct PFImage object
                // The image data is managed inside the class and released in the destructor
                pfBuffer->GetImage(pfImage);
                // Save image to a file
                pfImage.SaveToFile("image_error.bmp", pfImageFileType::BmpFileType);
            }
            else if (pfResult == PFSDK_ERROR_GETIMAGE_TIMEOUT){
                cout << "Timeout error!\r\n";
            }
            
            // Note: Release the image buffer. It's mandatory to call ReleaseBuffer() after each iteration.
                pfStream->ReleaseBuffer(pfBuffer);

        }
        fflush(stdout);
    }

    cout << endl << "\r\nEnd of grabbing process!" << endl;

    return 0;
}
