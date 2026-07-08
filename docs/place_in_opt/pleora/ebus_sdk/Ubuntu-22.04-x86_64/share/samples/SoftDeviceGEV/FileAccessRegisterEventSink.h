// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "IFileAccessRegisterEventSink.h"
#include "IFileAccessFile.h"

#include <fstream>
#include <istream>
#include <string>


///
/// \brief An implementation example of the GenICam File Transfer using 
/// IPvRegisterEventSink class.
///
/// This class uses the uses the features of the File Access Control from the
/// GenICam SFNC v2.7, section 2.16.
///
/// It allows easy file manipulation with any host implementing the counterpart.
///
/// Files must be added to create an appropriate selector in the
/// appropriate GenICam feature.
///
/// This class owns the files created and added through its interface.
///

class FileAccessRegisterEventSink
    : public IFileAccessRegisterEventSink
{
public:
    explicit FileAccessRegisterEventSink( uint32_t aRegisterBaseAddress, IPvRegisterMap* aRegisterMap )
        : mRegisterBaseAddress( aRegisterBaseAddress )
        , mRegisterMap( aRegisterMap ) {}

    // IPvRegisterEventSink interface
    PvResult PreRead( IPvRegister* aRegister ) override;
    void PostRead( IPvRegister* aRegister ) override;
    PvResult PreWrite( IPvRegister* aRegister ) override;
    void PostWrite( IPvRegister* aRegister ) override;
    //PvResult Persist( IPvRegister* aRegister, IPvRegisterStore* aStore ) override;

    // IFileAccessRegisterEventSink interface
    uint32_t FileSelectorAddr() const override { return mRegisterBaseAddress; }
    uint32_t FileOperationSelectorAddr() const override { return mRegisterBaseAddress + 0x4; }
    uint32_t FileOperationExecuteAddr() const override { return mRegisterBaseAddress + 0x8; }
    uint32_t FileOpenModeAddr() const override { return mRegisterBaseAddress + 0xC; }
    uint32_t FileAccessBufferAddr() const override { return mRegisterBaseAddress + 0x10; }
    uint32_t FileAccessBufferSize() const override { return FILE_BUFFER_SIZE; }
    uint32_t FileAccessOffsetAddr() const override { return mRegisterBaseAddress + 0x10 + FILE_BUFFER_SIZE; }
    uint32_t FileAccessLengthAddr() const override { return mRegisterBaseAddress + 0x14 + FILE_BUFFER_SIZE; }
    uint32_t FileOperationStatusAddr() const override { return mRegisterBaseAddress + 0x18 + FILE_BUFFER_SIZE; }
    uint32_t FileOperationResultAddr() const override { return mRegisterBaseAddress + 0x1C + FILE_BUFFER_SIZE; }
    uint32_t FileSizeAddr() const override { return mRegisterBaseAddress + 0x20 + FILE_BUFFER_SIZE; }

    const_iterator cbegin() const override { return mFiles.cbegin(); }
    const_iterator cend() const override { return mFiles.cend(); }

    void AddFile( file_ptr aFile );

private:

    uint32_t ReadFileSelector() const;
    uint32_t ReadFileAccesLength() const;
    uint32_t ReadFileAccessOffset() const;
    uint32_t ReadFileOpenMode() const;
    uint32_t ReadFileOperationSelector() const;
    uint32_t ReadFileSize() const;
    void WriteFileOpenMode( IFileAccessFile::FileOpenMode aFileOpenMode ) const;
    void WriteFileOperationStatus( IFileAccessFile::FileOperationStatus aFileOperationStatus ) const;
    void WriteFileOperationResult( uint32_t aResult ) const;
    void WriteFileSize( uint32_t aFileSize ) const;

    uint32_t mRegisterBaseAddress;
    IPvRegisterMap* mRegisterMap;

    std::vector<file_ptr> mFiles;

};
