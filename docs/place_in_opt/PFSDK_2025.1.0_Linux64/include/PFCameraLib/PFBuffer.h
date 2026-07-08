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
/// @file PFBuffer.h 
/// @brief Class containing the attributes and data of a received buffer. It provides access to the image data by a image pointer.
///
#ifndef PFBUFFER_H
#define PFBUFFER_H

#include "PFImage.h"
#include "PFCameraLib.h"
#include "pfcPixelTypes.h"
#include "PFResult.h"
#include "noexcept.h"

class BufferTest;

namespace pfcameralib
{
    /// 
    /// @class PFBuffer PFBuffer.h 
    ///
    /// Class containing the attributes and data of a received buffer. It provides access to contents of the image data.
    /// Data is copied by default. Use PFbufferRef for explicit data sharing.
    /// Internal structure uses shared_ptr-like functionality
    ///
    class PFCAMERALIB_API PFBuffer
    {
    public:

        /// @brief Default constructor
        PFBuffer();     

        /// 
        /// @brief Constructor
        ///
        /// @param[in] payloadSize Size in bytes required to store in memory a single image. This value is received from the camera. 
        ///
        explicit PFBuffer(uint64_t payloadSize);


        /// 
        /// @brief Constructor
        ///
        /// @param[in] payloadSize Size in bytes required to store in memory a single image. This value is received from the camera. 
        /// @param[in] index Index for this buffer, may be used for driver mapped buffers
        /// 
        explicit PFBuffer(uint64_t payloadSize, int64_t index);

        /// @brief Destructor
        ~PFBuffer();

        /// @brief Copy constructor
        PFBuffer(const PFBuffer &buffer);

        ///
        /// \brief PFBuffer move constructor
        /// \param buffer
        ///
        PFBuffer(PFBuffer &&buffer) NOEXCEPT;
        
        /// @brief Assignment operator
        PFBuffer& operator= (const PFBuffer &buffer);

        /// @brief Move assignment, transfer ownership
        PFBuffer& operator=(PFBuffer&& other) NOEXCEPT;
                        
        /// 
        /// @brief Get Image object
        ///
        /// You call this method to initialize the image object PFImage
        /// The image pointer used is the one managed by PFBuffer and therefore the image pointer is only assigned.
        /// To use this method you have to construct the PFImage using the default constructor.
        /// Once the image is loaded it can be manipulated by any other Image class method.
        /// Important Note: In order to avoid NULL pointer exceptions be aware not to call PFCamera::ReleaseBuffer() before doing any image operation required.
        /// @param[out] image Initialize an image with the buffer's pointer.
        /// @return PFResult
        PFResult GetImage(PFImage &image) const;
        
        /// 
        /// @brief Get Image object doing a deep copy 
        ///
        /// This method is intended to create an independant #PFImage separated from the source image data pointer stored in #PFBuffer. 
        /// The image data copied to #PFImage will be released in the destructor of this class.
        /// Once the image is loaded it can be manipulated by any other class method.
        /// @warning Using this method is discouraged, copying images has a lot of memory consumption and  has expensive CPU cost. \n
        /// Whenever you can call #GetImage for better performance.
        ///
        /// @param[out] image Initialize an image with a copy of the buffer data.
        /// @return PFResult
        PFResult GetImageCopy(PFImage &image) const;
        
        ///
        /// @brief Get the frame counter
        ///
        /// @return The number of the current image. This counter is reset to 0 when a new grabbing process starts
        /// 
        int64_t GetFrameCounter() const;

        ///
        /// @brief Get the timestamp of image
        ///
        /// @return Timestamp of image
        /// 
        uint64_t GetTimestamp() const;

        ///
        /// @brief Get the missing packets count
        ///
        /// @return The number of missing packets
        /// 
        uint32_t GetMissingPacketCount() const;
                
        ///
        /// @brief Get raw data
        ///
        /// @return Pointer to raw data
        /// 
        uint8_t* GetRawData() const;

        ///
        /// @brief Get size of payload
        ///
        /// @return Payload size in bytes
        ///
        uint64_t GetPayloadSize() const;

        ///
        /// @brief Get size of the image excluding padding and metadata
        ///
        /// size_x * size_y * bytes_per_pixel
        /// 
        /// @return image size in bytes
        ///
        uint64_t GetImageSize() const;

        ///
        /// @brief Get image width
        ///
        /// @return image width
        ///
        uint32_t GetSizeX() const;
        
        ///
        /// @brief Get image height
        ///
        /// @return image height
        ///
        uint32_t GetSizeY() const;

        ///
        /// @brief Get image offset x, used for ROI support
        ///
        /// @return image offset x in pixels
        ///
        uint32_t GetOffsetX() const;

        ///
        /// @brief Get image offset y, used for ROI support
        ///
        /// @return image offset y in pixels
        ///
        uint32_t GetOffsetY() const;

        ///
        /// @brief Horizontal padding in bytes.
        ///
        /// @return padding bytes at the end of each line.
        ///
        uint32_t GetPaddingX() const;

        ///
        /// @brief Vertical padding in bytes.
        ///
        /// @return padding bytes at the end of the image. (Note bytes, NOT lines)
        ///
        uint32_t GetPaddingY() const;

        ///
        /// @brief Get pixel type of the capture buffer
        ///
        /// @return pixel type
        ///
        pfPixelType GetPixelType() const;
        
        ///
        /// @brief Check if the frame is Corrupted
        ///
        /// @return True if frame is corrupted False otherwise
        bool IsFrameCorrupted() const;

        ///
        /// @brief Check if the buffer is Locked
        ///
        /// @return True if buffer is locked False otherwise
        bool IsBufferLocked() const;

        ///
        /// @brief Unlock buffer when the buffer can be reused for further processing 
        ///
        void UnlockBuffer();

        ///
        /// @brief Release buffer data
        ///
        PFResult ReleaseBuffer();

        ///
        /// \brief Create a deep Copy of this object
        /// \return Returns the copied object
        PFBuffer DeepCopy() const;

        ///
        /// \brief ShallowCopy
        /// Creates a shallow copy, using explicit data sharing. Any changes either object (this or the return) will propagate to the other
        /// \return a new PFBuffer Obect that shares the same data structure as the current object.
        ///
        PFBuffer ShallowCopy() const;

        ///
        /// \brief GetIndex
        /// Get the index of the buffer in case one was assigned.
        /// This is usually the case when mapping the buffers to the driver, or when the user explicitly sets the index in the constructor.
        ///
        /// If no index was assigned, returns a negative value.
        /// 
        int64_t GetIndex() const;
        
    private:
        friend class PFCamera;
        friend class PFImage;
        friend class PFStream;
        friend class ::BufferTest; //Friend class for internal unit testing.
        class PFBufferImpl;
        PFBufferImpl* m_pImpl;
    };
}

#endif //PFBUFFER_H
