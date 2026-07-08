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
//  \file ConfigAndGrab_Console_OpenCV.cpp
//
//  \brief
//  This example is an application in command line to show the cameras available,
//  how to connect with the selected camera and show a window with the images 
//  from the camera. 
//  OpenCV library is used to display the images.
//
//  Description: The main functionality of the example is to show the grayscale images 
//  from the camera using OpenCV libraries. The example shows how to capture the raw 
//  data coming from the camera and how to display the image.
//
//  First, all the cameras are shown in command line together with the properties of the 
//  all cameras discovered. The next step is to connect with the camera selected. Then 
//  the camera starts grabbing, and the specific information for each frame is displayed:
//  image counter and real time counter in microseconds (time stamp).
//
//  In a new window is printed the grayscale image corresponding from the camera connected. 
// 
//  IMPORTANT! OpenCV only use 8 bits (Mono8) or 16 bits (Mono16) to display images.
//
//  NOTE: this example does not show color images.
//
//  For more information about OpenCV, visit: http://opencv.org
//
*/
#include <cstdio>
#include <iostream>

#include "PFCamera.h"
#include "PFStreamGEV.h"
#include "PFDiscovery.h"
#include "PFImage.h"

// Headers used for OpenCV libraries
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdint.h>
#include <inttypes.h>

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

using namespace cv;
using namespace std;
using namespace pfcameralib;
using namespace pfcameralib::image::type_util;

int Configure(PFCamera& pfCamera, int64_t& width, int64_t& height);
int GrabAndDisplayImages(PFStream* pfStream, const int width, const int height);

int main()
{
    PFDiscovery pfDiscover;
    PFCamera pfCamera;
    PFCameraInfo *pfCameraInfo;     
    PFStream *pfStream;
    PFResult pfResult;
    uint8_t i, camera;
    uint16_t selected;
    int64_t width, height;

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
    else {
        cout << "No cameras found." << endl;
        _getch();
        return -1;
    }

    // Get the information of the selected camera and keep it in pfCameraInfo
    pfResult = pfDiscover.GetCameraInfo(pfCameraInfo, camera);
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
    Configure(pfCamera, width, height);

    // In order to grab images it is necessary to prepare a proper stream.
    if (pfCameraInfo->GetType() == CAMTYPE_GEV)
        pfStream = new PFStreamGEV(true, true, true, true);
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

    GrabAndDisplayImages(pfStream, (const int)width, (const int)height);

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

int Configure(PFCamera &pfCamera, int64_t &width, int64_t &height)
{
    PFFeatureParameters pfFeatureParams;
    PFResult pfResult;
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
    // OpenCV only works with Mono8 or Mono16 in grayscale pixel format 
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

int GrabAndDisplayImages(PFStream *pfStream, const int width, const int height)
{
    PFResult pfResult;
    PFBufferRef pfBuffer;
    int iter = 0;
    // Create OpenCV Matrix image:
   // 8 bits
   // 1 channel (grayscale)
    Mat img(height, width, CV_8UC1);

    fflush(stdin);

    // Grab images
    cout << "\r\nPress SPACE to stop grabbing...\r\n" << endl;
    while (!KeyPressed(' '))
    {
        // Get from camera image buffer
        pfResult = pfStream->GetNextBuffer(pfBuffer);

        if (pfResult == PFSDK_NOERROR)
        {
            if (iter % 2 == 0)  //Display 1 out of 2 images
            {
                // Set the data pointer to the OpenCV image to show
                // We only assign the pointer returned by the SDK without doing any image copy
                img.data = pfBuffer->GetRawData();

                // Display demodulated image with OpenCV
                imshow("Display window", img);

                // Save last image (OpenCV Mat) as PNG file
                // imwrite("image.png", *img);

                // Show our image inside it
                waitKey(1);

                iter = 0;
            }
            StreamStatistics statistics = pfStream->GetStreamStatistics();
    
            printf("FrameCounter: %" PRId64 " TimeStamp: %" PRId64 " FPS: %05.3f %05.3f Mbps \r", pfBuffer->GetFrameCounter(), pfBuffer->GetTimestamp(), statistics.m_fpsGrab, statistics.m_networkRate);
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
                printf("FrameCounter: %" PRId64 " TimeStamp: %" PRId64 " MissingPackets: %" PRId32 " FrameCorrupted:%u \r\n", pfBuffer->GetFrameCounter(),
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
            
            pfStream->ReleaseBuffer(pfBuffer);
        }
        fflush(stdout);
    }

    cout << endl << "\r\nEnd of grabbing process!" << endl;

    return 0;
}
