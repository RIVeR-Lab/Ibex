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
/// @file ImageTypes.h
/// @brief Image Types declarations
///

#ifndef IMAGE_TYPES
#define IMAGE_TYPES

#include "PFCameraLib.h"

namespace pfcameralib
{
namespace image
{
/// 
/// @namespace type_util
/// @brief Type declarations
/// 
namespace type_util
{
/// 
/// @enum pfBayerFilterType
///
/// Enumeration of the available Bayer filter types
///
typedef enum pfBayerFilterType {
  BayerFilterNearestNeighbor = 1,
  BayerFilter3x3 = 2,
  BayerFilterBilinear = 3,
  BayerFilterEdgeSensing2 = 4
}pfBayerFilterType;

/// 
/// @enum pfImageFileType
///
/// Enumeration of the available image file types
///
typedef enum pfImageFileType {
  BinaryFileType = 1,
  BmpFileType = 2,
  PngFileType = 3
}pfImageFileType;

}  // end namespace typeUtils 
}  // end namespace image
}  // end namespace pfcameralib

#endif