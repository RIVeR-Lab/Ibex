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
/// @file PFStreamGEV.h
/// @brief This class manages the streaming with the camera using GigE network protocol.
///

#ifndef PFSTREAMGEV_H
#define PFSTREAMGEV_H

#include "PFStream.h"

namespace pfcameralib
{
    
    /// 
    /// @class PFStreamGEV PFStreamGEV.h 
    ///
    /// This class manages the streaming with the camera using GEV network protocol.
    /// The basic streaming behaviour is inherited from PFStream class.
    ///
    class PFCAMERALIB_API PFStreamGEV : public PFStream
    {
    public:
        ///
        /// @brief Constructor 
        ///
        /// @param[in] enableFilterDriver Enables the filter driver
        /// @param[in] enablePacketResend   Enables packet resend
        /// @param[in] enableFindMaxPacketSize Configures appropiate packet size for streaming. For proper use it's recommended this parameter is always set to true. 
        /// @param[in] streamCorruptFrames Stream frames which are corrupted (in this case some parts of the frame may be missing or wrong)
        PFStreamGEV(bool enableFilterDriver = false, bool enablePacketResend = true, bool enableFindMaxPacketSize = true, bool streamCorruptFrames = false);

        /// 
        /// @brief Configure stream parameters
        ///
        /// @param[in] enableFilterDriver Enables the filter driver
        /// @param[in] enablePacketResend   Enables packet resend
        /// @param[in] enableFindMaxPacketSize Configures appropiate packet size for streaming. For proper use it's recommended this parameter is always set to true. 
        /// @param[in] streamCorruptFrames Stream frames which are corrupted (in this case some parts of the frame may be missing or wrong)
        void initParameters(bool enableFilterDriver, bool enablePacketResend, bool enableFindMaxPacketSize, bool streamCorruptFrames);

        ///
        /// @brief Destructor 
        ///
        ~PFStreamGEV() override;
       
        ///
        /// @brief Check if the FindMaxPacketSize is enabled
        ///
        bool IsFindMaxPacketSizeEnabled();
        
        ///
        /// @brief Find the maximum packet size available in the network
        ///
        PFResult FindMaxPacketSize();

        ///
        /// @brief Configure stream channel before starting transmision
        /// @return PFResult
        ///
        PFResult CheckStreamChannel();

        /// @brief Filter driver version.
        /// @param[out] *out_version Previouly allocated buffer where version will be copied
        /// @return PFResult
        PFResult GetFilterDriverVersion(char* out_version) override;

        /// 
        /// @brief Get streaming type.
        ///
        /// Returns the Streaming type used. CAMTYPE_GEV for this derived class
        ///
        /// @return CameraType  
        ///                 CAMTYPE_GEV      --> GigE Camera protocol
        ///
        ///
        CameraType GetStreamType() override;
      
    private:
        class PFGEVStreamImpl;
    };

    
}

#endif