// *****************************************************************************
//
// Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "Defines.h"
#include "MyEventSink.h"
#include "Utilities.h"

#include <limits>


///
/// \brief Constructor
///

MyEventSink::MyEventSink( IPvRegisterEventSink *aRegisterEventSink )
    : mRegisterEventSink( aRegisterEventSink )
{
}


///
/// \brief Destructor
///

MyEventSink::~MyEventSink()
{

}


///
/// \brief Application connection notification
///

void MyEventSink::OnApplicationConnect( IPvSoftDeviceGEV *aDevice, const PvString &aIPAddress, uint16_t aPort, PvAccessType aAccessType )
{
    PVUNREFPARAM( aDevice );
    PVUNREFPARAM( aAccessType );
    std::cout << "Application connected from " << aIPAddress.GetAscii() << ":" << aPort << std::endl;
}


///
/// \brief Application disconnection notification
///

void MyEventSink::OnApplicationDisconnect( IPvSoftDeviceGEV *aDevice )
{
    PVUNREFPARAM( aDevice );
    std::cout << "Application disconnected" << std::endl;
}


///
/// \brief Control channel start notification
///

void MyEventSink::OnControlChannelStart( IPvSoftDeviceGEV *aDevice, const PvString &aMACAddress, const PvString &aIPAddress, const PvString &aMask, const PvString &aGateway, uint16_t aPort )
{
    PVUNREFPARAM( aDevice );
    std::cout << "Control channel started on [" << aMACAddress.GetAscii() << "] ";
    std::cout << aIPAddress.GetAscii() << ":" << aPort << " ";
    std::cout << "Mask:" << aMask.GetAscii() << ", ";
    std::cout << "Gateway:" << aGateway.GetAscii() << std::endl;

    DumpRegisters( aDevice->GetRegisterMap() );
}


///
/// \brief Control channel stop notification
///

void MyEventSink::OnControlChannelStop( IPvSoftDeviceGEV *aDevice )
{
    PVUNREFPARAM( aDevice );
    std::cout << "Control channel stopped" << std::endl;
}


///
/// \brief Device reset notification
///

void MyEventSink::OnDeviceResetFull( IPvSoftDeviceGEV *aDevice )
{
    PVUNREFPARAM( aDevice );
    std::cout << "Device reset" << std::endl;
}


///
/// \brief Network reset notification
///

void MyEventSink::OnDeviceResetNetwork( IPvSoftDeviceGEV *aDevice )
{
    PVUNREFPARAM( aDevice );
    std::cout << "Network reset" << std::endl;
}

///
/// \brief Callback used to initiate GenApi features creation
///

void MyEventSink::OnCreateCustomGenApiFeatures( IPvSoftDeviceGEV *aDevice, IPvGenApiFactory *aFactory )
{
    PVUNREFPARAM( aDevice );
    CreateChunkParameters( aFactory );
}


///
/// \brief Shows how to create custom GenApi parameters for chunk data mapping
///

void MyEventSink::CreateChunkParameters( IPvGenApiFactory *aFactory )
{
    // Create GenApi feature used to map the chunk data count field
    aFactory->SetName( CHUNKCOUNTNAME );
    aFactory->SetDescription( CHUNKCOUNTDESCRIPTION );
    aFactory->SetToolTip( CHUNKCOUNTTOOLTIP );
    aFactory->SetCategory( CHUNKCATEGORY );
    aFactory->MapChunk( CHUNKID, 0, 4, PvGenEndiannessLittle );
    aFactory->CreateInteger( NULL, 0, std::numeric_limits<uint32_t>::max() );

    // Create GenApi feature used to map the chunk data time field
    aFactory->SetName( CHUNKTIMENAME );
    aFactory->SetDescription( CHUNKTIMEDESCRIPTION );
    aFactory->SetToolTip( CHUNKTIMETOOLTIP );
    aFactory->SetCategory( CHUNKCATEGORY );
    aFactory->MapChunk( CHUNKID, 4, 32, PvGenEndiannessLittle );
    aFactory->CreateString( NULL );
}
