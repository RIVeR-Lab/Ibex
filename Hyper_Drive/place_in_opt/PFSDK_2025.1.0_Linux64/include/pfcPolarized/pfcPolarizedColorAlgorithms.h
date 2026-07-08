#ifndef _pfcPolarizedColorAlgorithms_H_
#define _pfcPolarizedColorAlgorithms_H_

#include <stdint.h>
#include "pfcPolarizedDefs.h"
#include "pfcPixelTypes.h"
#include "pfpolclib_export.h"

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
///	@return									Pointer to the allocated buffer 
///
PFPOLCLIB_EXPORT uint8_t* pfcAllocateColorImage_u8(uint16_t m_originalImageWidth, uint16_t m_originalImageHeight);


///
/// @brief Deallocate a buffer previously allocated with pfcAllocateImage_u8
///
///	@param[in] p							Pointer of a buffer allocated with pfcAllocateColorImage_u8
///	@param[in] m_originalImageWidth			Width of the original image
///	@param[in] m_originalImageHeight		Height of the original image
///
PFPOLCLIB_EXPORT void pfcFreeColorImage_u8(uint8_t* p, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight);
///
/// @brief Allocates a buffer to store a raw polarized image of size  m_originalImageWidth*m_originalImageHeight
///
///	@param[in] m_originalImageWidth			Width of the original image
///	@param[in] m_originalImageHeight		Height of the original image
///
///
///	@return									Pointer to the allocated buffer 
///
PFPOLCLIB_EXPORT uint16_t* pfcAllocateColorImage_u16(uint16_t m_originalImageWidth, uint16_t m_originalImageHeight);


///
/// @brief Deallocate a buffer previously allocated with pfcAllocateImage_u16
///
///	@param[in] p							Pointer of a buffer allocated with pfcAllocateColorImage_u8
///	@param[in] m_originalImageWidth			Width of the original image
///	@param[in] m_originalImageHeight		Height of the original image
///
PFPOLCLIB_EXPORT void pfcFreeColorImage_u16(uint16_t* p, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight);
///
/// @brief Allocates a polarized image with format pppp_image[angle][color][row][col]
///
///	@param[in] m_originalImageWidth			Width of the original image
///	@param[in] m_originalImageHeight		Height of the original image
///
///	@param[out] o_ppppp						Pointer to polarized image
///
PFPOLCLIB_EXPORT int32_t pfcAllocatePolarizedImage(uint16_t***** o_ppppp, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight);


///
/// @brief For color polarized camera only, generates the polarized image with format pppp_image[angle][color][row][col], obtaining one
/// m_angleWidth x m_angleHeight image for each angle and color.
///
///	@param[in] bufferImage					Buffer with input image
///	@param[in] m_originalImageWidth			Width of the original image
///	@param[in] m_originalImageHeight		Height of the original image
///	@param[in] m_angleWidth					Width of the output hypercube (by default m_originalImageWidth/2)
///	@param[in] m_angleHeight				Height of the output hypercube (by default m_originalImageHeight/2)
///	@param[in] pixelType					Sets if pixel is MONO8, MONO10, MONO12 or MONO16
///
///
///	@param[out] pppp_result					Hypercube containing the images in fromat ppp_result[angle][color]
///
PFPOLCLIB_EXPORT int32_t pfcLoadPolarizedRGBImage(uint16_t**** pppp_result, uint16_t* bufferImage, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, uint16_t m_angleWidth, uint16_t m_angleHeight, pfPixelType pixelType);

///
/// @brief For color polarized camera only, uses the previously generated pppp_result hypercube to obtain mono images for each polarization angle.
/// The formula for luminosity is 0.21 R + 0.72 G + 0.07 B (ITU-R BT.709)
///
///	@param[in] pppp_result					Polarized image containing Red, Green and Blue images for each polarization angle
///	@param[in] m_originalImageWidth			Width of the original image
///	@param[in] m_originalImageHeight		Height of the original image
///	@param[in] m_angleWidth					Width of the output hypercube (by default m_originalImageWidth/2)
///	@param[in] m_angleHeight				Height of the output hypercube (by default m_originalImageHeight/2)
///
///
///	@param[out] ppppp_monoresult			Polarized image containing mono images in fromat ppp_monoresult[angle][color]
///
PFPOLCLIB_EXPORT int32_t pfcGeneratePolarizedMonoImage(uint16_t***** ppppp_monoresult, uint16_t**** pppp_result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, uint16_t m_angleWidth, uint16_t m_angleHeight);

/// @brief Computes Degree of Linear Polarization (DOLP) of the input image based on the four polarization angles, and outputs
/// a (m_originalImageWidth*m_originalImageHeight) image using bilinear interpolation.
///
/// If DOLP is out of a certain range (defined by input parameters stard_range, end_range), its value is automatically set to 255 (white pixel).
///
///	@param[in]	target					Pointer to hypercube containing all angles and colors target[angle][color]. Mono hypercube is also usable.
///	@param[in]	m_originalImageWidth	Width of the original image
///	@param[in]	m_originalImageHeight	Height of the original image
///	@param[in]	start_range				Defines the beggining of the relative range of valid values (start_range*255).
///	@param[in]	end_range				Defines the end of the relative range of valid values (end_range*255).
/// @param[in]	color					Selects the color to be used to compute AOLPDOLP. MonoHypercube can also be used in this function:
///											color = {Red, Green, Blue, Mono}
///	@param[in]  pixelType				Sets if pixel is MONO8, MONO10, MONO12 or MONO16
///
///	@param[out]	result					Output buffer containing the DOLP image (width x height)
///
PFPOLCLIB_EXPORT int32_t pfcGenerateDOLPColorImage_u16(uint16_t**** target, uint16_t* result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, float start_range, float end_range, uint16_t color, pfPixelType pixelType);

/// @brief Computes Angle of Linear Polarization (AOLP) of the input image based on the four polarization angles, and outputs
/// a (width*height) image using Nearest Neighbor.
///
/// @param[in]	target					Pointer to hypercube containing all angles and colors target[angle][color]. Mono hypercube is also usable.
/// @param[in]	m_originalImageWidth	Width of the original image
/// @param[in]	m_originalImageHeight	Height of the original image
/// @param[in]	color					Selects the color to be used to compute AOLPDOLP. MonoHypercube can also be used in this function:
///											color = {Red, Green, Blue, Mono}
///	@param[in]  pixelType				Sets if pixel is MONO8, MONO10, MONO12 or MONO16
///
/// @param[out]	result					Output buffer containing the AOLP image (width x height)
///
PFPOLCLIB_EXPORT int32_t pfcGenerateAOLPColorImage_u16(uint16_t**** target, uint16_t* result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, uint16_t color, pfPixelType pixelType);

/// @brief Computes a (width*height) RGB image that contains both DOLP and AOLP information.
///
/// Generates a DOLP and AOLP image and then treats AOLP as hue and DOLP as saturation.
///											
/// @param[in]	target					Pointer to hypercube containing all angles and colors target[angle][color]. Mono hypercube is also usable.
/// @param[in]	m_originalImageWidth	Width of the original image
/// @param[in]	m_originalImageHeight	Height of the original image
/// @param[in]	color					Selects the color to be used to compute AOLPDOLP. MonoHypercube can also be used in this function:
///											color = {Red, Green, Blue, Mono}
///	@param[in]  pixelType				Sets if pixel is MONO8, MONO10, MONO12 or MONO16
///
/// @param[out]	result					Output buffer containing the DOLPAOLP RGB image (width x height * 3)
///
PFPOLCLIB_EXPORT int32_t pfcGenerateAOLPDOLPColorImage_u8(uint16_t**** target, uint8_t* result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, uint16_t color, pfPixelType pixelType);

/// @brief Computes a (width*height) RGB image that contains AOLP information.
///
/// Generates an AOLP image and treats AOLP as hue.
///											
/// @param[in]	target					Pointer to hypercube containing all angles and colors target[angle][color]
/// @param[in]	m_originalImageWidth	Width of the original image
/// @param[in]	m_originalImageHeight	Height of the original image
/// @param[in]	color					Selects the color to be used to compute AOLPDOLP. MonoHypercube can also be used in this function:
///											color = {Red, Green, Blue, Mono}
///	@param[in]  pixelType				Sets if pixel is MONO8, MONO10, MONO12 or MONO16
///
/// @param[out]	result					Output buffer containing the AOLP RGB image (width x height * 3)
///
PFPOLCLIB_EXPORT int32_t pfcGenerateAOLP_HSVColorImage_u8(uint16_t**** target, uint8_t* result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, uint16_t color, pfPixelType pixelType);

/// @brief Generates a buffer containing RGB data of one angle.
///											
/// @param[in]	target					Pointer to hypercube containing all angles and colors target[angle][color]
/// @param[in]	m_originalImageWidth	Width of the original image
/// @param[in]	m_originalImageHeight	Height of the original image
/// @param[in]	angle					Selected angle {deg_0, deg_45, deg_90, deg_135}
///	@param[in]  pixelType				Sets if pixel is MONO8, MONO10, MONO12 or MONO16
///
/// @param[out]	result					Output buffer containing the RGB image (width x height * 3)
///
PFPOLCLIB_EXPORT int32_t pfcGenerateRGB_ColorImage_u8(uint16_t**** target, uint8_t* result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, uint16_t angle, pfPixelType pixelType);

///
/// @brief For color polarized camera, generates a RGB image of one virtual angle (0-180 degrees).
///
///	@param[in] target						Color Hypercube with format target[angle][color]
///	@param[in] m_originalImageWidth			Width of the original image
///	@param[in] m_originalImageHeight		Height of the original image
///	@param[in] angledegree					Angle of the desired virtual angle (0 - 180)
///	@param[in] pixelType					Sets if pixel is MONO8, MONO10, MONO12 or MONO16
///
///	@param[out] result						Output buffer containing the RGB image (width x height * 3)
///
PFPOLCLIB_EXPORT int32_t pfcGenerateRGB_VirtualColorImage_u8(uint16_t**** target, uint8_t* result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, uint16_t angledegree, pfPixelType pixelType);

///
/// @brief For color polarized camera, generate a virtual angle for one of the color channels. 
///
///	@param[in] target						Color Hypercube with format target[angle][color]
///	@param[in] m_originalImageWidth			Width of the original image
///	@param[in] m_originalImageHeight		Height of the original image
///	@param[in] angledegree					Angle of the desired virtual angle (0 - 180)
///	@param[in] color						Red, Green, Blue, Mono
///	@param[in] pixelType					Sets if pixel is MONO8, MONO10, MONO12 or MONO16
///
///	@param[out] result						Output buffer containing the virtual angle image (width x height)
///
PFPOLCLIB_EXPORT int32_t pfcGenerateVirtualAngleColorImage_u16(uint16_t**** target, uint16_t* result, uint16_t m_originalImageWidth, uint16_t m_originalImageHeight, uint16_t angledegree, PFPolarizedColor color, pfPixelType pixelType);

///
/// @brief For color polarized camera, frees the color image.
///
///	@param[in] target						Color image with format target[angle][color]
///
PFPOLCLIB_EXPORT int32_t pfcFreeColorPolarizedImage(uint16_t**** target);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif