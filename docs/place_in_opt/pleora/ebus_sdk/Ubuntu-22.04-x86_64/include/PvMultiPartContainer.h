// *****************************************************************************
//
//     Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVMULTIPARTCONTAINER_H__
#define __PVMULTIPARTCONTAINER_H__

#include <PvMultiPartSection.h>

///
/// \class IPvMultiPartContainerReader
/// \brief Interface used to read from a multi-part container.
///
/// To receive and access multi-part data from a GigE Vision device, use a PvStream or
/// PvPipeline to receive buffers and then use PvBuffer::GetMultiPartContainer to retrieve
/// an IPvMultiPartContainerReader or IPvMultiPartContainer interface.
///
/// Each part is represented by a multi-part section that can be accessed through
/// an IPvMultiPartSection interface.
///
/// Retrieve the section count with GetPartCount and then access the individual sections
/// with the GetPart or operator[] of this interface.
///
class PV_BUFFER_API IPvMultiPartContainerReader
{
public:

    virtual ~IPvMultiPartContainerReader() {}

    ///
    /// \brief Part count of the multi-part container.
    ///
    /// \return Part count.
    ///
    virtual uint32_t GetPartCount() = 0;

    ///
    /// \brief Indexed part (section) const accessor.
    ///
    /// \param [in] aIndex Zero-based index of the part.
    ///
    /// \return Returns a const section interface from its index.
    ///
    virtual const IPvMultiPartSection *GetPart( uint32_t aIndex ) const = 0;

    ///
    /// \brief Indexed part (section) accessor.
    ///
    /// \param [in] aIndex Zero-based index of the part.
    ///
    /// \return Returns a section interface from its index.
    ///
    virtual IPvMultiPartSection *GetPart( uint32_t aIndex ) = 0;

    ///
    /// \brief Indexed part (section) const accessor.
    ///
    /// \param [in] aIndex Zero-based index of the part.
    ///
    /// \return Returns a const section interface from its index.
    ///
    virtual const IPvMultiPartSection *operator[]( uint32_t aIndex ) const = 0;

    ///
    /// \brief Indexed part (section) accessor.
    ///
    /// \param [in] aIndex Zero-based index of the part.
    ///
    /// \return Returns a section interface from its index.
    ///
    virtual IPvMultiPartSection *operator[]( uint32_t aIndex ) = 0;

};


///
/// \class IPvMultiPartContainerWriter
/// \brief Interface used to write to a multi-part container.
///
/// This interface is used to create a multi-part PvBuffer than can be transmitted using PvSoftDeviceGEV.
///
/// A multi-part buffer would typically be prepared for transmission by:
///     - Creating a PvBuffer by specifying a payload type of PvPayloadTypeMultiPart to the constructor.
///     - Retrieving the IPvMultiPartContainerWriter interface using PvBuffer::GetMultiPartContainer.
///     - Defining the parts using the #AddImagePart, #AddJPEGPart and #AddChunkPart methods.
///     - Allocating/attaching memory using either #AllocAllParts or a mix of #AllocPart and #AttachPart.
///     - Filling the part data using IPvMultiPartContainerReader and IPvMultiPartSection.
///     - Using the #Validate method to make sure the multi-part container is valid.
///     - Transmitting the part from a streaming source of PvSoftDeviceGEV.
///
/// Adding parts with this interface only creates the part definition in the part index of the container.
/// Memory then needs to be allocated or attached for all parts. Make sure to call #Validate after defining
/// the parts and allocating or attaching memory to make sure the multi-part container is properly setup.
///
/// You can either have no data chunk part of one chunk part and that part needs to be the last part. Use
/// The #IPvChunkData interface to the section to define the chunk data and confirm the chunk length using
/// #SetPartFinalLength to confirm the chunk data length.
///
/// See SoftGEVDeviceMultiPart C++ eBUS SDK sample for more information.
///
/// GigE Vision restricts the maximum part count to 10. This limitation comes from the part index size
/// in the context of the maximum GVSP header size.
///
class PV_BUFFER_API IPvMultiPartContainerWriter
    : public IPvMultiPartContainerReader
{
public:

    virtual ~IPvMultiPartContainerWriter() {}

    ///
    /// \brief Resets the multi-part container index to empty - no parts.
    ///
    virtual void Reset() = 0;

    ///
    /// \brief Adds an image part to the index of a multi-part container.
    ///
    /// Calling this method only defines a new image in the multi-part container index. Memory needs to be allocated or attached
    /// using the AllocAllParts, AllocPart or AttachPart methods.
    ///
    /// The height is initially defined as maximum height. It is possible to later call SetPartFinalImageHeight to confirm
    /// the effective height of the image, as long as the confirmed height is less or equal to the maximum height defined here.
    /// This allows linescan-like scenario where the final height of the image is not known when the image part is defined
    /// and its memory allocated or attached.
    ///
    /// \param [in] aDataType Image data type. Must be between PvMultiPart2DImage and PvMultiPartConfidenceMap, inclusive.
    /// \param [in] aWidth Width of the image, in pixels.
    /// \param [in] aMaxHeight Height of the image, in pixels.
    /// \param [in] aPixelType Pixel type of the image.
    /// \param [in] aOffsetX Offset X of the image, in pixels.
    /// \param [in] aOffsetY Offset Y of the image, in pixels.
    /// \param [in] aPaddingX Padding X of the image, in bytes.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER aDataType does not represent an image, aWidth is zero or aHeight is zero.
    ///     - #PvResult::Code::NOT_SUPPORTED if the PvBuffer accessed from this interfaced is not defined as a multi-part buffer.
    ///     - #PvResult::Code::ERR_OVERFLOW if the maximum number of parts supported by the GigE Vision specification has already been reached.
    ///     - #PvResult::Code::STATE_ERROR if the previously added part is of type chunk data which is only valid as the last part.
    ///
    virtual PvResult AddImagePart( PvMultiPartDataType aDataType, uint32_t aWidth, uint32_t aMaxHeight, PvPixelType aPixelType, uint32_t aOffsetX = 0, uint32_t aOffsetY = 0, uint16_t aPaddingX = 0 ) = 0;
    
    ///
    /// \brief Adds a JPEG part to the index of a multi-part container.
    ///
    /// Calling this method only defines a new JPEG image in the multi-part container index. Memory needs to be allocated or attached
    /// using the AllocAllParts, AllocPart or AttachPart methods.
    ///
    /// The length is initially defined as maximum length, in bytes. It is possible to later call SetPartFinalLength to confirm
    /// the effective length of the compressed image, as long as the confirmed length is less or equal to the maximum length defined here.
    /// This allows reserving memory for the worse-case compression scenario and defining the real compressed image length when putting the
    /// part together before transmission.
    ///
    /// \param [in] aDataType Multi-part data type. Either PvMultiPartJPEGImage or PvMultiPartJPEG2000Image.
    /// \param [in] aMaxLength Maximum length for the compressed image. Defined here as worse-case compression, exact size later confirmed with SetPartFinalLength.
    /// \param [in] aFlag JPEG flags.
    /// \param [in] aTimestampTickFrequency Timestamp tick frequency. Can be used with JPEG2000 to turn buffer timestamp into real time units.
    /// \param [in] aDataFormat JPEG data format.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::NOT_SUPPORTED if the PvBuffer accessed from this interfaced is not defined as a multi-part buffer.
    ///     - #PvResult::Code::ERR_OVERFLOW if the maximum number of parts supported by the GigE Vision specification has already been reached.
    ///     - #PvResult::Code::STATE_ERROR  if the previously added part is of type chunk data which is only valid as the last part.
    ///
    virtual PvResult AddJPEGPart( PvMultiPartDataType aDataType, uint32_t aMaxLength, uint8_t aFlag, uint64_t aTimestampTickFrequency, uint32_t aDataFormat ) = 0;

    ///
    /// \brief Adds a chunk data part to the index of a multi-part container.
    ///
    /// The chunk data section is always last in a multi-part container. There can only be one chunk data
    /// section for a multi-part container. A chunk data section can contain one or more chunks.
    ///
    /// \param [in] aMaxLength Maximum length for chunk data.
    /// \param [in] aChunkLayoutID Chunk layoud ID for the chunk data. 
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::NOT_SUPPORTED if the PvBuffer accessed from this interfaced is not defined as a multi-part buffer.
    ///     - #PvResult::Code::ERR_OVERFLOW if the maximum number of parts supported by the GigE Vision specification has already been reached.
    ///     - #PvResult::Code::STATE_ERROR  if the previously added part is of type chunk data which is only valid as the last part.
    ///
    virtual PvResult AddChunkPart( uint32_t aMaxLength, uint32_t aChunkLayoutID ) = 0;

    ///
    /// \brief Sets the source, data purpose and region IDs of a part.
    /// 
    /// See #IPvMultiPartSection for more information about these IDs.
    ///
    /// If this method is not called after adding a part to the multi-part container index, all IDs are zero.
    ///
    /// \param aIndex [in] Zero-based index of the part.
    /// \param aSourceID [in] Source ID to assign to the part
    /// \param aDataPurposeID [in] Data purpose ID to assign to the part.
    /// \param aRegionID [in] Region ID to assign to the part.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_DATA_FORMAT if the indexed part is chunk data (which does not have these IDs defined) or if the buffer is not defined as multi-part.
    ///     - #PvResult::Code::INVALID_PARAMETER if aIndex is out of range.
    ///
    virtual PvResult SetPartIDs( uint32_t aIndex, uint32_t aSourceID, uint32_t aDataPurposeID, uint32_t aRegionID ) = 0;

    ///
    /// \brief Sets the zone info of a part.
    ///
    /// See #IPvMultiPartSection for more information about zone info.
    ///
    /// If this method is not called after adding a part to the multi-part container index, the number of additional zones
    /// and direction mask are both set to zero.
    ///
    /// \param aIndex [in] Zero-based index of the part.
    /// \param aAdditionalZones [in] Number of additional zones for this part.
    /// \param aZoneDirectionMask [in] Zone direction  of for all zones of this part.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_DATA_FORMAT if the indexed part is chunk data (which does not have these IDs defined) or if the buffer is not defined as multi-part.
    ///     - #PvResult::Code::INVALID_PARAMETER if aIndex is out of range.
    /// 
    virtual PvResult SetPartZoneInfo( uint32_t aIndex, uint8_t aAdditionalZones, uint32_t aZoneDirectionMask ) = 0;

    ///
    /// \brief Allocates memory for all parts defined by the part index of the multi-part container.
    ///
    /// With this method all parts are allocated as a single contiguous memory buffer.
    ///
    /// Memory is allocated based on each parts definition: image size is inferred from width, max height, pixel format and padding X.
    /// JPEG from the maximum length. Chunk from the maximum chunk length.
    ///
    /// If memory was allocated for all parts or individual parts, it is freed before being re-allocated. If memory
    /// was attached for any part, it is simply detached.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::STATE_ERROR if no parts are currently defined.
    ///     - #PvResult::Code::ERR_OVERFLOW if more memory is required than currently supported by the PvBuffer class.
    ///
    virtual PvResult AllocAllParts() = 0;

    ///
    /// \brief Allocates memory for one of the parts as defined by the index of the part.
    ///
    /// If memory has been previously allocated for all parts, new memory for this part alone is allocated
    /// and used instead. Unused memory for the part is still reserved in the contiguous buffer.
    ///
    /// Memory is allocated based on each parts definition: image size is inferred from width, max height, pixel format and padding X.
    /// JPEG from the maximum length. Chunk from the maximum chunk length.
    ///
    /// \param aIndex [in] Zero-based index of the part.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if aIndex is out of range.
    ///
    virtual PvResult AllocPart( uint32_t aIndex ) = 0;

    ///
    /// \brief Attaches a part to an external memory buffer.
    ///
    /// If memory has been previously allocated for all parts, new memory for this part alone is attached
    /// and used instead. Unused memory for the part is still reserved in the contiguous buffer.
    ///
    /// \param aIndex [in] Zero-based index of the part.
    /// \param aBuffer [in] Pointer to the external memory buffer.
    /// \param aLength [in] Length of the external memory buffer to attach.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if aIndex is out of range.
    ///
    virtual PvResult AttachPart( uint32_t aIndex, uint8_t *aBuffer, uint64_t aLength ) = 0;

    ///
    /// \brief Sets the final length of a part.
    ///
    /// Used to confirm final length of JPEG and chunk data parts. If this method is not called, the part length is assumed
    /// to be the full maximum part length as defined in AddJPEGPart or AddChunkPart.
    ///
    /// See AddJPEGPart and AddChunkPart for more information.
    ///
    /// \param aIndex [in] Zero-based index of the part.
    /// \param aLength [in] aLength Final length of the part.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if aIndex is out of range, aLength is bigger than the maximum part length or the part is an image.
    ///
    virtual PvResult SetPartFinalLength( uint32_t aIndex, uint32_t aLength ) = 0;

    ///
    /// \brief Sets the final height of an image part.
    ///
    /// Used to confirm final height of image parts. If this method is not called, the image height is assumed
    /// to be the full maximum image height as defined in AddImagePart.
    ///
    /// See AddImagePart for more information.
    ///
    /// \param aIndex [in] Zero-based index of the part.
    /// \param aHeight [in] Final height of the part.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if aIndex is out of range.
    ///
    ///     - #PvResult::Code::INVALID_PARAMETER if aIndex is out of range, aHeight is bigger than the maximum height for this part or the part is not an image.
    ///
    virtual PvResult SetPartFinalImageHeight( uint32_t aIndex, uint32_t aHeight ) = 0;

    ///
    /// \brief Validates the part index and allocated/attached part memory.
    ///
    /// This method goes through all parts defined in the part index of this container and makes sure
    /// that enough memory is allocated or attached to each individual part. It can also fail if not part
    /// has been defined.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if aIndex is out of range.
    ///
    virtual PvResult Validate() = 0;

};


///
/// \class IPvMultiPartContainer
/// \brief Interface to a multi-part container.
///
/// Implements both IPvMultiPartContainerReader and IPvMultiPartContainerWriter.
///
/// See IPvMultiPartContainerReader for more information on how to access a multi-part
/// buffer as received from a GigE Vision source.
///
/// See IPvMultiPartContainerWriter for more information on how to create and populate
/// a multi-part buffer in order to transmit it from a GigE Vision source.
///
class PV_BUFFER_API IPvMultiPartContainer
    : public IPvMultiPartContainerWriter
{
public:

    virtual ~IPvMultiPartContainer() {}

};


#endif
