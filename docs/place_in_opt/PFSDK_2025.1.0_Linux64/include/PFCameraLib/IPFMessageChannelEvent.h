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
/// @file IPFMessageChannelEvent.h
/// @brief The IPFMessageChannelEvent class defines an interface to receive notifications when message channel events are received
///


#ifndef IPFMESSAGECHANNELEVENT_H
#define IPFMESSAGECHANNELEVENT_H

#include "PFCameraLib.h"
#include "pfcCameraTypes.h"

#include <cstdint>

namespace pfcameralib
{
    /// 
    /// @class IPFMessageChannelEvent IPFMessageChannelEvent.h 
    ///
    /// @brief This interface allows an object which implements the public interface to receive notifications when message channel events are received
    ///
    class PFCAMERALIB_API IPFMessageChannelEvent
    {
    public:
      // Declaration of Event Ids according GigE 2.0
        enum EventId {
          TRIGGER_ID = 0x0002,
          START_EXPOSURE_ID = 0x0003,
          STOP_EXPOSURE_ID = 0x0004,
          START_TRANSFER_ID = 0x0005,
          STOP_TRANSFER_ID = 0x0006,
          PRIMARY_APP_SWITCH_ID = 0x0007,
          LINK_SPEED_CHANGE_ID = 0x0008,
          ACTION_LATE_ID = 0x0009,
          RESERVED_BEGIN_ID = 0x000A,
          RESERVED_END_ID = 0x8000,
          ERROR_BEGIN_ID = 0x8001,
          ERROR_END_ID = 0x8FFF,
          DEVICE_SPECIFIC_ID = 0x9000
        };


        /// 
        /// @brief Public class destructor
        ///
        virtual ~IPFMessageChannelEvent() {}

        /// 
        /// @brief Implement this method to receive Message Channel events
        ///
        /// ---------------------------------------------------------------------------------------------------------
        /// @param[in] cam_nr       Index of the camera in the list of available cameras that sends the message
        /// @param[in] mcparam      Event Channel parameters 
        ///                         
          ///
        virtual uint8_t OnPFEventMessage(const uint8_t cam_nr, PFMSGChannelParameter mcparam) = 0;
    };
}

#endif