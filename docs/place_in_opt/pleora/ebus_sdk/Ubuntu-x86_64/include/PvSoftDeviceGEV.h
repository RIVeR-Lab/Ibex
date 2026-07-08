// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVSOFTDEVICEGEV_H__
#define __PVSOFTDEVICEGEV_H__

#include <PvVirtualDeviceLib.h>

#include <PvSoftDeviceGEVInterfaces.h>


namespace PvVirtualDeviceLib
{
    class ISoftDeviceGEV;
    class IFactory;
}


class PV_VIRTUAL_DEVICE_API PvSoftDeviceGEV
    : public IPvSoftDeviceGEV
{
public:

    PvSoftDeviceGEV();
    virtual ~PvSoftDeviceGEV();

	// Configure device info
	IPvSoftDeviceGEVInfo *GetInfo();

	// Setup device streams
	PvResult AddStream( IPvStreamingChannelSource * aSource );

    // Defines supported user set count
    PvResult SetUserSetCount( uint32_t aCount );

    // Enable TCP transport prototype
    PvResult SetTCPTransportEnabled( bool aEnabled );

	// Direct register map access
	IPvRegisterMap *GetRegisterMap();

	// Start/stop device
	PvResult Start( const PvString &aIpAddress );
	PvResult Stop();

	// GVCP statistics
    IPvSoftDeviceGEVStatistics *GetStatistics();

    // Messaging channel event generation
    IPvMessageChannel *GetMessagingChannel();

	// Event sink management
	PvResult RegisterEventSink( IPvSoftDeviceGEVEventSink *aEventSink );
	PvResult UnregisterEventSink( IPvSoftDeviceGEVEventSink *aEventSink );
    
    // For safe register map access
    PvResult LockState();
    PvResult LockState( uint32_t aTimeout );
    void UnlockState();

    // Used to retrieve the GenICam XML (after Start success or failure)
    PvResult GetGenICamXMLFile( PvString &aString ) const;

protected:
    PvSoftDeviceGEV( PvVirtualDeviceLib::IFactory *aFactory );

    PvVirtualDeviceLib::IFactory *GetFactory() const { return mFactory; }
    PvVirtualDeviceLib::ISoftDeviceGEV *GetDevice() const { return mThis; }

    uint32_t GetStreamCount() const;

private:
    PvVirtualDeviceLib::ISoftDeviceGEV *mThis;
    PvVirtualDeviceLib::IFactory *mFactory;

};


#endif
