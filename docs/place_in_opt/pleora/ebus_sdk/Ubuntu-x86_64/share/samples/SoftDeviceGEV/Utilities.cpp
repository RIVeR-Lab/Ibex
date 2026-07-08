// *****************************************************************************
//
// Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "Defines.h"
#include "Utilities.h"

#include <PvSampleUtils.h>


///
/// \brief Utility: writes the current time to a byte arrray
///

void Time2UInt8( uint8_t *aBuffer, size_t aBufferSize )
{
    time_t lRawTime;
    time( &lRawTime );
#ifdef WIN32
    tm lTimeInfo;
    localtime_s( &lTimeInfo, &lRawTime );
    asctime_s( reinterpret_cast<char *>( aBuffer ), aBufferSize, &lTimeInfo );
#else
    strcpy( reinterpret_cast<char *>( aBuffer ), asctime( localtime( &lRawTime ) ) );
#endif
}


///
/// \brief This function shows how to send messaging channel events
///

void FireTestEvents( IPvMessageChannel *aMessageChannel )
{
    if ( aMessageChannel == NULL )
    {
        return;
    }

    static uint32_t lCount = 0;
    static uint64_t lLastEvent = PvGetTickCountMs();
    if ( aMessageChannel->IsOpened() )
    {
        uint64_t lNow = PvGetTickCountMs();
        if ( ( lNow - lLastEvent ) > 2500 )
        {
            uint8_t lData[ 36 ] = { 0 };

            // Add event count
            memcpy( lData, &lCount, sizeof( lCount ) );

            // Add current time (as a string)
            Time2UInt8( lData + 4, sizeof( lData ) - 4 );

            aMessageChannel->FireEvent( EVENTDATAID, lData, sizeof( lData ) );
            aMessageChannel->FireEvent( EVENTID );

            lLastEvent = lNow;
            lCount++;
        }
    }
}

// Shows how to go through the whole register map
void DumpRegisters( IPvRegisterMap *aRegisterMap )
{
    // Locking the register map garantees safe access to the registers
    if ( !aRegisterMap->Lock().IsOK() )
    {
        return;
    }

    size_t lCount = aRegisterMap->GetRegisterCount();
    for ( size_t i = 0; i < lCount; i++ )
    {
        IPvRegister *lRegister = aRegisterMap->GetRegisterByIndex( i );
        std::cout << lRegister->GetName().GetAscii();
        std::cout << " @ 0x" << std::uppercase << std::hex << std::setfill( '0' ) << std::setw( 8 ) << lRegister->GetAddress();
        std::cout << " " << std::dec << lRegister->GetLength() << " bytes";
        std::cout << ( lRegister->IsReadable() ? " {readable}" : "" );
        std::cout << ( lRegister->IsWritable() ? " {writable}" : "" );
        std::cout << std::endl;
    }

    // Always release a lock, failing to do so would deadlock the Software GigE Vision Device
    aRegisterMap->Release();
}
