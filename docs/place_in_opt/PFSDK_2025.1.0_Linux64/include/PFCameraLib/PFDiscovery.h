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
/// @file PFDiscovery.h
/// @brief Class containing the structures and functions to handle the discovery of GEV and U3V compliant devices.
///
#ifndef PFDISCOVERY_H
#define PFDISCOVERY_H

#include "PFResult.h"
#include "PFCameraLib.h"
#include "pfcCameraTypes.h"
#include "PFCameraInfo.h"
#include "PFCameraInfoGEV.h"
#include "PFCameraInfoU3V.h"

namespace pfcameralib
{
    /// 
    /// @class PFDiscovery PFDiscovery.h 
    ///
    /// This class contains the structures and functions to handle the discovery of GEV and U3V compliant devices.
    ///
    class PFCAMERALIB_API PFDiscovery
    {
    public:
        //uint8_t m_cameraIndex;                   ///< Index of the selected camera inside the pfCameraArray.
        
        ///
        /// @brief Public class constructor
        ///
        /// Sets ignore_subnet to false as default value.
        ///
        PFDiscovery();
                
        /// 
        /// @brief Public class destructor
        ///
        ~PFDiscovery();
        
        /// 
        /// @brief Copy constructor
        ///
        PFDiscovery(const PFDiscovery&);

        /// 
        /// @brief Move constructor
        ///
        PFDiscovery(PFDiscovery&&);                 // Move constructor
        
        /// 
        /// @brief Assignment operator
        ///
        PFDiscovery& operator=(const PFDiscovery&); // Copy assignment operator

        /// 
        /// @brief Discover GEV and U3V compliant devices.
        ///
        /// This function search for all connected devices taking into account ignore_subnet flag.
        /// @param[in] ignoreSubnet Set to true if you wish to ignore subnet network address for GEV cameras  (This parameter has no effect for U3V cameras).
        /// @param[in] port         Host side port for GEV discovery. Use 0 to let the OS find a suitable port.
        /// @return PFResult    
        /// Error Codes:
        ///   PFSDK_NOERROR                              
        ///   PFSDK_ERROR_DISCOVERY_NO_CAMERAS_FOUND
        ///
        PFResult DiscoverCameras(bool ignoreSubnet=false, uint16_t port = 0);

        ///
        /// @brief Force IP of the selected GEV camera.
        ///
        /// This function changes the IP, subnet and gateway of a GEV camera.
        ///
        /// @param[in,out] pfCameraInfo PFCameraInfo structure. 
        /// @param[in] new_ip Pointer to a string containing the new IP.
        /// @param[in] subnet Pointer to a string containing the subnet.
        /// @param[in] gateway Pointer to a string containing the gateway.
        /// @param[in] adapter_ip Pointer to a string containing adapter ip. 
        ///
        /// @return PFResult    
        /// Error Codes:
        ///   PFSDK_NOERROR                           
        ///   PFSDK_ERROR_NOT_GEV_CAMERA
        ///   PFSDK_ERROR_FORCEIP_FAILED
        ///
        PFResult ForceIP(PFCameraInfoGEV *pfCameraInfo, const char *new_ip, const char *subnet, const char *gateway, const char *adapter_ip);
    
        /// 
        /// @brief Get the total number of discovered cameras
        ///
        /// 
        ///
        /// @return  Number of discovered cameras
        ///
        uint8_t GetCameraCount();

        /// 
        /// @brief Get the total number of discovered GEVcameras
        ///
        ///
        /// @return  Number of discovered GEV cameras
        //
        uint8_t GetGEVCameraCount();

        /// 
        /// @brief Get the total number of discovered U3V cameras
        ///
        ///
        /// @return  Number of discovered U3V cameras
        ///
        uint8_t GetU3VCameraCount();

        /// 
        /// @brief Load the information of the camera in #pfcameralib::PFCameraInfo structure.
        ///
        /// The information is search in the discovery array.
        ///
        /// @param[in,out] cameraInfo  Camera info structure    
        /// @param[in] cameraIndex    Index for the position of the camera in the discovery array
        ///
        /// @return PFResult    
        /// Error Codes:
        ///   PFSDK_NOERROR                           
        ///   PFSDK_ERROR_WRONG_PARAMETERS   
        ///
        PFResult GetCameraInfo(PFCameraInfo *&cameraInfo, uint8_t cameraIndex);
            
    private:
        class PFDiscoveryImpl;
        PFDiscoveryImpl* m_pPfDiscoveryImpl;
    };
}

#endif //PFDISCOVERY_H