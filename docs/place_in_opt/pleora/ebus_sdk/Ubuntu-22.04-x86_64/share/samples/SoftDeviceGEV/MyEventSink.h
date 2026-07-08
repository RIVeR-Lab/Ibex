// *****************************************************************************
//
// Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvSoftDeviceGEVInterfaces.h>


///
/// \brief Software GigE Vision Device general event sink
///

class MyEventSink
    : public IPvSoftDeviceGEVEventSink
{
public:

    MyEventSink( IPvRegisterEventSink *aRegisterEventSink );
    virtual ~MyEventSink();

    // IPvSoftDeviceGEVEventSink implementation
    void OnApplicationConnect( IPvSoftDeviceGEV *aDevice, const PvString &aIPAddress, uint16_t aPort, PvAccessType aAccessType );
    void OnApplicationDisconnect( IPvSoftDeviceGEV *aDevice );
    void OnControlChannelStart( IPvSoftDeviceGEV *aDevice, const PvString &aMACAddress, const PvString &aIPAddress, const PvString &aMask, const PvString &aGateway, uint16_t aPort );
    void OnControlChannelStop( IPvSoftDeviceGEV *aDevice );
    void OnDeviceResetFull( IPvSoftDeviceGEV *aDevice );
    void OnDeviceResetNetwork( IPvSoftDeviceGEV *aDevice );
    void OnCreateCustomRegisters( IPvSoftDeviceGEV *aDevice, IPvRegisterFactory *aFactory );
    void OnCreateCustomGenApiFeatures( IPvSoftDeviceGEV *aDevice, IPvGenApiFactory *aFactory );

protected:

    void CreateSampleParameters( IPvRegisterMap *aMap, IPvGenApiFactory *aFactory );
    void CreateChunkParameters( IPvGenApiFactory *aFactory );
    void CreateEventParameters( IPvGenApiFactory *aFactory );

private:

    IPvRegisterEventSink *mRegisterEventSink;

};

