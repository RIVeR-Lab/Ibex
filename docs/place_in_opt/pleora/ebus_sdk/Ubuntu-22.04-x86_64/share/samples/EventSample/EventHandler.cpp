// *****************************************************************************
//
// Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include <PvSampleUtils.h>
#include <PvDevice.h>

#include "EventHandler.h"

///
/// OnEvent
///

void EventHandler::OnEvent( PvDevice *aDevice, uint16_t aEventID, uint16_t aChannel, uint64_t aBlockID, 
    uint64_t aTimestamp, const void *aData, uint32_t aDataLength )
{
    PVUNREFPARAM( aDevice );
    PVUNREFPARAM( aData );

    cout << endl << "OnEvent Callback : ";
    cout << endl << "Received Event ID 0x" << std::hex << aEventID << "    Timestamp " << std::dec << aTimestamp << endl;
    cout << "Channel 0x" << std::hex << aChannel << "    Block ID " << std::dec << aBlockID << "    Data Length " << std::dec << aDataLength << endl;
}


///
/// OnEventGenICam
///

void EventHandler::OnEventGenICam( PvDevice *aDevice, uint16_t aEventID, uint16_t aChannel, uint64_t aBlockID,
        uint64_t aTimestamp, PvGenParameterList *aData )
{
    PVUNREFPARAM( aDevice );

    if ( aData != NULL )
    {

        cout << endl << "OnEventGenICam Callback : ";
        cout << endl << "Received Event ID 0x" << std::hex << aEventID << "    Timestamp " << std::dec << aTimestamp << endl;
        cout << "Channel 0x" << std::hex << aChannel << "    Block ID " << std::dec << aBlockID;
        
        PvGenParameter *lParameter = aData->GetFirst();
        while ( lParameter != NULL )
        {
            cout << endl << "Parameter " << lParameter->GetName().GetAscii();
            int64_t lDataLength = 0;
            PvGenRegister *lRegister = dynamic_cast<PvGenRegister *>( lParameter );
            if ( lRegister == NULL )
            {
                cout << "    value: " << lParameter->ToString().GetAscii();
            }
            else
            {
                lRegister->GetLength( lDataLength );

                uint8_t *lData = new uint8_t[ static_cast<uint32_t>( lDataLength ) ];

                PvResult lResult = lRegister->Get( lData, static_cast<uint32_t>( lDataLength ) );
                cout << "    Data Length: " << std::dec << lDataLength;

                delete []lData;
                lData = NULL;
            } 
            lParameter = aData->GetNext();
        }
    }
}
