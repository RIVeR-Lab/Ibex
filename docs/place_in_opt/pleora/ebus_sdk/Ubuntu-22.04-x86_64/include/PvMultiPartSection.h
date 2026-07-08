// *****************************************************************************
//
//     Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVMULTIPARTSECTION_H__
#define __PVMULTIPARTSECTION_H__

#include <PvBufferLib.h>
#include <PvImage.h>
#include <PvChunkData.h>
#include <PvImageJPEG.h>

/// \file PvMultiPartSection.h
/// \brief Multi-part section interface and enums definition.

/// \enum PvMultiPartDataType
/// \brief Enumeration reprensenting the data type of a section of a multi-part container.
///

/// \var PvMultiPartInvalid
/// Invalid or uninitialized value.

/// \var PvMultiPart2DImage
/// 2D image.
/// Color or monochrome (2D) image. This part carries all the pixel data of the given image
/// (even if the image is represented by a single-plane pixel format). 

/// \var PvMultiPart2DPlaneOfBiPlanar
/// 2D plane of a two-plane image.
/// Single plane of a planar (2D) image. The data should be linked with the other planes to get the complete image.
/// The complete image consists of 2 planes. The planes of a given planar image MUST be placed as consecutive parts within the payload.

/// \var PvMultiPart2DPlaneOfTriPlanar
/// 2D plane of a three-plane image.
/// Single plane of a planar (2D) image. The data should be linked with the other planes to get the complete image.
/// The complete image consists of 3 planes. The planes of a given planar image MUST be placed as consecutive parts within the payload. 

/// \var PvMultiPart2DPlaneOfQuadPlanar
/// 2D plane of a four-plane image.
/// Single plane of a planar (2D) image. The data should be linked with the other planes to get the complete image.
/// The complete image consists of 4 planes.  The planes of a given planar image MUST be placed as consecutive parts within the payload. 

/// \var PvMultiPart3DImage
/// 3D image.
/// 3D image (pixel coordinates). This part carries all the pixel data of the given image
/// (even if the image is represented by a single-plane pixel format, for example when transferring the depth map only). 

/// \var PvMultiPart3DPlaneOfBiPlanar
/// 3D plane of a two-plane image.
/// Single plane of a planar 3D image. The data should be linked with the other coordinate planes to get the complete image.
/// The complete image consists of 2 planes. The planes of a given planar image MUST be placed as consecutive parts within the payload.

/// \var PvMultiPart3DPlaneOfTriPlanar
/// 3D plane of a three-plane image.
/// Single plane of a planar 3D image. The data should be linked with the other coordinate planes to get the complete image.
/// The complete image consists of 3 planes. The planes of a given planar image MUST be placed as consecutive parts within the payload. 

/// \var PvMultiPart3DPlaneOfQuadPlanar
/// 3D plane of a four-plane image.
/// Single plane of a planar 3D image. The data should be linked with the other coordinate planes to get the complete image.
/// The complete image consists of 4 planes.  The planes of a given planar image MUST be placed as consecutive parts within the payload. 

/// \var PvMultiPartConfidenceMap
/// Confidence map.
/// Confidence of the individual pixel values. Expresses the level of validity of given pixel values.
/// Confidence map is always used together with one or more additional image-based parts matching 1:1 dimension-wise.
/// Each value in the confidence map expresses level of validity of the image pixel at matching position. The data format must be a Confidence PFNC format.

/// \var PvMultiPartChunkData
/// Chunk data section. Always last in a multi-part container.
/// Part carrying the chunk data in the Extended Chunk Data Mode. This MUST be always the last part in the payload.
/// There MUST NOT be more than one chunk data part in the payload. 

/// \var PvMultiPartJPEGImage
/// JPEG image.
/// JPEG compressed image following the guidelines defined for the GVSP JPEG payload type.
/// The data_type_specific field is used to carry information found in the JPEG data leader packet.

/// \var PvMultiPartJPEG2000Image
/// JEPG 2000 image.
/// JPEG 2000 compressed image following the guidelines defined for the GVSP JPEG 2000 payload type.
/// The data type specific field is used to carry information found in the JPEG 2000 data leader packet.
EBUS_ENUM(PvMultiPartDataType)
{
    PvMultiPartInvalid = 0,
    PvMultiPart2DImage = 1,
    PvMultiPart2DPlaneOfBiPlanar = 2,
    PvMultiPart2DPlaneOfTriPlanar = 3,
    PvMultiPart2DPlaneOfQuadPlanar = 4,
    PvMultiPart3DImage = 5,
    PvMultiPart3DPlaneOfBiPlanar = 6,
    PvMultiPart3DPlaneOfTriPlanar = 7,
    PvMultiPart3DPlaneOfQuadPlanar = 8,
    PvMultiPartConfidenceMap = 9,
    PvMultiPartChunkData = 10,
    PvMultiPartJPEGImage = 11,
    PvMultiPartJPEG2000Image = 12

} PvMultiPartDataType;

/// \enum PvComponentType
/// \brief Enumeration reprensenting the data type of a data acquisition component.
///

/// \var PvComponentUndefined
/// Invalid or uninitialized value.

/// \var PvComponentIntensity
/// The acquisition of intensity(monochrome or color) of the visible reflected light.

/// \var PvComponentInfrared
/// The acquisition of non - visible infrared light.

/// \var PvComponentUltraviolet
/// The acquisition of non - visible ultraviolet light.

/// \var PvComponentRange
/// The acquisition of range(distance) data.
/// The data produced may be only range(2.5D) or a point cloud giving the 3D coordinates depending on the Scan3dControl.

/// \var PvComponentReflectance
/// The reflected intensity acquired together with Range in a Linescan3D sensor acquiring a single linescan profile for each exposure of the sensor.

/// \var PvComponentConfidence
/// The acquisition of confidence map of the acquired image.
/// Confidence data may be binary(0 - invalid) or an integer where 0 is invalid and increasing value is increased confidence in the data in the corresponding pixel.
/// If floating point representation is used the confidence image is normalized to the range[0, 1], for integer representation the maximum possible integer represents maximum confidence.

/// \var PvComponentScatter
/// The acquisition of data measuring how much light is scattered around the reflected light.
/// In processing this is used as an additional intensity image, often together with the standard intensity or reflectance.

/// \var PvComponentDisparity
/// The acquisition of stereo camera disparity data.
/// Disparity is a more specific range format approximately inversely proportional to distance.Disparity is typically given in pixel units.
EBUS_ENUM(PvComponentType)
{
    PvComponentUndefined = 0,
    PvComponentIntensity = 1,
    PvComponentInfrared = 2,
    PvComponentUltraviolet = 3,
    PvComponentRange = 4,
    PvComponentReflectance = 5,
    PvComponentConfidence = 6,
    PvComponentScatter = 7,
    PvComponentDisparity = 8,
    PvComponentMultispectral = 9

} PvComponentType;

///
/// \class IPvMultiPartSection
/// \brief One section (part) of a multi-part container.
///
class PV_BUFFER_API IPvMultiPartSection
{
public:

    virtual ~IPvMultiPartSection() {}

    ///
    /// \brief Returns the section's data pointer.
    ///
    /// \return A pointer to the memory pointer.
    ///
    virtual const uint8_t *GetDataPointer() const = 0;

    ///
    /// \brief Returns the section's data pointer.
    ///
    /// \return A pointer to the memory pointer.
    ///
    virtual uint8_t *GetDataPointer() = 0;

    ///
    /// \brief Returns the section's maximum possible data size in bytes.
    ///
    /// \return Maximum possible data size in bytes.
    ///
    virtual uint32_t GetSize() const = 0;

    ///
    /// \brief Returns the section's maximum possible data size in bytes.
    ///
    /// \return Maximum possible data size in bytes.
    ///
    virtual uint32_t GetEffectiveSize() const = 0;

    ///
    /// \brief Returns the data type of the section.
    ///
    /// \return Section data type.
    ///
    virtual PvMultiPartDataType GetDataType() const = 0;

    ///
    /// \brief IPvImage pointer cast operator.
    ///
    /// See GetImage for more information.
    ///
    /// \return IPvImage interface to the section.
    ///
    virtual operator IPvImage *() = 0;

    ///
    /// \brief IPvImageJPEG pointer cast operator.
    ///
    /// See GetImageJPEG for more information.
    ///
    /// \return IPvImageJPEG interface to the section.
    ///
    virtual operator IPvImageJPEG *() = 0;

    ///
    /// \brief IPvChunkData pointer cast operator.
    ///
    /// See GetChunkData for more information.
    ///
    /// \return IPvChunkData interface to the section.
    ///
    virtual operator IPvChunkData *() = 0;

    ///
    /// \brief Returns an IPvImage interface to the section.
    ///
    /// Returns a point to an IPvImage interface to the section if the section is 
    /// PvMultiPart2DImage, PvMultiPart2DPlaneOfBiPlanar, PvMultiPart2DPlaneOfTriPlanar,
    /// PvMultiPart2DPlaneOfQuadPlanar, PvMultiPart3DImage, PvMultiPart3DPlaneOfBiPlanar,
    /// PvMultiPart3DPlaneOfTriPlanar, PvMultiPart3DPlaneOfQuadPlanar or PvMultiPartConfidenceMap.
    ///
    /// Return NULL if any other data type.
    ///
    /// The returned pointer is owned by the section. The caller must not attempt to
    /// free or delete it.
    ///
    /// \return IPvImage interface to the section.
    ///
    virtual IPvImage *GetImage() = 0;

    ///
    /// \brief Returns an IPvImageJPEG interface to the section.
    ///
    /// Returns a point to an IPvImageJPEG interface to the section if the section is 
    /// PvMultiPartJPEGImage or PvMultiPartJPEGImage2000.
    ///
    /// Return NULL if any other data type.
    ///
    /// The returned pointer is owned by the section. The caller must not attempt to
    /// free or delete it.
    ///
    /// \return IPvImageJPEG interface to the section.
    ///
    virtual IPvImageJPEG *GetJPEG() = 0;

    ///
    /// \brief Returns an IPvChunkData interface to the section.
    ///
    /// Returns a point to an IPvChunkData interface to the section if the section is 
    /// PvMultiPartChunkData.
    ///
    /// Return NULL if any other data type.
    ///
    /// The returned pointer is owned by the section. The caller must not attempt to
    /// free or delete it.
    ///
    /// The chunk data section is always last in a multi-part container.
    ///
    /// \return IPvChunkData interface to the section.
    ///
    virtual IPvChunkData *GetChunkData() = 0;

    ///
    /// \brief Returns the source ID of the section.
    ///
    /// The source ID is an integer used to identify data produced by the same source. 
    ///
    /// See GigE Vision specification for more information on sources in the multi-part context.
    ///
    /// \return Source ID of the section.
    ///
    virtual uint32_t GetSourceID() const = 0;

    ///
    /// \brief Returns the data purpose ID of the section.
    ///
    /// The data purpose ID is integer used to tag parts having a different purpose. Different color planes would share the same 
    /// Data Purpose ID, while a 3D data and validity mask parts would use different Data Purpose ID. 
    ///
    /// See GigE Vision specification for more information on the data purpose ID in the multi-part context.
    ///
    /// \return Data purpose ID of the section.
    ///
    virtual uint32_t GetDataPurposeID() const = 0;

    ///
    /// \brief Returns the region ID of the section.
    ///
    /// The registion ID is an integer used to tag parts that belong to the same region.
    /// This can be used when data from different regions is transmitted in the same block. 
    ///
    /// See GigE Vision specification for more information on the region ID in the multi-part context.
    ///
    /// \return Region ID of the section.
    ///
    virtual uint32_t GetRegionID() const = 0;

    ///
    /// \brief Returns additional zone count of the section.
    ///
    /// Reports the number of additional zones in the data part. The number of zones (contiguous bands) is 
    /// equal to the value of this field plus one (0 indexed value). 
    ///
    /// See GigE Vision specification for more information on zones in the multi-part context.
    ///
    /// \return Addtional zone count for the section.
    ///
    virtual uint32_t GetAdditionalZones() const = 0;

    ///
    /// \brief Returns the zone direction mask of the section.
    ///
    /// Reports the transmission direction of each zone. bit 0 (msb)
    ///     - Direction of zone 0. Reports the transmission direction of zone ID 0. When set to zero, the zone is 
    ///         transmitted top-down (lowest data addresses first). Otherwise, it is transmitted bottom-up (highest data addresses first). bit 1 
    ///     - Direction of zone 1. Reports the transmission direction of zone ID 1. When set to zero, the zone is transmitted top-down 
    ///         (lowest data addresses first). Otherwise, it is transmitted bottom up (highest data addresses first). 
    ///     - ...
    ///
    /// See GigE Vision specification for more information on zones in the multi-part context.
    ///
    /// \return Zone direction mask of the section.
    ///
    virtual uint32_t GetZoneDirectionMask() const = 0;

};


#endif
