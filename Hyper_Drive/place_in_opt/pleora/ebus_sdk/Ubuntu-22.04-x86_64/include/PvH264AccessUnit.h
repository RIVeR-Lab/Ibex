// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVH264ACCESSUNIT_H__
#define __PVH264ACCESSUNIT_H__

#include <PvBufferLib.h>
#include <PvPixelType.h>


namespace PvBufferLib
{
    class H264AccessUnit;
    class Buffer;
}

///
/// \class IPvH264AccessUnit
///
/// \brief Generic, read-only H264 access unit interface.
///
/// A #PvBuffer represents a generic buffer with no specific payload type. If the payload type
/// is H264 access unitdata, the user can obtain a pointer to a #IPvH264AccessUnit buffer interface through PvBuffer::GetH264AccessUnit.
///
/// This #IPvH264AccessUnit pointer is owned by the PvBuffer and does not need to be released
/// after use. It is valid as long as the PvBuffer exists.
///
class PV_BUFFER_API IPvH264AccessUnit
{
public:

    virtual ~IPvH264AccessUnit() {}

    ///
    /// \brief Returns the total buffer size, including chunk and/or unused space.
    ///
    /// \return Buffer size in bytes.
    ///
    virtual uint32_t GetSize() const = 0;

    ///
    /// \brief Returns the total raw NAL data size.
    ///
    /// All NALs of the access unit are stored contiguously. Use GetNALCount, GetNALPtr
    /// to access individual NALs.
    ///
    /// \return NAL data size in bytes.
    ///
    virtual uint32_t GetNALDataSize() const = 0;

    ///
    /// \brief Returns a pointer to the raw NAL data buffer.
    ///
    /// All NALs of the access unit are stored contiguously. Use GetNALCount, GetNALPtr
    /// to access individual NALs.
    ///
    /// \return Pointer to the first NAL.
    ///
    virtual const uint8_t *GetNALDataPtr() const = 0;

    ///
    /// \brief Returns how many NALs are currently stored in the access unit.
    ///
    /// \return NAL count.
    ///
    virtual uint32_t GetNALCount() const = 0;

    ///
    /// \brief Returns a pointer to a NAL stored in this access unit.
    ///
    /// \param[in] aIndex Zero-based index of the NAL.
    /// \param[out] aPtr Pointer to the NAL.
    /// \param[out] aLength NAL length in bytes.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::NOT_FOUND if aIndex is invalid.
    ///
    virtual PvResult GetNALPtr( uint32_t aIndex, const uint8_t **aPtr, uint32_t &aLength ) const = 0;

    virtual uint64_t GetDuration() const = 0;

    virtual bool HasSPS() const = 0;

    ///
    /// \brief Width of the image represented by this access unit
    ///
    /// \return Width or zero if not set.
    ///
    virtual uint32_t GetWidth() const = 0;

    ///
    /// \brief Width of the image represented by this access unit
    ///
    /// \return Width or zero if not set.
    ///
    virtual uint32_t GetHeight() const = 0;
    virtual uint32_t GetOffsetTop() const = 0;
    virtual uint32_t GetOffsetLeft() const = 0;
    virtual uint32_t GetOffsetBottom() const = 0;
    virtual uint32_t GetOffsetRight() const = 0;

    ///
    /// \brief Allocates memory for this access unit.
    ///
    /// Allocs an access unit of specific payload length
    ///
    /// \param[in]  aPayloadLength        The raw payload length of the H264 Access Unit data, in bytes. See GetNALDataSize.
    /// \param[in]  aMaximumChunkLength   Maximum size in bytes to store chunks ( must include extra 64 bits by chunks for headers ).
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::NOT_ENOUGH_MEMORY
    ///
    virtual PvResult Alloc( uint64_t aPayloadLength, uint32_t aMaximumChunkLength = 0 ) = 0;

    ///
    /// \brief Frees (de-allocates) the buffer's internal memory.
    ///
    /// \sa Alloc
    ///
    virtual void Free() = 0;
    virtual void Reset() = 0;

    ///
    /// \brief Copies a NAL to this access unit.
    ///
    /// \param[in] aPtr Pointer to the NAL to copy.
    /// \param[in] aLength Length of the NAL to copy.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    ///
    virtual PvResult AddNAL( const uint8_t *aPtr, uint32_t aLength ) = 0;

    ///
    /// \brief Copies NAL data to the buffer
    ///
    /// \param[in] aPtr Pointer to the NAL data.
    /// \param[in] aLength Data length in bytes.
    /// \param[in] aWidth Width of the compressed video stream.
    /// \param[in] aHeight Height of the compressed video stream.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER
    ///
    virtual PvResult CopyNALData( const uint8_t *aPtr, uint32_t aLength, uint32_t aWidth, uint32_t aHeight ) = 0;
};


#endif
