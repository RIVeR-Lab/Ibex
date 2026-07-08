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
/// @file PFCameraConfig.h
/// @brief Class containing methods to get current camera configuration
///
#ifndef PFCAMERACONFIG_H
#define PFCAMERACONFIG_H

#include "PFCamera.h"

namespace pfcameralib
{
    /// 
    /// @class PFCameraConfig PFCameraConfig.h
    ///
    /// This class containing methods to get current camera configuration.
    ///
    class PFCAMERALIB_API PFCameraConfig
    {
    
    public:
        enum BufferReaderType 
        {
            BUFFERREADER_2D = 0, 
            BUFFERREADER_3D = 1 
        }; 
        
        /// 
        /// @brief You must call this method to obtain the current camera configuration in a string
        ///
        /// @param[in]  pfCamera            PFCamera object already connnected
        /// @param[out] configurationString String containing the camera configuration. Sample string: "3D06#Width=1280;LineFinder_Mode=1;LineFinder_DataFormat3D=2;FrameCombine_NrOfFrames=1"
        /// @param[in]  bufferReaderType    If the desired acquisition mode is 2D or 3D
        /// @return PFResult with result code error   
        static PFResult GetConfigString(PFCamera &pfCamera, char * const configurationString, PFCameraConfig::BufferReaderType bufferReaderType);

        /// 
        /// @brief Get string containg camera class.
        ///
        /// @param[in]  pfCamera            PFCamera object already connnected
        /// @param[out] camera_class        String containing the camera class e.g. 3D06, 3D07, etc."
        /// @return PFResult with result code error   
        static PFResult GetClass(PFCamera &pfCamera, char * const camera_class);

        /// 
        /// @brief Call to get the type code of the camera. (First 4 chars of device ID).
        ///
        /// @param[in]  pfCamera            PFCamera object already connnected
        /// @param[out] type_code           Camera type code as integer
        /// @return PFResult with result code error   
        static PFResult GetTypeCode(PFCamera& pfCamera, int64_t& type_code);
    };  
}   
#endif //PFCAMERACONFIG_H