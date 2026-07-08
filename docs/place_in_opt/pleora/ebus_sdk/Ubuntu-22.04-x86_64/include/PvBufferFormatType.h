// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVBUFFERFORMATTYPE_H__
#define __PVBUFFERFORMATTYPE_H__

///
/// \file PvBufferFormatType.h
///

///
/// \enum PvBufferFormatType
///
/// The #PvBufferFormatType enumeration lets you select the format 
/// of the image saved by the #PvBufferWriter::Store method.
///

/// \var PvBufferFormatBMP
/// \brief Bitmap format. The image is formatted as a 32-bit BMP.

/// \var PvBufferFormatRaw
/// \brief Raw format. The data is saved exactly as it is stored in the #PvBuffer object in a raw binary file.

/// \var PvBufferFormatTIFF
/// \brief TIFF format. The image is formatted as either a 8-bit or 16-bit TIFF file to preserve as much information as possible.

/// \var PvBufferFormatPNG
/// \brief PNG format, The image is formatted as only a 8-bit PNG file.

/// \var PvBufferFormatPTCDecompressedRaw
/// \brief For Pleora Compressed data, to be decompressed into raw format.
typedef enum
{ 
    PvBufferFormatBMP = 0,
    PvBufferFormatRaw = 1,
    PvBufferFormatTIFF = 2,
    PvBufferFormatPNG = 3,
    PvBufferFormatPTCDecompressedRaw = 4

} PvBufferFormatType;


#endif
