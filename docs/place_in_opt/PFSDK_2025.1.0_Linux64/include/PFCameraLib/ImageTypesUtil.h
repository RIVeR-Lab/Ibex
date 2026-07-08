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
/// @file ImageTypesUtil.h
/// @brief Image Types utility functions
///


#ifndef IMAGE_TYPES_UTIL
#define IMAGE_TYPES_UTIL

#include "PFCameraLib.h"
#include "pfcPixelTypes.h"
#include "PFResult.h"
#include "PFImage.h"

namespace pfcameralib
{
namespace image
{
namespace type_util
{

///
/// @brief Returns the unpacked type with same size, e.g. PixelMono10 for PixelMono10p or PixelMono10Packed 
///
/// @param[in] pixelType pixelType enum value accorded to GigE Vision specification
///
/// @return pfPixelType, PixelUnknown if input type is wrong or unsupported                 
/// 
PFCAMERALIB_API pfPixelType GetUnpackedPixelType(pfPixelType pixelType);

///
/// @brief Returns the mono type with same size, e.g. PixelMono10 for PixelRGB10  
///
/// @param[in] pixelType pixelType enum value accorded to GigE Vision specification
///
/// @return pfPixelType, PixelUnknown if input type is wrong or unsupported                 
/// 
PFCAMERALIB_API pfPixelType GetMonoPixelType(pfPixelType pixelType);

///
/// @brief Returns RGB type which corresponds to Pixel Bayer type, e.g. PixelRGB10 for PixelBayerRG10  
///
/// @param[in] pixelType pixelType enum value accorded to GigE Vision specification
///
/// @return  pfPixelType, PixelUnknown if input type is wrong or unsupported               
/// 
PFCAMERALIB_API pfPixelType GetRGBPixelType(pfPixelType pixelType);


// Get the number of bits associated to the supported pixel types
///
/// @brief Returns depth associated to each pixel type: e.g: 8 for PixelMono8 , 10 for PixelMono10, 12 for PixelRGB12 and so on.
/// Note that for color pixel types (3 channels) the function will return the depth for a single channel.
///  
///
/// @param[in] pixelType pixelType enum value accorded to GigE Vision specification
///
/// @return  Number of bits for any pixel, PixelUnknown if the type is unsupported
/// 
PFCAMERALIB_API int16_t GetPixelDepth(pfPixelType pixelType);
                        
///
/// @brief Check if pixel type is bayer format
///
/// @param[in] pixelType pixelType enum value accorded to GigE Vision specification
///
/// @return  true if pixel type is in Bayer mode              
/// 
PFCAMERALIB_API bool IsBayerPixelType(pfPixelType pixelType);
            
///
/// @brief Check if pixel type is RGB
///
/// @param[in] pixelType pixelType enum value accorded to GigE Vision specification
///
/// @return  true if pixel type is in RGB mode              
/// 
PFCAMERALIB_API bool IsRGBPixelType(pfPixelType pixelType);

///
/// @brief Check if pixel type is color format
///
/// @param[in] pixelType pixelType enum value accorded to GigE Vision specification
///
/// @return  true if pixel type is in Bayer or RGB mode              
/// 
PFCAMERALIB_API bool IsColorPixelType(pfPixelType pixelType);


///
/// @brief Check if pixel type is color format
///
/// @param[in] image input image
///
/// @return  true if pixel type is in Bayer or RGB mode              
/// 
PFCAMERALIB_API bool IsColorPixelType(const PFImage& image); 

///
/// @brief Check if pixel type is mono format
///
/// @param[in] pixelType pixelType enum value accorded to GigE Vision specification
///
/// @return  true if pixel type is Mono mode              
/// 
PFCAMERALIB_API bool IsMonoPixelType(pfPixelType pixelType);

///
/// @brief Check if pixel type is packed format
///
/// @param[in] pixelType pixelType enum value accorded to GigE Vision specification
///
/// @return  true if pixel type is in Packed format
/// 
PFCAMERALIB_API bool IsPackedPixelType(pfPixelType pixelType);

/// 
/// @brief Get the number of bytes required to allocate a single #pfPixelType.
///
/// @param[in] pixelType    pixelType enum value accorded to GigE Vision specification
/// @return  Number of bytes
/// 
PFCAMERALIB_API float GetPixelSizeBytes(pfPixelType pixelType);

/// 
/// @brief Get pixel type associated to its pixel type name
///
/// @param[in] pixelTypeName   pixelType enum value accorded to GenICam PFNC (see https://www.emva.org/wp-content/uploads/GenICamPixelFormatValues.pdf)
/// @return  Associated pixel type enum value. returns pixelUnknown if pixelTypeName is wrong or not supported in current PFSDK version
/// 
PFCAMERALIB_API pfPixelType GetPixelType(const char* pixelTypeName);

/// 
/// @brief Get pixel type associated to its pixel type name
///
/// @param[in] pixelType   pixelType enum value accorded to GenICam PFNC (see https://www.emva.org/wp-content/uploads/GenICamPixelFormatValues.pdf)
/// @param[out] pixelName   pixelType name value. Returns unknown if pixelType is not supported in current PFSDK version
/// @return  True if pixel supported, false if not
/// 
PFCAMERALIB_API bool GetPixelName(const pfPixelType pixelType, char* pixelName);

//
/// @brief Check if source pixel type is RGB and destination pixel type is mono
///
/// @param[in] srcType source pixelType enum value accorded to GigE Vision specification
/// @param[in] dstType destination pixelType enum value accorded to GigE Vision specification
///
/// @return  true if pixel type is convertible from RGB to Mono, false otherwise
/// 
PFCAMERALIB_API bool IsRGBToMonoConvertible(pfPixelType srcType, pfPixelType dstType);

}  // end namespace typeUtils 
}  // end namespace image
}  // end namespace pfcameralib

#endif