// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "eBUSPlayerShared.h"
#include "StreamRTP.h"

#include <PvDeviceInfoGEV.h>
#include <PvNetworkAdapter.h>

#include <assert.h>
#include <sstream>

#include "Device.h"


#define DEVICE_IP_ADDRESS ( "DeviceIPAddress" )


#ifdef _AFXDLL
    IMPLEMENT_DYNAMIC( StreamRTP, Stream )
#endif // _AFXDLL


///
/// \brief Constructor
///

StreamRTP::StreamRTP( IMessageSink *aSink )
    : StreamNetwork( aSink )
{
    SetStream( new PvStreamRTP );
    SetPipeline( new PvPipeline( GetStream() ) );
}


///
/// \brief Destructor
///

StreamRTP::~StreamRTP()
{

}


///
/// \brief Opens a PvStreamRTP from a device info
///

PvResult StreamRTP::Open( Setup *aSetup, const PvDeviceInfo *aDeviceInfo, uint16_t aChannel )
{
    PV_LOGINFO( mLogger, "Opening GigE Vision stream: " << aDeviceInfo->GetConnectionID().GetAscii() << " channel " << aChannel );

    const PvDeviceInfoGEV *lDeviceInfo = dynamic_cast<const PvDeviceInfoGEV *>( aDeviceInfo );
    if ( lDeviceInfo == nullptr )
    {
        return PvResult::Code::INVALID_PARAMETER;
    }

    const PvNetworkAdapter *lInterface = dynamic_cast<const PvNetworkAdapter *>( aDeviceInfo->GetInterface() );
    if ( lInterface == nullptr )
    {
        return PvResult::Code::INVALID_PARAMETER;
    }

    std::string lDeviceIP = lDeviceInfo->GetIPAddress().GetAscii();
    std::string lInterfaceIP = lInterface->GetIPAddress( 0 ).GetAscii();

    return Open( aSetup, lDeviceIP, lInterfaceIP, aChannel );
}


///
/// \brief Opens a PvStreamRTP from its IP address
///

PvResult StreamRTP::Open( Setup *aSetup, const std::string &aDeviceIP, const std::string &aLocalIP, uint16_t aChannel )
{
    PVUNREFPARAM( aChannel );

    PvStreamRTP *lStream = dynamic_cast<PvStreamRTP *>( GetStream() );
    assert( lStream != nullptr );

    if ( ( aSetup->GetRole() == Setup::RoleCtrlData ) ||
         ( aSetup->GetRole() == Setup::RoleData ) )
    {
        // Open stream
        if ( aSetup->GetDestination() == Setup::DestinationUnicastAuto )
        {
            PV_LOGINFO( mLogger, "Opening from device " << aDeviceIP.c_str() << " in DestinationUnicastAuto mode" );

            return lStream->Open( aLocalIP.c_str(), 0 );
        }
        else if ( aSetup->GetDestination() == Setup::DestinationUnicastSpecific )
        {
            PV_LOGINFO( mLogger, "Opening from device " << aDeviceIP.c_str() << " in DestinationUnicastSpecific mode on local port " << 
                aSetup->GetUnicastSpecificPort() );

            return lStream->Open( aLocalIP.c_str(), aSetup->GetUnicastSpecificPort() );
        }
        else if ( aSetup->GetDestination() == Setup::DestinationMulticast )
        {
            PV_LOGINFO( mLogger, "Opening from device " << aDeviceIP.c_str() << " in DestinationMulticast mode on group " 
                << aSetup->GetMulticastIP().GetAscii() << ":" << aSetup->GetMulticastPort() );

            return lStream->Open( aSetup->GetMulticastIP(), aSetup->GetMulticastPort(), aLocalIP.c_str() );
        }
        else
        {
            assert( 0 );
        }
    }

    return PvResult::Code::OK;
}


///
/// \brief Opens an RTP TCP stream
///

PvResult StreamRTP::OpenTCP( const std::string &aServerIP, uint16_t aServerPort )
{
    PvStreamRTP *lStream = dynamic_cast<PvStreamRTP *>( GetStream() );
    assert( lStream != nullptr );

    return lStream->OpenTCP( aServerIP.c_str(), aServerPort );
}


///
/// \brief Returns the local port for an opened GEV stream object
///

uint16_t StreamRTP::GetLocalPort()
{
    if ( !IsOpened() )
    {
        return 0;
    }

    PvStreamRTP *lStream = dynamic_cast<PvStreamRTP *>( GetStream() );
    assert( lStream != nullptr );

    return lStream->GetLocalPort();
}


///
/// \brief Returns the local IP address for an opened GEV stream object
///

std::string StreamRTP::GetLocalIPAddress()
{
    if ( !IsOpened() )
    {
        return "0.0.0.0";
    }

    PvStreamRTP *lStream = dynamic_cast<PvStreamRTP *>( GetStream() );
    assert( lStream != nullptr );

    return lStream->GetLocalIPAddress().GetAscii();
}


///
/// \brief Sets image attributes for the RTP stream
///

void StreamRTP::SetImageAttributes( StreamingAttributes &aSA )
{
    if ( !IsOpened() )
    {
        return;
    }

    PvStreamRTP *lStream = static_cast<PvStreamRTP *>( GetStream() );
    if ( aSA.Compression == CompressionH264 )
    {
        lStream->PrepareForH264();
    }
    else
    {
        lStream->PrepareFor( aSA.Width, aSA.Height, aSA.PixelFormat, aSA.PaddingX );
    }
}


///
/// \brief Returns the device IP address for an opened GEV stream object
///

std::string StreamRTP::GetDeviceIPAddress()
{
    if ( !IsOpened() )
    {
        return "0.0.0.0";
    }

    PvGenInteger *lDeviceIPAddress = GetParameters()->GetInteger( DEVICE_IP_ADDRESS );

    int64_t lValue = 0;
    lDeviceIPAddress->GetValue( lValue );

    std::stringstream lSS;
    lSS << ( ( lValue & 0xFF000000 ) >> 24 ) << ".";
    lSS << ( ( lValue & 0x00FF0000 ) >> 16 ) << ".";
    lSS << ( ( lValue & 0x0000FF00 ) >> 8 ) << ".";
    lSS << ( ( lValue & 0x000000FF ) );

    return lSS.str();
}

