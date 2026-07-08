// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "Defines.h"
#include "PvSoftDeviceGEVInterfaces.h"

#include <array>
#include <utility>


///
/// \brief The interface for a file of the GenICam File Access.
///

class IFileAccessFile
{
public:
    enum FileOperationSelector
    {
        FOS_OPEN = 0,
        FOS_CLOSE,
        FOS_READ,
        FOS_WRITE,
        FOS_DELETE,
        FOS_QTY
    };

    enum FileOperationStatus
    {
        Success = 0,
        Failure
    };

    enum FileOpenMode
    {
        FOM_READ = 0,
        FOM_WRITE,
        FOM_READ_WRITE
    };

    explicit IFileAccessFile( const char * const aDisplayName, PvGenAccessMode const aAccessMode )
        : mDisplayName( aDisplayName )
        , mAccessMode( aAccessMode )
	    , mSize( 0 ) {}
	virtual ~IFileAccessFile() {}

    typedef std::pair<FileOperationStatus, uint32_t> Result;

    virtual Result Open( FileOpenMode aMode ) = 0;
    virtual Result Close() = 0;
    virtual Result Read( uint32_t aReqLen, uint32_t aOffset, IPvRegister *aFileAccessBufferReg ) = 0;
    virtual Result Write( uint32_t aReqLen, uint32_t aOffset, IPvRegister* aFileAccessBufferReg ) = 0;
    virtual Result Delete() = 0;

    const std::string &DisplayName() const { return mDisplayName; }
    PvGenAccessMode AccessMode() const { return mAccessMode; }
    uint32_t Size() const { return mSize; }
    uint32_t Offset( uint32_t const aFileOperationSelector ) const { return mOffsets.at( aFileOperationSelector ); }
    uint32_t Length( uint32_t const aFileOperationSelector ) const { return mLengths.at( aFileOperationSelector ); }

    uint32_t OperationStatus( uint32_t const aFileOperationSelector ) const { return mStatus.at( aFileOperationSelector ); }
    uint32_t OperationResult( uint32_t const aFileOperationSelector ) const { return mResults.at( static_cast<uint32_t>( aFileOperationSelector ) ); }

    void SetOffset( uint32_t const aFileOperationSelector, uint32_t const aFileAccessOffset )
    {
        mOffsets.at( aFileOperationSelector ) = aFileAccessOffset;
    }

    void SetLength( uint32_t const aFileOperationSelector, uint32_t const aFileAccessLength )
    {
        mLengths.at( aFileOperationSelector ) = aFileAccessLength;
    }

    void SetOperationStatus( uint32_t const aFileOperationSelector, uint32_t const aFileOperationStatus )
    {
        mStatus.at( aFileOperationSelector ) = aFileOperationStatus;
    }

    void SetOperationResult( uint32_t const aFileOperationSelector, uint32_t const aFileOperationResult )
    {
        mResults.at( aFileOperationSelector ) = aFileOperationResult;
    }

protected:

    void SetSize( uint32_t const aSize ) { mSize = aSize; }

    // [MG] FIND WAY TO SIZE THIS BUFFER APPROPRIATELY.
    // IT SHOULD MATCH THE SIZE OF FileAccessBuffer, CREATED IN FileAccessEventSink.
    static std::array<uint8_t, FILE_BUFFER_SIZE> sTempBuf;

private:

    std::string mDisplayName;

    PvGenAccessMode mAccessMode;
    uint32_t mSize;

    // Values that require buffering while FileAccess protocol is performed.
    // Some values are not buffered for some operations, but it is simpler to
    // allocate them uniformly.
    std::array<uint32_t, static_cast<int>( FOS_QTY )> mOffsets;
    std::array<uint32_t, static_cast<int>( FOS_QTY )> mLengths;
    std::array<uint32_t, static_cast<int>( FOS_QTY )> mStatus;
    std::array<uint32_t, static_cast<int>( FOS_QTY )> mResults;

};
