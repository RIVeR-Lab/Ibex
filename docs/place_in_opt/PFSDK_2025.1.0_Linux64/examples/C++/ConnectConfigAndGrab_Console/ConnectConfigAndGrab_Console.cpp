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
//  \file ConnectConfigAndGrab_Console.cpp
//
//  \brief
//  This example shows how to connect to a camera with different connection parameters, such as IP address or MAC address.
//  After the camera is connected, it grabs images until the 'space' key is pressed. Then, the camera is freezed and disconnected.
//
//  Description: The main functionality of the example is to connect to a camera with
//  a set of input arguments:
//
//  Available input parameters:
//      TACS: {ip, mac, deviceid, file}
//      TACS VALUES: i.e. {"192.168.1.1","00:11:1C:F5:AE:9A",char *deviceID, char *FileName}
//
//  The first argument is the number of input parameters: (2, 4, 6 ...)
//
//   Option 1:
//      pfResult = pfCamera.Connect(2, "mac", "00:11:1C:F5:AE:9B");
//   Option 2:
//      pfResult = pfCamera.Connect(2, "ip", "192.168.1.1");
//   Option 3:
//      pfResult = pfCamera.Connect(2, "deviceid", deviceID);
//   Option 4:
//      pfResult = pfCamera.Connect(2, "file", FileName);
//
//  After the camera is connected, it grabs images until the 'space' key is pressed. Then, the camera is freezed and disconnected.
//
*/
#include <cstdio>
#include <iostream>
#include <cinttypes>

#include "PFCamera.h"
#include "PFStreamGEV.h"
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


using namespace pfcameralib;
using namespace pfcameralib::image::type_util;

using std::cout;
using std::endl;

int Configure(PFCamera &pfCamera);
int GrabImages(PFStream *pfStream);
void checkFilterDriverVersion(PFStream* pfStream);

int main()
{
    PFCamera pfCamera;
    PFCameraInfo *pfCameraInfo;
    PFStream *pfStream;
    PFResult pfResult;

    cout << "Connecting camera..." << endl;

    // Change the function input parameters to match your device:
    // Connect using MAC
    //pfResult = pfCamera.Connect(2, "mac", "00:11:1C:F5:AF:9B");
    //Connect using IP Address
    pfResult = pfCamera.Connect(2, "ip", "169.254.3.45");
    if (pfResult != PFSDK_NOERROR)
    {
        cout << "Error: " << pfResult.GetDescription() << endl;
        _getch();
        return -1;
    }

    // Get the information of the connected camera and keep it in pfCameraInfo
    pfResult = pfCamera.GetCameraInfo(pfCameraInfo);
    if (pfResult != PFSDK_NOERROR)
    {
        cout << "Error: " << pfResult.GetDescription() << endl;
        _getch();
        return -1;
    }
    cout << "\r\nConnected to camera: " << endl;
    pfCameraInfo->printCameraInfo();

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
    pfStream = new PFStreamGEV(ENABLE_FILTER_DRIVER, ENABLE_PACKET_RESEND, ENABLE_FIND_MAX_PACKET, STREAM_CORRUPT_FRAMES);
    // It is mandatory to add this stream to the camera before grabbing images.
    pfResult = pfCamera.AddStream(pfStream);
    if (pfResult != PFSDK_NOERROR)
    {
        cout << "Error: " << pfResult.GetDescription() << endl;
        _getch();
        return -1;
    }

    checkFilterDriverVersion(pfStream);

    //Start grabbing images
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

    return 0;
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
                printf("FrameCounter: %" PRId64 " TimeStamp: %" PRIu64 " MissingPackets: %" PRId32 " FrameCorrupted:%u \r\n", pfBuffer->GetFrameCounter(), 
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
