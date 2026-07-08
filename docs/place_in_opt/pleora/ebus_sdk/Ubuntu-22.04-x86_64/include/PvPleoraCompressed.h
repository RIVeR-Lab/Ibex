// *****************************************************************************
//
//     Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVPLEORACOMPRESSED_H__
#define __PVPLEORACOMPRESSED_H__

#include <PvResult.h>
#include <PvTypes.h>
#include <PvPixelType.h>

///
/// \class PvPleoraCompressed
///
/// \brief Pleora compressed interface to a PvBuffer.
///
/// A #PvBuffer represents a generic buffer with no specific payload type. If the
/// payload type is Pleora compressed, the user can obtain a pointer to a
/// PvPleoraCompressed buffer interface through #PvBuffer::GetPleoraCompressed.
///
/// This #PvPleoraCompressed pointer is owned by the PvBuffer and does not need to
/// be released after use. It is valid as long as the PvBuffer exists.
///
class PV_BUFFER_API PvPleoraCompressed
{
public:

    virtual ~PvPleoraCompressed() {}

    ///
    /// \brief Get the payload length specified in the leader information of this block
    ///
    /// If you wish to construct a buffer with payload type "Pleora Compressed",
    /// simply call #PvPleoraCompressed::Alloc or #PvPleoraCompressed::Attach and the
    /// payload length field will be set automatically at the time the block is transmitted.
    ///
    /// \return The payload length specified in the leader information of this block
    ///
    virtual uint64_t GetPayloadLength() const = 0;

    ///
    /// \brief Sets the effective payload length for the buffer.
    ///
    /// Sufficient memory must already be allocated greater than or equal to payload length.
    ///
    virtual void SetPayloadLength( uint64_t aLength ) = 0;

    ///
    /// \brief Allocates memory for this #PvPleoraCompressed.
    ///
    /// Allocs a PvPleoraCompressed of specific payload length
    ///
    /// \param[in]  aPayloadLength    The payload length of the compressed data, in pixels. See #GetPayloadLength.
    ///
    /// \return
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_ENOUGH_MEMORY
    ///
    virtual PvResult Alloc( uint64_t aPayloadLength ) = 0;

    ///
    /// \brief Frees (de-allocates) the buffer's internal memory.
    ///
    /// \sa Alloc
    ///
    virtual void Free() = 0;

    ///
    /// \brief Attach this #PvPleoraCompressed to an external memory buffer.
    ///
    /// This method is identical to #Alloc(uint64_t,uint32_t),
    /// with the following exceptions:
    /// - This method uses an external memory buffer, controlled by the caller.
    ///
    /// \param[in]  aRawBuffer       A pointer to the buffer.
    /// \param[in]  aPayloadLength   The payload length of the raw data, in pixels. See #GetPayloadLength.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    virtual PvResult Attach( void * aRawBuffer, uint64_t aPayloadLength ) = 0;

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

private:

    // Not implemented
    //PvPleoraCompressed( const PvPleoraCompressed & );
    //const PvPleoraCompressed &operator=( const PvPleoraCompressed & );
};

#endif // __PVPLEORACOMPRESSED_H__
