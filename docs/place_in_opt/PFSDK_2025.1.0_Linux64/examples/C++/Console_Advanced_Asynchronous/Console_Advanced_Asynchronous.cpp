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
//  \file Console_Advanced_Asynchronous.cpp
//
//  \brief
//  This example is a command line application that shows a more advanced and explicit process of performing image acquisition.
//  The API presented enables finer grained control which allows better resource reuse and faster AcquisitionStart/AcquisitionStop cycles.
//
//  This sample makes use of the PFAsyncGrabber, but this is not required to use the advanced modes.
//
//  The operations performed by default when grabbing are as follows:
//
//  Lock stream parameters (and allocate resources for the grabbing process)
//      Open stream (which resets the frame counter)
//          AcquisitionStart (grab)
//            ...
//          AcquisitionStop (freeze)
//      Close stream
//  Unlock stream parameters (release resources)
//
//  For instance if it is know parameters do not change, you may keep the parameters locked and open/close for each cycle. 
//  Or keep the stream open and only perform AcquisitionStart and AcquisitionStop commands if the frame counter does not need to be reset.
//
*/

#include <cstdio>
#include <iostream>
#include <inttypes.h>
#include <atomic>
#include <chrono>
#include <thread>
#include <sstream>
#include <memory>

#include <stdint.h>
#include <inttypes.h>

#include "PFCamera.h"
#include "PFStreamGEV.h"
#include "PFStreamU3V.h"
#include "PFAsyncGrabber.h"
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

#define Sleep(x) usleep(x*1000)

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
  return (_kbhit() != 0) && (_getch() == key);
}

using namespace std;
using namespace PFCameraDLL;

void PrintAndExit(const PFResult& result, int code) {
  std::cout << "Error: " << result.GetDescription() << endl;
  _getch();
  exit(code);
}

void PrintAndExit(const std::string& message, int code) {
  std::cout << message << endl;
  _getch();
  exit(code);
}

int Configure(PFCamera& pfCamera, PFStream* pfStream);
std::unique_ptr<PFCameraInfo> DiscoverAndSelect();


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
  void OnNewBuffer(PFBufferRef &buffer) override {
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

    buffer->UnlockBuffer();

    last_error = PFResult();
    error_count = 0;
  }

  /// 
  /// @brief Callback for error while grabbing.
  ///
  /// @param[in] error  PFResult of the error that was produced while grabbing.
  ///
  void OnGrabError(const PFResult &error) override {
    //Avoid flooding output if the same error repeats
    if (error != last_error) {
      last_error = error;
      printf("\nOnGrabError: %s\n", error.GetDescription());
    }
    else {
      printf("Same error count: %" PRIu64 "\r", ++error_count);
    }
  }

private:
  size_t frame_count_save {0};
  PFResult last_error;
  size_t error_count {0};
};

void AdvancedGrab(PFCamera& pfCamera, PFStream* pfStream) {
  //PFCamera is configured to not manage the stream. This must be done explicitly.
  //Lock parameters related to acquisition, this allows allocating the resources for grabbing.
  pfStream->LockParams();
  //Open the stream channel, this resets the frame counter to 0.
  pfStream->Open();

  // PFAsyncGrabber will acquire images from the PFStream and notify the event handler.
  // PFStream::Open causes the the async grabber to start waiting for images.

  bool do_grab = true;
  std::chrono::milliseconds ms_until_next_status_change = std::chrono::milliseconds(0);
  auto timer_start = std::chrono::steady_clock::now();

  while (!KeyPressed(' ')) {
    auto timer_now = std::chrono::steady_clock::now();
    auto time_diff_ms = std::chrono::duration_cast<std::chrono::milliseconds>(timer_now - timer_start);
    if (time_diff_ms >= ms_until_next_status_change) {
      if (do_grab) {
        pfStream->ResetStatistics();      //Optional
        pfCamera.ResetTimestampCounter(); //Optional
        pfCamera.Grab();                  //Send AcquisitionStart command

        ms_until_next_status_change = std::chrono::milliseconds(500);
        printf("\n\nGrab for ~%" PRId64 "ms\n", ms_until_next_status_change.count());
      }
      else {
        pfCamera.Freeze();  //Send AcquisitionStop command

        auto stream_stats = pfStream->GetStreamStatistics();
        printf("\nStream statistics (FPS: %f, Mbps: %f, Frames: %" PRIu64", Lost Frames: %" PRIu64", Error Frames: %" PRIu64", Missing packets: %" PRIu64")\n",
          stream_stats.m_fpsGrab, stream_stats.m_networkRate, stream_stats.m_totalFrames, stream_stats.m_lostFrames, stream_stats.m_errorFrames, stream_stats.m_missingPackets);

        ms_until_next_status_change = std::chrono::milliseconds(100);
        printf("\n\nFreeze for ~%" PRId64 "ms\n", ms_until_next_status_change.count());
        //Beware, this only stops the camera from sending frames. The PFAsyncGrabber event thread may still keep running depending on how you configure it
      }
      do_grab = !do_grab;
      timer_start = std::chrono::steady_clock::now(); //Retake time to account for op time
    }
  }

  // Stop grabbing
  pfCamera.Freeze();

  pfStream->Close();
  pfStream->UnlockParams();
}

int main()
{
  PFCamera pfCamera;
  PFResult pfResult;
  std::unique_ptr<PFStream> pfStream;

  auto pfCameraInfo = DiscoverAndSelect();

  // Connect the camera using pfCameraInfo
  pfResult = pfCamera.Connect(*pfCameraInfo);
  if (pfResult != PFSDK_NOERROR) {
    PrintAndExit(pfResult, -1);
  }

  // While debugging it is advisable to configure a HeartbeatRate of at least 10 seconds.
#if !defined(NDEBUG)
  pfResult = pfCamera.SetHeartbeatRate(10000);
  if (pfResult != PFSDK_NOERROR) {
    PrintAndExit(pfResult, -1);
  }
#endif

  // In order to grab images it is necessary to prepare a proper stream.
  if (pfCameraInfo->GetType() == CAMTYPE_GEV) {
    pfStream = std::make_unique<PFStreamGEV>(true, false, false, true);
  }
  else {
    pfStream = std::make_unique<PFStreamU3V>();
  }

  //Override default behaviour of camera grab/freeze
  PFCamera::Control camera_control;
  camera_control.acquisition.manage_streams = false;          //Do not synchronize stream open or close to camera grab and freeze
  camera_control.acquisition.reset_timestamp_counter = false; //Do not reset timestamp counter in camera
  pfCamera.SetControl(camera_control);

  //Override default behavior of PFStream.
  PFStream::Control stream_control;
  stream_control.lock_params_on_open = false;     //Require explicit locking
  stream_control.unlock_params_on_close = false;  //Require explicit unlocking
  pfStream->SetControl(stream_control);

  //Buffer Pool Count
  pfStream->SetBufferCount(100);
  //In case the buffer pool is exhausted, the async event queue may get flooded with error messages.
  //Setting a timeout helps control this.
  pfStream->SetBufferPoolTimeout(1000);

  // It is mandatory to add this stream to the camera before grabbing images.
  pfResult = pfCamera.AddStream(pfStream.get());
  if (pfResult != PFSDK_NOERROR) {
    PrintAndExit(pfResult, -1);
  }

  // Configure some camera features
  Configure(pfCamera, pfStream.get());

  //The grabber takes the PFStream as input. 
  //When acquisition is started with PFCamera::Grab and the stream is opened, the grabber also starts.
  PFAsyncGrabber pfGrabber(pfStream.get());

  //Set the frame handler
  CustomHandler frame_handler;
  pfGrabber.SetHandler(&frame_handler);

  PFAsyncGrabber::Control async_control;
  async_control.grab.prio = PFAsyncGrabber::Thread::Priority::Normal;
  async_control.event.prio = PFAsyncGrabber::Thread::Priority::Normal;
  // The pending and stop policies affect the behaviour of pfCamera.Freeze()
  async_control.event.pending_policy = PFAsyncGrabber::Thread::PendingPolicy::Process;
  async_control.event.stop_policy = PFAsyncGrabber::Thread::StopPolicy::Detach;
  pfGrabber.SetControl(async_control);

  AdvancedGrab(pfCamera, pfStream.get());

  // Disconnect the camera
  pfCamera.Disconnect();

  std::cout << "\r\nPress any key to finish...\r\n" << endl;
  _getch();
  return 0;
}

std::unique_ptr<PFCameraInfo> DiscoverAndSelect() {
  PFDiscovery discovery;
  PFCameraInfo* pfCameraInfo = nullptr;
  PFResult pfResult;
  uint8_t camera;
  uint16_t selected;

  // Discover the cameras available in the computer or network
  pfResult = discovery.DiscoverCameras();
  if (pfResult == PFSDK_ERROR_DISCOVERY_NO_CAMERAS_FOUND) {
    PrintAndExit("No cameras found.", 0);
  }

  // For each discovered camera, print the model name, manufacturer, version, etc
  std::cout << "\nCameras found: \n" << endl;
  uint8_t found_camera_count = discovery.GetCameraCount();
  for (uint8_t i = 0; i < found_camera_count; i++) {
    pfResult = discovery.GetCameraInfo(pfCameraInfo, i);
    if (pfResult == PFSDK_NOERROR) {
      std::cout << i + 1 << "- " << pfCameraInfo->GetModelName() << endl;
      pfCameraInfo->printCameraInfo();
    }
  }

  std::cout << "Select a camera from the list: ";
  cin >> selected;
  camera = static_cast<uint8_t>(selected) - 1;

  if (camera < 0 || camera >= found_camera_count) {
    std::stringstream ss;
    ss << "Error: " << "Input must be [0," << found_camera_count - 1 << "]";
    PrintAndExit(ss.str(), -1);
  }

  // Get the information of the selected camera and keep it in pfCameraInfo
  pfResult = discovery.GetCameraInfo(pfCameraInfo, camera);
  if (pfResult != PFSDK_NOERROR) {
    PrintAndExit(pfResult, -1);
  }

  return std::unique_ptr<PFCameraInfo>(pfCameraInfo->clone());
}

int Configure(PFCamera& pfCamera, PFStream *pfStream)
{
  PFFeatureParameters pfFeatureParams;
  PFResult pfResult;
  int64_t width, height;
  double double_value;
  char enum_str[64];

  // Check if the value is inside the limits
  pfResult = pfCamera.GetFeatureParams("Width", &pfFeatureParams);
  width = pfFeatureParams.Max;

  // Set the corresponding value
  pfResult = pfCamera.SetFeatureInt("Width", width);
  if (pfResult != PFSDK_NOERROR) {
    std::cout << "Error: " << pfResult.GetDescription() << endl;
  }

  // Read back Width value
  pfResult = pfCamera.GetFeatureInt("Width", width);
  if (pfResult != PFSDK_NOERROR) {
    std::cout << "Error: " << pfResult.GetDescription() << endl;
  }
  else {
    std::cout << "Width: " << (uint16_t)width << endl;
  }

  // Check if the value is inside the limits
  pfResult = pfCamera.GetFeatureParams("Height", &pfFeatureParams);
  height = pfFeatureParams.Max;

  // Set the corresponding value
  pfResult = pfCamera.SetFeatureInt("Height", height);
  if (pfResult != PFSDK_NOERROR) {
    std::cout << "Error: " << pfResult.GetDescription() << endl;
  }
  // Read back Height value
  pfResult = pfCamera.GetFeatureInt("Height", height);
  if (pfResult != PFSDK_NOERROR) {
    std::cout << "Error: " << pfResult.GetDescription() << endl;
  }
  else {
    std::cout << "Height: " << (uint16_t)height << endl;
  }
  // Set pixel format
  pfResult = pfCamera.SetFeatureEnum("PixelFormat", "Mono8");
  if (pfResult != PFSDK_NOERROR) {
    std::cout << "Error: " << pfResult.GetDescription() << endl;
  }
  // Read back pixel format
  pfResult = pfCamera.GetFeatureEnum("PixelFormat", enum_str);
  if (pfResult != PFSDK_NOERROR) {
    std::cout << "Error: " << pfResult.GetDescription() << endl;
  }
  else {
    std::cout << "PixelFormat: " << enum_str << endl;
  }

  // Set Exposure Time value
  double_value = 1000.0;
  // Check if the value is inside the limits
  pfResult = pfCamera.GetFeatureParams("ExposureTime", &pfFeatureParams);
  if (double_value > pfFeatureParams.FloatMax) {
    double_value = pfFeatureParams.FloatMax;
  }
  else if (double_value < pfFeatureParams.FloatMin) {
    double_value = pfFeatureParams.FloatMin;
  }
  // Set the corresponding value
  pfResult = pfCamera.SetFeatureFloat("ExposureTime", double_value);
  if (pfResult != PFSDK_NOERROR) {
    std::cout << "Error: " << pfResult.GetDescription() << endl;
  }
  // Read back Exposure Time value
  pfResult = pfCamera.GetFeatureFloat("ExposureTime", double_value);
  if (pfResult != PFSDK_NOERROR) {
    std::cout << "Error: " << pfResult.GetDescription() << endl;
  }
  else {
    std::cout << "ExposureTime: " << double_value << endl;
  }

  // Set the SCPS PacketSize for a proper streaming
  if (pfStream->GetStreamType() == CAMTYPE_GEV) {
    // Find which is the recommended Packet Size
    pfResult = static_cast<PFStreamGEV*>(pfStream)->FindMaxPacketSize();
    if (pfResult != PFSDK_NOERROR)
      cout << "Error: " << pfResult.GetDescription() << endl;
    else {
      int64_t value;
      pfResult = pfCamera.GetFeatureInt("GevSCPSPacketSize", value);
      if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
      else {
        cout << "GevSCPSPacketSize: " << value << endl;
      }
    }
  }

  return 0;
}
