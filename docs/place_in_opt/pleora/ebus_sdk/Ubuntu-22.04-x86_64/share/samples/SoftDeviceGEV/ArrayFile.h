// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "IFileAccessFile.h"

#include <memory>


///
/// \brief ArrayFile class
///
/// An example of a binary WO file.
/// File is volatile and will be lost after application power down.
///

class ArrayFile
    : public IFileAccessFile
{
public:

    explicit ArrayFile( const char *aName, uint32_t aMaxFileSize );

    // IFileAccesFile interface.
    Result Open( IFileAccessFile::FileOpenMode aMode ) override;
    Result Close() override;
    Result Read( uint32_t aReqLen, uint32_t aOffset, IPvRegister* aFileAccessBufferReg ) override;
    Result Write( uint32_t aReqLen, uint32_t aOffset, IPvRegister* aFileAccessBufferReg ) override;
    Result Delete() override;

private:

    static const auto sMaxArraySize = 10 * 1024 * 1024;

    uint32_t mMaxSize;
    bool mIsOpened;

    std::unique_ptr<uint8_t[]> mData;

};
