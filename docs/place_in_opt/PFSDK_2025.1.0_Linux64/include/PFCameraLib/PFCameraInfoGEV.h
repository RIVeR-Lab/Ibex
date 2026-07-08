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
/// @file PFCameraInfoGEV.h
/// @brief This class manages information to identify GEV cameras
///

#ifndef PFCAMERAINFOGEV_H
#define PFCAMERAINFOGEV_H

#include "PFCameraInfo.h"

namespace pfcameralib
{

    /// 
    /// @class PFCameraInfoGEV PFCameraInfoGEV.h
    ///
    /// This class contains the structures and functions to handle the information of the GEV camera.
    ///
    class PFCAMERALIB_API PFCameraInfoGEV : public PFCameraInfo
    {
    public:
        /// 
        /// @brief Public class constructor GEV
        ///
        PFCameraInfoGEV();

        /// 
        /// @brief Public class constructor GEV using pfCameraItem
        ///
        PFCameraInfoGEV(const pfCameraItem& cameraItem);

        /// 
        /// @brief Copy constructor
        ///
        PFCameraInfoGEV(const PFCameraInfoGEV &cameraInfo);
        /// 
        /// @brief Destructor
        ///
        virtual ~PFCameraInfoGEV();
        /// 
        /// @brief Assignment operator
        ///
        PFCameraInfoGEV & operator= (const PFCameraInfoGEV &cameraInfo);

        /// 
        /// @brief Get Model Name
        ///
        const char *GetModelName() const override;
        /// 
        /// @brief Get Device Version
        ///
        const char *GetDeviceVersion() const override;
        /// 
        /// @brief Get Manufacturer Info
        ///
        const char *GetManufacturerInfo() const override;
        /// 
        /// @brief Get Serial Number
        ///
        const char *GetSerial() const override;
        /// 
        /// @brief Get User Defined Name
        ///
        const char *GetUserDefinedName() const override;
        /// 
        /// @brief Get Status
        ///
        uint8_t GetStatus() const override;

        /// 
        /// @brief Get Camera MAC
        ///
        const char *GetCamMAC() const;
        /// 
        /// @brief Get Camera IP
        ///
        const char *GetCamIP() const;
        /// 
        /// @brief Get Camera Subnet
        ///
        const char *GetCamSubnet() const;
        /// 
        /// @brief Get Camera Gateway
        ///
        const char *GetCamGateway() const;
        /// 
        /// @brief Get Adapter IP
        ///
        const char *GetAdapterIP() const;
        /// 
        /// @brief Get Adapter Mask
        ///
        const char *GetAdapterMask() const;
        /// 
        /// @brief Get Adapter Name
        ///
        const char *GetAdapterName() const;
        /// 
        /// @brief Get Adapter Description
        ///
        const char *GetAdapterDescription() const;
        /// 
        /// @brief Get Adapter MAC
        ///
        const char *GetAdapterMAC() const;

        /// 
        /// @brief Get the camera info in a formatted string. Only for display purposes.
        ///
        const char* GetFormattedOutput() const override;

        /// 
        /// @brief Get a clone of the PFCameraInfo regardless of its runtime type (GEV, U3V).
        ///
        virtual PFCameraInfo* clone() const;

    private:
        friend class PFDiscovery;
        friend class PFCamera;
    };
}
#endif