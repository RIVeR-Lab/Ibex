// *****************************************************************************
//
// Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

class PvDeviceEventSink;

///
/// \brief EventHandler class
///
/// Extends PvDeviceEventSink class to handle incoming events.
///

class EventHandler : public PvDeviceEventSink
{
// Construction
public:
    EventHandler() {}

// Implementation
protected:

    void OnEvent( PvDevice *aDevice, uint16_t aEventID, uint16_t aChannel, uint64_t aBlockID, 
        uint64_t aTimestamp, const void *aData, uint32_t aDataLength );
    void OnEventGenICam( PvDevice *aDevice, uint16_t aEventID, uint16_t aChannel, uint64_t aBlockID, 
        uint64_t aTimestamp, PvGenParameterList *aData );
};

