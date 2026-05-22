// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVIMAGE_H__
#define __PVIMAGE_H__

#include <PvPixelType.h>


class PvBuffer;
class PvTruesenseConverter;

///
/// \class IPvImage
/// \brief Generic image interface.
///
class PV_BUFFER_API IPvImage
{
public:

    virtual ~IPvImage() {}

    ///
    /// \brief Get the image's data pointer.
    ///
    /// \return A pointer to the memory pointer.
    ///
    virtual const uint8_t *GetDataPointer() const = 0;

    ///
    /// \brief Get the image's data pointer.
    ///
    /// Const version.
    ///
    /// \return A pointer to the memory pointer.
    ///
    ///
    virtual uint8_t *GetDataPointer() = 0;

    ///
    /// \brief Actual image size within the payload or buffer.
    ///
    /// Uses the GEV leader for the actual width.
    /// Uses the GEV trailer for line count which is the actual height.
    ///
    /// \return The actual image size within the payload or buffer
    ///
    virtual uint32_t GetImageSize() const = 0;

    ///
    /// \brief Get the image's width.
    ///
    /// \return The image's width in pixels.
    ///
    virtual uint32_t GetWidth() const = 0;

    ///
    /// \brief Get the image's height.
    ///
    /// \return The image's height, in pixels.
    ///
    virtual uint32_t GetHeight() const = 0;

    ///
    /// \brief The buffer's pixel type.
    ///
    /// \return The pixel type. The pixel depth, colorspace, and other information are coded into the value.
    ///
    virtual PvPixelType GetPixelType() const = 0;

    ///
    /// \brief Returns how many bits are required to hold a pixel using the current buffer's pixel type
    ///
    /// \return Bits per pixel for the current buffer's pixel type.
    ///
    virtual uint32_t GetBitsPerPixel() const = 0;

    ///
    /// \brief Get the image's horizontal offset.
    ///
    /// \return The image's horizontal offset, in pixels.
    ///
    virtual uint32_t GetOffsetX() const = 0;

    ///
    /// \brief Get the image's vertical offset.
    ///
    /// \return The image's vertical offset, in pixels.
    ///
    virtual uint32_t GetOffsetY() const = 0;

    ///
    /// \brief Get the horizontal image padding.
    ///
    /// Images lines could be grabbed in increments of, let's say, 32 bytes. For an image 511 bytes across, 
    /// the video interface adds a meaningless byte to make the line total 512 bytes (divisible by 32). 
    /// In this case, the <i>paddingX</i> value would be 1 (byte).
    ///
    /// Other devices may have different requirements on line length. Divible by 4, 8, 16, etc.
    /// or no specific limitations or requirements.
    ///
    /// \return The horizontal image padding, in bytes; the number of bytes at the end of an image's line 
    ///         that contain meaningless data.
    ///
    virtual uint16_t GetPaddingX() const = 0;

    ///
    /// \brief Get the vertical image padding.
    ///
    /// Additional bytes appended at the end of an image.
    ///
    /// See #GetPaddingX.
    ///
    /// \return The vertical image padding, in bytes.
    ///
    /// \sa GetPaddingX
    ///
    virtual uint16_t GetPaddingY() const = 0;

    ///
    /// \brief Return the block ID.
    ///
    /// This method returns the block ID. The GigE Vision or USB3 Vision transmitter typically increments
    /// the value by 1 for each new image. You can use this value to ensure the
    /// blocks are in order and that none are missing. The value is unsigned and wraps
    /// around to 1 (skipping 0) when it reaches 65536.
    ///
    /// \return The block ID
    ///
    virtual uint64_t GetBlockID() const = 0;

    ///
    /// \brief Get the image's timestamp.
    ///
    /// With hardware timestamps and GigE Vision devices, the device defines the timestamp tick frequency. The effective value
    /// can be retrieved from the GenApi interface of the device.
    ///
    /// With hardware timestamps and USB3 Vision devices, timestamps are always defined in nanoseconds.
    ///
    /// With software timestamps the timestamps are in micro seconds.
    ///
    /// Use the PvStream GenApi interface to configure whether hardware or software time stamps are used.
    ///
    /// \return The image's hardware timestamp; For Pleora devices, this is the value of the relative 
    /// timestamp clock when the grabber began receiving data from the camera head - on the rising edge of the FVAL. 
    ///
    virtual uint64_t GetTimestamp() const = 0;

    ///
    /// \brief Get the buffer's reception timestamp.
    ///
    /// \return The buffer's reception time on the host, in micro seconds. Not an absolute time value.
    ///
    virtual uint64_t GetReceptionTime() const = 0;

};


///
/// \class PvImage
///
/// \brief Image interface to a PvBuffer.
///
/// A #PvBuffer represents a generic buffer with no specific payload type. If the payload type
/// is image, the user can obtain a pointer to a PvImage buffer interface through #PvBuffer::GetImage.
///
/// This #PvImage pointer is owned by the PvBuffer and does not need to be released
/// after use. It is valid as long as the PvBuffer exists.
///
/// <h2 id="accessing">Accessing image specific data</h2>
///
/// <b>To determinate the payload type and access image specific data:</b>
///     - Retrieve the #PvPayloadType using #PvBuffer::GetPayloadType
///     - If #PvPayloadType is PvPayloadTypeImage, retrieve #PvImage pointer from your #PvBuffer
///     - Access image specific data through the #PvImage pointer, like #PvImage::GetWidth, #PvImage::GetHeight, etc.
///     - There is no need to release or delete the #PvImage pointer. It is valid as long as the #PvBuffer exists.
///
class PV_BUFFER_API PvImage
    : public IPvImage
{
public:

    ///
    /// \brief Returns the maximum chunk length possible for the currently allocated memory vs image attributes.
    ///
    /// \return Maximum chunk length, in bytes.
    ///
    virtual uint32_t GetMaximumChunkLength() const = 0;

    ///
    /// \brief Static method returning the pixel size in bits for a specific pixel type
    ///
    /// \param[in] aPixelType Pixel type.
    ///
    /// \return Pixel size, in bits.
    ///
    static uint32_t GetPixelSize( PvPixelType aPixelType );

    ///
    /// \brief Static method returning whether a pixel type is color or not
    ///
    /// \param[in] aPixelType Pixel type.
    ///
    /// \return True if color.
    ///
    static bool IsPixelColor( PvPixelType aPixelType );

    ///
    /// \brief Static method returning whether a pixel type is higher than 8-bit per channel or not
    ///
    /// 10, 12, 14 and 16-bit pixel type are considered high resolution. RGB8 counts as 8 so
    /// high resolution here means per channel.
    ///
    /// \param[in] aPixelType Pixel type.
    ///
    /// \return True if higher than 8 bit per channel.
    ///
    static bool IsPixelHighRes( PvPixelType aPixelType );

    ///
    /// \brief Static method returning the number of bit per components for a pixel type.
    ///
    /// PvPixelMono8 and PvPixelRGBa8 would be 8. Mono16, PvPixelRGB16, PvPixelBayerRG16 
    /// would be 16, etc.
    ///
    /// \param[in] aPixelType Pixel type.
    ///
    /// \return True if higher than 8 bit per channel.
    ///
    static uint32_t GetBitsPerComponent( PvPixelType aPixelType );

    ///
    /// \brief Static method returning a string representation of a pixel type.
    ///
    /// \param[in] aPixelType Pixel type.
    ///
    /// \return String representation of the pixel type.
    ///
    static PvString PixelTypeToString( PvPixelType aPixelType );

    ///
    /// \brief Gets the size of buffer required to hold the acquired image.
    ///
    /// This attribute is valid if the last image acquisition was successful or if it
    /// failed because the buffer was too small. It can be used to re-allocate the acquisition 
    /// buffers if the acquisition failed because the buffer was too small.
    ///
    /// The required size is only computed for the last image - based on the number
    /// of lines as provided in the GigE Vision or USB3 Vision streaming protocol trailer. 
    ///
    /// It is possible that with devices outputting images of varying length (like line
    /// scan cameras) that the last image required size may be smaller than the maximum
    /// possible size.
    ///
    /// In order to be 100% sure of allocating buffers correctly, you should either hard code
    /// the buffer size to known big enough sizes or use the device GenICam GevPayloadSize
    /// parameter for dynamic cases.
    ///
    /// \return The size, in bytes, required to hold the image in this buffer.
    ///
    virtual uint32_t GetRequiredSize() const = 0;

    ///
    /// \brief Effective image size.
    ///
    /// Uses same size x and y as GetImageSize() but excludes all padding.
    ///
    /// \return The effective image size
    ///
    virtual uint32_t GetEffectiveImageSize() const = 0;

    ///
    /// \brief Sets the Offset X attribute of the image.
    ///
    /// \param[in] aValue New value.
    ///
    virtual void SetOffsetX( uint32_t aValue ) = 0;

    ///
    /// \brief Sets the Offset X attribute of the image.
    ///
    /// \param[in] aValue New value.
    ///
    virtual void SetOffsetY( uint32_t aValue ) = 0;

    virtual bool IsAllocated() const = 0;
    virtual bool IsAttached()  const = 0;

    ///
    /// \brief Allocates memory for this #PvImage.
    ///
    /// Allocs a PvImage of specific width, height and pixel format
    ///
    /// \param[in]  aSizeX The width of the image, in pixels. See #GetWidth.
    /// \param[in]  aSizeY The height of the image, in pixels. See #GetHeight.
    /// \param[in]  aPixelType The GEV pixel type from which the pixel depth is extracted. For supported pixel types, see PvPixelType.h.
    /// \param[in]  aPaddingX The horizontal padding of the image, in pixels. See #GetPaddingX.
    /// \param[in]  aPaddingY The vertical padding of the image, in pixels. See #GetPaddingY.
    /// \param[in]  aMaximumChunkLength The chunk size in bytes.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_ENOUGH_MEMORY
    ///
    virtual PvResult Alloc( uint32_t aSizeX, uint32_t aSizeY, PvPixelType aPixelType, uint16_t aPaddingX = 0, uint16_t aPaddingY = 0, uint32_t aMaximumChunkLength = 0 ) = 0;
    
    ///
    /// \brief Frees (de-allocates) the buffer's internal memory.
    ///
    /// \sa Alloc
    ///    
    virtual void Free() = 0;

    ///
    /// \brief Attach this #PvImage to an external memory buffer.
    ///
    /// This method is identical to #Alloc(uint32_t,uint32_t,PvPixelType,uint16_t,uint16_t,uint32_t),
    /// with the following exceptions:
    ///     - This method uses an external memory buffer, controlled by the caller.
    ///
    /// \param[in]  aRawBuffer A pointer to the buffer.
    /// \param[in]  aSizeX See #Alloc(uint32_t,uint32_t,PvPixelType,uint16_t,uint16_t,uint32_t).
    /// \param[in]  aSizeY See #Alloc(uint32_t,uint32_t,PvPixelType,uint16_t,uint16_t,uint32_t).
    /// \param[in]  aPixelType See #Alloc(uint32_t,uint32_t,PvPixelType,uint16_t,uint16_t,uint32_t).
    /// \param[in]  aPaddingX See #Alloc(uint32_t,uint32_t,PvPixelType,uint16_t,uint16_t,uint32_t).
    /// \param[in]  aPaddingY See #Alloc(uint32_t,uint32_t,PvPixelType,uint16_t,uint16_t,uint32_t).
    /// \param[in]  aMaximumChunkLength See #Alloc(uint32_t,uint32_t,PvPixelType,uint16_t,uint16_t,uint32_t).
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    virtual PvResult Attach( void * aRawBuffer, uint32_t aSizeX, uint32_t aSizeY, PvPixelType aPixelType, uint16_t aPaddingX = 0, uint16_t aPaddingY = 0, uint32_t aMaximumChunkLength = 0 ) = 0;
    
    ///
    /// \brief Releases an attached memory buffer.
    ///
    /// Does nothing other then resetting the memory buffer pointer to NULL.
    ///
    /// \return The buffer's location in memory.
    ///
    /// \sa Attach
    ///    
    virtual uint8_t *Detach() = 0;

    ///
    /// \brief If true, the grabber (on the device) was missing at least one pixel in at least one of the lines when the frame was grabbed.
    ///
    /// \return True if at least one pixel was missing, false otherwise
    ///
    virtual bool IsPartialLineMissing() const = 0;

    ///
    /// \brief If true, the grabber (on the device) was missing at least one line when the frame was grabbed.
    ///
    /// \return True if full lines were missing, false otherwise
    ///
    virtual bool IsFullLineMissing() const = 0;

    ///
    /// \brief Simulates the FullLineMissing used by Pleora video interfaces.
    ///
    /// \param[in] aValue      True to enable, false to disable
    /// \param[in] aValue      The Actual line number in the image.
    ///
    virtual void SetFullLineMissing( bool aValue, uint32_t aLineNumber ) = 0;

    ///
    /// \brief Simulates the EOFByLineCount used by Pleora video interfaces.
    ///
    /// \param[in] aValue      True to enable, false to disable
    ///
    virtual void SetEOFByLineCount( bool aValue = true ) = 0;

    ///
    /// \brief Set the EoFByLineCount and FullLineMissing Status of PvImage to the default value false.
    /// 
    /// This method should be called on every PvImage if the EoFByLineCount feature is used for streaming.
    /// It set the EoFByLineCount and FullLineMissing Status of the PvImage to the default value false, the user
    /// could then decide if and which status needs to be set on the current PvImage.
    /// 
    virtual void ClearEOFStatus() = 0;

    ///
    /// \brief End of Frame by Line Count status bit.
    /// 
    /// \return True if previous frame was NOT the last one of a sequence of sub frames. False otherwise.
    ///
    virtual bool IsEOFByLineCount() const = 0;

    ///
    /// \brief The image contained in the #PvImage is an interlaced Even field.
    /// 
    /// \return True interlaced Even field. False otherwise.
    ///
    virtual bool IsInterlacedEven() const = 0;

    ///
    /// \brief The image contained in the #PvImage is an interlaced Odd field.
    /// 
    /// \return True interlaced Odd field. False otherwise.
    ///
    virtual bool IsInterlacedOdd() const = 0;

    ///
    /// \brief Image dropped status bit.
    /// 
    /// \return True if the previous frame was dropped. False otherwise.
    ///
    virtual bool IsImageDropped() const = 0;

    ///
    /// \brief Data overrun status bit.
    /// 
    /// \return True if there was a data overrun. False otherwise.
    ///
    virtual bool IsDataOverrun() const = 0;

    ///
    /// \brief Returns the PvBuffer this PvImage belongs to.
    ///
    /// \return Pointer to a PvBuffer.
    ///
    virtual PvBuffer *GetBuffer() = 0;

    ///
    /// \brief Returns the watermark tag.
    ///
    /// A watermark is applied onto the image if the system is not provided with a valid eBUS receiver license.
    ///
    /// Use this method to check if a watermark was applied to the image or not.
    ///
    /// \return The watermark tag.
    ///
    virtual bool HasWatermark() const = 0;

private:

	friend class PvTruesenseConverter;

};


#endif
