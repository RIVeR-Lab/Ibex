// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDEVICEEVENTSINK_H__
#define __PVDEVICEEVENTSINK_H__

#include <PvDevice.h>
#include <PvGenParameterList.h>


class PvDevice;

///
/// \class PvDeviceEventSink
/// \brief Observer interface for an observable PvDevice object
///
/// #PvDeviceEventSink defines a set of callback methods from
/// a PvDevice object to any object implementing this interface.
/// 
/// <h2>GigE Vision Recovery</h2>
///
/// A #PvDeviceGEV object continually sends a heartbeat message every <a href="devicecommunicationGEV.html#HeartbeatInterval">HeartbeatInterval</a> milliseconds to the connected GigE Vision device 
/// to maintain its control channel privilege and ensure that the two entities are aware of each other's continued existence. 
///
/// If the GigE Vision device fails to receive a heartbeat message after a given period of time 
/// (see the GevHeartbeatTimeout GigE Vision parameter and <a href="devicecommunicationGEV.html#DefaultHeartbeatTimeout">DefaultHeartbeatTimeout</a>),
/// it loses its control channel privilege so that another controller may connect to the device in the event of a connection loss.
///
/// Each heartbeat sent by the #PvDeviceGEV object is acknowledged by the device.
/// If the GigE Vision device fails to respond to the heartbeat (within <a href="devicecommunicationGEV.html#AnswerTimeout">AnswerTimeout</a> milliseconds) after <a href="devicecommunicationGEV.html#CommandRetryCount">CommandRetryCount</a> retries, it considers itself disconnected
/// and notifies any registered #PvDeviceEventSink objects using the #OnLinkDisconnected callback.
///
/// Device recovery (or reconnection) must be performed at the application level. After a #OnLinkDisconnected callback, the current session should
/// be cleaned up (disconnecting or deleting your PvDevice controller, closing your PvStream, etc.) and you should start polling with FindDevice from PvSystem
/// to see when the device is available again. When the device is available, reconnect and restore any previously known good application
/// configuration.
///
/// For a complete working example of how to initiate recovery and continue streaming in the event of a connection loss, refer to the ConnectionRecovery sample.
///
/// <h2>USB3 Vision Recovery</h2>
///
/// A #PvDeviceU3V object continually sends a heartbeat message every <a href="devicecommunicationU3V.html#HeartbeatInterval">HeartbeatInterval</a> milliseconds to the connected USB3 Vision device 
/// to ensure that the two entities are aware of each other's continued existence. 
///
/// If a #PvDeviceU3V fails to respond to the heartbeat (within <a href="devicecommunicationU3V.html#AnswerTimeout">AnswerTimeout</a> milliseconds), it considers itself disconnected
/// and notifies any registered #PvDeviceEventSink objects using the #OnLinkDisconnected callback.
/// 
/// USB3 Vision devices do not require the heartbeat. There is no concept of control channel or even a heartbeat being required in order
/// to keep the control channel opened. The heartbeat is only used for the host to know that the device is still connected and responsive.
///
/// See GigE Vision section above for instructions on how to perform application level recovery after a disconnect event.
///
/// <h2>Messaging channel events</h2>
///
/// <b>To receive raw events notifications:</b>
///     - Create an instance of your MySink class.
///     - Connect to the GigE Vision device as you normally would. See #PvDevice.
///     - Register your MySink class with the #PvDevice object. Use #PvDevice::RegisterEventSink.
///     - Control the GigE Vision device as you normally would. See #PvDevice.
///     - Handle events when #PvDevice calls your #OnEvent overridden method.
///
class PV_DEVICE_API PvDeviceEventSink
{
public:
    ///
    /// \brief Constructor.
    ///
    PvDeviceEventSink();

    ///
    /// \brief Destructor.
    ///
    virtual ~PvDeviceEventSink();

    ///
    /// \brief Notification that the #PvDevice unexpectedly lost contact with the device.
    ///
    /// \param[in] aDevice Pointer to the device object that invoked this callback.
    ///
    virtual void OnLinkDisconnected( PvDevice *aDevice );

    ///
    /// \brief Deprecated, no longer used.
    ///
    /// PvDevice no longer attempts automatic reconnection. After receiving an #OnLinkDisconnected notification,
    /// it is recommend to properly complete disconnection at the application level and then either attempting 
    /// reconnection until it  succeeds or trying find the device in a loop with #PvSystem::FindDevice until it 
    /// succeeds and then finally reconnecting PvDevice, re-opening PvStream, etc.
    ///
    /// \param[in] aDevice Pointer to the device object that invoked this callback.
    ///
    virtual void OnLinkReconnected( PvDevice *aDevice );

    ///
    /// \brief Messaging channel raw event notification.
    ///
    /// This #OnEvent handler lets users access the raw GigE Vision or USB3 Vision event before
    /// it is forwarded to the GenApi layer. 
    ///
    /// \note This callback is an advanced feature that should only be used by 
    /// developers who are aware of the event IDs that are being sent from their device. It is
    /// recommended to use OnEventGenICam whenever possible.
    ///
    /// Depending on the nature of the event being received, there may be data returned that is pointed to by aData (otherwise aData is NULL).
    /// This additional data is received on the wire in the device's Endianess.
    ///
    /// All other parameters are provided in the Endianess of the host.
    ///
    /// \param[in] aDevice     Pointer to the PvDevice object that triggered the callback.
    /// \param[in] aEventID    Event ID. Some standard event IDs are defined but usually device dependent.
    /// \param[in] aChannel    Streaming channel associated with the event, if any. 0xFFFF is used if the event is not associated to a streaming channel.
    /// \param[in] aBlockID    Streaming block ID associated with the event, if any. Can be used to pair one or more than one event with a block received from a device. 0 is used if the event is not associated to a block.
    /// \param[in] aTimestamp  Timestamp of the event on the time domain of the device. Time unit is defined by the device. 0 if timestamps are not supported by the device that generated the event.
    /// \param[in] aData       Pointer to Event Data, if applicable. NULL otherwise.
    /// \param[in] aDataLength Data length, in bytes.  
    ///
    virtual void OnEvent( PvDevice *aDevice, 
        uint16_t aEventID, uint16_t aChannel, uint64_t aBlockID, uint64_t aTimestamp, 
        const void *aData, uint32_t aDataLength );

    ///
    /// \brief Messaging channel GenICam event notification.
    ///
    /// This #OnEvent handler lets users access the event as
    /// mapped through the GenApi interface of the device.
    ///
    /// Unlike the other OnEvent that provides a pointer to the raw event data,
    /// the data is provided as a list of GenApi parameters that have been invalidated
    /// when the event has been attached to the GenApi node map of the device.
    ///
    /// If events with no data or events with data not mapped in the GenApi interface
    /// of the device are received, this callback is still invoked by the parameter
    /// list containing is not provided, it is simply set to NULL.
    ///
    /// \param[in] aDevice     Pointer to the PvDevice object that triggered the callback.
    /// \param[in] aEventID    Event ID. Some standard event IDs are defined but usually device dependent.
    /// \param[in] aChannel    Streaming channel associated with the event, if any. 0xFFFF is used if the event is not associated to a streaming channel.
    /// \param[in] aBlockID    Streaming block ID associated with the event, if any. Can be used to pair one or more than one event with a block received from a device. 0 is used if the event is not associated to a block.
    /// \param[in] aTimestamp  Timestamp of the event on the time domain of the device. Time unit is defined by the device. 0 if timestamps are not supported by the device that generated the event.
    /// \param[in] aData       List containing all parameters invalidated by this event. Can be NULL if the event carried no data or the data was does not invalidate any GenApi parameters of the device.
    ///
    virtual void OnEventGenICam( PvDevice *aDevice,
        uint16_t aEventID, uint16_t aChannel, uint64_t aBlockID, uint64_t aTimestamp,
        PvGenParameterList *aData );


	///
    /// \brief Callback invoked after a port read.
    ///
    /// Used to report read activity  on the GenApi IPort implementation of the command link of the PvDevice.
    ///
    /// The callback is invoked AFTER the read operation so the content of aBuffer
    /// contains the data that was read from the device.
    ///
    /// This callback is only invoked on a successful read operation.
    ///
    /// If aLength is 4 bytes, that data is represented as a 4 bytes value formatted as big endian.
    /// If data is more that 4 bytes, it is presented as a stream of bytes requiring
    /// proper interpretation.
    ///
    /// \param[in] aBuffer  Pointer to the data read.
    /// \param[in] aAddress Address of the data read on the device.
    /// \param[in] aLength  Length of the data, in bytes.
    ///
	virtual void OnCmdLinkRead( const void *aBuffer, int64_t aAddress, int64_t aLength );

    ///
    /// \brief Callback invoked after a port write.
    ///
    /// Used to report write activity  on the GenApi IPort implementation of the command link of the PvDevice.
    ///
    /// The callback is invoked AFTER the write operation. The content of aBuffer
    /// represents the data that was written to the device.
    ///
    /// This callback is only invoked on a successful write operation.
    ///
    /// If aLength is 4 bytes, that data is represented as a 4 bytes value formatted as big endian.
    /// If data is more that 4 bytes, it is presented as a stream of bytes requiring
    /// proper interpretation.
    ///
    /// \param[in] aBuffer  Pointer to the data that was written.
    /// \param[in] aAddress Address of the data written to the device.
    /// \param[in] aLength  Length of the data, in bytes.
    ///
	virtual void OnCmdLinkWrite( const void *aBuffer, int64_t aAddress, int64_t aLength );

};

#endif
