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
/// @file PFCameraInfo.h
/// @brief This class manages information to identify any type of camera
///
#ifndef PFCAMERAINFO_H
#define PFCAMERAINFO_H

#include "pfcCameraTypes.h"
#include "PFCameraLib.h"
#include "PFResult.h"
#include <string>

namespace pfcameralib
{
    class PFCameraInfoGEV;
    class PFCameraInfoU3V;

    /// 
    /// @class PFCameraInfo PFCameraInfo.h
    ///
    /// This class contains the structures and functions to handle the information of the camera.
    ///
    class PFCAMERALIB_API PFCameraInfo
    {
    public:
        /// 
        /// @brief Public class constructor
        ///
        PFCameraInfo(CameraType type);
        /// 
        /// @brief Public class constructor
        ///
        PFCameraInfo(const pfCameraItem &cameraItem);
        /// 
        /// @brief Copy constructor
        ///
        PFCameraInfo(const PFCameraInfo &cameraInfo);
        /// 
        /// @brief Assignment operator
        ///
        PFCameraInfo & operator= (const PFCameraInfo &cameraInfo);
        /// 
        /// @brief Destructor
        ///
        virtual ~PFCameraInfo();
        /// 
        /// @brief Get Camera Type
        ///
        virtual CameraType GetType() const;
        /// 
        /// @brief Get Model Name
        ///
        virtual const char *GetModelName() const = 0;
        /// 
        /// @brief Get Device Version
        ///
        virtual const char *GetDeviceVersion() const = 0;
        /// 
        /// @brief Get Manufacturer Info
        ///
        virtual const char *GetManufacturerInfo() const = 0;
        /// 
        /// @brief Get Serial Number
        ///
        virtual const char *GetSerial() const = 0;
        /// 
        /// @brief Get User Defined Name
        ///
        virtual const char *GetUserDefinedName() const = 0;
        /// 
        /// @brief Get Status
        ///
        virtual uint8_t GetStatus() const = 0;

        /// 
        /// @brief Print to console camera info. Prints the result of GetFormattedOutput().
        ///
        void printCameraInfo() const;

        /// 
        /// @brief Get the camera info in a formatted string. Only for display purposes.
        ///
        virtual const char* GetFormattedOutput() const = 0;
        
        /// 
        /// @brief Get a clone of the PFCameraInfo regardless of its runtime type (GEV, U3V).
        ///
        virtual PFCameraInfo* clone() const = 0;


        //DEPRECATED FUNCTIONS HERE
        PFCAMERALIB_DEPRECATED PFCameraInfoGEV* GetCameraInfoGEV() const;
        PFCAMERALIB_DEPRECATED PFCameraInfoU3V* GetCameraInfoU3V() const;

    protected:
        friend class PFDiscovery;
        friend class PFCamera;
        class Impl;
        Impl* m_pImpl;
    };
}

#endif //PFCAMERAINFO_H
