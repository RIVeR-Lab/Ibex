// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVRAWDATA_H__
#define __PVRAWDATA_H__

#include <PvBufferLib.h>
#include <PvPixelType.h>

///
/// \class PvRawData
///
/// \brief Raw data interface to a PvBuffer.
///
/// A #PvBuffer represents a generic buffer with no specific payload type. If the payload type
/// is raw data, the user can obtain a pointer to a PvRawData buffer interface through #PvBuffer::GetRawData.
///
/// This #PvRawData pointer is owned by the PvBuffer and does not need to be released
/// after use. It is valid as long as the PvBuffer exists.
///
class PV_BUFFER_API PvRawData
{
public:

    virtual ~PvRawData() {}

    ///
    /// \brief Get the payload length specified in the leader information of this block
    ///
    /// If you wish to construct a buffer with payload type "Raw Data", 
    /// simply call #PvRawData::Alloc or #PvRawData::Attach and the payload length field 
    /// will be set automatically at the time the block is transmitted.
    ///
    /// \return The payload length specified in the leader information of this block
    ///
    virtual uint64_t GetPayloadLength() const = 0;

    ///
    /// \brief Allocates memory for this #PvRawData.
    ///
    /// Allocs a PvRawData of specific payload length
    ///
    /// \param[in]  aPayloadLength        The payload length of the raw data, in pixels. See #GetPayloadLength.
    /// \param[in]  aMaximumChunkLength   Maximum size in bytes to store chunks ( must include extra 64 bits by chunks for headers ).
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_ENOUGH_MEMORY
    ///
    virtual PvResult Alloc( uint64_t aPayloadLength, uint32_t aMaximumChunkLength = 0 ) = 0;

    ///
    /// \brief Frees (de-allocates) the buffer's internal memory.
    ///
    /// \sa Alloc
    ///
    virtual void Free() = 0;

    ///
    /// \brief Attach this #PvRawData to an external memory buffer.
    ///
    /// This method is identical to #Alloc(uint64_t,uint32_t),
    /// with the following exceptions:
    /// - This method uses an external memory buffer, controlled by the caller.
    ///
    /// \param[in]  aRawBuffer       A pointer to the buffer.
    /// \param[in]  aPayloadLength   The payload length of the raw data, in pixels. See #GetPayloadLength.
    /// \param[in]  aMaximumChunkLength   See #Alloc(uint64_t,uint32_t).
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    virtual PvResult Attach( void * aRawBuffer, uint64_t aPayloadLength, uint32_t aMaximumChunkLength = 0 ) = 0;

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

};


#endif
