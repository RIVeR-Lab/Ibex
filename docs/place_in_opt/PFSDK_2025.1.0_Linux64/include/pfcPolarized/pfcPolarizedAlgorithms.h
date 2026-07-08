#ifndef _POLARIZEDALGORITHMS_H_
#define _POLARIZEDALGORITHMS_H_

#include <stdint.h>
#include "pfcPolarizedDefs.h"
#include "pfpolclib_export.h"
#include "pfcPixelTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

///
/// @brief Allocates a buffer to store a raw polarized image of size  m_originalImageWidth*m_originalImageHeight
///
///	@param[in] m_originalImageWidth			Width of the original image
///	@param[in] m_originalImageHeight		Height of the original image
///
///
///	@return					Pointer to the allocated buffer 
///
PFPOLCLIB_EXPORT uint8_t* pfcAllocateImage_u8(uint16_t m_originalImageWidth, uint16_t m_originalImageHeight);


///
/// @brief Deallocate a buffer previously allocated with pfcAllocateImage_u8
///
///	@param[in] p							Pointer of a buffer allocated with pfcAllocateImage_u8
///	@param[in] m_originalImageWidth			Width of the original image
///	@param[in] m_originalImageHeight		Height of the original image
///
PFPOLCLIB_EXPORT void pfcFreeImage_u8(uint8_t* p, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight);
///
/// @brief Allocates a buffer to store a raw polarized image of size  m_originalImageWidth*m_originalImageHeight
///
///	@param[in] m_originalImageWidth			Width of the original image
///	@param[in] m_originalImageHeight		Height of the original image
///
///
///	@return									Pointer to the allocated buffer 
///
PFPOLCLIB_EXPORT uint16_t* pfcAllocateImage_u16(uint16_t m_originalImageWidth, uint16_t m_originalImageHeight);


///
/// @brief Deallocate a buffer previously allocated with pfcAllocateImage_u16
///
///	@param[in] p							Pointer of a buffer allocated with pfcAllocateImage_u16
///	@param[in] m_originalImageWidth			Width of the original image
///	@param[in] m_originalImageHeight		Height of the original image
///
PFPOLCLIB_EXPORT void pfcFreeImage_u16(uint16_t* p, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight);

///
/// @brief Reads .raw image from path and stores it in target buffer
///
///	@param[in] fullpath						Path to input raw image
///	@param[in] m_originalImageWidth			Width of the original image
///	@param[in] m_originalImageHeight		Height of the original image
///	@param[in] m_pixelType					Sets if pixel is MONO8, MONO10 or MONO12 (only MONO8 supported in this version)
///
///
///	@param[out]	target						Buffer containing read image (width x height)
///
PFPOLCLIB_EXPORT int32_t pfcReadRawImage(uint16_t* target, const char* fullpath, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, pfPixelType m_pixelType);

///
/// @brief Saves raw image in disk
///
///	@param[in] inputBuffer					Buffer containing the image to be written to disk
///	@param[in] m_angleWidth					Width of the output image
///	@param[in] m_angleHeight				Height of the output image
///	@param[in] path							Path to output destination
///	@param[in] filename						Output file name
///	@param[in] m_pixelType					Sets if pixel is MONO8, MONO10, MONO12 or MONO16
///
PFPOLCLIB_EXPORT int32_t pfcSavePolarizedRawImage(uint16_t* inputBuffer, uint16_t m_angleWidth, uint16_t m_angleHeight, const char* path, const char* filename, pfPixelType m_pixelType);

///
/// @brief Saves Png image in disk
///
///	@param[in] inputBuffer					Buffer containing the image to be written to disk
///	@param[in] m_angleWidth					Width of the output image
///	@param[in] m_angleHeight				Height of the output image
///	@param[in] path							Path to output destination
///	@param[in] filename						Output file name
///	@param[in] m_pixelType					Sets if pixel is MONO8, MONO10, MONO12 or MONO16
///
PFPOLCLIB_EXPORT int32_t pfcSavePolarizedPngImage(uint16_t* inputBuffer, uint16_t m_angleWidth, uint16_t m_angleHeight, const char* path, const char* filename, pfPixelType m_pixelType);

///
/// @brief Saves a color Png image in disk
///
///	@param[in] inputRGBBuffer				Buffer containing the RGB image to be written to disk
///	@param[in] m_angleWidth					Width of the output image
///	@param[in] m_angleHeight				Height of the output image
///	@param[in] path							Path to output destination
///	@param[in] filename						Output file name
///
PFPOLCLIB_EXPORT int32_t pfcSavePolarizedColorPngImage(uint8_t* inputRGBBuffer, uint16_t m_angleWidth, uint16_t m_angleHeight, const char* path, const char* filename);

///
/// @brief Generates a (width x height) image of the selected angle for MONO camera.
///
///	@param[in] target						Original image buffer with a 2x2 pattern containing 90_45_135_0 polarization angles
///	@param[in] m_originalImageWidth			Width of the original image
///	@param[in] m_originalImageHeight		Height of the original image
/// @param[in]	angle						Selected angle {deg_0, deg_45, deg_90, deg_135}
///
///	@param[out]	result						Buffer containing output image (width x height)
///
PFPOLCLIB_EXPORT int32_t pfcGenerateAngleImage_u16(uint16_t* target, uint16_t* result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, uint16_t angle);

/// @brief Generates a virtual angle mono image (0 - 180 degrees) for MONO camera.
///
///
///	@param[in]	target						Original image buffer with a 2x2 pattern containing 90_45_135_0 polarization angles
///	@param[in]	m_originalImageWidth		Width of the original image
///	@param[in]	m_originalImageHeight		Height of the original image
///	@param[in] angledegree					Angle of the desired virtual angle (0 - 180)
///
///	@param[out]	result						Buffer containing the output image at an estimated virtual polarization angle
///
PFPOLCLIB_EXPORT int32_t pfcGenerateVirtualAngleImage_u16(uint16_t* target, uint16_t* result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, uint16_t angledegree);

/// @brief Generates a (2*width x 2*height) image containing all the polarization angles (each angle is located in a corner of the image) for MONO camera.
///
///
///	@param[in]	target						Original image buffer with a 2x2 pattern containing 90_45_135_0 polarization angles
///	@param[in]	m_originalImageWidth		Width of the original image
///	@param[in]	m_originalImageHeight		Height of the original image
///
///	@param[out]	result						Buffer containing output image (4*width x 4*height)
///
PFPOLCLIB_EXPORT int32_t pfcGenerateAnglesGrid_u16(uint16_t* target, uint16_t* result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight);

/// @brief Computes Degree of Linear Polarization (DOLP) of the input image and outputs a (width*height) image for MONO camera.
///
/// If DOLP is out of a certain range (defined by input parameters stard_range, end_range), its value is automatically set to format max value (for instance, for Mono8 images pixel is set to 255).
/// Pixels out of range will then appear as white pixels.
///
///	@param[in]	target						Original image buffer with a 2x2 pattern containing 90_45_135_0 polarization angles
///	@param[in]	m_originalImageWidth		Width of the original image
///	@param[in]	m_originalImageHeight		Height of the original image
///	@param[in]	start_range					Defines the beggining of the relative range of valid values (start_range*max_format_val).
///	@param[in]	end_range					Defines the end of the relative range of valid values (end_range*max_format_val).
///
///	@param[out]	result						Output buffer containing the DOLP image (width x height)
///	@param[in] pixelType					Sets if pixel is MONO8, MONO10, MONO12 or MONO16
///
PFPOLCLIB_EXPORT int32_t pfcGenerateDOLPImage_u16(uint16_t* target, uint16_t* result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, float start_range, float end_range, pfPixelType pixelType);

/// @brief Computes Angle of Linear Polarization (AOLP) of the input image based on the four polarization angles, and outputs
/// an interpolated (width*height) image for MONO camera.
///
/// @param[in]	target						Original image buffer with a 2x2 pattern containing 90_45_135_0 polarization angles
/// @param[in]	m_originalImageWidth		Width of the original image
/// @param[in]	m_originalImageHeight		Height of the original image
///
/// @param[out]	result						Output buffer containing the AOLP image (width x height)
///	@param[in] pixelType					Sets if pixel is MONO8, MONO10, MONO12 or MONO16
///
PFPOLCLIB_EXPORT int32_t pfcGenerateAOLPImage_u16(uint16_t* target, uint16_t* result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, pfPixelType pixelType);

/// @brief Computes a (width*height) RGB image that contains both DOLP and AOLP information for MONO camera.
///
/// Generates a DOLP and AOLP image and then treats AOLP as hue and DOLP as saturation.
///											
///	@param[in]	target						Original image buffer with a 2x2 pattern containing 90_45_135_0 polarization angles
///	@param[in]	m_originalImageWidth		Width of the original image
///	@param[in]	m_originalImageHeight		Height of the original image

///	@param[out]	result						Output buffer containing the DOLPAOLP RGB image (width x height * 3)
///	@param[in] pixelType					Sets if pixel is MONO8, MONO10, MONO12 or MONO16
///
PFPOLCLIB_EXPORT int32_t pfcGenerateAOLPDOLPImage_u8(uint16_t* target, uint8_t* result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, pfPixelType pixelType);

/// @brief Computes a (width*height) RGB image that contains AOLP information for MONO camera.
///
/// Generates an AOLP image and treats AOLP as hue.
///											
/// @param[in]	target						Original image buffer with a 2x2 pattern containing 90_45_135_0 polarization angles
/// @param[in]	m_originalImageWidth		Width of the original image
/// @param[in]	m_originalImageHeight		Height of the original image
/// 
/// @param[out]	result						Output buffer containing the AOLP RGB image (width x height * 3)
///	@param[in] pixelType					Sets if pixel is MONO8, MONO10, MONO12 or MONO16
///
PFPOLCLIB_EXPORT int32_t pfcGenerateAOLP_HSVImage_u8(uint16_t* target, uint8_t* result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, pfPixelType pixelType);

/// @brief Fuses an AOLP and DOLP image into its HSV representation. AOLP being the hue and DOLP being the saturation.
/// in case DOLP is a nullptr, saturation is set to 1.
///											
/// @param[in]	aolp		AOLP
/// @param[in]	dolp		DOLP, may be null
/// @param[out]	result		Output buffer containing the AOLP RGB image (width x height * 3)
/// @param[in]	width		Width of the original image
/// @param[in]	height		Height of the original image
///	@param[in] pixelType	Sets if pixel is MONO8, MONO10, MONO12 or MONO16
/// @param[in] format		Color format of the output image 
///
PFPOLCLIB_EXPORT int32_t pfcConvertAOLPDOLPToHSV(const uint16_t* aolp, const uint16_t* dolp, uint8_t* result, size_t width, size_t height, pfPixelType pixelType, pfColorImageFormat format = BGR);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif