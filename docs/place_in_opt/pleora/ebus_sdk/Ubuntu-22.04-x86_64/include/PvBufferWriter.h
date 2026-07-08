// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVBUFFERWRITTER_H__
#define __PVBUFFERWRITTER_H__

#include <PvBuffer.h>
#include <PvBufferConverter.h>
#include <PvBufferFormatType.h>


namespace PvBufferLib
{
    class BufferWriter;
}

///
/// \class PvBufferWriter
///
/// \brief Saves the contents of a PvBuffer object to a raw data file or bitmap.
///
/// <b>To save an image buffer to disk:</b>
///     - Acquire an image. See #PvBuffer.
///     - Create an image save. Use #PvBufferWriter::PvBufferWriter.
///     - Save the image to disk. Use #Store.
///
class PV_BUFFER_API PvBufferWriter
{

public:

    ///
    /// \brief Constructor.
    ///
    PvBufferWriter();

    ///
    /// \brief Destructor.
    ///
    virtual ~PvBufferWriter();

    /// \brief Saves a #PvBuffer to disk.
    ///
    /// This method lets you save an image buffer (#PvBuffer object) to disk in a variety of formats.
    /// If a conversion to color is required, it happens automatically within the #Store method. If no
    /// conversion to the required color format is available, #Store fails.
    ///
    /// \param[in]  aBuffer       The image to save.
    /// \param[in]  aFilename     The filename, including extension. The format uses the same standard as the C fopen function. 
    ///                            In C/C++, valid filenames include "myfile.raw", "..\\myfile.bmp", 
    ///                            "C:\\myfile.bmp". 
    /// \param[in]  aType         The format of the image; a PvBufferFormatType enumeration.
    /// \param[out] aBytesWritten If not NULL (default) contains the number of bytes effectively written when storing the buffer.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER (e.g. invalid PvBufferFormatType.)
    ///     - #PvResult::Code::CANNOT_OPEN_FILE
    ///     - #PvResult::Code::INVALID_DATA_FORMAT (The buffer can't be converted to the specified type.)
    ///
    PvResult Store( PvBuffer* aBuffer, const PvString& aFilename, PvBufferFormatType aType = PvBufferFormatBMP, uint32_t *aBytesWritten = NULL );

    ///
    /// \brief Returns a reference to the internal buffer converter.
    ///
    /// When writing the buffer to a file requires a conversion, it is 
    /// performed automatically by the PvBufferWriter. The converter
    /// used for this operation can be configured using this method.
    /// As an example, a RGB filter could be set or the Bayer
    /// interpolation method could be selected.
    ///
    /// \return The internal buffer converter.
    ///
    PvBufferConverter &GetConverter();

protected:

private:

    // Not implemented
	PvBufferWriter( const PvBufferWriter & );
	const PvBufferWriter &operator=( const PvBufferWriter & );

    PvBufferLib::BufferWriter *mThis;
};

#endif
