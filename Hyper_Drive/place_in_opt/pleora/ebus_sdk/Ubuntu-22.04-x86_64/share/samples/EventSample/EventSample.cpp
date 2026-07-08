// *****************************************************************************
//
//      Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

//
// Shows how to use a PvDevice object to handle events from a GigE Vision or
// USB3 Vision device.
//

#include <PvSampleUtils.h>
#include <PvDevice.h>
#include <PvDeviceGEV.h>
#include <PvDeviceU3V.h>

#include "EventHandler.h"

PV_INIT_SIGNAL_HANDLER();

///
/// Function Prototypes
///
PvDevice *ConnectToDevice( const PvString &aConnectionID );

//
// Main function
//
int main()
{
    PvDevice *lDevice = NULL;

    PV_SAMPLE_INIT();

    cout << "EventSample:" << endl << endl;

    PvString lConnectionID;
    if ( PvSelectDevice( &lConnectionID ) )
    {
        lDevice = ConnectToDevice( lConnectionID );
        if ( NULL != lDevice )
        {
            EventHandler lEventHandler;
            // Register EventHandler class as event sink. See EventHandler.cpp.
            lDevice->RegisterEventSink( &lEventHandler );

            cout << endl;
            cout << "<press a key to disconnect>" << endl;
            PvWaitForKeyPress();

            lDevice->UnregisterEventSink( &lEventHandler );

            // Disconnect the device
            cout << "Disconnecting device" << endl;
            lDevice->Disconnect();
            PvDevice::Free( lDevice );
        }
    }

    cout << endl;
    cout << "<press a key to exit>" << endl;
    PvWaitForKeyPress();

    PV_SAMPLE_TERMINATE();

    return 0;
}

PvDevice *ConnectToDevice( const PvString &aConnectionID )
{
    PvDevice *lDevice;
    PvResult lResult;

    // Connect to the GigE Vision or USB3 Vision device
    cout << "Connecting to device." << endl;
    lDevice = PvDevice::CreateAndConnect( aConnectionID, &lResult );
    if ( lDevice == NULL )
    {
        cout << "Unable to connect to device: "
        << lResult.GetCodeString().GetAscii()
        << " ("
        << lResult.GetDescription().GetAscii()
        << ")" << endl;
    }

    return lDevice;
}
