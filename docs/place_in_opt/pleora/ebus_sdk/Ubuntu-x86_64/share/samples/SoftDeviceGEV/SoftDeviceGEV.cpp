// *****************************************************************************
//
// Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "Defines.h"

#include <PvSampleUtils.h>
#include <PvSoftDeviceGEV.h>
#include <PvBuffer.h>
#include <PvFPSStabilizer.h>
#include <PvSampleTransmitterConfig.h>

#include "Utilities.h"
#include "MySource.h"
#include "MyEventSink.h"
#include "MyRegisterEventSink.h"


PV_INIT_SIGNAL_HANDLER();

#define SOURCE_COUNT ( 4 )
#define USERSET_COUNT ( 2 )


int main( int aCount, const char **aArgs )
{
    PVUNREFPARAM( aCount );
    PVUNREFPARAM( aArgs );

    // Select interface if MAC was not provided
    PvString lInterface = ( aCount > 1 ) ? PvString( aArgs[1] ) : PvSelectInterface();
    if ( lInterface.GetLength() == 0 )
    {
        std::cout << "No interface selected, terminating" << std::endl;
        return -1;
    }

    // Instantiate interface implementations
    MySource lSources[ SOURCE_COUNT ];
    MyRegisterEventSink lRegisterEventSink;
    MyEventSink lEventSink( &lRegisterEventSink );

    // Instantiate the device itself
    PvSoftDeviceGEV lDevice;

    // Set device identify
    IPvSoftDeviceGEVInfo *lInfo = lDevice.GetInfo();
    lInfo->SetModelName( "SoftDeviceGEV" );
    lInfo->SetGenICamXMLVersion( 1, 2, 3 );
    lInfo->SetGenICamXMLGUIDs( "BA07A10F-969E-4900-9B11-EE914F7A5D7F", "F87E8639-DE12-404E-A079-BBFF5FC9D82AA" );

    // Add stream, register event sink
    for ( int i = 0; i < SOURCE_COUNT; i++ )
    {
        lDevice.AddStream( &( lSources[ i ] ) );
    }
    lDevice.RegisterEventSink( &lEventSink );

    // Configure user-set count
    lDevice.SetUserSetCount( USERSET_COUNT );

    // Start device
    const std::string lModelName( lInfo->GetModelName().GetAscii() );
    PvResult lResult = lDevice.Start( lInterface );
    if ( !lResult.IsOK() )
    {
        std::cout << "Error starting " << lModelName << std::endl;
        if ( lResult.GetCode() == PvResult::Code::GENICAM_XML_ERROR )
        {
            std::cout << "The error is possibly in the dynamically generated GenICam XML file:" << std::endl;
            std::cout << lResult.GetDescription().GetAscii() << std::endl;

            PvString lXML;
            lDevice.GetGenICamXMLFile( lXML );
            std::cout << lXML.GetAscii() << std::endl;
        }

        return -1;
    }

    std::cout << lModelName << " started" << std::endl;
    
    // Loop until keyboard hit
    PvFlushKeyboard();
    while ( !PvKbHit() )
    {
        FireTestEvents( lDevice.GetMessagingChannel() );
        PvSleepMs( 100 );
    }

    // Stop device
    lDevice.Stop();
    std::cout << lModelName << " stopped" << std::endl;

    return 0;
}


