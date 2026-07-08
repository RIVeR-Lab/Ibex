// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

//
// This sample shows how to find GEV Devices, U3V and Pleora protocol device on a network.
//

#include <PvSampleUtils.h>
#include <PvInterface.h>
#include <PvDevice.h>


PV_INIT_SIGNAL_HANDLER();


//
// To find GEV, U3V and Pleora protocol devices on a network.
//

int DeviceFinding()
{
    PvResult lResult;
    const PvDeviceInfo* lLastDeviceInfo = NULL;

    // Find all devices on the network.
    PvSystem lSystem;
    lResult = lSystem.Find();
    if ( !lResult.IsOK() )
    {
        cout << "PvSystem::Find Error: " << lResult.GetCodeString().GetAscii();
        return -1;
    }

    // Go through all interfaces 
    uint32_t lInterfaceCount = lSystem.GetInterfaceCount();
    for ( uint32_t x = 0; x < lInterfaceCount; x++ )
    {
        cout << "Interface " << x << endl;

        // Get pointer to the interface.
        const PvInterface* lInterface = lSystem.GetInterface( x );

        // Is it a PvNetworkAdapter?
        const PvNetworkAdapter* lNIC = dynamic_cast<const PvNetworkAdapter*>( lInterface );
        if ( lNIC != NULL )
        {
            cout << "  MAC Address: " << lNIC->GetMACAddress().GetAscii() << endl;

            uint32_t lIPCount = lNIC->GetIPAddressCount();
            for ( uint32_t i = 0; i < lIPCount; i++ )
            {
                cout << "  IP Address " << i << ": " << lNIC->GetIPAddress( i ).GetAscii() << endl;
                cout << "  Subnet Mask " << i << ": " << lNIC->GetSubnetMask( i ).GetAscii() << endl << endl;
            }
        }

        // Is it a PvUSBHostController?
        const PvUSBHostController* lUSB = dynamic_cast<const PvUSBHostController*>( lInterface );
        if ( lUSB != NULL )
        {
            cout << "  Name: " << lUSB->GetName().GetAscii() << endl << endl;
        }

        // Go through all the devices attached to the interface
        uint32_t lDeviceCount = lInterface->GetDeviceCount();
        for ( uint32_t y = 0; y < lDeviceCount ; y++ )
        {
            const PvDeviceInfo *lDeviceInfo = lInterface->GetDeviceInfo( y );

            cout << "  Device " << y << endl;
            cout << "    Display ID: " << lDeviceInfo->GetDisplayID().GetAscii() << endl;
            
            const PvDeviceInfoGEV* lDeviceInfoGEV = dynamic_cast<const PvDeviceInfoGEV*>( lDeviceInfo );
            const PvDeviceInfoU3V *lDeviceInfoU3V = dynamic_cast<const PvDeviceInfoU3V *>( lDeviceInfo );
            const PvDeviceInfoUSB *lDeviceInfoUSB = dynamic_cast<const PvDeviceInfoUSB *>( lDeviceInfo );
            const PvDeviceInfoPleoraProtocol* lDeviceInfoPleora = dynamic_cast<const PvDeviceInfoPleoraProtocol*>( lDeviceInfo );

            if ( lDeviceInfoGEV != NULL ) // Is it a GigE Vision device?
            {
                cout << "    MAC Address: " << lDeviceInfoGEV->GetMACAddress().GetAscii() << endl;
                cout << "    IP Address: " << lDeviceInfoGEV->GetIPAddress().GetAscii() << endl;
                cout << "    Serial number: " << lDeviceInfoGEV->GetSerialNumber().GetAscii() << endl << endl;
                lLastDeviceInfo = lDeviceInfo;
            }
            else if ( lDeviceInfoU3V != NULL ) // Is it a USB3 Vision device?
            {
                cout << "    GUID: " << lDeviceInfoU3V->GetDeviceGUID().GetAscii() << endl;
                cout << "    S/N: " << lDeviceInfoU3V->GetSerialNumber().GetAscii() << endl;
                cout << "    Speed: " << lUSB->GetSpeed() << endl << endl;
                lLastDeviceInfo = lDeviceInfo;
            }
            else if ( lDeviceInfoUSB != NULL ) // Is it an unidentified USB device?
            {
                cout << endl;
            }
            else if ( lDeviceInfoPleora != NULL ) // Is it a Pleora Protocol device?
            {
                cout << "    MAC Address: " << lDeviceInfoPleora->GetMACAddress().GetAscii() << endl;
                cout << "    IP Address: " << lDeviceInfoPleora->GetIPAddress().GetAscii() << endl;
                cout << "    Serial number: " << lDeviceInfoPleora->GetSerialNumber().GetAscii() << endl << endl;
            }
        }
    }

    // Connect to the last device found
    if ( lLastDeviceInfo != NULL )
    {
        cout << "Connecting to " << lLastDeviceInfo->GetDisplayID().GetAscii() << endl;

        // Creates and connects the device controller based on the selected device.
        PvDevice* lDevice = PvDevice::CreateAndConnect( lLastDeviceInfo, &lResult );
        if ( !lResult.IsOK() )
        {
            cout << "Unable to connect to " << lLastDeviceInfo->GetDisplayID().GetAscii() << endl;
        }
        else
        {
            cout << "Successfully connected to " << lLastDeviceInfo->GetDisplayID().GetAscii() << endl;
            cout << "Disconnecting the device "  << lLastDeviceInfo->GetDisplayID().GetAscii() << endl;
            PvDevice::Free( lDevice );
        }
    }
    else
    {
        cout << "No device found" << endl;
    }

    return 0;
}


//
// Main function.
//

int main()
{
    PV_SAMPLE_INIT();

    cout << "DeviceFinder sample" << endl << endl;
    // Find devices.
    cout << "1. Find devices" << endl << endl;
    DeviceFinding();

    cout << endl;
    cout << "<press a key to exit>" << endl;
    PvWaitForKeyPress();

    PV_SAMPLE_TERMINATE();

    return 0;
}

