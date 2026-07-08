// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDEVICEADAPTER_H__
#define __PVDEVICEADAPTER_H__

#include <IPvDeviceAdapter.h>
#include <PvDevice.h>


class IPvDeviceEventSinkList;

namespace PtUtilsLib
{
    class Mutex;
}

///
/// \class PvDeviceAdapter
/// \brief IPvDeviceAdapter default implementation for the eBUS SDK.
///
/// The IPvDeviceAdapter interface can be implemented for any SDK that can control
/// GigE Vision and/or USB3 Vision devices in order to use PvDeviceSerialPort.
///
/// PvDeviceAdapter provides a default implementation of the IPvDeviceAdapter that
/// can be used with the eBUS SDK.
/// 
class PV_DEVICE_API PvDeviceAdapter : public IPvDeviceAdapter, PvDeviceEventSink, PvGenEventSink
{
public:

    ///
    /// \brief Constructor.
    ///
    /// \param[in] aDevice A device.
    ///
    PvDeviceAdapter( PvDevice *aDevice );

    ///
    /// \brief Destructor.
    ///
    virtual ~PvDeviceAdapter();

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter to test for.
    ///
    /// \return True if the parameter is present, false if not.
    ///
    bool IsGenIntegerInNodeMap( const PvString &aParameterName );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter to test for.
    ///
    /// \return True if the parameter is present, false if not.
    ///
    bool IsGenEnumInNodeMap( const PvString &aParameterName );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter to test for.
    ///
    /// \return True if the parameter is present, false if not.
    ///
    bool IsGenBooleanInNodeMap( const PvString &aParameterName );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter to test for.
    ///
    /// \return True if the parameter is present, false if not.
    ///
    bool IsGenRegisterInNodeMap( const PvString &aParameterName );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter to test for.
    ///
    /// \return True if the parameter is present and readable, false if either condition fails.
    ///
    bool IsGenReadable( const PvString &aParameterName );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter to test for.
    ///
    /// \return True if the parameter is present and writable, false if either condition fails.
    ///
    bool IsGenWritable( const PvString &aParameterName );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter to test for.
    /// \param[in] aEnumEntry Name of the enum entry to test for.
    ///
    /// \return True if the parameter is present and contains the specific enum entry, false if either condition fails.
    ///
    bool IsGenEnumEntryAvailable( const PvString &aParameterName, const PvString &aEnumEntry );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[out] aValue Value of the parameter.
    ///
    /// \return OK on success.
    ///
    PvResult GetGenIntegerValue( const PvString &aParameterName, int64_t &aValue );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[out] aList A list containing the string representation of all available enum entries.
    ///
    /// \return OK on success.
    ///
    PvResult GetGenEnumEntriesAvailable( const PvString &aParameterName, PvStringList &aList );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[out] aEnumEntry Value of the parameter.
    ///
    /// \return OK on success.
    ///
    PvResult GetGenEnumValue( const PvString &aParameterName, PvString &aEnumEntry );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[in] aEnumEntry New value for the enumeration.
    ///
    /// \return OK on success.
    ///
    PvResult SetGenEnumValue( const PvString &aParameterName, const PvString &aEnumEntry );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[out] aValue Value of the parameter.
    ///
    /// \return OK on success.
    ///
    PvResult GetGenBooleanValue( const PvString &aParameterName, bool &aValue );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[in] aValue New value for the enumeration.
    ///
    /// \return OK on success.
    ///
    PvResult SetGenBooleanValue( const PvString &aParameterName, bool aValue );

    ///
    /// \brief Returns the string value of string parameter identified by its name.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[out] aValue Value of the parameter.
    ///
    /// \return OK on success.
    ///
    PvResult GetGenStringValue( const PvString &aParameterName, PvString &aValue );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[in] aLength Length (in bytes) of the register.
    ///
    /// \return OK on success.
    ///
    PvResult GetGenRegisterLength( const PvString &aParameterName, int64_t &aLength );
    
    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[out] aDataBuffer Buffer where to copy the data.
    /// \param[in] aByteCount How many bytes to copy to aDataBuffer.
    ///
    /// \return OK on success.
    ///
    PvResult GetGenRegisterData( const PvString &aParameterName, uint8_t *aDataBuffer, int64_t aByteCount );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[out] aDataBuffer Buffer to copy the data from.
    /// \param[in] aByteCount How many bytes to copy from aDataBuffer to the register.
    ///
    /// \return OK on success.
    ///
    PvResult SetGenRegisterData( const PvString &aParameterName, const uint8_t *aDataBuffer, int64_t aByteCount );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter.
    ///
    /// \return OK on success.
    ///
    PvResult RegisterGenInvalidator( const PvString &aParameterName );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aParameterName Name of the parameter.
    ///
    /// \return OK on success.
    ///
    PvResult UnregisterGenInvalidator( const PvString &aParameterName );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aAddress Address of the register.
    /// \param[in] aValue Value to write to the register.
    ///
    /// \return OK on success.
    ///
    PvResult WriteRegister( int64_t aAddress, uint32_t aValue );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aAddress Address of the register.
    /// \param[out] aValue Value read from the register.
    ///
    /// \return OK on success.
    ///
    PvResult ReadRegister( int64_t aAddress, uint32_t &aValue );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aBuffer Pointer to the data to write.
    /// \param[in] aAddress Address where to write the data.
    /// \param[in] aLength Length of the data to write, in bytes.
    ///
    /// \return OK on success.
    ///
	PvResult WriteMemory( const uint8_t *aBuffer, int64_t aAddress, int64_t aLength );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aBuffer Pointer of the buffer where the data will be written.
    /// \param[in] aAddress Address where to read the daa.
    /// \param[in] aLength Length of the data to read, in bytes.
    ///
    /// \return OK on success.
    ///
	PvResult ReadMemory( uint8_t *aBuffer, int64_t aAddress, int64_t aLength );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aTimeout A timeout.
    ///
    /// \return OK if the messaging channel is idle. TIMEOUT if the timeout elapsed before it becomes idle.
    ///
    PvResult WaitForMessagingChannelIdle( uint32_t aTimeout );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \return True if the device is connected, false if not.
    ///
    bool IsConnected();

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \return True if the device runs on a Pleora design, false if not.
    ///
    bool IsPleoraPowered();

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \return True if using the GigE Vision protocol, false if not.
    ///
    bool IsGigEVision();

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \return True if using the USB3 Vision protocol, false if not.
    ///
    bool IsUSB3Vision();
   
    ///
    /// \brief See IPvDeviceAdapter for description.
    /// 
    /// \param[in] aEventSink Pointer to an object implementing the IPvDeviceEventSink interface.
    ///
    /// \return OK on success.
    ///
    PvResult RegisterEventSink( IPvDeviceEventSink *aEventSink );

    ///
    /// \brief See IPvDeviceAdapter for description.
    ///
    /// \param[in] aEventSink Pointer to an object implementing the IPvDeviceEventSink interface.
    ///
    /// \return OK on success.
    ///
    PvResult UnregisterEventSink( IPvDeviceEventSink *aEventSink );

    ///
    /// \brief Returns a unique string identifier for the device.
    ///
    /// \param[out] aID Unique string identifier for the device.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult GetUniqueID( PvString &aID );

protected:

    ///
    /// \brief PvDeviceEventSink callback from PvDevice
    ///
    /// \param[in] aDevice Device that triggered the callback.
    /// \param[in] aEventID Event ID.
    /// \param[in] aChannel Channel of the event.
    /// \param[in] aBlockID Block ID of the event.
    /// \param[in] aTimestamp Timestamp of the event.
    /// \param[in] aData Data pointer (can be NULL if EVENT_CMD instead of EVENTDATA_CMD)
    /// \param[in] aDataLength Data Length (can be 0 if EVENT_CMD instead of EVENTDATA_CMD)
    ///
    void OnEvent( PvDevice *aDevice, 
        uint16_t aEventID, uint16_t aChannel, uint64_t aBlockID, uint64_t aTimestamp, 
        const void *aData, uint32_t aDataLength );

    ///
    /// \brief PvGenParameter callback for GenICam parameters invalidators
    ///
    /// \param[in] aParameter The invalidated parameter.
    ///
	void OnParameterUpdate( PvGenParameter *aParameter );

private:

    PvDevice *mDevice;

    IPvDeviceEventSinkList *mEventSinkList;
    PtUtilsLib::Mutex *mEventSinkListMutex;

};

#endif
