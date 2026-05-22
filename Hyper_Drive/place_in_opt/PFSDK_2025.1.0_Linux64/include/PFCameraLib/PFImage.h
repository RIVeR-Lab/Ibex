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
/// @file PFImage.h 
/// @brief Class containing the attributes and data of a received image. It has methods to modify the image.
///

#ifndef PFIMAGE_H
#define PFIMAGE_H

#include "PFResult.h"
#include "PFCameraLib.h"
#include "pfcPixelTypes.h"
#include "ImageTypes.h"

namespace pfcameralib
{
     /// 
    /// @deprecated since release 1.1.7 use GetPixelSizeBytes() from namespace image::type_util
    /// @brief Get the number of bytes required to allocate a single #pfPixelType.
    ///
    /// @param[in] pixelType    #pfPixelType accorded to GigE Vision specification
    /// @return  Number of bytes
    /// 
    float PFCAMERALIB_API GetPixelSizeBytes(pfPixelType pixelType);

    /// 
    /// @deprecated since release 1.1.7 use IsPackedPixelType() from namespace image::type_util
    /// @brief Check if the pixel type is packed.
    ///
    /// @param[in] pixelType    pixelType enum value accorded to GigE Vision specification
    /// @return  Boolean indicating packed pixel type.
    /// 
    bool PFCAMERALIB_API IsPackedPixelType(pfPixelType pixelType);
      
    /// 
    /// @class PFImage PFImage.h
    ///
    /// This class contains the attributes of a received image plus some methods to convert it to another #pfPixelType or to apply to debayer filter.
    /// The image can be stored in a file in different formats such as png, bmp, or binary (raw data).
    ///
    class PFCAMERALIB_API PFImage
    {
    public:
        /// 
        /// @brief Default class constructor
        ///
        /// This is the default constructor called by the #PFBuffer class. It creates an image of zero size before being filled by the received data.
        ///
        PFImage();

        /// 
        /// @brief Public class constructor
        ///
        /// This constructor is intended to create an independent #PFImage.  
        /// @param[in] PixelType type used in the image
        /// @param[in] Width number of pixels
        /// @param[in] Height number of pixels
        /// @param[in] OffsetX X 
        /// @param[in] OffsetY Y 
        /// @param[in] PaddingX X 
        /// @param[in] PaddingY Y 
        ///
        PFImage(pfPixelType PixelType, uint32_t Width, uint32_t Height, uint32_t OffsetX = 0, uint32_t OffsetY = 0, uint16_t PaddingX = 0, uint16_t PaddingY = 0);
        
        /// 
        /// @brief Public class constructor
        ///
        /// This constructor is intended to create a #PFImage.  
        /// @param[in] PixelType type used in the image
        /// @param[in] Width number of pixels
        /// @param[in] Height number of pixels
        /// @param[in] OffsetX X 
        /// @param[in] OffsetY Y 
        /// @param[in] PaddingX X 
        /// @param[in] PaddingY Y 
        /// @param[in] ImageSize size 
        /// @param[in] ImageData data
        ///
        PFImage(pfPixelType PixelType, uint32_t Width, uint32_t Height, uint32_t OffsetX, uint32_t OffsetY, uint16_t PaddingX, uint16_t PaddingY, uint64_t ImageSize, uint8_t *ImageData);

        ///
        /// @brief Copy constructor
        ///
        PFImage(const PFImage& image);

        ///
        /// @brief Public class destructor
        ///
        ~PFImage();
        
        /// 
        /// @brief Assignment operator. The buffer for the image is reserved if m_memManaged is true
        /// @param[in] image to be assigned 
        ///
        PFImage & operator= (const PFImage &image);

        ///
        /// @brief PFImage move constructor
        /// @param[in] image to be moved
        ///
        PFImage(PFImage&& image);

        ///
        /// @brief Move assignment, transfer ownership
        /// @param[in] image to be assigned
        /// @return reference to PFImage object with image contents
        ///
        PFImage& operator=(PFImage&& image);

        ///
        /// @brief Assign image. Assign each property from image. The buffer image is not allocated.
        /// @param[in] image to be assigned 
        ///
        void AssignImage(const PFImage &image);

        ///
        /// @brief Copy the input image, allocating memory and copying data.
        /// @param[in] image to be cloned 
        ///
        void CloneImage(const PFImage& image);

        /// 
        /// @brief Reserve the image buffer. This method reserves memory buffer to allocate an image of sizeX * SizeY space. 
        /// @param[in] pixelTypeDest Depending on the pixelType used, the memory requirements will be different. 
        /// @param[in] sizeX Width number of pixels
        /// @param[in] sizeY Height number of pixels
        PFResult ReserveImage(pfPixelType pixelTypeDest, uint32_t sizeX, uint32_t sizeY);

        /// 
        /// @brief Release the image buffer 
        ///
        void ReleaseImage();
                    
        /// @name Image Attributes
        /// Methods to get the #PFImage attributes.
        ///
        ///@{

        /// 
        /// @brief Get the image's height. 
        ///
        /// @return  The height of the image in pixels  
        /// 
        uint32_t GetHeight() const;

        /// 
        /// @brief Get the image's width. 
        ///
        /// @return  The width of the image in pixels  
        /// 
        uint32_t GetWidth() const;

        /// 
        /// @brief Get the image's horizontal offset. 
        ///
        /// @return  The horizontal offset of the image in pixels  
        /// 
        uint32_t GetOffsetX() const;

        /// 
        /// @brief Get the image's vertical offset
        ///
        /// @return  The vertical offset of the image in pixels  
        /// 
        uint32_t GetOffsetY() const;

        /// 
        /// @brief Get the actual image size
        ///
        /// @return  The actual image size
        /// 
        uint64_t GetImageSize() const;

        /// 
        /// @brief Get a pointer to the image data
        ///
        /// @return  A pointer to the memory image data 
        /// 
        uint8_t* GetImageData() const;

        /// 
        /// @brief Get the Pixel type of the image
        ///
        /// @return  The #pfPixelType of the image
        /// 
        pfPixelType GetPixelType() const;
                
        /// 
        /// @brief Get the Padding X of the image
        ///
        /// @return  The padding of the image
        /// 
        uint16_t GetPaddingX() const;

        /// 
        /// @brief Get the Padding Y of the image
        ///
        /// @return  The padding of the image
        /// 
        uint16_t GetPaddingY() const;

        /// 
        /// @brief Returns true if memory has been allocated within the object.
        ///
        /// @return  True if memory has been allocated within the object.
        /// 
        bool IsMemAllocated() const;

        ///
        /// @deprecated since release 1.1.7 use Unpack() from namespace image::core 
        /// @brief Unpacks the current #PFImage into a new image with the corresponding #pfPixelType.
        ///
        /// If the pixel type of the current #PFImage is one of the packed modes, this method unpacks the data into a new image 
        /// with the corresponding unpacked #pfPixelType. For instance from Mono10Packed or Mono10p to Mono10.
        /// This method only needs a pointer to the destination #PFImage. By default, memory allocation is performed inside this method.
        /// Nevertheless, the user can provide a destination #PFImage with an already allocated memory buffer.
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
        /// @param[out] *DestImage                      Pointer to destination #PFImage where current image is unpacked.
        /// @param[in] AllocMemory                      Allocate the necessary memory to unpack the current image.
        ///
        /// @return PFResult                
        /// Error Codes:
        ///      PFSDK_NOERROR
        ///      PFSDK_ERROR_UNPACK_NOT_PERFORMED
        ///
        PFResult Unpack(PFImage &DestImage, bool AllocMemory = true) const;

        ///
        /// @deprecated since release 1.1.7 use ConvertTo() from namespace image::core  
        /// @brief Convert the current #PFImage into another one with a different #pfPixelType.
        ///
        /// This function coverts the current #PFImage into another #PFImage according to the #pfPixelType of the destination #PFImage.
        /// The destination #PFImage should be created in advance to perform a valid conversion.
        /// Not all the possible conversions between different pixel types are allowed. @ref valid_conversions1
        /// For packed formats the image will be internally unpacked. \n
        /// In case of color formats you can choose the Debayer filter used ( #image::type_util::pfBayerFilterType ).
        ///
        /// <table>
        /// <caption id="valid_conversions1">List of valid conversions</caption>
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

        /// @param[out] dst                      Pointer to the resulting #PFImage
        /// @param[in] filter_type               Desired Debayer filter in case it is needed for the conversion. 
        ///
        /// @return PFResult                
        /// Error Codes:
        ///      PFSDK_NOERROR
        ///      PFSDK_ERROR_CONVERSION_NOT_ALLOWED
        ///      PFSDK_ERROR_DEBAYER_OPERATION_NOT_ALLOWED
        /// 
        PFResult ConvertTo(PFImage &dst, image::type_util::pfBayerFilterType filter_type = image::type_util::pfBayerFilterType::BayerFilterNearestNeighbor) const;

        ///
        /// @deprecated since release 1.1.7 use Debayer() from namespace image::core 
        /// @brief Applies a debayer filter and stores the result in a new #PFImage.
        ///
        /// Applies a debayer filter and stores the result in a new #PFImage according to the #pfPixelType of the destination PFImage.
        /// The destination should have enough memory allocated to store the image converted. \n 
        /// #pfPixelType in destination image must be a valid RGB type format (only can be converted pixel types with the same number of bits or less). \n
        /// If the conversion is not valid you will receive the error PFSDK_ERROR_DEBAYER_OPERATION_NOT_ALLOWED.\n 
        /// In this release you have two debayer methods available: BayerFilterBilinear and BayerFilterNearestNeighbor. \n
        /// Currently supported bayer formats are: PixelBayerGB8, PixelBayerGB10, PixelBayerGB12, PixelBayerRG8, PixelBayerRG10, PixelBayerRG12, PixelBayerGR8, PixelBayerGR10, PixelBayerGR12.
        ///
        /// @param[out] dst Pointer to the resulting #PFImage
        /// @param[in] filter_type Desired Debayer filter
        ///
        /// @return PFResult                
        /// Error Codes:
        ///      PFSDK_NOERROR
        ///      PFSDK_ERROR_DEBAYER_OPERATION_NOT_ALLOWED
        /// 
        PFResult Debayer(PFImage& dst, image::type_util::pfBayerFilterType filter_type) const;

        ///
        /// @deprecated since release 1.1.7 use Demodulate() from namespace image::ImageUtils
        /// @brief Applies a demodulation for Double Rate cameras and stores the result in a new #PFImage.
        ///
        /// Applies a demodulation to the input image when the Double Rate feature is enabled and stores the result in a new #PFImage according to 
        /// the #pfPixelType of the destination PFImage.
        /// The destination PFImage should be created in advance to perform a valid demodulation.
        /// The Double Rate cameras can be Color or Greyscale. No matter which is #pfPixelType, the input argument colorDR has to be set to TRUE if the camera is Color.
        ///
        /// @param[out] dst Pointer to the resulting #PFImage
        /// @param[in] color_sensor Boolean to know if the DR camera is color
        ///
        /// @return PFResult                
        /// Error Codes:
        ///      PFSDK_NOERROR
        ///      PFSDK_ERROR_DEMODULATE_IMAGE_DR
        /// 
        PFResult DemodulateDR(PFImage &dst, bool color_sensor) const;
    
        ///
        /// @deprecated since release 1.1.7 use RgbToMono() from namespace image::core 
        /// @brief Convert RGB image to mono image.
        ///
        /// Convert RGB image to mono image and stores the result in a new #PFImage according to the #pfPixelType of the destination PFImage.
        /// The destination PFImage should be created in advance to perform a valid conversion.
        ///  A table with the implemented conversions can be found in the SDK manual.
        ///
        /// @param[out] dst Pointer to the resulting #PFImage
        ///
        /// @return PFResult                
        /// Error Codes:
        ///      PFSDK_NOERROR
        ///      PFSDK_ERROR_COVERSION_NOT_ALLOWED
        /// 
        PFResult RgbToMono(PFImage &dst) const;

        ///@}


        /// @name Store and Load    
        /// Functions to save the image into a file and create a new image out of a raw file
        ///
        ///@{

        /// 
        /// @deprecated since release 1.1.7 use SaveToFile() from namespace image::core 
        /// @brief Store the PFImage into an image file
        ///
        /// This function stores the PFImage into a system file in different formats such as png, bmp or binary (raw data).
        ///
        /// @param[in] file_name     File name including path.
        /// @param[in] file_type     File type or extension. Only few extensions are available
        ///
        /// @return PFResult                
        /// Error Codes:
        ///      PFSDK_ERROR_OPEN_FILE_FAILED
        ///      PFSDK_ERROR_SAVE_IMAGE_TO_FILE
        ///      PFSDK_NOERROR
        /// 
        PFResult SaveToFile(const char* file_name, image::type_util::pfImageFileType file_type) const;

        ///
        /// @deprecated since release 1.1.7 use LoadFromFile() from namespace image::core 
        /// @brief Creates a PFImage from a stored image file in disk
        ///
        /// This function creates a PFImage from a binary file stored in (raw data).
        ///
        /// @param[in] file_name File name including path and extension.
        ///
        /// @return PFResult                
        /// Error Codes:
        ///      PFSDK_ERROR_OPEN_FILE_FAILED
        ///      PFSDK_ERROR_LOAD_IMAGE_FROM_FILE
        ///
        PFResult LoadFromBinaryFile(const char* file_name);
                        
        
    private:
        friend class PFCamera;
        friend class PFBuffer;
        class PFImageImpl;
        PFImageImpl* m_pImpl;


    };
}

#endif //PFIMAGE_H