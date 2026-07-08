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


//*************************************************************************************
// Program:		pfDoubleRate demodulation
// 
//
// Revision history:
//
// V1.02, 2012-01-24:
//    - Procedures pfDoubleRate_GetVersion and pfDoubleRate_DeModulateImageUnmatchedBufferWidth added.
//
//
// SVN:			$Id: pfDoubleRate.h 1674 2013-04-08 11:59:16Z hod $
//*************************************************************************************

#ifndef PFDOUBLERATE_H
#define PFDOUBLERATE_H

#include "pfDoubleRateLib.h"

#define PFDOUBLERATE_ERROR -1
#define PFDOUBLERATE_SUCCESS 0

#ifdef __cplusplus
extern "C" {
#endif

typedef void* pfdoublerate_context_handle;

///
/// \brief Gives the version of the pfDoubleRate software.
/// 
///
/// \return Return pfDoubleRate version. Format for version major.minor is: 100*major+minor: 230 = 2.30
///
PFDOUBLERATE_API int pfDoubleRate_GetVersion();


///
/// \brief Create a new execution context.
/// Each execution context has its own thread pool and associated structures.
/// A context is created with 4 threads by default.
/// \return Handle to created context if successful, null pointer otherwise.
///
PFDOUBLERATE_API pfdoublerate_context_handle pfDoubleRate_Context_Create();

///
/// \brief Gives the version of the pfDoubleRate software.
/// 
///
/// \return Return pfDoubleRate version. Format for version major.minor is: 100*major+minor: 230 = 2.30
///
PFDOUBLERATE_API void pfDoubleRate_Context_Destroy(pfdoublerate_context_handle hContext);

///
/// \brief Set the number of Threads for the demodulation process.
/// 0: No Thread are used, 1-4: Use 1, 2, 3 or 4 Threads.
/// Default is 4. For small ROI, set to 0.
/// 
/// \param[in] nrOfThreads Number of Threads.
///
/// \return Return 0 if OK, else -1
///
PFDOUBLERATE_API int pfDoubleRate_Context_SetNrOfThreads(pfdoublerate_context_handle hContext, int nrOfThreads);

///
/// \brief Set the number of Threads for the demodulation process for the default context.
/// Execution is done using a global thread pool. Only one thread can be using the global context at a given time, the rest are blocked.
/// Default is 4. For small ROI, set to 0.
/// 
/// \param[in] nrOfThreads Number of Threads.
///
/// \return Return 0 if OK, else -1
///
PFDOUBLERATE_API int pfDoubleRate_SetNrOfThreads(int nrOfThreads);

///
/// \brief Gives the width of the demodulated image buffer.
/// Call this function before demodulating the image to alloc memory.
/// 
/// \param[in] modImage Pointer to the modulated buffer.
/// \param[out] demodWidth Width of the demodulated buffer.
///
/// \return Return 0 if OK, else -1
///
PFDOUBLERATE_API int pfDoubleRate_GetDeModulatedWidth(unsigned char* modImage, int* demodWidth);


///
/// \brief Gives the width of the modulated image buffer.
/// Call this function before modulate the original image to alloc memory.
/// 
/// \param[in] Width Width of the original image.
/// \param[out] modWidth Width of the modulated buffer.
///
/// \return Return 0 if OK, else -1
///
PFDOUBLERATE_API int pfDoubleRate_GetModulatedWidth(int Width, int* modWidth);


///
/// \brief Demodulate the image. The input image buffer will be demodulated.
/// 
/// \param[out] Image Pointer to the demodulated buffer.
/// \param[in] modImage Pointer to the modulated buffer.
/// \param[in] Width Width of the demodulated image.
/// \param[in] Height Height of both buffers.
/// \param[in] modWidth Width of the modulated buffer.
///
/// \return Return 0 if OK, else -1
///
PFDOUBLERATE_API int pfDoubleRate_DeModulateImage(unsigned char* Image, unsigned char* modImage, int Width, int Height, int modWidth);


///
/// \brief Demodulate the image. The input image buffer will be demodulated.
/// 
/// \param[in] hContext Handle to execution context.
/// \param[out] Image Pointer to the demodulated buffer.
/// \param[in] modImage Pointer to the modulated buffer.
/// \param[in] Width Width of the demodulated image.
/// \param[in] Height Height of both buffers.
/// \param[in] modWidth Width of the modulated buffer.
///
/// \return Return 0 if OK, else -1
///
PFDOUBLERATE_API int pfDoubleRate_Context_DeModulateImage(pfdoublerate_context_handle hContext, unsigned char* Image, unsigned char* modImage, int Width, int Height, int modWidth);


///
/// \brief Demodulate the image. The input image buffer will be demodulated.
/// Call this procedure if width of a buffer is not equal to the corresponding image width.
/// 
/// \param[out] Image Pointer to the demodulated buffer.
/// \param[in] modImage Pointer to the modulated buffer.
/// \param[in] Width Width of the demodulated image.
/// \param[in] Height Height of both buffers.
/// \param[in] modWidth Width of the modulated buffer.
/// \param[in] bufWidth Width of the *Image buffer.
/// \param[in] modBufWidth Width of the modulated buffer *modImage.
///
/// \return Return 0 if OK, else -1
///
PFDOUBLERATE_API int pfDoubleRate_DeModulateImageUnmatchedBufferWidth(unsigned char* Image, unsigned char* modImage, int Width, int Height, int modWidth, int bufWidth, int modBufWidth);

///
/// \brief Demodulate the image. The input image buffer will be demodulated.
/// Call this procedure if width of a buffer is not equal to the corresponding image width.
/// 
/// \param[in] hContext Handle to execution context.
/// \param[out] Image Pointer to the demodulated buffer.
/// \param[in] modImage Pointer to the modulated buffer.
/// \param[in] Width Width of the demodulated image.
/// \param[in] Height Height of both buffers.
/// \param[in] modWidth Width of the modulated buffer.
/// \param[in] bufWidth Width of the *Image buffer.
/// \param[in] modBufWidth Width of the modulated buffer *modImage.
///
/// \return Return 0 if OK, else -1
///
PFDOUBLERATE_API int pfDoubleRate_Context_DeModulateImageUnmatchedBufferWidth(pfdoublerate_context_handle hContext, unsigned char* Image, unsigned char* modImage, int Width, int Height, int modWidth, int bufWidth, int modBufWidth);


// Color functions:


///
/// \brief Gives the width of the demodulated image buffer for color images.
/// Call this function before demudulater the image to alloc memory.
/// 
/// \param[in] modImage Pointer to the modulated buffer (color image).
/// \param[out] demodWidth Width of the demodulated buffer.
///
/// \return Return 0 if OK, else -1
///
PFDOUBLERATE_API int pfDoubleRate_GetDeModulatedWidthColor(unsigned char* modImage, int* demodWidth);


///
/// \brief Gives the width of the modulated image buffer (for color images).
/// Call this function before modulate the original image to alloc memory.
/// 
/// \param[in] Width Width of the original image.
/// \param[out] modWidth Width of the modulated buffer.
///
/// \return Return 0 if OK, else -1
///
PFDOUBLERATE_API int pfDoubleRate_GetModulatedWidthColor(int Width, int* modWidth);


///
/// \brief Demodulate the color image. The input image buffer will be demodulated.
/// 
/// \param[out] Image Pointer to the demodulated buffer (colour image).
/// \param[in] modImage Pointer to the modulated buffer.
/// \param[in] Width Width of the demodulated image.
/// \param[in] Height Height of both buffers.
/// \param[in] modWidth Width of the modulated buffer.
///
/// \return Return 0 if OK, else -1
///
PFDOUBLERATE_API int pfDoubleRate_DeModulateImageColor(unsigned char* Image, unsigned char* modImage, int Width, int Height, int modWidth);

///
/// \brief Demodulate the color image. The input image buffer will be demodulated.
/// 
/// \param[in] hContext Handle to execution context.
/// \param[out] Image Pointer to the demodulated buffer (colour image).
/// \param[in] modImage Pointer to the modulated buffer.
/// \param[in] Width Width of the demodulated image.
/// \param[in] Height Height of both buffers.
/// \param[in] modWidth Width of the modulated buffer.
///
/// \return Return 0 if OK, else -1
///
PFDOUBLERATE_API int pfDoubleRate_Context_DeModulateImageColor(pfdoublerate_context_handle hContext, unsigned char* Image, unsigned char* modImage, int Width, int Height, int modWidth);


///
/// \brief Demodulate the color image. The input image buffer will be demodulated.
/// Call this procedure if width of a buffer is not equal to the corresponding image width.
/// 
/// \param[out] Image Pointer to the demodulated buffer.
/// \param[in] modImage Pointer to the modulated buffer.
/// \param[in] Width Width of the demodulated image.
/// \param[in] Height Height of both buffers.
/// \param[in] modWidth Width of the modulated buffer.
/// \param[in] bufWidth Width of the *Image buffer.
/// \param[in] modBufWidth Width of the modulated buffer *modImage.
///
/// \return Return 0 if OK, else -1
///
PFDOUBLERATE_API int pfDoubleRate_DeModulateImageUnmatchedBufferWidthColor(unsigned char* Image, unsigned char* modImage, int Width, int Height, int modWidth, int bufWidth, int modBufWidth);

///
/// \brief Demodulate the color image. The input image buffer will be demodulated.
/// Call this procedure if width of a buffer is not equal to the corresponding image width.
/// 
/// \param[in] hContext Handle to execution context.
/// \param[out] Image Pointer to the demodulated buffer.
/// \param[in] modImage Pointer to the modulated buffer.
/// \param[in] Width Width of the demodulated image.
/// \param[in] Height Height of both buffers.
/// \param[in] modWidth Width of the modulated buffer.
/// \param[in] bufWidth Width of the *Image buffer.
/// \param[in] modBufWidth Width of the modulated buffer *modImage.
///
/// \return Return 0 if OK, else -1
///
PFDOUBLERATE_API int pfDoubleRate_Context_DeModulateImageUnmatchedBufferWidthColor(pfdoublerate_context_handle hContext, unsigned char* Image, unsigned char* modImage, int Width, int Height, int modWidth, int bufWidth, int modBufWidth);

#ifdef __cplusplus
}
#endif //extern "C"
#endif //PFDOUBLERATE_H
