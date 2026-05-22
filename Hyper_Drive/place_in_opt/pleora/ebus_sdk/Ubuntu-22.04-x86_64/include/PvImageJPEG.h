// *****************************************************************************
//
//     Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVIMAGEJPEG_H__
#define __PVIMAGEJPEG_H__

#include <PvBufferLib.h>

///
/// \class IPvImageJPEG
/// \brief Generic interface to a JPEG image.
///
/// Preliminary JPEG support in the context of GigE Vision multi-part support. The data
/// is provided as-received and the eBUS SDK does not currently offer more support for
/// JPEG payloads.
///
class PV_BUFFER_API IPvImageJPEG
{
public:

    virtual ~IPvImageJPEG() {}

    ///
    /// \brief Get the buffer's data pointer.
    ///
    /// \return A pointer to the memory pointer.
    ///
    virtual const uint8_t *GetDataPointer() const = 0;

    ///
    /// \brief Get the buffer's data pointer.
    ///
    /// \return A pointer to the memory pointer.
    ///
    virtual uint8_t *GetDataPointer() = 0;

    ///
    /// \brief Returns the native JPEG flags of the compressed image.
    ///
    /// \return JPEG flags of the image.
    ///
    virtual uint8_t GetFlag() const = 0;

    ///
    /// \brief Returns the timetstamp tick frequency as provided in the GVSP header.
    ///
    /// \return Timestamp tick frequency.
    ///
    virtual uint64_t GetTimestampTickFrequency() const = 0;

    ///
    /// \brief Returns the JPEG data format.
    ///
    /// \return JPEG data format.
    ///
    virtual uint32_t GetDataFormat() const = 0;

    ///
    /// \brief Returns the JPEG data size.
    ///
    /// \return JPEG data size.
    ///
    virtual uint64_t GetDataSize() const = 0;

};


#endif // __PVIMAGEJPEG_H__
