// *****************************************************************************
//
// Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "IFileAccessFile.h"

#include <PvSoftDeviceGEVInterfaces.h>

#include <memory>
#include <vector>


///
/// \brief Interface for IPvRegisterEventSink related to GenICam FileAccess.
/// 

class IFileAccessRegisterEventSink
    : public IPvRegisterEventSink
{
public:
    typedef std::unique_ptr<IFileAccessFile> file_ptr;
    typedef std::vector<file_ptr> files_t;
    typedef files_t::const_iterator const_iterator;

    virtual ~IFileAccessRegisterEventSink() {}

    virtual uint32_t FileSelectorAddr() const = 0;
    virtual uint32_t FileOperationSelectorAddr() const = 0;
    virtual uint32_t FileOperationExecuteAddr() const = 0;
    virtual uint32_t FileOpenModeAddr() const = 0;
    virtual uint32_t FileAccessBufferAddr() const = 0;
    virtual uint32_t FileAccessBufferSize() const = 0;
    virtual uint32_t FileAccessOffsetAddr() const = 0;
    virtual uint32_t FileAccessLengthAddr() const = 0;
    virtual uint32_t FileOperationStatusAddr() const = 0;
    virtual uint32_t FileOperationResultAddr() const = 0;
    virtual uint32_t FileSizeAddr() const = 0;

    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

    virtual void AddFile( file_ptr aFile ) = 0;

};
