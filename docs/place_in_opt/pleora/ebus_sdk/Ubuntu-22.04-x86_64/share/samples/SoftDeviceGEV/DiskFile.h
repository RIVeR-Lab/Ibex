// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "IFileAccessFile.h"

#include <fstream>
#include <string>


///
/// \brief DiskFile class
///
/// An example of a disk file, accessible from disk drive.
///

class DiskFile
    : public IFileAccessFile
{
public:

    explicit DiskFile( const char *aName, PvGenAccessMode aMode, bool aIsBinary )
        : IFileAccessFile( aName, aMode ), mName( aName ), mIsBinary( aIsBinary ) {}

    // IFileAccesFile interface.
    Result Open( IFileAccessFile::FileOpenMode aMode ) override;
    Result Close() override;
    Result Read( uint32_t aReqLen, uint32_t aOffset, IPvRegister* aFileAccessBufferReg ) override;
    Result Write( uint32_t aReqLen, uint32_t aOffset, IPvRegister* aFileAccessBufferReg ) override;
    Result Delete() override;

protected:

    bool IsBinary() const { return mIsBinary; }

private:

    uint32_t PadFile( uint32_t aReqLen );

    std::string mName;
    
    bool mIsBinary;

    std::fstream mHandle;
};