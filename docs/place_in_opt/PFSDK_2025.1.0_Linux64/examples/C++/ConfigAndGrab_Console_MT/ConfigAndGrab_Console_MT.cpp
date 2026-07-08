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
//  \file ConfigAndGrab_Console_MT.cpp
//
//  \brief
//
//  This example is a command line application that shows the generic process to discover the cameras
//  in the computer or network, connect to a camera, grab images and disconnect.
//  Finally the camera is freezed and disconnected.
//
//  Description: This sample is based on ConfigAndGrab_Console.  
//  The purpouse of this sample is to show how to grab and process images by using two different threads.
//  By separating the capture and the process with two different threads the capture process is always responding
//  while the process thread is processing the image increasing the efficiency. 
//  Both threads use a queue to share the images available.
//  The Grab thread (Producer) will be adding images to the queue when they are available.
//  The Render thread (Consumer) will be popping images from the queue.
//
//  This sample uses standard C++11 threads, in order to compile you need a compiler that supports C++11.
//  All C++ compilers since 2012 support this standard.
*/

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <inttypes.h>

#include <vector>
#include <mutex>        // std::mutex
#include <thread>       // std::thread
#include <future>

#include "PFCamera.h"
#include "PFDiscovery.h"
#include "PFStreamGEV.h"

#include "MT_Queue.h"

#ifdef WIN32
#include <Windows.h>
#include <conio.h>
#include <direct.h>
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>  
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

using namespace std;
using namespace pfcameralib;
using namespace pfcameralib::image::type_util;

// Stream options
#define ENABLE_FILTER_DRIVER   true
#define ENABLE_PACKET_RESEND   true
#define ENABLE_FIND_MAX_PACKET true
#define STREAM_CORRUPT_FRAMES  true

// Declaration of generic functions
int discoverCamerasAndSelect(PFCameraInfo*& pfCameraInfo);
int connectCamera(PFCameraInfo* pfCameraInfo);
int configure(int64_t& width, int64_t& height);
void unlockAllBuffers();
void checkFilterDriverVersion(PFStream* pfStream);

// Thread functions
int captureImages(PFStream *pfStream);
int renderImages();

// Captured camera data
struct PFBufferItem {
  PFBufferRef m_bufferRef;
  double fps;
};

// Capture is cancelled
bool m_cancelled = false;
// Cameras discovered
PFDiscovery pfDiscover;
PFCamera pfCamera;
// Queue
MT_Queue<PFBufferItem> m_queue;


int KeyPressed(char key)
{
  return (_kbhit() != 0) && (tolower(_getch()) == tolower(key));
}

int main()
{
    PFCameraInfo* pfCameraInfo;    
       
    //cout << "Start create camera thread " << endl;
    int ret = discoverCamerasAndSelect(pfCameraInfo);
    if (ret != 0)
    {
      return -1;
    }
       
    std::unique_ptr<PFStream> pfStream = std::make_unique<PFStreamGEV>(ENABLE_FILTER_DRIVER, ENABLE_PACKET_RESEND, ENABLE_FIND_MAX_PACKET, STREAM_CORRUPT_FRAMES);
  
    // We discover the camera, connect and configure
    if (connectCamera(pfCameraInfo) != PFSDK_NOERROR) {
      std::cout << "Cannot connect to camera. Aborted application! " << std::endl;
      return -1;
    }

    pfStream->SetBufferCount(500);
    pfCamera.AddStream(pfStream.get());

    checkFilterDriverVersion(pfStream.get());
           
    // We create two threads: one is used to capture images, the second thread is used to display images
    auto futureCapture = std::thread(captureImages, pfStream.get());
    auto futureProcess = std::thread(renderImages);

    // Wait the threads to finish
    futureCapture.join();
    futureProcess.join();
    
    // All threads are finished
    cout << "All threads are gone" << endl;
       
    return 0;
}

void checkFilterDriverVersion(PFStream *pfStream)
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

void unlockAllBuffers() {
  PFBufferItem pfBufferItem;
  bool hasItems;

  // Unlock all buffers before finish
  do {
    hasItems = m_queue.getItem(pfBufferItem);
    if (hasItems)
    {
      pfBufferItem.m_bufferRef->UnlockBuffer();
    }
  } while (hasItems);

  cout << endl;
  cout << "Unlocked buffers!" << endl;
}

int discoverCamerasAndSelect(PFCameraInfo*& pfCameraInfo)
{
  PFResult pfResult;
  int i;
  int selected;

  // Discover cameras
  pfResult = pfDiscover.DiscoverCameras();
  if (pfResult == PFSDK_ERROR_DISCOVERY_NO_CAMERAS_FOUND)
  {
    cout << "No cameras found." << endl;
    return -1;
  }

  // Print cameras discovered
  // For each camera prints the model Name, manufacturer, version, etc
  cout << "\nCameras found: \n" << endl;
  for (i = 0; i < pfDiscover.GetCameraCount(); i++)
  {
    pfDiscover.GetCameraInfo(pfCameraInfo, (uint8_t)i);
    cout << i + 1 << "- " << pfCameraInfo->GetModelName() << endl;
    pfCameraInfo->printCameraInfo();
  }

  // Select one of the following cameras to connect
  if (i > 0)
  {
    cout << "Select Device: ";
    cin >> selected;
    cout << "camera: " << selected << endl;
    pfDiscover.GetCameraInfo(pfCameraInfo, (uint8_t)(selected - 1));
  }

  return 0;
}


// create camera
// Discover, select camera, connect and config
int connectCamera(PFCameraInfo *pfCameraInfo)
{
    PFResult pfResult;
    int64_t width, height;
                  
      // Connect camera selected
    pfResult = pfCamera.Connect(*pfCameraInfo);
    if (pfResult != PFSDK_NOERROR)
    {
        cout << "Error: " << pfResult.GetDescription() << endl;
        return -2;
    }


    // After connect configure the camera
    configure(width, height);
    
      
    return 0;
}


int configure(int64_t &width, int64_t &height)
{
    // Read the properties of the camera
    
    // Get the Width properties
    PFResult pfResult = pfCamera.GetFeatureInt("Width", width);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "Width: " << (uint16_t)width << endl;

    // Get the Height properties
    pfResult = pfCamera.GetFeatureInt("Height", height);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "Height: " << (uint16_t)height << endl;

    // Setup the camera configuration

    // Set the pixel format properties
    pfResult = pfCamera.SetFeatureEnum("PixelFormat", "Mono8");
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "PixelFormat: " << "Mono8" << endl;

    // Set the maximum packet size 
    pfResult = pfCamera.SetFeatureInt("GevSCPSPacketSize", 9000);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "GevSCPSPacketSize: " << (uint16_t)9000 << endl;

    return 0;
}


// This function is executed in specific thread 
// This thread is intended to grab images (Producer). All the images are stored on a global queue MT_Queue.
int captureImages(PFStream *pfStream)
{
    PFBufferItem pfBuffer;
    PFImage pfImage;
    // int index = 0;
        
    cout << fixed;
    cout.precision(2);
    std::cout.precision(2);

    cout << "Start capture thread " << endl;
    // Start grabbing
    PFResult pfResult = pfCamera.Grab();

    fflush(stdin);
    cout << "Press 'S' to stop grabbing..." << endl;
    while (!KeyPressed('S'))
    {
        pfResult = pfStream->GetNextBuffer(pfBuffer.m_bufferRef);
        if (pfResult == PFSDK_NOERROR )
        {
          pfStream->GetFPS(pfBuffer.fps);
          m_queue.push(pfBuffer);
        }
        else 
        {
          cout << "\nError: " << pfResult.GetDescription() << "\r\n";
          if (pfResult == PFSDK_ERROR_GETIMAGE_MISSING_PACKETS || pfResult == PFSDK_ERROR_GETIMAGE_GRAB_ERROR)
          {
            // If the streamCorruptFrames option of the PFStreamGEV is enabled, pfBuffer will contain the corrupted frame and metadata, including MissingPacketCount.
            printf("FrameCounter: %" PRId64 " TimeStamp: %" PRIu64 " MissingPackets: %" PRIu32 " FrameCorrupted:%u \r\n", pfBuffer.m_bufferRef->GetFrameCounter(),
              pfBuffer.m_bufferRef->GetTimestamp(), pfBuffer.m_bufferRef->GetMissingPacketCount(), pfBuffer.m_bufferRef->IsFrameCorrupted());
           
            // Construct PFImage object
            // The image data is managed inside the class and released in the destructor
            pfBuffer.m_bufferRef->GetImage(pfImage);
            // Save image to a file
            pfImage.SaveToFile("image_error.bmp", pfImageFileType::BmpFileType);
          }
          else if (pfResult == PFSDK_ERROR_GETIMAGE_TIMEOUT) {
            cout << "Timeout error!\r\n";
          }

        }
        fflush(stdout);
    }
        
    unlockAllBuffers();

    // Stop camera
    pfCamera.Freeze();
       
    // Disconnect camera
    pfCamera.Disconnect();

    cout << endl;
    cout << "Exit capture thread " << endl;
   
    m_cancelled = true;

    return 0;
}

// This function is executed in specific thread 
// This thread is intended to consume images (Consumer). 
// The images are obtained from MT_Queue that is filled from Producer function
int renderImages()
{
    PFImage pfImage;
    PFBufferItem pfBufferItem;
    int nItems = 0;
    int index = 1;
    cout << fixed;
    cout.precision(2);
    std::cout.precision(2);

    cout << "Start display thread " << endl;
   
    fflush(stdin);
    while (!m_cancelled)
    {
      bool hasItem = m_queue.getItem(pfBufferItem);
      if (hasItem) {
        // Construct PFImage object
        // The image data is managed inside the class and released in the destructor
        pfBufferItem.m_bufferRef->GetImage(pfImage);

        if (nItems % 100 == 0)
        {
          char filename[256];
                    
          sprintf(filename, "image_%04d.bmp", index);
          index++;
                    
          // Save image to a file
          pfImage.SaveToFile(filename, pfImageFileType::BmpFileType);
        }

        cout << "Pending " << m_queue.getSize() << " images. FrameCounter: " << pfBufferItem.m_bufferRef->GetFrameCounter() << " TimeStamp: " << pfBufferItem.m_bufferRef->GetTimestamp() << " FPS: " << pfBufferItem.fps << "\r";
        nItems++;

        // Mark Buffer as processed
        pfBufferItem.m_bufferRef->UnlockBuffer();
      }
      fflush(stdout);
    }
       
    cout << endl;
    cout << "Exit render thread " << endl;

    return 0;
}
