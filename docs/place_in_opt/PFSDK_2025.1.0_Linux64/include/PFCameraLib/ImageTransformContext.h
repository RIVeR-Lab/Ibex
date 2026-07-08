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
/// @file ImageTransformContext.h
/// @brief Image Transform Context 
///


#ifndef IMAGE_TRANSFORM_CTX_H
#define IMAGE_TRANSFORM_CTX_H

#include "PFImage.h"
#include "pfcPixelTypes.h"
#include <cstddef>

namespace pfcameralib
{
namespace image
{
namespace core
{
	///< Double rate context handle definition
	typedef void *pfdoublerate_context_handle;

	/// 
	/// @struct ImageTransformContext 
	///
	/// @brief Stores data required for image conversions
	/// 
	struct ImageTransformContext {
		void* red = nullptr;									///< Buffer for Red component, size is channel_size
		void* green = nullptr;								///< Buffer for Green component, size is channel_size
		void* blue = nullptr;									///< Buffer for Blue component, size is channel_size	
		void* rgb_to_mono = nullptr;					///< Buffer for RGB to Mono conversion, size is rgb_to_mono_size
		std::size_t channel_size = 0;					///< Size for RGB conversion
		std::size_t rgb_to_mono_size = 0;			///< Size for RGB to Mono conversion 
		pfdoublerate_context_handle dr_ctxt = nullptr;  ///< Context for double rate conversion
		bool disable_checks = false;					///< If true no checking is performed during image operations
		bool auto_update = false;							///< If true the image must be auto-updated
		pfPixelType src_type = PixelUnknown;	///< Pixel type of source image
	};

	/// 
	/// @class TransformContext_Manager 
	///
	/// Utility class to manage ImageTransformContext in order to free context when the object is destroyed
	///
	class PFCAMERALIB_API TransformContext_Manager {
	public:
		/// @brief Default constructor
		TransformContext_Manager();
		
		/// @brief Destructor. Context is released if default context has been created
		~TransformContext_Manager();

		///
		/// @brief CreateContext
		///
		/// Creates context
		///
		/// @param[in] src Source #pfcameralib::PFImage image 
		/// @param[in] dst Resulting #pfcameralib::PFImage 
		/// @param[in] dr_ctxt_threads If DR required, dr_ctxt_threads == 0 -> use global context otherwise create a new context
		///
		/// @return ImageTransformContext                
		/// 
		ImageTransformContext* CreateContext(const PFImage& src, const PFImage& dst, int dr_ctxt_threads);
		
		///
		/// @brief CreateContext
		///
		/// Allocates ImageTrasformContext
		///
		/// @param[in] src Source #pfcameralib::PFImage image 
		/// @param[in] dst_type destination pixel type
		/// @param[in] dr_ctxt_threads If DR required, dr_ctxt_threads == 0 -> use global context otherwise create a new context
		///
		/// @return ImageTransformContext                
		/// 
		ImageTransformContext* CreateContext(const PFImage& src, pfPixelType dst_type, int dr_ctxt_threads);

		///
		/// @brief CreateContext
		///
		/// Allocates ImageTrasformContext
		///
		/// @param[in] width Image width
		/// @param[in] height Image height
		/// @param[in] src_pixel_type Pixel type of image
		/// @param[in] dst_type destination pixel type
		/// @param[in] dr_ctxt_threads If DR required, dr_ctxt_threads == 0 -> use global context otherwise create a new context
		///
		/// @return ImageTransformContext                
		/// 
		ImageTransformContext* CreateContext(uint64_t width, uint64_t height, pfPixelType src_pixel_type, pfPixelType dst_type, int dr_ctxt_threads);

		///
		/// @brief SetContext
		///
		/// Set context
		///
		/// @param[in] ctxt Transformation context
		///
		void SetContext(ImageTransformContext* ctxt);

		///
		/// @brief GetContext
		///
		/// Get context
		///
		/// @return ImageTransformContext                
		///
		ImageTransformContext* GetContext();
		
		
		
	private:
		ImageTransformContext* m_ctxt = nullptr;		///< Internal image transformation context
		bool m_bDefault;														///< True if Default context has been created
	};
		
	///
	/// @brief AllocateImageTransformContext.
	///
	/// Allocates ImageTrasformContext
	///
	/// @param[in] src Source #pfcameralib::PFImage image 
	/// @param[in] dst Resulting #pfcameralib::PFImage 
	/// @param[in] dr_ctxt_threads If DR required, dr_ctxt_threads == 0 -> use global context otherwise create a new context
	///
	/// @return ImageTransformContext                
	/// 
	PFCAMERALIB_API ImageTransformContext* AllocateImageTransformContext(const PFImage& src, const PFImage& dst, int dr_ctxt_threads);


	///
	/// @brief AllocateImageTransformContext
	///
	/// Allocates ImageTrasformContext
	///
	/// @param[in] src Source #pfcameralib::PFImage image 
	/// @param[in] dst_type destination pixel type
	/// @param[in] dr_ctxt_threads If DR required, dr_ctxt_threads == 0 -> use global context otherwise create a new context
	///
	/// @return ImageTransformContext                
	/// 
	PFCAMERALIB_API ImageTransformContext* AllocateImageTransformContext(const PFImage& src, pfPixelType dst_type, int dr_ctxt_threads);

	///
	/// @brief AllocateImageTransformContext
	///
	/// Allocates ImageTrasformContext
	///
	/// @param[in] width Image width
	/// @param[in] height Image height
	/// @param[in] src_pixel_type Pixel type of image
	/// @param[in] dst_type destination pixel type
	/// @param[in] dr_ctxt_threads If DR required, dr_ctxt_threads == 0 -> use global context otherwise create a new context
	///
	/// @return ImageTransformContext                
	/// 
	PFCAMERALIB_API ImageTransformContext* AllocateImageTransformContext(uint64_t width, uint64_t height, pfPixelType src_pixel_type, pfPixelType dst_type, int dr_ctxt_threads);

	///
	/// @brief CheckContextStatus
	///
	/// Checks if current context is right for required conversion. If the context is not appropiate can be reinitialized with InitTransformContext() function
	///
	/// @param[in] src Source #pfcameralib::PFImage image 
	/// @param[in] dst Resulting #pfcameralib::PFImage 
	/// @param[in] ctxt Current context
	///
	/// @return True if the context is OK false otherwise
	/// 
	PFCAMERALIB_API bool CheckContextStatus(const PFImage& src, const PFImage& dst, ImageTransformContext* ctxt);

	///
	/// @brief InitTransformContext
	///
	/// Initialize image transformation context depending on the image conversion requirements
	///
	/// @param[in] src Source #pfcameralib::PFImage image 
	/// @param[in] dst_type destination pixel type
	/// @param[out] ctxt Context initialized
	///
	/// 
	PFCAMERALIB_API void InitTransformContext(const PFImage& src, pfPixelType dst_type, ImageTransformContext* ctxt);

	///
	/// @brief InitTransformContext
	///
	/// Initialize image transformation context depending on the image conversion requirements
	///
	/// @param[in] width Image width
	/// @param[in] height Image height
	/// @param[in] src_pixel_type Pixel type of source image
	/// @param[in] dst_pixel_type Pixel type of destination image
	/// @param[out] ctxt Context initialized
	///
	/// 
	PFCAMERALIB_API void InitTransformContext(const uint64_t width, const uint64_t height, const pfPixelType src_pixel_type, const pfPixelType dst_pixel_type, ImageTransformContext* ctxt);

	///
	/// @brief FreeImageTransformContext
	///
	/// Release image transformation context. After calling this function the context is assigned to nullptr.
	///
	/// @param[in] ctxt Current context
	/// 	
	///
	PFCAMERALIB_API void FreeImageTransformContext(ImageTransformContext* ctxt);

}
}
}


#endif