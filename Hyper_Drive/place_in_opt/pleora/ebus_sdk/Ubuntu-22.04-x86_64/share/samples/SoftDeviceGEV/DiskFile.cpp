// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "DiskFile.h"


///
/// Open
///

IFileAccessFile::Result DiskFile::Open( IFileAccessFile::FileOpenMode const aMode )
{
    // Already opened?
    if ( mHandle && mHandle.is_open() )
    {
        return std::make_pair( IFileAccessFile::Failure, 0 );
    }

    std::ios_base::openmode lMode = ( aMode == IFileAccessFile::FOM_READ )
        ? std::ios::in
        : ( aMode == IFileAccessFile::FOM_WRITE )
            ? std::ios::out
            : std::ios::in | std::ios::out;
    if ( mIsBinary )
    {
        lMode |= std::ios::binary;
    }

    mHandle.open( mName, lMode );
    if ( !mHandle.is_open() )
    {
        return std::make_pair( IFileAccessFile::Failure, 0 );
    }

    // Update file size.
    mHandle.seekg( 0, std::ios_base::end );
    SetSize( static_cast<uint32_t>( mHandle.tellg() ) );

    return std::make_pair( IFileAccessFile::Success, 0 );
}


///
/// Close
///

IFileAccessFile::Result DiskFile::Close()
{
    // Already closed?
    if ( mHandle && !mHandle.is_open() )
    {
        return std::make_pair<>( IFileAccessFile::Failure, 0 );
    }

    mHandle.flush();
    mHandle.close();

    return std::make_pair( IFileAccessFile::Success, 0 );
}


///
/// Read
///

IFileAccessFile::Result DiskFile::Read( uint32_t const aReqLen, uint32_t const aOffset, IPvRegister * const aFileAccessBufferReg )
{
    // Fail right away if file not opened.
    if ( ( !mHandle.is_open() ) || ( aOffset >= Size() ) )
    {
        return std::make_pair( IFileAccessFile::Failure, 0 );
    }

    // Adjustement of the requested length, based on file size and current offset.
    // Move file pointer to requested offset.
    mHandle.seekp( aOffset );

    // Adjust requested length to remaining unread portion.
    uint32_t lAdjustedLen = aReqLen;
    if ( ( aOffset + aReqLen ) > Size() )
    {
        lAdjustedLen = Size() - aOffset;
    }

    if ( lAdjustedLen ) {
        // Write the part of the file to read back into FileAccessBuffer.
        mHandle.read( reinterpret_cast<char *>( IFileAccessFile::sTempBuf.data() ), lAdjustedLen );
        auto const lResult = aFileAccessBufferReg->Write( IFileAccessFile::sTempBuf.data(), lAdjustedLen );
        if ( !lResult.IsOK() )
        {
            return std::make_pair( IFileAccessFile::Failure, 0 );
        }
    }

    return std::make_pair( IFileAccessFile::Success, lAdjustedLen );
}


///
/// Write
///

IFileAccessFile::Result DiskFile::Write( uint32_t const aReqLen, uint32_t const aOffset, IPvRegister * const aFileAccessBufferReg )
{
    PVUNREFPARAM( aOffset );

    // Fail right away if file not opened.
    if ( !mHandle.is_open() )
    {
        return std::make_pair( IFileAccessFile::Failure, 0 );
    }

    if ( aReqLen )
    {
        // Read FileAccessBuffer and write to Disk File.
        auto const lResult = aFileAccessBufferReg->Read( IFileAccessFile::sTempBuf.data(), aReqLen );
        if ( !lResult.IsOK() )
        {
            return std::make_pair( IFileAccessFile::Failure, 0 );
        }

        mHandle.write( reinterpret_cast<const char *>( IFileAccessFile::sTempBuf.data() ), aReqLen );
        auto const lAdjustedLen = PadFile( aReqLen );

        uint32_t const lSize = Size();
        SetSize( lSize + lAdjustedLen );
    }

    return std::make_pair( IFileAccessFile::Success, aReqLen );
}


///
/// Delete
///

IFileAccessFile::Result DiskFile::Delete()
{
    // Operation not supported: OS-dependant.
    // For effectively erasing file on disk, sub-class and provide suitable override
    // for this method vs host OS.
    return std::make_pair( IFileAccessFile::Failure, 0 );
}


///
/// Pad the file with "missing" bytes.
///
/// GenICam imposes transfers that are %4 in length.
///

uint32_t DiskFile::PadFile( uint32_t const aReqLen )
{
    uint32_t lAdjustedLen = aReqLen;
    const char lPadChar = mIsBinary ? static_cast<char>( 0x00 ) : ' ';
    switch ( aReqLen % 4 )
    {
    case 1: mHandle.write( &lPadChar, 1 ); lAdjustedLen++; //[[fallthrough]]; C++17
    case 2: mHandle.write( &lPadChar, 1 ); lAdjustedLen++; //[[fallthrough]];
    case 3: mHandle.write( &lPadChar, 1 ); lAdjustedLen++; break;
    default:
        break;
    }

    return lAdjustedLen;
}
