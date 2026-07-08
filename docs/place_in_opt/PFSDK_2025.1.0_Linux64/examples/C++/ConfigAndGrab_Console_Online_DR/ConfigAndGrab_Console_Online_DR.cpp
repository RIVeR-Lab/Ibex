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
//  \file ConfigAndGrab_Console_Online_DR.cpp
//
//  \brief
//  This example is a command line application that shows the generic process to discover the cameras 
//  in the computer or network, connect to a double rate camera, grab images, demodulate images, save images demodulated to files  
//  and disconnect camera.
//
//  Description: The purpouse of this sample is to show how images in double rate cameras must be captured and demodulated:

//  1. Set "DoubleRate_Enable" feature value to true to capture images at double rate speed.
//  2. Configure parameters for capture images. In this example the camera will capture images using Mono8 pixel type.
//  3. Get "Window_W" feature value to discover which is the real size of the images demodulated.
//  4. After the image is captured can be demodulated with method PFImage::DemodulateDR()
//  5. PFImage object with enough space allocated to perform the demodulation (you need to know the width of image demodulated "Window_W") will be passed to this method.
//  6. After the image is demodulated will be saved to file.

//  The application  will execute  this method until the 'space' key is pressed.
//  Finally the camera is freezed and disconnected.
//
*/
#include <cinttypes>
#include <iostream>

#include "PFCamera.h"
#include "PFStreamGEV.h"
#include "PFStreamU3V.h"
#include "PFDiscovery.h"
#include "PFImage.h"

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


int GrabImages(PFStream *pfStream, int64_t widthDR, int64_t height, bool isColor, pfPixelType pixelType);

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

    bool drEnabled;
    pfResult = pfCamera.GetFeatureBool("DoubleRate_Enable", drEnabled);
    if (pfResult != PFSDK_NOERROR) {
      std::cout << "Expected Double Rate Camera !";
      pfCamera.Disconnect();
      return -1;
    }

    if (!drEnabled) {
      // Set double rate enable
      pfResult = pfCamera.SetFeatureBool("DoubleRate_Enable", true);
      if (pfResult != PFSDK_NOERROR)
      {
        std::cout << "Error: " << pfResult.GetDescription() << "  " << endl;
        _getch();
        return -1;
      }
    }
           
    int64_t widthDR = 0;
    int64_t binX = 0;
   
    pfCamera.GetFeatureInt("Window_W", widthDR);
    // Consider BinningHorizontal
    pfCamera.GetFeatureInt("BinningHorizontal", binX);

    if (binX > 1)
    {
        // Update Width depending on binX value
        widthDR /= binX;
    }

    int64_t width = 0;
    pfCamera.GetFeatureInt("Width", width);
        
    int64_t height = 0;
    pfCamera.GetFeatureInt("Height", height);
 
    // While debugging it is advisable to configure a HeartbeatRate of at least 10 seconds.
    #ifdef _DEBUG
    pfResult = pfCamera.SetHeartbeatRate(10000);
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << endl;
        _getch();
        return -1;
    }
    #endif
        
    // Set pixel format for mochrome capture
    pfPixelType pixelType = PixelMono8;
    pfResult = pfCamera.SetFeatureEnum("PixelFormat", "Mono8");
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << endl;
        _getch();
        return -1;
    }

    // Set the SCPS PacketSize for a proper streaming
    int packetSize = 8164;
    pfResult = pfCamera.SetFeatureInt("GevSCPSPacketSize", packetSize);
    if (pfResult != PFSDK_NOERROR)
        std::cout << "Error: " << pfResult.GetDescription() << endl;
    else
        std::cout << "GevSCPSPacketSize: " << packetSize << endl;

    // In order to grab images it is necessary to prepare a proper stream.
    if (pfCameraInfo->GetType() == CAMTYPE_GEV)
        pfStream = new PFStreamGEV(false, true, true, false);
    else
        pfStream = new PFStreamU3V();
    
    // Default Ring buffer Count
    pfStream->SetBufferCount(500);
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
    
    GrabImages(pfStream, widthDR, height, pfCamera.isColorCamera(), pixelType);
    
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

int GrabImages(PFStream *pfStream, int64_t widthDR, int64_t height, bool isColor, pfPixelType pixelType)
{
    PFImage pfImageDest;
    PFImage pfImage;
    PFResult pfResult;
    PFBufferRef pfBuffer;
    int iter = 0, iter_file = 0;
    double fps;
    double networkRate;
    int64_t oldFrameCounter = 0;
    char filename[256];

    // Allocate image for demodulation
    pfImageDest.ReserveImage(pixelType, (uint32_t)widthDR, (uint32_t)height);
    
    // Grab images
    fflush(stdin);

    std::cout << "\r\nPress SPACE to stop grabbing...\r\n" << endl;
    while (!KeyPressed(' '))
    {
        // Get from camera image buffer
        pfResult = pfStream->GetNextBuffer(pfBuffer);
        
        if (pfResult == PFSDK_NOERROR)
        {
            if (iter == 50)
            {
                // Construct PFImage object
                // The image data is managed inside the class and released in the destructor
                pfBuffer->GetImage(pfImage);
                // Cameras with support for color formats decode the image different, this is why isColor may be true even using mono formats
                pfResult = pfImage.DemodulateDR(pfImageDest, isColor);

                if (pfResult == PFSDK_NOERROR)
                {
                    sprintf(filename, "image_%d_demod.bmp", iter_file++);
                    // Save demodulated image to a file
                    pfImageDest.SaveToFile(filename, pfImageFileType::BmpFileType);
                    
                    // To save original image
                    // sprintf(filename, "image_%d_mod.bmp", iter_file++);
                    // pfImage.SaveToFile(filename, pfImageFileType::BmpFileType);
                }
                iter = 0;
            }
         
            fps = pfStream->GetStreamStatistics().m_fpsGrab;
            networkRate = pfStream->GetStreamStatistics().m_networkRate;
            printf("FrameCounter: %" PRId64 " TimeStamp: %" PRIu64 " FPS: %05.3f %05.3f Mbps \r", pfBuffer->GetFrameCounter(), pfBuffer->GetTimestamp(), fps, networkRate);
            
            if (pfBuffer->GetFrameCounter() != oldFrameCounter && pfBuffer->GetFrameCounter() - oldFrameCounter > 1)
            {
                printf("\nLost frames: %" PRId64 "\n", pfBuffer->GetFrameCounter() - oldFrameCounter);
            }
            oldFrameCounter = pfBuffer->GetFrameCounter();

            iter++;
        }
        else
        {
            if (pfResult == PFSDK_ERROR_GETIMAGE_MISSING_PACKETS || pfResult == PFSDK_ERROR_GETIMAGE_GRAB_ERROR)
            {
                // If the streamCorruptFrames option of the PFStreamGEV is enabled, pfBuffer will contain the corrupted frame and metadata, including MissingPacketCount.
                printf("FrameCounter: %" PRId64 " TimeStamp: %" PRId64 " MissingPackets: %" PRIu32 " FrameCorrupted:%u \r\n", pfBuffer->GetFrameCounter(),
                    pfBuffer->GetTimestamp(), pfBuffer->GetMissingPacketCount(), pfBuffer->IsFrameCorrupted());
                // Construct PFImage object
                // The image data is managed inside the class and released in the destructor
                pfBuffer->GetImage(pfImage);
                // Save image to a file
                pfImage.SaveToFile("image_error.bmp", pfImageFileType::BmpFileType);
            }
            else if (pfResult == PFSDK_ERROR_GETIMAGE_TIMEOUT){
                std::cout << "Timeout error!\r\n";
            }
        }
        
        pfStream->ReleaseBuffer(pfBuffer);
    }


    pfImageDest.ReleaseImage();
    // Note: Release the image buffer. It's mandatory to call ReleaseBuffer() 
    // pfStream->ReleaseBuffer(&pfBuffer);

    std::cout << endl << "\r\nEnd of grabbing process!" << endl;

    return 0;
}
