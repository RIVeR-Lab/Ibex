// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVPAYLOADTYPE_H__
#define __PVPAYLOADTYPE_H__

#ifndef EBUS_ENUM
#define EBUS_ENUM(x) typedef enum
#endif

///
/// \file PvPayloadType.h
/// \brief GVSP payload type constants.
///

///
/// \enum PvPayloadType
/// \brief GVSP payload type.
///

/// \var PvPayloadTypeUndefined
/// Undefined or non initialized payload type

/// \var PvPayloadTypeNone
/// A proprietary type used for GigE Vision Validation Framework testing. Refer to IPvStreamingChannelSource::SetTestPayloadFormatMode for more details.

/// \var PvPayloadTypeImage
/// Image payload type. Access from #PvBuffer using PvBuffer::GetImage.

/// \var PvPayloadTypeRawData
/// Raw data payload type. Access from #PvBuffer using PvBuffer::GetRawData.

/// \var PvPayloadTypeFile
/// File payload type. Not currently supported.

/// \var PvPayloadTypeChunkData
/// Chunk data payload type. Not currently supported.

/// \var PvPayloadTypeExtendedChunkData
/// Extended chunk data payload type. Not currently supported.

/// \var PvPayloadTypeJPEG
/// JPEG compressed image payload type. Not currently supported.

/// \var PvPayloadTypeJPEG2000
/// JPEG 2000 compressed image payload type. Not currently supported.

/// \var PvPayloadTypeH264
/// H.264 compressed video stream payload type. Not currently supported.

/// \var PvPayloadTypeMultiZoneImage
/// Multi-zone image payload type. Not currently supported.

/// \var PvPayloadTypeMultiPart
/// Multi-part payload type.

/// \var PvPayloadTypeDeviceSpecificBase
/// Base value for device specific payload type. Not currently supported.

/// \var PvPayloadTypePleoraCompressed
/// Pleora compressed image payload type, proprietary payload type.

EBUS_ENUM(PvPayloadType)
{ 
    PvPayloadTypeUndefined = -1,
    PvPayloadTypeNone = 0,
    PvPayloadTypeImage = 0x0001,
    PvPayloadTypeRawData = 0x0002,
    PvPayloadTypeFile = 0x0003,
    PvPayloadTypeChunkData = 0x0004,
	PvPayloadTypeExtendedChunkData = 0x0005,
    PvPayloadTypeJPEG = 0x0006,
    PvPayloadTypeJPEG2000 = 0x0007,
    PvPayloadTypeH264 = 0x0008,
    PvPayloadTypeMultiZoneImage = 0x0009,
    PvPayloadTypeMultiPart = 0x000A,
    PvPayloadTypeDeviceSpecificBase = 0x8000,
    PvPayloadTypePleoraCompressed = 0x80E7

} PvPayloadType;


#endif
