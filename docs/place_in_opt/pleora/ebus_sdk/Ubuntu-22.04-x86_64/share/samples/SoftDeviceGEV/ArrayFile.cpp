// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ArrayFile.h"

#include <cstring>


///
/// \brief Constructor
///

ArrayFile::ArrayFile( const char * const aName, uint32_t const aMaxFileSize )
    : IFileAccessFile( aName, PvGenAccessModeWriteOnly )
    , mMaxSize( ( aMaxFileSize <= sMaxArraySize ) ? aMaxFileSize : sMaxArraySize )
    , mIsOpened( false )
{
    mData = std::unique_ptr<uint8_t[]>( new uint8_t [mMaxSize] );
}


///
/// Open
///

IFileAccessFile::Result ArrayFile::Open( IFileAccessFile::FileOpenMode const aMode )
{
    PVUNREFPARAM( aMode );

    // Already opened?
    if ( mIsOpened )
    {
        return std::make_pair( IFileAccessFile::Failure, 0 );
    }

    mIsOpened = true;
    return std::make_pair( IFileAccessFile::Success, 0 );
}


///
/// Close
///

IFileAccessFile::Result ArrayFile::Close()
{
    // Already closed?
    if ( !mIsOpened )
    {
        return std::make_pair<>( IFileAccessFile::Failure, 0 );
    }

    mIsOpened = false;
    return std::make_pair( IFileAccessFile::Success, 0 );
}


///
/// Read
///
/// File is RO: GenICam should prevent this getting called, but return error anyway.
///

IFileAccessFile::Result ArrayFile::Read( uint32_t const aReqLen, uint32_t const aOffset, IPvRegister * const aFileAccessBufferReg )
{
    PVUNREFPARAM(aReqLen);
    PVUNREFPARAM(aOffset);
    PVUNREFPARAM( aFileAccessBufferReg );

    return std::make_pair( IFileAccessFile::Failure, 0 );
}


///
/// Write
///

IFileAccessFile::Result ArrayFile::Write( uint32_t const aReqLen, uint32_t const aOffset, IPvRegister * const aFileAccessBufferReg )
{
    // Fail right away if file not opened.
    if ( ( !mIsOpened ) || ( aOffset >= mMaxSize ) )
    {
        return std::make_pair( IFileAccessFile::Failure, 0 );
    }

    // Adjustement of the requested length, based on file size and current offset.
    uint32_t lAdjustedLen = aReqLen;
    if ( ( aOffset + aReqLen ) > mMaxSize )
    {
        // Adjust requested length to remaining unread portion.
        lAdjustedLen = mMaxSize - aOffset;
    }

    if ( lAdjustedLen )
    {
        // Read FileAccessBuffer and write to data vector.
        aFileAccessBufferReg->Read( IFileAccessFile::sTempBuf.data(), lAdjustedLen );
        memcpy( &mData[ aOffset ], IFileAccessFile::sTempBuf.data(), lAdjustedLen );

        uint32_t const lSize = Size();
        SetSize( lSize + aReqLen );
    }

    return std::make_pair( IFileAccessFile::Success, aReqLen );
}


///
/// Delete
///

IFileAccessFile::Result ArrayFile::Delete()
{
    // Simulate some kind of delete.
    // Set the size back to 0. No need to clear the whole array.
    SetSize( 0 );
    return std::make_pair( IFileAccessFile::Success, 0 );
}
