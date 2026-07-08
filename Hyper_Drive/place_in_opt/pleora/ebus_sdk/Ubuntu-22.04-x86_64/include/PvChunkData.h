// *****************************************************************************
//
//     Copyright (c) 2017, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVCHUNKDATA_H__
#define __PVCHUNKDATA_H__

#include <PvBufferLib.h>
#include <PvPixelType.h>

///
/// \class IPvChunkData
/// \brief Generic chunk data interface.
///
class PV_BUFFER_API IPvChunkData
{
public:

    virtual ~IPvChunkData() {}

    ///
    /// \brief See #PvBuffer::ResetChunks
    ///
    /// Same as PvBuffer equivalent. Provided with this interface for convenience.
    ///
    virtual void ResetChunks() = 0;

    ///
    /// \brief See #PvBuffer::AddChunk.
    ///
    /// Same as PvBuffer equivalent. Provided with this interface for convenience.
    ///
    /// \param[in] aID Chunk data ID for aIndex.
    /// \param[in] aData Payload data of the chunk data.
    /// \param[in] aLength Length of the payload data of the chunk, in bytes. Must be a multiple of 4 bytes.
    ///
    /// \return 
    ///    A PvResult is used to capture the success or failure of the operation:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::BUFFER_TOO_SMALL There is not enough space in the buffer to add the chunk data.
    ///     - #PvResult::Code::INVALID_PARAMETER One of the input arguments is invalid.
    ///
    virtual PvResult AddChunk( uint32_t aID, const uint8_t *aData, uint32_t aLength ) = 0;

    ///
    /// \brief Set the chunk data layout ID.
    ///
    /// \param[in] aChunkLayoutID Chunk data layout ID.
    ///
    virtual void SetChunkLayoutID( uint32_t aChunkLayoutID ) = 0;

    ///
    /// \brief Returns true if the buffer has data chunks.
    ///
    /// Unlike GetChunkCount, chunks do not need to be parsed or resolved 
    /// with this method.
    ///
    /// \return True if the buffer holds data chunks, false if not.
    ///
    virtual bool HasChunks() const = 0;

    ///
    /// \brief See #PvBuffer::GetChunkCount. 
    ///
    /// Same as PvBuffer equivalent. Provided with this interface for convenience.
    ///
    /// \return Data chunks count. 0 if no chunks are available.
    ///
    virtual uint32_t GetChunkCount() = 0;

    ///
    /// \brief See #PvBuffer::GetChunkIDByIndex.
    ///
    /// Same as PvBuffer equivalent. Provided with this interface for convenience.
    ///
    /// \param[in] aIndex Data chunk index.
    /// \param[out] aID Data chunk ID for aIndex.
    ///
    /// \return 
    ///    A PvResult is used to captured the success or failure of the operation as 0 can be a valid chunk ID:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the index is out of range or the buffer does not have any chunks
    ///
    virtual PvResult GetChunkIDByIndex( uint32_t aIndex, uint32_t &aID ) = 0;

    ///
    /// \brief See #PvBuffer::GetChunkSizeByIndex. 
    ///
    /// Same as PvBuffer equivalent. Provided with this interface for convenience.
    ///
    /// \param[in] aIndex Data chunk index.
    ///
    /// \return Data chunk size. 0 if the index is out of range.
    ///
    virtual uint32_t GetChunkSizeByIndex( uint32_t aIndex ) = 0;

    ///
    /// \brief See #PvBuffer::GetChunkSizeByID.
    ///
    /// Same as PvBuffer equivalent. Provided with this interface for convenience.
    ///
    /// \param[in] aID Data chunk ID.
    ///
    /// \return Data chunk size. 0 if the chunk ID is not found.
    ///
    virtual uint32_t GetChunkSizeByID( uint32_t aID ) = 0;

    ///
    /// \brief See #PvBuffer::GetChunkRawDataByIndex. 
    ///
    /// Same as PvBuffer equivalent. Provided with this interface for convenience.
    ///
    /// \param[in] aIndex Data chunk index.
    ///
    /// \return Pointer to chunk data. NULL if the index is out of range.
    ///
    virtual const uint8_t *GetChunkRawDataByIndex( uint32_t aIndex ) = 0;

    ///
    /// \brief See #PvBuffer::GetChunkRawDataByID.
    ///
    /// Same as PvBuffer equivalent. Provided with this interface for convenience.
    ///
    /// \param[in] aID Data chunk ID.
    ///
    /// \return Pointer to chunk data. NULL if the chunk ID is not found.
    ///
    virtual const uint8_t *GetChunkRawDataByID( uint32_t aID ) = 0;

    ///
    /// \brief Get the chunk data layout ID.
    ///
    /// \return The current chunk data layout ID.
    ///
    virtual uint32_t GetChunkLayoutID() = 0;

    ///
    /// \brief Returns the current chunk data size.
    ///
    /// In a transmission context this method returns how many bytes are used for extra chunk data.
    /// It includes the chunk headers for all chunk data and if applicable the chunk header
    /// for the main chunk (typically the image).
    ///
    /// This method has no use in a reception context where the data receiver delivers the buffer.
    /// Use either the direct chunk access methods or the GenApi interface of the device to
    /// access chunk data.
    ///
    /// \return Chunk data size in bytes.
    ///
    virtual uint32_t GetChunkDataSize() const = 0;

    ///
    /// \brief Returns the current chunk data capacity.
    ///
    /// In a transmission context where you create the buffer, memory is allocated for
    /// chunks. This method returns how many bytes can still be added as chunk data.
    ///
    /// One chunk is always made of 8-bytes of headers plus the chunk data length. You
    /// also need to reserve another 8-bytes for the main chunk (typically the image) unless
    /// you buffer only contains chunk data.
    ///
    /// This method has no use in a reception context where the data receiver delivers the buffer.
    /// Use either the direct chunk access methods or the GenApi interface of the device to
    /// access chunk data.
    ///
    /// \return Chunk data capacity.
    ///
    virtual uint32_t GetChunkDataCapacity() const = 0;

};


///
/// \class PvChunkData
///
/// \brief Raw data interface to a PvBuffer.
///
/// A #PvBuffer represents a generic buffer with no specific payload type. If the payload type
/// is chunk data, the user can obtain a pointer to a PvChunkData buffer interface through #PvBuffer::GetChunkData.
///
/// This #PvChunkData pointer is owned by the PvBuffer and does not need to be released
/// after use. It is valid as long as the PvBuffer exists.
///
/// To use in a receiver context, retrieve PvBuffer from PvStream or PvPipeline and when #PvBuffer::GetPayloadType
/// is PvPayloadChunkData use #PvBuffer::GetChunkData to retrieve a pointer to this interface. Use the various
/// chunk methods in this class to access the chunk data by ID or index or alternatively attach the PvBuffer
/// to the GenApi interface of the transmitter with #PvGenParameterArray::AttachDataChunks to access the chunk data
/// using GenApi.
///
/// To use in a transmitter context, create a PvBuffer, set its payload type to PvPayloadTypeChunkData in the constructor.
/// Use #PvBuffer::GetChunkData to retrieve a point to this interface, use PvChunkData::Alloc to allocate enough
/// memory for all chunks and their headers (64 bytes per chunk for the headers) and then add chunks to the buffer
/// using #PvChunkData::AddChunk. Use PvTransmitterGEV to transmit the buffer.
///
class PV_BUFFER_API PvChunkData
    : public IPvChunkData
{
public:

    virtual ~PvChunkData() {}

    ///
    /// \brief Get the chunk data payload length specified in the trailer information of this block.
    ///
    /// \return The payload length specified in the trailer information of this block.
    ///
    virtual uint64_t GetChunkDataPayloadLength() const = 0;

    ///
    /// \brief Allocates memory for this #PvChunkData.
    ///
    /// Allocs a PvChunkData of specific payload length. No extra data can or need to be priovisioned
    /// for chunks as the whole payload is made of chunks.
    ///
    /// \param[in]  aMaximumChunkLength Maximum size in bytes to store chunk data (must include extra 64 bits for each chunk header).
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_ENOUGH_MEMORY
    ///
    virtual PvResult Alloc( uint32_t aMaximumChunkLength ) = 0;

    ///
    /// \brief Frees (de-allocates) the buffer's internal memory.
    ///
    virtual void Free() = 0;

    ///
    /// \brief Attach this #PvChunkData to an external memory buffer.
    ///
    /// \param[in]  aRawBuffer A pointer to the buffer.
    /// \param[in]  aMaximumChunkLength Attached buffer size: chunk length sum + 64 bits for each chunk header.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    virtual PvResult Attach( void * aRawBuffer, uint32_t aMaximumChunkLength ) = 0;

    ///
    /// \brief Releases an attached memory buffer.
    ///
    /// Does nothing other then resetting the memory buffer pointer to NULL.
    ///
    /// \return The buffer's location in memory.
    ///
    virtual uint8_t *Detach() = 0;

};


#endif
