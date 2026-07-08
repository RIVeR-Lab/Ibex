// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __IPVDEVICEADAPTER_H__
#define __IPVDEVICEADAPTER_H__

#include <PvTypes.h>
#include <PvString.h>
#include <PvResult.h>

#include <PvStringList.h>
#include <IPvDeviceEventSink.h>


// If implementing your own IPvDeviceAdapter, #define PV_CUSTOM_DEVICE_ADAPTER
// in order to prevent DLL-import of IPvDeviceAdapter
#if defined( WIN32 ) && !defined( PT_LIB_STATIC ) && !defined( PV_CUSTOM_DEVICE_ADAPTER )
	#if defined( PV_DEVICE_EXPORTS )
		#define IPV_DEVICE_ADAPTER_API __declspec( dllexport )
	#else
		#define IPV_DEVICE_ADAPTER_API __declspec( dllimport )
	#endif
#else
	#define IPV_DEVICE_ADAPTER_API
#endif


///
/// \class IPvDeviceAdapter
/// \brief Interface used by PvDeviceSerialPort to interact with a device controller.
///
/// The IPvDeviceAdapter interface can be implemented for any SDK that can control
/// GigE Vision and/or USB3 Vision devices in order to use PvDeviceSerialPort.
///
/// In order to use PvDeviceSerialPort with a third-party SDK, inherit your own class
/// from IPvDeviceAdapter and implement all pure methods.
///
/// You need to forward GenICam invalidation events for all IPvDeviceEventSink interfaces
/// registered using RegisterEventSink up until UnregisterEventSink is called.
///
/// Use IPvDeviceEventSink::NotifyInvalidatedGenParameter for parameters that RegisterGenInvalidator was called 
/// for. You also need to forward all messaging channel events to the PvDeviceSerialPort using 
/// IPvDeviceEventSink::NotifyEvent.
///
class IPV_DEVICE_ADAPTER_API IPvDeviceAdapter
{
public:

	IPvDeviceAdapter() {}
    virtual ~IPvDeviceAdapter() {}

    ///
    /// \brief Returns true if the device node map contains an integer of the specified name.
    ///
    /// \param[in] aParameterName Name of the parameter to test for.
    ///
    /// \return True if the parameter is present, false if not.
    ///
    virtual bool IsGenIntegerInNodeMap( const PvString &aParameterName ) = 0;

    ///
    /// \brief Returns true if the device node map contains an enumeration of the specified name.
    ///
    /// \param[in] aParameterName Name of the parameter to test for.
    ///
    /// \return True if the parameter is present, false if not.
    ///
    virtual bool IsGenEnumInNodeMap( const PvString &aParameterName ) = 0;

    ///
    /// \brief Returns true if the device node map contains a Boolean of the specified name.
    ///
    /// \param[in] aParameterName Name of the parameter to test for.
    ///
    /// \return True if the parameter is present, false if not.
    ///
    virtual bool IsGenBooleanInNodeMap( const PvString &aParameterName ) = 0;

    ///
    /// \brief Returns true if the device node map contains a raw register of the specified name.
    ///
    /// \param[in] aParameterName Name of the parameter to test for.
    ///
    /// \return True if the parameter is present, false if not.
    ///
    virtual bool IsGenRegisterInNodeMap( const PvString &aParameterName ) = 0;

    ///
    /// \brief Returns true if a parameter is present and readable in the node map.
    ///
    /// \param[in] aParameterName Name of the parameter to test for.
    ///
    /// \return True if the parameter is present and readable, false if either condition fails.
    ///
    virtual bool IsGenReadable( const PvString &aParameterName ) = 0;

    ///
    /// \brief Returns true if a parameter is present and writable in the node map.
    ///
    /// \param[in] aParameterName Name of the parameter to test for.
    ///
    /// \return True if the parameter is present and writable, false if either condition fails.
    ///
    virtual bool IsGenWritable( const PvString &aParameterName ) = 0;

    ///
    /// \brief Returns true if an enumeration is present in the node map and has a specific enum entry.
    ///
    /// \param[in] aParameterName Name of the parameter to test for.
    /// \param[in] aEnumEntry Name of the enum entry to test for.
    ///
    /// \return True if the parameter is present and contains the specific enum entry, false if either condition fails.
    ///
    virtual bool IsGenEnumEntryAvailable( const PvString &aParameterName, const PvString &aEnumEntry ) = 0;

    ///
    /// \brief Returns the value of an integer identified by its name.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[out] aValue Value of the parameter.
    ///
    /// \return OK on success.
    ///
    virtual PvResult GetGenIntegerValue( const PvString &aParameterName, int64_t &aValue ) = 0;

    ///
    /// \brief Returns a list of all available enum entries for an enumeration
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[out] aList A list containing the string representation of all available enum entries.
    ///
    /// \return OK on success.
    ///
    virtual PvResult GetGenEnumEntriesAvailable( const PvString &aParameterName, PvStringList &aList ) = 0;

    ///
    /// \brief Returns the string value of an enumeration identified by its name.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[out] aEnumEntry Value of the parameter.
    ///
    /// \return OK on success.
    ///
    virtual PvResult GetGenEnumValue( const PvString &aParameterName, PvString &aEnumEntry ) = 0;

    ///
    /// \brief Sets the value of an enumeration identified by its name.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[in] aEnumEntry New value for the enumeration.
    ///
    /// \return OK on success.
    ///
    virtual PvResult SetGenEnumValue( const PvString &aParameterName, const PvString &aEnumEntry ) = 0;

    ///
    /// \brief Returns the value of a Boolean identified by its name.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[out] aValue Value of the parameter.
    ///
    /// \return OK on success.
    ///
    virtual PvResult GetGenBooleanValue( const PvString &aParameterName, bool &aValue ) = 0;

    ///
    /// \brief Sets the value of a Boolean identified by its name.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[in] aValue New value for the Boolean.
    ///
    /// \return OK on success.
    ///
    virtual PvResult SetGenBooleanValue( const PvString &aParameterName, bool aValue ) = 0;

    ///
    /// \brief Returns the string value of string parameter identified by its name.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[out] aValue Value of the parameter.
    ///
    /// \return OK on success.
    ///
    virtual PvResult GetGenStringValue( const PvString &aParameterName, PvString &aValue ) = 0;

    ///
    /// \brief Gets the length (in bytes) of a register identified by its name.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[in] aLength Length (in bytes) of the register.
    ///
    /// \return OK on success.
    ///
    virtual PvResult GetGenRegisterLength( const PvString &aParameterName, int64_t &aLength ) = 0;

    ///
    /// \brief Gets the data contained in a register identified by its name.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[out] aDataBuffer Buffer where to copy the data.
    /// \param[in] aByteCount How many bytes to copy to aDataBuffer.
    ///
    /// \return OK on success.
    ///
    virtual PvResult GetGenRegisterData( const PvString &aParameterName, uint8_t *aDataBuffer, int64_t aByteCount ) = 0;

    ///
    /// \brief Sets the data contained in a register identified by its name.
    ///
    /// \param[in] aParameterName Name of the parameter.
    /// \param[out] aDataBuffer Buffer to copy the data from.
    /// \param[in] aByteCount How many bytes to copy from aDataBuffer to the register.
    ///
    /// \return OK on success.
    ///
    virtual PvResult SetGenRegisterData( const PvString &aParameterName, const uint8_t *aDataBuffer, int64_t aByteCount ) = 0;

    ///
    /// \brief Registers (and forward to the device serial object) invalidation events for a parameter identified by its name.
    ///
    /// \param[in] aParameterName Name of the parameter.
    ///
    /// \return OK on success.
    ///
    virtual PvResult RegisterGenInvalidator( const PvString &aParameterName ) = 0;

    ///
    /// \brief Unregisters (and forward to the device serial object) invalidation events for a parameter identified by its name.
    ///
    /// \param[in] aParameterName Name of the parameter.
    ///
    /// \return OK on success.
    ///
    virtual PvResult UnregisterGenInvalidator( const PvString &aParameterName ) = 0;

    ///
    /// \brief Writes a 32 bit value to a GigE Vision register.
    ///
    /// Should simply return NOT_IMPLEMENTED if the device is not GigE Vision. Serial 
    /// communication methods supported by USB3 Vision devices do not use WriteRegister.
    ///
    /// \param[in] aAddress Address of the register.
    /// \param[in] aValue Value to write to the register.
    ///
    /// \return OK on success.
    ///
    virtual PvResult WriteRegister( int64_t aAddress, uint32_t aValue ) = 0;

    ///
    /// \brief Reads a 32 bit value from a GigE Vision register.
    ///
    /// Should simply return NOT_IMPLEMENTED if the device is not GigE Vision. Serial 
    /// communication methods supported by USB3 Vision devices do not use ReadRegister.
    ///
    /// \param[in] aAddress Address of the register.
    /// \param[out] aValue Value read from the register.
    ///
    /// \return OK on success.
    ///
    virtual PvResult ReadRegister( int64_t aAddress, uint32_t &aValue ) = 0;

    ///
    /// \brief Writes data to a the memory of a device.
    ///
    /// \param[in] aBuffer Pointer to the data to write.
    /// \param[in] aAddress Address where to write the data.
    /// \param[in] aLength Length of the data to write, in bytes.
    ///
    /// \return OK on success.
    ///
	virtual PvResult WriteMemory( const uint8_t *aBuffer, int64_t aAddress, int64_t aLength ) = 0;

    ///
    /// \brief Reads data from the memory of a device.
    ///
    /// \param[in] aBuffer Pointer of the buffer where the data will be written.
    /// \param[in] aAddress Address where to read the data.
    /// \param[in] aLength Length of the data to read, in bytes.
    ///
    /// \return OK on success.
    ///
	virtual PvResult ReadMemory( uint8_t *aBuffer, int64_t aAddress, int64_t aLength ) = 0;

    ///
    /// \brief Waits for the messaging channel queue to be empty.
    ///
    /// \param[in] aTimeout How long to wait for the messaging channel to become idle.
    ///
    /// \return OK on success.
    ///
    virtual PvResult WaitForMessagingChannelIdle( uint32_t aTimeout ) = 0;

    ///
    /// \brief Returns true if the device is connected.
    ///
    /// \return True if the device is connected, false if not.
    ///
    virtual bool IsConnected() = 0;

    ///
    /// \brief Returns true if the device runs on a Pleora design.
    ///
    /// \return True if the device runs on a Pleora design, false if not.
    ///
    virtual bool IsPleoraPowered() = 0;

    ///
    /// \brief Returns true if the device uses the GigE Vision protocol.
    ///
    /// \return True if using the GigE Vision protocol, false if not.
    ///
    virtual bool IsGigEVision() = 0;

    ///
    /// \brief Returns true if the device uses the USB3 Vision protocol.
    ///
    /// \return True if using the USB3 Vision protocol, false if not.
    ///
    virtual bool IsUSB3Vision() = 0;

    ///
    /// \brief Registers an event sink that will want to receive the raw events and GenICam invalidator events.
    /// 
    /// \param[in] aEventSink Pointer to an object implementing the IPvDeviceEventSink interface.
    ///
    /// \return OK on success.
    ///
    virtual PvResult RegisterEventSink( IPvDeviceEventSink *aEventSink ) = 0;

    ///
    /// \brief Unregisters an event sink registered with RegisterEventSink.
    ///
    /// \param[in] aEventSink Pointer to an object implementing the IPvDeviceEventSink interface.
    ///
    /// \return OK on success.
    ///
    virtual PvResult UnregisterEventSink( IPvDeviceEventSink *aEventSink ) = 0;

    ///
    /// \brief Returns a unique string identifier for the device.
    ///
    /// \param[out] aID Unique string identifier for the device.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER
    ///
    virtual PvResult GetUniqueID( PvString &aID ) = 0;

protected:

private:

};

#endif
