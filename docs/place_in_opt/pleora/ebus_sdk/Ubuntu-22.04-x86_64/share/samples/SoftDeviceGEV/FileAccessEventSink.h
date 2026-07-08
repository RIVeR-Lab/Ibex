// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "IFileAccessRegisterEventSink.h"

#include <memory>
#include <string>
#include <vector>


///
/// \brief File access event sink.
/// 
/// Creates the registers and Gen Api nodes required for the File Access,
/// based on the list of files added to the object.
/// 
/// Object of this class manages the life of the IPvRegisterEventSink pointer passed to it.
/// 

class FileAccessEventSink
    : public IPvSoftDeviceGEVEventSink
{
public:

    explicit FileAccessEventSink( std::unique_ptr<IFileAccessRegisterEventSink> aRegisterEventSink )
        : mRegisterEventSink( std::move( aRegisterEventSink ) ) {}

    // IPvSoftDeviceGEVEventSink implementation
    void OnCreateCustomRegisters( IPvSoftDeviceGEV* aDevice, IPvRegisterFactory* aFactory ) override;
    void OnCreateCustomGenApiFeatures( IPvSoftDeviceGEV* aDevice, IPvGenApiFactory* aFactory ) override;

private:

    void AddFileSelector( IPvRegisterMap *aMap, IPvGenApiFactory *aFactory );
    void AddFileOperationSelector( IPvRegisterMap *aMap, IPvGenApiFactory *aFactory );
    void AddFileOperationExecute( IPvRegisterMap *aMap, IPvGenApiFactory *aFactory );
    void AddFileOpenMode( IPvRegisterMap *aMap, IPvGenApiFactory *aFactory );
    void AddFileAccessBuffer( IPvRegisterMap *aMap, IPvGenApiFactory *aFactory );
    void AddFileAccessOffset( IPvRegisterMap *aMap, IPvGenApiFactory *aFactory );
    void AddFileOperationStatus( IPvRegisterMap *aMap, IPvGenApiFactory *aFactory );
    void AddFileOperationResult( IPvRegisterMap *aMap, IPvGenApiFactory *aFactory );
    void AddFileSize( IPvRegisterMap *aMap, IPvGenApiFactory *aFactory );

    std::unique_ptr<IFileAccessRegisterEventSink> mRegisterEventSink;
};
