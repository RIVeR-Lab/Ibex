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

#include "FileAccessEventSink.h"
#include "FileAccessRegisterEventSink.h"
#include "ArrayFile.h"
#include "DiskFile.h"

#include "Utilities.h"
#include "MySource.h"
#include "MyEventSink.h"
#include "MyRegisterEventSink.h"
#include "MyUserSetNotify.h"


PV_INIT_SIGNAL_HANDLER();

#define SOURCE_COUNT ( 4 )
#define USERSET_COUNT ( 2 )


/// To retrieve the log file using use GenICam file transfer:
/// First, enable logging, and change LOG_FILE to your log file name.
/// Then uncomment the line '#define TRANSFER_LOG_FILE'.
///
//#define LOG_FILE ( "C:\\Users\\username\\AppData\\Roaming\\example.log" )
#undef  TRANSFER_LOG_FILE
//#define TRANSFER_LOG_FILE


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
    MyUserSetNotify lUserSetNotify;

    // Instantiate the device itself
    PvSoftDeviceGEV lDevice;

    // GenICam File Access.
    auto lFileAccessRegisterEventSink =
		std::unique_ptr<FileAccessRegisterEventSink>( new FileAccessRegisterEventSink( FILE_SELECTOR_ADDR, lDevice.GetRegisterMap() ) );
    // The two files managed by this object.
    /// FileSelector:
    /// 0: Large Binary File: a virtual file stored in a large array: WO
    /// 1: Disk file: a file that is located on disk storage: RW.
    /// The binary file allocates 5MB of heap storage upon opening it.
    /// Failure to do so will result in an operation failure.
    /// The buffer is freed on file close.
    /// Files larger than 5MB will result in write failure.
    ///
    /// The disk file has no space limit besides the disk capacity,
    /// and the max value of a uint32_t to hold the size.
    /// It is open in binary mode.
    /// For the disk file, transfer time and average speed are provided.
    /// The transfer time and average transfer speed is calculated from open
    /// to close operations.
    ///
    /// Some results writing the disk file:
    /// Size (B):         Time (ms):               Speed (B/s):
    ///                   IRegister/IString        IRegister/IString
    /// 3725648           14365/16159              259342/230557
    /// 7984027           30514/35271              261647/226362
    /// 31549907          132308/137230            238457/229204
    ///
    /// *****************************************************************
    /// *** For experiment:                                           ***
    /// *** FileAccessBuffer can be changed from IRegister to IString ***
    /// *** in file FileAccessEventSink.cpp                           ***
    /// *** Statistically, both led to similar results.               ***
    /// *****************************************************************
    ///
    /// All these files can be manipulated with eBUS Player's File Transfer:
    /// (menu Tools -> File Transfer...)
    auto lArrayFile = std::unique_ptr<ArrayFile>( new ArrayFile( "TheNewWOArrayFile", 5 * 1024 * 1024 ) );
    lFileAccessRegisterEventSink->AddFile( std::move( lArrayFile ) );
    static const auto sIsBinaryFile = true;
    auto lDiskFile = std::unique_ptr<DiskFile>( new DiskFile( "TheNewRWDiskFile", PvGenAccessModeReadWrite, sIsBinaryFile ) );
    lFileAccessRegisterEventSink->AddFile( std::move( lDiskFile ) );
#ifdef TRANSFER_LOG_FILE
    auto lLogFile = std::unique_ptr<DiskFile>( new DiskFile( LOG_FILE, PvGenAccessModeReadOnly, false ) );
    lFileAccessRegisterEventSink->AddFile( std::move( lLogFile ) );
#endif
    FileAccessEventSink lFileAccessEventSink( std::move( lFileAccessRegisterEventSink ) );

    // Set device identify
    IPvSoftDeviceGEVInfo *lInfo = lDevice.GetInfo();
    lInfo->SetModelName( "SoftDeviceGEV" );
    lInfo->SetGenICamXMLRegDescModelName( "RegDescSoftDeviceGEV" );
    lInfo->SetGenICamXMLRegDescVendorName( "RegDescPleoraTech" );
    lInfo->SetDeviceFirmwareVersion( "a.test.of.firmware.version" );
    lInfo->SetGenICamXMLVersion( 1, 2, 3 );
    lInfo->SetGenICamXMLGUIDs( "BA07A10F-969E-4900-9B11-EE914F7A5D7F", "F87E8639-DE12-404E-A079-BBFF5FC9D82AA" );

    // Add stream, register event sink
    for ( int i = 0; i < SOURCE_COUNT; i++ )
    {
        lDevice.AddStream( &( lSources[ i ] ) );
    }
    lDevice.RegisterEventSink( &lEventSink );
    lDevice.RegisterEventSink( &lFileAccessEventSink );

    // Configure user-set count
    lDevice.SetUserSetCount( USERSET_COUNT );
    lDevice.SetUserSetNotify( &lUserSetNotify );

    // Support RTP
    lDevice.SetRTPProtocolEnabled( true );

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


