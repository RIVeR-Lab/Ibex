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

///
/// @file PFAsyncGrabber.h
/// @brief Implements Asynchronous Grabbing. By using this mode buffers captured are notified to clients through asynchronous messages.
///

#ifndef PF_ASYNC_GRABBER_H
#define PF_ASYNC_GRABBER_H

#include "PFCameraLib.h"
#include "IPFAsyncGrabHandler.h"
#include "PFStream.h"

namespace pfcameralib {
  /// 
  /// @class PFAsyncGrabber PFAsyncGrabber.h 
  ///
  /// Wrapper class for PFStream which enables asynchronous frame grabbing.
  /// Receiver of notifications must implement the IPFAsyncGrabHandler interface.
  ///
  class PFCAMERALIB_API PFAsyncGrabber {
  public:

    ///
    ///	@struct Thread
    ///
    /// @brief Thread configuration options
    ///
    struct Thread {
      ///
      ///	@enum Priority
      ///
      /// @brief Thread priority levels
      ///
      enum class Priority {
        Idle,
        Lowest,
        BelowNormal,
        Normal,
        AboveNormal,
        Highest,
        TimeCritical
      };

      ///
      ///	@enum StopPolicy
      ///
      /// @brief Thread stop policy
      /// 
      /// Defines wether the thread is joined or detached on acquisition stop.
      /// This affects behaviour for PFCamera::Freeze  
      ///
      enum class StopPolicy {
        Join,   /// Wait until the thread has terminated
        Detach  /// The thread is detached
      };

      ///
      ///	@enum PendingPolicy
      ///
      /// @brief Define what to do with pending events on acquisition stop
      /// 
      /// Combine with StopPolicy to achieved the desired behaviour.  
      ///
      enum class PendingPolicy {
        Discard,  /// Discard all pending events, causing thread to terminate
        Process   /// Keep processing all events even after acquisition stop
      };
    };

    ///
    ///	@struct Control
    ///
    /// @brief Configuration parameters for PFAsyncGrabber
    ///
    struct Control {
      ///
      ///	@struct Grab
      ///
      /// @brief Grab parameters
      ///
      struct Grab {
        Thread::Priority prio = Thread::Priority::Normal;
      } grab;
      
      ///
      ///	@struct Event
      ///
      /// @brief Event parameters
      ///
      struct Event {
        Thread::Priority prio = Thread::Priority::Highest;
        Thread::PendingPolicy pending_policy = Thread::PendingPolicy::Discard;
        Thread::StopPolicy  stop_policy = Thread::StopPolicy::Join;
      } event;
    };

    /// 
    /// @brief Constructor
    ///
    /// @param[in] stream PFStream instance, which must be kept alive as long as the PFAsyncGrabber is alive. 
    ///
    PFAsyncGrabber(PFStream* stream);
    
    /// 
    /// @brief Destructor
    ///
    ~PFAsyncGrabber();

    /// 
    /// @brief Set the parameters for asychronous grabbing
    ///
    /// @param[in] control            New configuration.
    ///
    /// @return  PFResult   
    /// 
    PFResult SetControl(const Control& control);

    /// 
    /// @brief Get current control parameters
    ///
    /// @param[out] control            Current configuration.
    ///
    /// @return  PFResult   
    /// 
    PFResult GetControl(Control& control);

    /// 
    /// @brief Set the handler to which new frame events will be notified.
    ///
    /// @param[in] *handler Implementation of IPFAsyncGrabHandler interface
    ///
    /// @return  PFResult   
    /// 
    PFResult SetHandler(IPFAsyncGrabHandler* handler);

  private:
    class Impl;
    Impl* m_pImpl;
  };

}


#endif