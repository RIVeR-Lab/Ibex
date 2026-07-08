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
/// @file IPFEventFeatureChanged.h
/// @brief The PFEventFeatureChanged class defines an interface to receive notifications when the value of a feature changes.
///

#ifndef IPF_EVENT_FEATURE_CHANGED_H
#define IPF_EVENT_FEATURE_CHANGED_H

#include "PFCameraLib.h"
#include "pfcFeatureTypes.h"
#include <cstdint>

namespace pfcameralib
{
    /// 
    /// @interface IPFEventFeatureChanged IPFEventFeatureChanged.h 
    ///
    /// This interface allows an object inheriting from it to be registered in a PFCamera 
    /// to receive notifications to the specified feature value changes.
    ///
    class PFCAMERALIB_API  IPFEventFeatureChanged
    {
    public:
        /// 
        /// @brief Default class constructor
        ///
        IPFEventFeatureChanged();

        /// 
        /// @brief Default class destructor
        ///
        virtual ~IPFEventFeatureChanged();

        /// 
        /// @brief Receive a notification when the value of a feature changes.
        ///
        /// @param[in] feature_name     Name of the feature for which the callback has been triggered.
        /// @param[in] type             Type of the feature (Integer, Float, String, etc.)                  
        /// @param[in] value            Union structure containing the new value. Access using the member depending on type.
        ///
        virtual void OnFeatureChanged(const char* feature_name, uint8_t type, PFFeatureValueUnion value) = 0;
    private:
        class IPFEventFeatureChangedImpl;
        IPFEventFeatureChangedImpl* m_pImpl;
        friend class PFCameraInternal;
    };
}


#endif