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
/// @file ImageCore.h
/// @brief Image transform and serialize functions 
///


#ifndef IMAGECORE_H
#define IMAGECORE_H

#include "PFCameraLib.h"
#include "pfcPixelTypes.h"
#include "PFResult.h"
#include "PFImage.h"
#include "ImageTypesUtil.h"

/// 
/// @namespace pfcameralib
/// @brief Main PFSDK namespace
/// 
namespace pfcameralib
{
/// 
/// @namespace image
/// @brief Image namespace declarations
/// 
namespace image
{         
/// 
/// @namespace core
/// @brief Core namespace declarations 
/// 
namespace core
{
using image::type_util::pfImageFileType;
using image::type_util::pfBayerFilterType;

// Forward declaration
struct ImageTransformContext;

/// 
/// @brief Algorithm use to Demodulate image
///
/// The modulation algorithm can be Double or Quad Rate
///
enum class ModulationAlgorithm {
  DoubleRate,
  QuadRate
};

/// 
/// @brief Store the PFImage into an image file
///
/// This function stores the PFImage into a system file in different formats such as png, bmp or binary (raw data).
/// For Binary format the image is not converted and is stored respecting it's original pixel type.
/// For bmp and png formats images some requirements are met:
/// 1) For mono, image will be stored in 8 bit mode.  If the image is packed it will be previously unpacked.
/// 2) For RGB, if the pixel type is in bayer mode it will be debayered, if the image is packed it will be unpacked and stored to disk in 24 bits (8 bit channel).
/// All the conversions required (Unpack, Debayer, 8 bit conversions) will be done internally before storing data to disk.
/// 
/// NOTE: If the user requires to store the data in the original pixel type must use Binary format (raw).
///
/// @param[in] image         Input image to store
/// @param[in] file_name     File name including path.
/// @param[in] file_type     File type or extension. Formats available are png, bmp, and raw. 
///
/// @return PFResult                
/// Error Codes:
///      PFSDK_ERROR_OPEN_FILE_FAILED
///      PFSDK_ERROR_SAVE_IMAGE_TO_FILE
///      PFSDK_NOERROR
/// 
PFCAMERALIB_API PFResult SaveToFile(const PFImage& image, const char* file_name, const pfImageFileType file_type);


///
/// @brief Loads data stored on disk and set PFImage image data memory.
///
/// This function loads file data from disk and set PFImage image data memory.
/// PFImage must be initialized in appropiate width, height and pixel type before loading data. 
///
/// @param[in] dst           Input image where data pointer is initialized
/// @param[in] file_name     File name including path.
/// @param[in] file_type     Formats supported are binary, bmp, png.
///
/// @return PFResult                
/// Error Codes:
///      PFSDK_ERROR_OPEN_FILE_FAILED
///      PFSDK_ERROR_LOAD_IMAGE_FROM_FILE
///      PFSDK_ERROR_IMAGE_UNITIALIZED
///
PFCAMERALIB_API PFResult LoadFromFile(PFImage& dst, const char* file_name, const pfImageFileType file_type);


///
/// @brief Unpacks the current PFImage into a new image with the corresponding #pfPixelType.
///
/// If the pixel type of the current PFImage is one of the packed modes, this method unpacks the data into a new image 
/// with the corresponding unpacked #pfPixelType. For instance from Mono10Packed or Mono10p to Mono10.
/// This method only needs a pointer to the destination PFImage. By default, memory allocation is performed inside this method.
/// Nevertheless, the user can provide a destination PFImage with an already allocated memory buffer.
/// In case no unpack process is performed, for instance because the origin #pfPixelType is Mono8, then DestImage will 
/// point to the original image and no memory will be allocated.
/// Following packed pixel types are supported:
/// PixelMono10p
/// PixelMono10Packed
/// PixelMono12p
/// PixelMono12Packed
/// PixelBayerGB10Packed
/// PixelBayerRG10Packed
//  PixelBayerGR10Packed
/// PixelBayerGB12Packed
/// PixelBayerGR12Packed
/// PixelBayerRG12Packed
///
/// @param[in]  src           Source PFImage with packed data  
/// @param[out] dst           Destination PFImage where image is unpacked.
/// @param[in] alloc_mem      Allocate the necessary memory to unpack the current image.
/// @param[in] ctxt           Image transform context
///
/// @return PFResult                
/// Error Codes:
///      PFSDK_NOERROR
///      PFSDK_ERROR_UNPACK_NOT_PERFORMED
///
PFCAMERALIB_API PFResult Unpack(const PFImage& src, PFImage& dst, bool alloc_mem, ImageTransformContext* ctxt = nullptr);

///
/// @brief Applies a debayer filter and stores the result in a new PFImage.
///
/// Applies a debayer filter and stores the result in a new #pfcameralib::PFImage according to the #pfPixelType of the destination PFImage.
/// The destination should have enough memory allocated to store the image converted. \n 
/// #pfPixelType in destination image must be a valid RGB type format (only can be converted pixel types with the same number of bits or less). \n
/// If the conversion is not valid you will receive the error PFSDK_ERROR_DEBAYER_OPERATION_NOT_ALLOWED.\n 
/// In this release you have two debayer methods available: BayerFilterBilinear and BayerFilterNearestNeighbor. \n
/// Currently supported bayer formats are: PixelBayerGB8, PixelBayerGB10, PixelBayerGB12, PixelBayerRG8, PixelBayerRG10, PixelBayerRG12, PixelBayerGR8, PixelBayerGR10, PixelBayerGR12.
///
/// @param[in] src Source #pfcameralib::PFImage with bayer data
/// @param[out] dst Destination #pfcameralib::PFImage where image is debayered
/// @param[in] filter_type Desired Debayer filter
/// @param[in] ctxt Image transformation context
///
/// @return PFResult                
/// Error Codes:
///      PFSDK_NOERROR
///      PFSDK_ERROR_DEBAYER_OPERATION_NOT_ALLOWED
/// 
PFCAMERALIB_API PFResult  Debayer(const PFImage& src, PFImage& dst, const image::type_util::pfBayerFilterType filter_type, ImageTransformContext* ctxt = nullptr);

///
/// @brief Convert RGB image to mono image.
///
/// Convert RGB image to mono image and stores the result in a new #pfcameralib::PFImage according to the #pfPixelType of the destination PFImage.
/// The destination PFImage should be created in advance to perform a valid conversion.
///  A table with the implemented conversions can be found in the SDK manual.
///
/// @param[in] src Source #pfcameralib::PFImage with RGB data
/// @param[out] dst Resulting #pfcameralib::PFImage with Mono data
/// @param[in] ctxt Image transform context
///
/// @return PFResult                
/// Error Codes:
///      PFSDK_NOERROR
///      PFSDK_ERROR_COVERSION_NOT_ALLOWED
/// 
PFCAMERALIB_API PFResult RgbToMono(const PFImage& src, PFImage& dst, ImageTransformContext* ctxt = nullptr);

///
/// @brief Applies a demodulation for Double Rate cameras and stores the result in a new PFImage.
///
/// Applies a demodulation to the input image when the Double Rate feature is enabled and stores the result in a new PFImage according to 
/// the #pfPixelType of the destination PFImage.
/// The destination PFImage should be created in advance to perform a valid demodulation.
/// The Double Rate cameras can be Color or Greyscale. No matter which is #pfPixelType, the input argument color_sensor has to be set to TRUE if the camera is Color.
///
/// @param[in] src source #pfcameralib::PFImage 
/// @param[out] dst resulting #pfcameralib::PFImage
/// @param[in] color_sensor Boolean to know if the DR camera is color
/// @param[in] defaultWidth Double rate width default value if dst image is not initialized
/// @param[in] algorithm Modulation algorithm applied, default value is Double Rate
/// @param[in] ctxt Image transform context
///
/// @return PFResult                
/// Error Codes:
///      PFSDK_NOERROR
///      PFSDK_ERROR_DEMODULATE_IMAGE_DR
/// 
PFCAMERALIB_API PFResult Demodulate(const PFImage& src, PFImage& dst, const bool color_sensor, const uint32_t defaultWidth = 0, const ModulationAlgorithm algorithm = ModulationAlgorithm::DoubleRate, ImageTransformContext* ctxt = nullptr);

///
// @brief Convert the current PFImage into another one with a different #pfPixelType.
///
/// This function coverts the current PFImage into another PFImage according to the #pfPixelType of the destination PFImage.
/// The destination PFImage should be created in advance to perform a valid conversion.
/// Not all the possible conversions between different pixel types are allowed. @ref valid_conversions
/// For packed formats the image will be internally unpacked. \n
/// In case of color formats you can choose the Debayer filter used ( #image::type_util::pfBayerFilterType ).
///
/// <table>
/// <caption id="valid_conversions">List of valid conversions</caption>
/// <tr><th>Source type         <th>Destination type
/// <tr><td> PixelMono10  <td>PixelMono8
/// <tr><td> PixelMono10p <td>PixelMono8, PixelMono10
/// <tr><td> PixelMono10Packed <td>PixelMono8, PixelMono10
/// <tr><td> PixelMono12 <td>PixelMono8, PixelMono10
/// <tr><td> PixelMono12p<td>PixelMono8, PixelMono10, PixelMono12
/// <tr><td> PixelMono12Packed<td>PixelMono8, PixelMono10, PixelMono12
/// <tr><td> PixelMono16<td>PixelMono8, PixelMono10, PixelMono12
/// <tr><td> PixelBayerGB8<td>PixelMono8, PixelRGB8
/// <tr><td> PixelBayerGB10<td>PixelMono8, PixelMono10, PixelBayerGB8, PixelRGB8, PixelRGB10
/// <tr><td> PixelBayerGB10Packed<td>PixelMono8, PixelMono10, PixelBayerGB8, PixelBayerGB10, PixelRGB8, PixelRGB10
/// <tr><td> PixelBayerGB12<td>PixelMono8, PixelMono10, PixelMono12, PixelBayerGB8, PixelBayerGB10, PixelRGB8, PixelRGB10, PixelRGB12
/// <tr><td> PixelBayerGB12Packed<td>PixelMono8, PixelMono10, PixelMono12, PixelBayerGB8, PixelBayerGB10, PixelBayerGB12, PixelRGB8, PixelRGB10, PixelRGB12
/// <tr><td> PixelRGB8<td>PixelMono8
/// <tr><td> PixelRGB10<td>PixelMono8, PixelMono10, PixelRGB8
/// <tr><td> PixelRGB12<td>PixelMono8, PixelMono10, PixelMono12, PixelRGB8, PixelRGB10
/// <tr><td> PixelBayerRG8<td>PixelMono8, PixelRGB8
/// <tr><td> PixelBayerRG10<td>PixelMono8, PixelMono10, PixelBayerRG8, PixelRGB8, PixelRGB10
/// <tr><td> PixelBayerRG10Packed<td>PixelMono8, PixelMono10, PixelBayerRG8, PixelBayerRG10, PixelRGB8, PixelRGB10
/// <tr><td> PixelBayerRG12<td>PixelMono8, PixelMono10, PixelMono12, PixelBayerRG8, PixelBayerRG10, PixelRGB8, PixelRGB10, PixelRGB12
/// <tr><td> PixelBayerRG12Packed<td>PixelMono8, PixelMono10, PixelMono12, PixelBayerRG8, PixelBayerRG10, PixelBayerRG12, PixelRGB8, PixelRGB10, PixelRGB12
/// <tr><td> PixelBayerGR8<td>PixelMono8, PixelRGB8
/// <tr><td> PixelBayerGR10<td>PixelMono8, PixelMono10, PixelBayerGR8, PixelRGB8, PixelRGB10
/// <tr><td> PixelBayerGR10Packed<td>PixelMono8, PixelMono10, PixelBayerGR8, PixelBayerGR10, PixelRGB8, PixelRGB10
/// <tr><td> PixelBayerGR12<td>PixelMono8, PixelMono10, PixelMono12, PixelBayerGR8, PixelBayerGR10, PixelRGB8, PixelRGB10, PixelRGB12
/// <tr><td> PixelBayerGR12Packed<td>PixelMono8, PixelMono10, PixelMono12, PixelBayerGR8, PixelBayerGR10, PixelBayerGR12, PixelRGB8, PixelRGB10, PixelRGB12
/// </table>

/// @param[in] src                      Source #pfcameralib::PFImage input image
/// @param[out] dst                     Destination #pfcameralib::PFImage converted image
/// @param[in] dst_type                 Destination type
/// @param[in] filter_type              Desired Debayer filter in case it is needed for the conversion. 
/// @param[in] ctxt                     Image transform context
///
/// @return PFResult                
/// Error Codes:
///      PFSDK_NOERROR
///      PFSDK_ERROR_CONVERSION_NOT_ALLOWED
///      PFSDK_ERROR_DEBAYER_OPERATION_NOT_ALLOWED
/// 
PFCAMERALIB_API PFResult Convert(const PFImage& src, PFImage& dst, const pfPixelType dst_type, const pfBayerFilterType filter_type = pfBayerFilterType::BayerFilterNearestNeighbor, ImageTransformContext* ctxt = nullptr);

///
// @brief Convert the current #pfcameralib::PFImage into another one with a different #pfPixelType.
///
/// This function coverts the current #pfcameralib::PFImage into another #pfcameralib::PFImage according to the #pfPixelType of the destination #pfcameralib::PFImage
/// The destination #pfcameralib::PFImage should be created in advance to perform a valid conversion.
/// Not all the possible conversions between different pixel types are allowed. @ref valid_conversions
/// For packed formats the image will be internally unpacked. \n
/// In case of color formats you can choose the Debayer filter used ( #image::type_util::pfBayerFilterType ).
///
/// @param[out] src                      Source #pfcameralib::PFImage input image
/// @param[out] dst                      Destination #pfcameralib::PFImage converted image
/// @param[in] filter_type               Desired Debayer filter in case it is needed for the conversion. 
/// @param[in] ctxt                      Image transform context
///
/// @return PFResult                
/// Error Codes:
///      PFSDK_NOERROR
///      PFSDK_ERROR_CONVERSION_NOT_ALLOWED
///      PFSDK_ERROR_DEBAYER_OPERATION_NOT_ALLOWED
/// 
PFCAMERALIB_API PFResult Convert(const PFImage& src, PFImage& dst, const pfBayerFilterType filter_type = pfBayerFilterType::BayerFilterNearestNeighbor, ImageTransformContext* ctxt = nullptr);

} // end namespace core 
} // end namespace image 
} // end namespace pfcameralib


#endif
