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
/// @file IPFEventError.h
/// @brief The PFEventError class defines an interface to capture Communication errors raised by the interface driver.
///


#ifndef IPFEVENTERROR_H
#define IPFEVENTERROR_H

#include "PFCameraLib.h"
#include <cstdint>


namespace pfcameralib
{
    /// 
    /// @interface IPFEventError IPFEventError.h 
    ///
    /// This interface defines how to capture communication errors raised by the interface driver.
    /// Usually you will need to monitorize and manage these errors in long and intensive processes using the camera like capturing images.
    /// You must implement the pure virtual methods from this interface in a class in your application. 
    ///
    class PFCAMERALIB_API IPFEventError
    {
    public:
        
        /// 
        /// @brief Public class destructor
        ///
        virtual ~IPFEventError() {}

        /// 
        /// @brief Capture interface driver communication errors in application.
        ///
        /// You have to implement this method in your application to manage communication errors. 
        /// @param[in] cam_nr       Index of the camera in the list of available cameras that has produced the error.
        /// @param[in] errorCode     
        ///                         PFSDK_ERROR_HEARTBEAT_TIMEOUT   A heartbeat error has been received. 
        ///                         PFSDK_ERROR_RECONNECTION        The driver is reconnecting. If you are grabbing you can recovery to the heartbeat error.
        ///                         PFSDK_ERROR_CONNECTION_FAILURE  The connection is not recoverable. If you are grabbing you must restart the action. 
        ///                         
        /// @param[in] errorMsg     Description of error.
        ///
        virtual void OnPFEventError(const uint8_t cam_nr, const int errorCode, const char *errorMsg) = 0;
    };
}

#endif