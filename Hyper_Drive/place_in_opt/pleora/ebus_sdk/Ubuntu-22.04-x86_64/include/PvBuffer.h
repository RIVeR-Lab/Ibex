// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVBUFFER_H__
#define __PVBUFFER_H__

#include <PvBufferLib.h>
#include <PvPayloadType.h>
#include <PvImage.h>
#include <PvImage.h>
#include <PvMultiPartContainer.h>
#include <PvRawData.h>
#include <PvH264AccessUnit.h>
#include <PvPleoraCompressed.h>

namespace PvBufferLib
{
    class Buffer;
}

namespace PvStreamLib
{
	class Pipeline;
}

namespace PvVirtualDeviceLib
{
    class PacketizerGVSP;
    class PacketizerGVSPMultiPart;
    class PacketizerRTPUncompressed;
    class PacketizerRTPH264;
    class Transmitter;
    class AcquisitionManager;
}

class PvPipeline;
class PvStream;
class PvBufferConverter;
class PvBufferConverterRGBFilter;
class PvDeInterlacer;
class PvImage;
class PvTransmitterGEV;

///
/// \class PvBuffer
///
/// \brief Represents a block of GigE Vision or USB3 Vision data in memory
///
/// A PvBuffer object is typically used to receive data from a GigE Vision or USB3 Vision transmitter or to transmit data to a GigE Vision receiver.
/// To learn about receiving data from a GigE Vision or USB3 Vision transmitter, see #PvStream.
/// To learn about transmitting GigE Vision data to a GigE Vision receiver, see #PvTransmitterGEV.
///
/// A block of GigE Vision or USB3 Vision data has an associated payload which can be an image, raw data, file, chunk data, extended chunk data or device specific.
///
/// In order to access the payload specific data, use methods like #PvBuffer::GetImage in order to get a pointer 
/// to an interface giving you access to payload specific methods and properties.
///
/// The current payload type of a #PvBuffer can be determined by using the #PvBuffer::GetPayloadType method.
///
/// In this section:
///     - <a href="#configuring">Creating and configuring buffers</a>
///     - <a href="#accessing">Accessing payload type specific data</a>
///     - <a href="#processing">Processing images</a>
///
/// <h2 id="configuring">Creating and configuring buffers</h2>
///
/// <b>To create and configure buffers:</b>
///     - Create several buffers using the default constructor.
///     - Set the size of the buffer. The method you use depends on whether you want to use
///         an internally or externally controlled block of memory:
///         - To use memory that the #PvBuffer object allocates and owns, use #Alloc. 
///         - To use memory that you control (external to the #PvBuffer object), use  #Attach.
///
/// <h2 id="accessing">Accessing payload type specific data</h2>
///
/// <b>To determine the payload type and access payload type specific data:</b>
///     - Retrieve the #PvPayloadType using #PvBuffer::GetPayloadType
///     - If #PvPayloadType is #PvPayloadTypeImage, retrieve #PvImage pointer from your #PvBuffer
///     - Access image specific data through the #PvImage pointer, like #PvImage::GetWidth, #PvImage::GetHeight, etc.
///     - There is no need to release or delete the #PvImage pointer. It is valid as long as the #PvBuffer exists.
///
/// <h2 id="processing">Processing images</h2>
///
/// <b>To process an image:</b>
///     - Retrieve a #PvImage interface to the buffer use #GetImage
///     - Test the success of the image acquisition. Use the <i>aOperationResult</i> parameter in #PvStream::RetrieveBuffer or #PvBuffer::GetOperationResult
///     - Process the image using your own code. You may process the image in place, if you wish. Use:
///         - #PvImage::GetAcquiredSize
///         - #PvImage::GetHeight and #PvImage::GetWidth
///         - #PvImage::GetOffsetX and #PvImage::GetOffsetY
///
class PV_BUFFER_API PvBuffer
    : public IPvChunkData
{
public:
    ///
    /// \brief Constructor.
    ///
    /// \param[in] aPayloadType The block type (default PvPayloadTypeImage).
    ///
    PvBuffer( PvPayloadType aPayloadType = PvPayloadTypeImage );

    ///
    /// \brief Destructor.
    ///
    virtual ~PvBuffer();

    ///
    /// \brief Returns the payload type currently used by the buffer
    ///
    /// If data chunks are available, the buffer payload type is the same as if no data chunks 
    /// were available. Use GetChunkCount to determine if chunks are available.
    ///
    /// \return PvPayloadType The current payload type for the #PvBuffer. See #PvPayloadType enum.
    ///
    PvPayloadType GetPayloadType() const;

    ///
    /// \brief Returns the #PvImage interface to the buffer.
    ///
    /// If the payload type of the #PvBuffer is PvPayloadTypeImage, a pointer to
    /// the #PvImage interface of the buffer is returned.
    ///
    /// If the payload type of the #PvBuffer is NOT PvPayloadTypeImage, a NULL
    /// pointer is returned.
    ///
    /// This #PvImage pointer is owned by the PvBuffer and does not need to be released
    /// after use. It is valid as long as the PvBuffer exists.
    ///
    /// \return Pointer to #PvImage interface or NULL, depending on payload type.
    ///
    PvImage *GetImage();

    ///
    /// \brief Returns the #PvImage interface to the buffer.
    ///
    /// Const version.
    ///
    /// \return Pointer to #PvImage interface or NULL, depending on payload type.
    ///
    const PvImage *GetImage() const;

    ///
    /// \brief Returns the #PvRawData interface to the buffer.
    ///
    /// If the payload type of the #PvBuffer is PvPayloadTypeRawData, a pointer to
    /// the #PvRawData interface of the buffer is returned.
    ///
    /// If the payload type of the #PvBuffer is NOT PvPayloadTypeRawData, a NULL
    /// pointer is returned.
    ///
    /// This #PvRawData pointer is owned by the PvBuffer and does not need to be released
    /// after use. It is valid as long as the PvBuffer exists.
    ///
    /// \return Pointer to #PvRawData interface or NULL, depending on payload type.
    ///
    PvRawData *GetRawData();

    ///
    /// \brief Returns the #PvRawData interface to the buffer.
    ///
    /// Const version.
    ///
    /// \return Pointer to #PvRawData interface or NULL, depending on payload type.
    ///
    const PvRawData *GetRawData() const;

    ///
    /// \brief Returns the #PvPleoraCompressed interface to the buffer.
    ///
    /// If the payload type of the #PvBuffer is PvPayloadTypePleoraCompressed, a pointer to
    /// the #PvPleoraCompressed interface of the buffer is returned.
    ///
    /// If the payload type of the #PvBuffer is NOT PvPayloadTypePleoraCompressed, a NULL
    /// pointer is returned.
    ///
    /// This #PvPleoraCompressed pointer is owned by the PvBuffer and does not need to be released
    /// after use. It is valid as long as the PvBuffer exists.
    ///
    /// \return Pointer to #PvPleoraCompressed interface or NULL, depending on payload type.
    ///
    PvPleoraCompressed *GetPleoraCompressed();

    ///
    /// \brief Returns the #PvPleoraCompressed interface to the buffer.
    ///
    /// Const version.
    ///
    /// \return Pointer to #PvPleoraCompressed interface or NULL, depending on payload type.
    ///
    const PvPleoraCompressed *GetPleoraCompressed() const;

    ///
    /// \brief Returns the #PvChunkData interface to the buffer.
    ///
    /// If the payload type of the #PvBuffer is PvPayloadTypeChunkData, a pointer to
    /// the #PvChunkData interface of the buffer is returned.
    ///
    /// If the payload type of the #PvBuffer is NOT PvPayloadTypeChunkData, a NULL
    /// pointer is returned.
    ///
    /// This #PvChunkData pointer is owned by the PvBuffer and does not need to be released
    /// after use. It is valid as long as the PvBuffer exists.
    ///
    /// \return Pointer to #PvChunkData interface or NULL, depending on payload type.
    ///
    PvChunkData *GetChunkData();

    ///
    /// \brief Returns the #PvChunkData interface to the buffer.
    ///
    /// Const version.
    ///
    /// \return Pointer to #PvChunkData interface or NULL, depending on payload type.
    ///
    const PvChunkData *GetChunkData() const;

    ///
    /// \brief Returns the #IPvMultiPartContainer interface to the buffer.
    ///
    /// If the payload type of the #PvBuffer is PvPayloadTypeMultiPart, a pointer to
    /// the #IPvMultiPartContainer interface of the buffer is returned.
    ///
    /// If the payload type of the #PvBuffer is NOT PvPayloadTypeMultiPart, a NULL
    /// pointer is returned.
    ///
    /// This #IPvMultiPartContainer pointer is owned by the PvBuffer and does not need to be released
    /// after use. It is valid as long as the PvBuffer exists.
    ///
    /// \return Pointer to #IPvMultiPartContainer interface or NULL, depending on payload type.
    ///
    IPvMultiPartContainer *GetMultiPartContainer();

    ///
    /// \brief Returns the #IPvMultiPartContainer interface to the buffer.
    ///
    /// Const version.
    ///
    /// \return Pointer to #IPvMultiPartContainer interface or NULL, depending on payload type.
    ///
    const IPvMultiPartContainer *GetMultiPartContainer() const;

    ///
    /// \brief Returns the #IPvH264AccessUnit interface to the buffer.
    ///
    /// If the payload type of the #PvBuffer is PvPayloadTypeH264, a pointer to
    /// the #IPvH264AccessUnit interface of the buffer is returned.
    ///
    /// If the payload type of the #PvBuffer is NOT #IPvH264AccessUnit, a NULL
    /// pointer is returned.
    ///
    /// This #IPvH264AccessUnit pointer is owned by the PvBuffer and does not need to be released
    /// after use. It is valid as long as the PvBuffer exists.
    ///
    /// \return Pointer to #IPvH264AccessUnit interface or NULL, depending on payload type.
    ///
    IPvH264AccessUnit *GetH264AccessUnit();

    ///
    /// \brief Returns the #IPvH264AccessUnit interface to the buffer.
    ///
    /// Const version.
    ///
    /// \return Pointer to #IPvH264AccessUnit interface or NULL, depending on payload type.
    ///
    const IPvH264AccessUnit *GetH264AccessUnit() const;
    
    ///
    /// \brief Get the buffer's data pointer.
    ///
    /// The #GetDataPointer method returns a pointer to the buffer. To set
    /// the memory location, use #Alloc or #Attach.
    /// 
    /// Const version.
    /// 
    /// \return A pointer to the memory pointer.
    ///
    const uint8_t *GetDataPointer() const;

    ///
    /// \brief Get the buffer's data pointer.
    ///
    /// \return A pointer to the memory pointer.
    ///
    uint8_t *GetDataPointer();

    ///
    /// \brief Get the buffer's ID.
    ///
    /// See #SetID.
    ///
    /// \return The buffer's ID. See #SetID.
    ///
    uint64_t GetID() const;

    ///
    /// \brief Set the buffer's ID.
    ///
    /// The #SetID method lets you assign an arbitrary number to a buffer. When you retrieve the buffer
    /// later, you can also recover the number you assigned.
    ///
    /// The ID is completely arbitrary and solely available for your own use. It can be useful to
    /// keep track of buffers in an application.
    /// 
    /// \param[in] aID The buffer's ID.
    ///
    void SetID( uint64_t aValue );

    ///
    /// \brief Check if the buffer is currently in extended ID mode.
    ///
    /// When the extended ID mode is not enabled, the maximum block ID is 0xFFFFFF 
    /// and the maximum packet ID is 0xFFFFFF as per the GEV 1.X specification. 
    /// When the extended ID mode is enabled, the maximum block ID is 0xFFFFFFFFFFFFFFFF 
    /// and the maximum packet ID is 0xFFFFFFFF as per the GEV 2.X specification.
    ///
    /// When a PvBuffer is received, the extended ID mode is initialized based on the information 
    /// received by #PvStream. When a PvBuffer is sent, the extended ID mode will be initialized by 
    /// the #PvTransmitterGEV::PvTransmitterGEV according to its configuration.
    ///
    /// \return true if the extended if mode is in usage:
    ///
    bool IsExtendedID() const;

    ///
    /// \brief Returns true if the buffer has been attached (instead of allocated)
    ///
    /// \return True if attached.
    ///
    bool IsAttached() const;

    ///
    /// \brief Returns true if the buffer has been attached (instead of attached)
    ///
    /// \return True if allocated.
    ///
    bool IsAllocated() const;

    ///
    /// \brief Gets the size in bytes of the block received by the data receiver including, if present, additional chunk data and headers, optional padding.
    ///
    /// \return The size, in bytes, of bytes received for this buffer.
    ///
    uint32_t GetAcquiredSize() const;

    ///
    /// \brief Gets the size of buffer required to hold the acquired block including, if present, additional chunk data and headers, padding.
    ///
    /// \return The size, in bytes, required to hold the block in this buffer.
    ///
    uint32_t GetRequiredSize() const;

    ///
    /// \brief Get the buffer's allocated or attached size (total capacity) in bytes.
    ///
    /// \return The buffer's (allocated or attached) size in bytes
    ///
    uint32_t GetSize() const;

    ///
    /// \brief Reconstruct the object with a new payload type.
    ///
    /// \param[in] aPayloadType The payload type to reset the buffer to.
    ///
    /// \return #PvResult::Code::OK if successful.
    ///
    PvResult Reset( PvPayloadType aPayloadType = PvPayloadTypeImage );

    ///
    /// \brief Allocates memory for this #PvBuffer.
    ///
    /// \param[in] aSize  The size of the buffer, in bytes.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_ENOUGH_MEMORY
    ///
    PvResult Alloc( uint32_t aSize );

    ///
    /// \brief Alloc a separate space for chunk data in this #PvBuffer.
    ///
    /// Designed for use when attaching external data buffers that cannot
    /// be resized to accomodate chunk data.
    ///
    /// \param[in] aSize    The size, in bytes, required for the buffer.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    /// \sa FreeChunk
    ///
    PvResult AllocChunk( uint32_t aSize );

    ///
    /// \brief Frees (de-allocates) the buffer's internal memory.
    ///
    /// \sa Alloc
    ///
    void Free();

    ///
    /// \brief Frees (de-allocates) an attached memory buffer of chunk data.
    ///
    /// \sa AllocChunk
    ///
    void FreeChunk();

    ///
    /// \brief Attach this #PvBuffer to an external memory buffer.
    ///
    /// To use an internal memory buffer, use #Alloc.
    ///
    /// \param[in] aBuffer  A pointer to the buffer.
    /// \param[in] aSize    The size, in bytes, of the buffer.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    /// \sa Detach
    ///
    PvResult Attach( void * aBuffer, uint32_t aSize );

    ///
    /// \brief Releases an attached memory buffer.
    ///
    /// Does nothing other then resetting the memory buffer pointer to NULL.
    ///
    /// \return The buffer's location in memory.
    ///
    /// \sa Attach
    ///
    uint8_t *Detach();

    ///
    /// \brief Return the block ID.
    ///
    /// This method returns the block ID. The GigE Vision or USB3 Vision transmitter typically increments
    /// the value by 1 for each new image. You can use this value to ensure the
    /// blocks are in order and that none are missing. The value is unsigned and wraps
    /// around to 1 (skipping 0) when it reaches 65536 (for legacy 16bit Block ID mode) 
    /// or 2^64 (for 64 bit Extended Block ID mode).
    ///
    /// \return The block ID
    ///
    uint64_t GetBlockID() const;

    ///
    /// \brief Returns the value of aOperationResult when this buffer was last received through a PvStream.
    ///
    /// Buffers with BUFFER_TOO_SMALL, ABORTED, NOT_CONNECTED and NO_MORE_ITEM operation results are never returned to
    /// the user with PvPipeline. PvPipeline handles these operation results internally without returning the buffers.
    /// Buffers with these operation results can only reach the user when using PvStream directly.
    ///
    /// \return 
    ///    Result of the acquisition operation; Includes:
    ///     - #PvResult::Code::NOT_INITIALIZED
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::TIMEOUT
    ///     - #PvResult::Code::ABORTED
    ///     - #PvResult::Code::MISSING_PACKETS
    ///     - #PvResult::Code::BUFFER_TOO_SMALL: 
    ///     - #PvResult::Code::IMAGE_ERROR for images with grabber issues like data overrun or missing lines (complete or partial)
    ///     - #PvResult::Code::RESENDS_FAILURE
    ///     - #PvResult::Code::TOO_MANY_RESENDS
    ///     - #PvResult::Code::TOO_MANY_CONSECUTIVE_RESENDS
    ///     - #PvResult::Code::INVALID_DATA_FORMAT
    ///     - #PvResult::Code::AUTO_ABORTED
    ///     - #PvResult::Code::ERR_OVERFLOW
    ///
    /// \sa #PvStream::RetrieveBuffer, #PvPipeline::RetrieveNextBuffer
    ///
    PvResult GetOperationResult() const;

    ///
    /// \brief Get the buffer's timestamp.
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
    /// \return The buffer's hardware timestamp; For Pleora devices, this is the value of the relative 
    /// timestamp clock when the grabber began receiving data from the camera head - on the rising edge of the FVAL. 
    ///
    uint64_t GetTimestamp() const;

    ///
    /// \brief Get the buffer's reception timestamp.
    ///
    /// \return The buffer's reception time on the host, in micro seconds. Not an absolute time value.
    ///
    uint64_t GetReceptionTime() const;

    ///
    /// \brief Get the buffer's duration.
    ///
    /// Buffer duration when applicable. Typically used by stream decoders to control
    /// frame rate of a sequence of frames.
    ///
    /// \return Buffer duration.
    ///
    uint64_t GetDuration() const;

    ///
    /// \brief Set the timestamp to place in the block leader.
    ///
    /// When transmitting blocks to a GigE Vision receiver, by default, the timestamp on outgoing blocks is zero.
    /// However, the calling application may set a timestamp on the outgoing blocks if desired by calling #SetTimestamp.
    ///
    /// \param aTimestamp The timestamp to set in the block leader
    ///
    /// \return #PvResult::Code::OK if successful.
    ///
    PvResult SetTimestamp( uint64_t aTimestamp );

    ///
    /// \brief Set the duration of the buffer in the context of a stream.
    ///
    /// \param[in] aDuration Duration of the buffer.
    ///
    /// \return #PvResult::Code::OK if successful.
    ///
    PvResult SetDuration( uint64_t aDuration );

    ///
    /// \brief Sets the block ID of a buffer.
    ///
    /// Should only be used for creating synthetic test PvBuffers or copies of a PvBuffer which caries 
    /// some information over from the original.
    ///
    /// \param aBlockID Block ID to assign to the buffer.
    ///
    /// \return #PvResult::Code::OK if successful.
    ///
    PvResult SetBlockID( uint64_t aBlockID );

    ///
    /// \brief Sets the reception time of a buffer.
    ///
    /// Should only be used for creating synthetic test PvBuffers or copies of a PvBuffer which caries 
    /// some information over from the original.
    ///
    /// \param aReceptionTime Reception tiome to assign to the buffer.
    ///
    /// \return #PvResult::Code::OK if successful.
    ///
    PvResult SetReceptionTime( uint64_t aReceptionTime );

    ///
    /// \brief The number of lost packets successfully recovered  by packet resend requests. Duplicate received packets are not counted.
    /// 
    /// \return The expected resend count
    ///
    uint32_t GetPacketsRecoveredCount() const;

    ///
    /// \brief The number of lost packets successfully recovered  by packet resend requests without any resend request retries.
    /// 
    /// \return The expected resend count
    ///
    uint32_t GetPacketsRecoveredSingleResendCount() const;

    ///
    /// \brief The number of resend requests issued by the data receiver.
    /// 
    /// \return The resend requested count
    ///
    uint32_t GetResendGroupRequestedCount() const;

    ///
    /// \brief The number of packets the data receiver requests with ResendGroupRequested.
    /// 
    /// \return The received resend requested packet count
    ///
    uint32_t GetResendPacketRequestedCount() const;

    ///
    /// \brief Packets that were not successfully delivered when this PvBuffer was filled by the data receiver.
    /// 
    /// \return The lost packet count
    ///
    uint32_t GetLostPacketCount() const;

    ///
    /// \brief Packets received that were ignored when this buffer was filled by the data receiver.
    /// 
    /// \return The ignored packet count
    ///
    uint32_t GetIgnoredPacketCount() const;

    ///
    /// \brief Number of packets received more than once
    /// 
    /// \return The redundant packet count
    ///
    uint32_t GetRedundantPacketCount() const;

    ///
    /// \brief Number of packets received out of order
    /// 
    /// \return The packet out of order count
    ///
    uint32_t GetPacketOutOfOrderCount() const;

    ///
    /// \brief Get the number of missing packet groups that represent unpopulated areas of memory in this buffer
    ///
    /// \note This is an advanced feature that requires knowledge of how GigE Vision data is transmitted.
    ///
    /// \note Does not apply to USB3 Vision devices which do not have the concept of packet resends.
    /// 
    /// When receiving data from a GigE Vision transmitter, network conditions may prevent the successful delivery
    /// of all the data for a given block. Although the eBUS data receiver provides a mechanism for requesting
    /// lost packets, there are cases when it is desirable to ignore missing packets and make use of a buffer that
    /// contains missing information. In some cases, there are portions of a block that are less important and can thus
    /// be disregarded. #GetMissingPacketIdsCount and #GetMissingPacketIds can provide information about
    /// which packets were lost. To use this feature take note of the following procedure:
    ///
    ///     - Enable the feature that causes the data receiver to keep track of missing packets 
    ///         (see the <a href=streamparametersGEV.html#EnableMissingPacketsList>EnableMissingPacketsList</a> property of #PvStream).
    ///     - Optionally disable the mechanism that requests lost packets 
    ///         (see the <a href=streamparametersGEV.html#RequestMissingPackets>RequestMissingPackets</a> property of #PvStream).
    ///     - Check the operation result when a new block is received (see #PvBuffer::GetOperationResult).
    ///     - If the operation result is #PvResult::Code::MISSING_PACKETS, call #GetMissingPacketIdsCount to determine how many groups of packets are missing.
    ///     - Iterate through the missing packet groups using #GetMissingPacketIds.
    ///     - Note that there is no particular order to the missing packet groups that are returned.
    ///     - Take note of the packet size set on the GigE Vision transmitter from which you are receiving 
    ///         to determine the areas of the payload where information is missing.
    ///
    /// \param aCount The number of missing packet groups that represent unpopulated areas of memory in this buffer
    ///
    /// \return #PvResult::Code::OK or #PvResult::Code::NOT_IMPLEMENTED.
    ///
    PvResult GetMissingPacketIdsCount( uint32_t& aCount );

    ///
    /// \brief Get a missing packet group that represents an unpopulated area of memory in this buffer
    ///
    /// \note This is an advanced feature that requires knowledge of how GigE Vision data is transmitted.
    /// Refer to #GetMissingPacketIdsCount for further details on how this feature is used.
    ///
    /// \note Does not apply to USB3 Vision devices which do not have the concept of packet resends.
    /// 
    /// \param aIndex The index of the missing packet group being retrieved.
    /// \param aPacketIdLow The first packet of a range of missing packets.
    /// \param aPacketIdHigh The last packet of a range of missing packets. A value of 0xFFFFFF ( or 0xFFFFFFFF for ExtendedID )
    ///  indicates that the last packet of the block went missing and therefore the actual size of the block is unknown.
    ///
    /// \return #PvResult::Code::OK or #PvResult::Code::NOT_IMPLEMENTED.
    ///
    PvResult GetMissingPacketIds( uint32_t aIndex, uint32_t& aPacketIdLow, uint32_t& aPacketIdHigh );

    ///
    /// \brief Resets the internal chunk state.
    ///
    /// Used when creating PvBuffer with chunks for transmission. This method should always be called
    /// before adding the first chunk block to a buffer. It removes chunk data from the previous
    /// sequence of AddChunk.
    ///
    /// If you are adding more than one chunk block to a buffer, you would normally perform this
    /// sequence for every buffer setup for transmission:
    ///
    ///     - lBuffer->ResetChunks();
    ///     - lBuffer->SetChunkLayoutID( CHUNKLAYOUTID );
    ///     - lBuffer->AddChunk( CHUNKID1, lData1, sizeof( lData1 ) );
    ///     - lBuffer->AddChunk( CHUNKID2, lData2, sizeof( lData2 ) );
    ///
    void ResetChunks();

    ///
    /// \brief Add chunk data to the buffer.
    ///
    /// Add chunk data to the buffer payload section by specifying the ID of the chunk data.
    ///
    /// See #ResetChunks for more information.
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
    PvResult AddChunk( uint32_t aID, const uint8_t *aData, uint32_t aLength );

    ///
    /// \brief Set the chunk data layout ID.
    ///
    /// \param[in] aChunkLayoutID Chunk data layout ID.
    ///
    void SetChunkLayoutID( uint32_t aChunkLayoutID );

    ///
    /// \brief Returns true if the buffer has data chunks.
    ///
    /// Unlike GetChunkCount, chunks do not need to be parsed or resolved 
    /// with this method.
    ///
    /// \return True if the buffer holds data chunks, false if not.
    ///
    bool HasChunks() const;

    ///
    /// \brief Returns the number of data chunks in the PvBuffer. 
    ///
    /// If no data chunks are available, 0 is returned.
    ///
    /// Counting chunks requires the chunks to be parsed and resolved which has a 
    /// performance hit. 
    ///
    /// If you need to access chunks directly, you need to use this method to enumerate 
    /// the chunks. If you access the chunks through GenICam, just use HasChunks to see 
    /// if you need to attach the buffer the GenICam parameter array of the device.
    ///
    /// \return Data chunks count. 0 if no chunks are available.
    ///
    uint32_t GetChunkCount();

    ///
    /// \brief Returns the ID of a chunk based on its index.
    ///
    /// If the index is out of range or the PvBuffer does not have any data 
    /// chunk 0 is returned in aID.
    ///
    /// \param[in] aIndex Data chunk index.
    /// \param[out] aID Data chunk ID for aIndex.
    ///
    /// \return A PvResult is used to captured the success or failure of the operation as 0 can be a valid chunk ID:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::NOT_FOUND if the index is out of range or the buffer does not have any chunks
    ///
    PvResult GetChunkIDByIndex( uint32_t aIndex, uint32_t &aID );

    ///
    /// \brief Returns the size in bytes of the data chunk at zero-based aIndex. 
    ///
    /// Chunks are enumerated from the end of the payload data to its beginning.
    /// If the index is out of range or the PvBuffer does not have any data 
    /// chunk 0 is returned.
    ///
    /// \param[in] aIndex Data chunk index.
    ///
    /// \return Data chunk size. 0 if the index is out of range.
    ///
    uint32_t GetChunkSizeByIndex( uint32_t aIndex );

    ///
    /// \brief Returns the size in bytes of the data chunk identified by aID.
    ///
    /// If the aID chunk is not found or the PvBuffer does not have any 
    /// chunks 0 is returned.
    ///
    /// \param[in] aID Data chunk ID.
    ///
    /// \return Data chunk size. 0 if the chunk ID is not found.
    ///
    uint32_t GetChunkSizeByID( uint32_t aID );

    ///
    /// \brief Returns a const pointer to the data chunk at zero-based aIndex. 
    ///
    /// Chunks are enumerated from the end of the payload data to its beginning.
    /// If the index is out of range or the PvBuffer does not have any data 
    /// chunk NULL is returned.
    ///
    /// \param[in] aIndex Data chunk index.
    ///
    /// \return Point to chunk data. NULL if the index is out of range.
    ///
    const uint8_t *GetChunkRawDataByIndex( uint32_t aIndex );

    ///
    /// \brief Returns a const pointer to the data chunk with its ID matching aID.
    ///
    /// If the aID chunk is not found or the PvBuffer does not have any chunks 
    /// NULL is returned.
    ///
    /// \param[in] aID Data chunk ID.
    ///
    /// \return Point to chunk data. NULL if the chunk ID is not found.
    ///
    const uint8_t *GetChunkRawDataByID( uint32_t aID );

    ///
    /// \brief Returns payload size in bytes, excluding any extra padding.
    ///
    /// Gets the size in bytes of the block received by the data receiver including, 
    /// if present, additional chunk data and headers.
    ///
    /// Different than GetAcquiredSize as it does not include optional additional padding
    /// when data chunks are present.
    ///
    /// If data chunks are not present, this method returns the same value as GetAcquiredSize 
    /// as extra padding is only an issue when data chunks are present.
    ///
    /// This method is required in order to parse the chunk data using GenICam. 
    ///
    /// \return Payload size in bytes, excluding any extra padding.
    ///
    uint32_t GetPayloadSize() const;

    ///
    /// \brief Get the chunk data layout ID.
    ///
    /// \return The current chunk data layout ID.
    ///
    uint32_t GetChunkLayoutID();

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
    uint32_t GetChunkDataSize() const;

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
    uint32_t GetChunkDataCapacity() const;

    ///
    /// \brief Returns whether this buffer has valid header (GVSP) information.
    ///
    /// If the buffer header is not valid (missing) the buffer has to be manipulated
    /// with extreme caution. To start with, it is impossible to confirm the payload type.
    /// The timestamp and other shared header information is not valid. Finally, information
    /// specific to some payload types (like with, height, pixel format, etc. for the image
    /// payload type) is not available or valid this buffer.
    ///
    /// When a buffer is attached or allocated for use other than receiving GVSP data,
    /// this attribute evaluates to true in order to appear as valid.
    ///
    /// \return True if the GVSP header is valid.
    ///
    bool IsHeaderValid() const;

    ///
    /// \brief Returns whether this buffer has valid trailer (GVSP) information.
    ///
    /// If the buffer trailer is not valid (missing) some information about the buffer
    /// could be missing. As an example, the image payload type confirms the number
    /// of lines received in the trailer.
    ///
    /// When a buffer is attached or allocated for use other than receiving GVSP data,
    /// this attribute evaluates to true in order to appear as valid.
    ///
    /// \return True if the GVSP header is valid.
    ///
    bool IsTrailerValid() const;

    ///
    /// \brief Returns whether this buffer has large data leader and data trailer enabled.
    ///
    /// If the buffer is used for Multi-Part streaming, the maximum allowed number of parts is limited by
    /// whether Large data leader and trailer is enabled.
    /// 
    /// \return True if the Large Data Leader and Trailer is enabled on the buffer which contains Multi-part.
    ///
    bool IsLargeLeaderTrailerEnabled() const;

private:

    // Not implemented
    PvBuffer( const PvBuffer & );
    const PvBuffer &operator=( const PvBuffer & );

    friend class PvStreamLib::Pipeline;
    friend class PvPipeline;
    friend class PvStream;
    friend class PvBufferConverter;
    friend class PvBufferConverterRGBFilter;
    friend class PvDeInterlacer;
    friend class PvTransmitterGEV;
    friend class PvVirtualDeviceLib::PacketizerGVSP;
    friend class PvVirtualDeviceLib::PacketizerGVSPMultiPart;
    friend class PvVirtualDeviceLib::PacketizerRTPUncompressed;
    friend class PvVirtualDeviceLib::PacketizerRTPH264;
    friend class PvVirtualDeviceLib::Transmitter;
    friend class PvVirtualDeviceLib::AcquisitionManager;

    PvBufferLib::Buffer * mThis;
};


#endif
