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
/// @file PFStreamU3V.h
/// @brief This class manages the streaming with the camera using U3V network protocol.
///
#ifndef PFSTREAMU3V_H
#define PFSTREAMU3V_H

#include "PFStream.h"

namespace pfcameralib
{

    /// 
    /// @class PFStreamU3V PFStreamU3V.h 
    ///
    /// This class manages the streaming with the camera using U3V network protocol.
    /// The basic streaming behaviour is inherited from PFStream class.
    ///
    class PFCAMERALIB_API PFStreamU3V : public PFStream
    {
    public:
        ///
        /// @brief Constructor 
        ///
        PFStreamU3V();
        ///
        /// @brief Destructor 
        ///
        ~PFStreamU3V() override;

        /// @brief Filter driver version.
        /// @param[out] *out_version Previouly allocated buffer where version will be copied
        /// @return PFResul
        PFResult GetFilterDriverVersion(char* out_version) override;

        /// @brief Get streaming type.
        ///
        /// Returns the Streaming type used. CAMTYPE_U3V for this derived class
        ///
        /// @return CameraType  
        ///                 CAMTYPE_U3V      --> USB3 Camera protocol
        ///  
        CameraType GetStreamType() override;
    };
}


#endif