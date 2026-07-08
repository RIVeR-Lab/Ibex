// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __IPVDEVICEEVENTSINK_H__
#define __IPVDEVICEEVENTSINK_H__

#include <PvDeviceLib.h>
#include <PvStringList.h>

///
/// \class IPvDeviceEventSink
/// \brief Interface used by a class implementing IPvDeviceAdapter to forward notifications to PvDeviceSerialPort.
///
/// This interface is implemented classes internal to PvDeviceSerialPort.
///
/// You need to forward GenICam invalidation events for all IPvDeviceEventSink interfaces
/// registered using RegisterEventSink up until UnregisterEventSink is called.
///
/// Use IPvDeviceEventSink::NotifyInvalidatedGenParameter for parameters that RegisterGenInvalidator was called 
/// for. You also need to forward all messaging channel events to the PvDeviceSerialPort using 
/// IPvDeviceEventSink::NotifyEvent.
/// 
class IPvDeviceEventSink
{
public: 

    ///
    /// \brief Forwards a controller event (GigE Vision or USB3 Vision) to a registered interface.
    ///
    /// \param[in] aEventID    Event ID. Some standard event IDs are defined but usually device dependent.
    /// \param[in] aChannel    Streaming channel associated with the event, if any. 0xFFFF is used if the event is not associated to a streaming channel.
    /// \param[in] aBlockID    Streaming block ID associated with the event, if any. Can be used to pair one or more than one event with a block received from a device. 0 is used if the event is not associated to a block.
    /// \param[in] aTimestamp  Timestamp of the event on the time domain of the device. Time unit is defined by the device. 0 if timestamps are not supported by the device that generated the event.
    /// \param[in] aData       Pointer to Event Data, if applicable. NULL otherwise.
    /// \param[in] aDataLength Data length, in bytes.  
    ///
    virtual void NotifyEvent( 
        uint16_t aEventID, uint16_t aChannel, uint64_t aBlockID, uint64_t aTimestamp, 
        const void *aData, uint32_t aDataLength ) = 0;

    ///
    /// \brief Forwards a GenICam parameter invalidation event to a registered interface.
    ///
    /// \param[in] aParameterName Name of the invalidated parameter.
    ///
    virtual void NotifyInvalidatedGenParameter( const PvString &aParameterName ) = 0;

};

#endif
