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
//  \file Console_SoftwareTrigger_Asynchronous.cpp
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
#include <mutex>
#include <condition_variable>

#include "PFCamera.h"
#include "PFStreamGEV.h"
#include "PFStreamU3V.h"
#include "PFDiscovery.h"
#include "PFImage.h"
#include "PFAsyncGrabber.h"

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
using namespace PFCameraDLL;

int Configure(PFCamera &pfCamera);

#define SEQUENCE_LENGTH 1

class CustomHandler : public IPFAsyncGrabHandler {
public:
  ~CustomHandler() override {
  }

  /// 
  /// @brief Callback for new buffer available.
  ///
  /// The buffer is locked. The user MUST unlock the buffer once it is processed.
  ///
  /// @param[in] buffer Reference object to PFBuffer.
  ///
  void OnNewBuffer(PFBufferRef& buffer) override {
    printf("FrameCounter: %" PRId64 " \t\tTimeStamp: %" PRIu64 " \t Corrupted: %d\n",
      buffer->GetFrameCounter(), buffer->GetTimestamp(), buffer->IsFrameCorrupted());

    ++frame_count_save;
    if (frame_count_save % 500 == 0) { //Save one out of 500
      PFImage pfImage;
      // Construct PFImage object
      // The image data is managed by the PFBuffer! Use GetImageCopy to get a hard copy.
      buffer->GetImage(pfImage);
      // Save image to a file
      pfImage.SaveToFile("image.bmp", image::type_util::BmpFileType);
      frame_count_save = 0;
    }
          
    last_error = PFResult();
    error_count = 0;

    bool notify;
    {
      std::lock_guard<std::mutex> lk(mutex);
      ++sequence_count;
      printf("Received image (%d)\n", sequence_count);
      notify = sequence_count == sequence_target;
    }
    
    if (notify) {
      cv.notify_all();
    }

    // Unlock buffer must be done after notifying the buffer has been processed
    buffer->UnlockBuffer();
  }

  /// 
  /// @brief Callback for error while grabbing.
  ///
  /// @param[in] error  PFResult of the error that was produced while grabbing.
  ///
  void OnGrabError(const PFResult& error) override {
    //Avoid flooding output if the same error repeats
    if (error != last_error) {
      last_error = error;
      printf("\nOnGrabError: %s\n", error.GetDescription());
    }
    else {
      printf("Same error count: %" PRIu64 "\r", ++error_count);
    }
  }

  void WaitForImages() {
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait(lk, [this]() {return sequence_count == sequence_target || force_wake; });
    sequence_count = 0;
    force_wake = false;
  }

  void ForceWake() {
    std::unique_lock<std::mutex> lk(mutex);
    force_wake = true;
    cv.notify_all();
  }

private:
  std::mutex mutex;
  std::condition_variable cv;
  int sequence_target = SEQUENCE_LENGTH;
  int sequence_count = 0;
  bool force_wake = false;

  size_t frame_count_save{ 0 };
  PFResult last_error;
  size_t error_count{ 0 };
};

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
            std::cout << i + 1 << "- " << pfCameraInfo->GetModelName() <<  endl;
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

    // Configure acquisition
    PFChannelParameter sPFChannelParameter;
    pfCamera.GetChannelParameter(sPFChannelParameter);

    sPFChannelParameter.ChannelTimeout = 500;
    sPFChannelParameter.StreamImageWaitTimeout = 1000;
    sPFChannelParameter.StreamTimeout = 400;
    sPFChannelParameter.StreamPacketResend = 10;
    pfCamera.SetChannelParameter(sPFChannelParameter);

    // In order to grab images it is necessary to prepare a proper stream.
    if (pfCameraInfo->GetType() == CAMTYPE_GEV)
        pfStream = new PFStreamGEV(false, false, true, true);
    else
        pfStream = new PFStreamU3V();
    
    // It is mandatory to add this stream to the camera before grabbing images.
    pfResult = pfCamera.AddStream(pfStream);
    if (pfResult != PFSDK_NOERROR)
    {
        std::cout << "Error: " << pfResult.GetDescription() << endl;
        _getch();
        return -1;
    }

    pfStream->SetBufferCount(100);
    
    //Set the frame handler
    CustomHandler frame_handler;
    PFAsyncGrabber pfGrabber(pfStream);
    pfGrabber.SetHandler(&frame_handler);

    PFAsyncGrabber::Control async_control;
    async_control.grab.prio = PFAsyncGrabber::Thread::Priority::Highest;
    async_control.event.prio = PFAsyncGrabber::Thread::Priority::Highest;
    // The pending and stop policies affect the behaviour of pfCamera.Freeze()
    async_control.event.pending_policy = PFAsyncGrabber::Thread::PendingPolicy::Process;
    async_control.event.stop_policy = PFAsyncGrabber::Thread::StopPolicy::Join;
    pfGrabber.SetControl(async_control);

    int64_t counter;
    pfCamera.GetFeatureInt("CounterValue", counter);
    std::cout << endl << "CounterValue " << counter << endl;
      
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

    std::atomic_bool stop_thread{ false };

    std::thread trigger_thread([&]() {
      int trigger_count = 0;
      int trigger_target = SEQUENCE_LENGTH;
      std::chrono::milliseconds ms_burst_restart_delay = std::chrono::milliseconds(10);
      std::chrono::milliseconds ms_trigger_spacing = std::chrono::milliseconds(50);
      auto timer_next_trigger = std::chrono::steady_clock::now();
      auto timer_start_burst = timer_next_trigger;
         
      while (!stop_thread) {
        auto timer_now = std::chrono::steady_clock::now();
  
        if (trigger_count < trigger_target) {
          if (timer_now >= timer_next_trigger) {
            auto triggerResult = pfCamera.SetFeatureCommand("TriggerSoftware", 1);
            if (triggerResult != PFSDK_NOERROR) {
              std::cout << "Error: " << triggerResult.GetDescription() << endl;
            }
            else {
              printf("\nTrigger sent (%d)\n", ++trigger_count);
              if (trigger_count == trigger_target) {
                printf("\nDisable trigger until all images received\n");
                frame_handler.WaitForImages();
                printf("\nAll images received. Trigger ready\n");
                timer_start_burst = std::chrono::steady_clock::now();
              }
              timer_next_trigger = timer_now + ms_trigger_spacing; //Retake time to account for op time
            }
          }
        }
        else {
          auto time_diff_burst_ms = std::chrono::duration_cast<std::chrono::milliseconds>(timer_now - timer_start_burst);
          if (time_diff_burst_ms >= ms_burst_restart_delay) {
            trigger_count = 0;
            timer_next_trigger = std::chrono::steady_clock::now();
          }
        }
      }
    });


    while (!KeyPressed(' ')) {}
    //pfCamera.GetFeatureInt("CounterValue", counter);
    //std::cout << endl << "CounterValue " << counter << endl;

    stop_thread = true;
    frame_handler.ForceWake(); 
    trigger_thread.join(); 
    
    auto start = std::chrono::steady_clock::now();
    // Stop grabbing
    pfCamera.Freeze();
    // Disconnect the camera
    pfCamera.Disconnect();

    auto stop = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " ms freeze" << std::endl;

    // Ensure that PFAsyncGrabber is destroyed before pfStream!
    //if (pfStream != nullptr)
    //    delete pfStream;
    
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

    // Set the SCPS PacketSize for a proper streaming
    pfResult = pfCamera.GetFeatureParams("GevSCPSPacketSize", &pfFeatureParams);
    if (pfResult != PFSDK_NOERROR)
      cout << "Error: " << pfResult.GetDescription() << endl;
    else {
      pfResult = pfCamera.SetFeatureInt("GevSCPSPacketSize", pfFeatureParams.Max);
      if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
      else
        cout << "GevSCPSPacketSize: " << pfFeatureParams.Max << endl;
    }
    
  
   return 0;
}