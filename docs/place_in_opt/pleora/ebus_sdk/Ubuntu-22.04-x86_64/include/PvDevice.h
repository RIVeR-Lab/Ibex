// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDEVICE_H__
#define __PVDEVICE_H__

#include <PvDeviceLib.h>
#include <PvDeviceInfo.h>
#include <PvGenParameterArray.h>
#include <PvDeviceEnums.h>
#include <PvDeviceEventSink.h>


class PvDeviceEventSink;

namespace PvDeviceLib
{
    class Device;
}


class PvDeviceEventSink;
class PvConfigurationWriter;
class PvConfigurationReader;

///
/// \class PvDevice
/// \brief Connect, configure and control a GigE Vision or USB3 Vision device.
///
/// It is not possible to instantiate a generic PvDevice. PvDeviceGEV and PvDeviceU3V can
/// be instantiated directly.
///
/// If order to write hybrid code one could use the static CreateAndConnect methods. These
/// methods dynamically allocated a PvDevice of the right type to connect to the requested
/// device. Just remember to free the dynamically allocated device object using the static 
/// Free method.
///
class PV_DEVICE_API PvDevice 
{
public:
    ///
    /// \brief Destructor
    ///
    virtual ~PvDevice();

    ///
    /// \brief Dynamically allocates a PvDevice of the right type.
    ///
    /// This static method takes a generic PvDeviceInfo, creates the right type of PvDevice (PvDeviceGEV
    /// or PvDeviceU3V) and connects it.
    ///
    /// Objects allocated with CreateAndConnect should be freed with #PvDevice::Free.
    ///
    /// \param[in] aDeviceInfo Pointer to the device information used to connect the device.
    /// \param[out] aResult Outcome of the operation:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER (if NULL aDeviceInfo, or bad parameter passed)
    /// - #PvResult::Code::NOT_CONNECTED (if device was found, initially connected but disconnect during the connection process)
    /// - #PvResult::Code::GENERIC_ERROR (if an unexpected exception was thrown or if socket cannot bind or connect[GEV] or usb driver is busy [U3V])
    /// - #PvResult::Code::NETWORK_ERROR (if sending packets failed, GEV)
    /// - #PvResult::Code::TIMEOUT (if socket binds and sends but times out on receive, GEV)
    /// - #PvResult::Code::THREAD_ERROR (if heartbeat thread will not start)
    /// - #PvResult::Code::GENICAM_XML_ERROR (if creating device genicam node map failed with xml error)
    /// - #PvResult::Code::NOT_FOUND (if the device specified is not found)
    /// - #PvResult::Code::NOT_SUPPORTED (if no message channels are supported)
    /// - #PvResult::Code::CANT_READ_MANIFEST (if manifest table could not be read. This can sometimes happen if the U3V device is not powered properly)
    ///
    /// \return A pointer to the new object on success, NULL on failure. See aResult for more information on the failure.
    ///
    static PvDevice *CreateAndConnect( const PvDeviceInfo *aDeviceInfo, PvResult *aResult );

    ///
    /// \brief Dynamically allocates a PvDevice of the right type.
    ///
    /// This static method takes a string identifying a device, creates the right type of PvDevice (PvDeviceGEV
    /// or PvDeviceU3V) and connects it.
    ///
    /// Objects allocated with CreateAndConnect should be freed with #PvDevice::Free.
    ///
    /// \param[in] aInfo String identifying the device. See PvDevice::Connect for more information.
    /// \param[out] aResult Outcome of the operation.
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER (if empty aInfo, or bad parameter passed)
    /// - #PvResult::Code::NOT_CONNECTED (if device was found, initially connected but disconnect during the connection process)
    /// - #PvResult::Code::NETWORK_ERROR (if sending packets failed, GEV)
    /// - #PvResult::Code::TIMEOUT (if socket binds and sends but times out on receive, GEV)
    /// - #PvResult::Code::THREAD_ERROR (if heartbeat thread will not start)
    /// - #PvResult::Code::GENERIC_ERROR (if an unexpected exception was thrown or if socket cannot bind or connect[GEV] or usb driver is busy [U3V])
    /// - #PvResult::Code::GENICAM_XML_ERROR (if creating device genicam node map failed with xml error)
    /// - #PvResult::Code::NOT_FOUND (if the device specified is not found)
    /// - #PvResult::Code::NOT_SUPPORTED (if no message channels are supported)
    /// - #PvResult::Code::CANT_READ_MANIFEST (if manifest table could not be read. This can sometimes happen if the U3V device is not powered properly)
    /// \return A pointer to the new object on success, NULL on failure. See aResult for more information on the failure.
    ///
    static PvDevice *CreateAndConnect( const PvString &aInfo, PvResult *aResult );

    ///
    /// \brief Frees an object allocated with CreateAndConnect.
    ///
    /// Even though calling delete on a PvDevice created with CreateAndConnect should
    /// work in most cases, sometime it is necessary to call "delete" in the same context where
    /// "new" was called to avoid false positives on memory leaks.
    ///
    /// It is a good practice to use this method to free a PvDevice allocated with CreateAndConnect.
    ///
    /// \param[in] aDevice Pointer to the device object to delete.
    ///
    static void Free( PvDevice *aDevice );

    ///
    /// \brief Returns the type of the instantiated device.
    ///
    /// \return Type of device controller. See #PvDeviceType enum
    ///
    PvDeviceType GetType() const;

    ///
    /// \brief Connect to a GigE Vision or USB3 Vision device.
    ///
    /// The #Connect method connects to a GigE Vision device found using a #PvSystem or device finder dialog object.
    /// Only one device can be connected to at a time.
    /// 
    /// \param[in] aDeviceInfo    The GigE Vision device's network connectivity information; a #PvDeviceInfo object.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER (if NULL aDeviceInfo, or bad parameter passed)
    ///     - #PvResult::Code::NOT_CONNECTED (if device was found, initially connected but disconnect during the connection process)
    ///     - #PvResult::Code::NETWORK_ERROR (if sending packets failed, GEV)
    ///     - #PvResult::Code::TIMEOUT (if socket binds and sends but times out on receive, GEV)
    ///     - #PvResult::Code::THREAD_ERROR (if heartbeat thread will not start)
    ///     - #PvResult::Code::GENERIC_ERROR (if an unexpected exception was thrown or if socket cannot bind or connect[GEV] or usb driver is busy [U3V])
    ///     - #PvResult::Code::GENICAM_XML_ERROR (if creating device genicam node map failed with xml error)
    ///     - #PvResult::Code::NOT_FOUND (if the device specified is not found)
    ///     - #PvResult::Code::NOT_SUPPORTED (if no message channels are supported)
    ///     - #PvResult::Code::CANT_READ_MANIFEST (if manifest table could not be read. This can sometimes happen if the U3V device is not powered properly)
    ///
	virtual PvResult Connect( const PvDeviceInfo *aDeviceInfo );

    ///
    /// \brief Connect to a GigE Vision or USB3 Vision device.
    ///
    /// If you know the GigE Vision device's IP address, MAC address or DeviceUserID, you can use this method to connect to it.
    ///
    /// If you know the USB3 Vision device's GUID or DeviceUserID, you can use this method to connect it.
    ///
    ///     - If aInfo contains an IP address, the connection attempt connecting a GigE Vision device is direct.
    ///     - If aInfo contains a MAC address:
    ///         - An internal #PvSystem is used to find all reachable GigE Vision devices
    ///         - Found devices are browsed, looking for a match on the MAC address
    ///         - If the device is found, connection is attempted.
    ///     - If aInfo contains a USB3 Vision GUID:
    ///         - All USB3 Vision devices are enumerated
    ///         - Found devices are browsed, looking for a match on the GUID
    ///         - If the device is found, connection is attempted.
    ///     - If aInfo contains a device user ID:
    ///         - All GigE Vision and USB3 Vision are enumerated
    ///         - Found devices are browsed, looking for a match on device user ID
    ///         - If the device is found, connection is attempted.
    ///
    /// \param[in] aInfo          Information that uniquely identifies the device. You may use one of the following:
    ///                               - GigE Vision device's MAC address. Use the form <tt>00:11:22:33:44:5F</tt>.
    ///                               - GigE Vision device's IP Address. Use the form <tt>192.168.1.114</tt>.
    ///                               - GigE Vision or USB3 Vision device's DeviceUserID.
    ///                               - USB3 Vision GUID.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER (if empty aInfo, or bad parameter passed)
    ///     - #PvResult::Code::NOT_CONNECTED (if device was found, initially connected but disconnect during the connection process)
    ///     - #PvResult::Code::NETWORK_ERROR (if sending packets failed, GEV)
    ///     - #PvResult::Code::TIMEOUT (if socket binds and sends but times out on receive, GEV)
    ///     - #PvResult::Code::THREAD_ERROR (if heartbeat thread will not start)
    ///     - #PvResult::Code::GENERIC_ERROR (if an unexpected exception was thrown or if socket cannot bind or connect[GEV] or usb driver is busy [U3V])
    ///     - #PvResult::Code::GENICAM_XML_ERROR (if creating device genicam node map failed with xml error)
    ///     - #PvResult::Code::NOT_FOUND (if the device specified is not found)
    ///     - #PvResult::Code::NOT_SUPPORTED (if no message channels are supported)
    ///     - #PvResult::Code::CANT_READ_MANIFEST (if manifest table could not be read. This can sometimes happen if the U3V device is not powered properly)
    ///
	virtual PvResult Connect( const PvString &aInfo );

    ///
    /// \brief Disconnect the #PvDevice object from the GigE Vision or USB3 Vision device to which it is currently connected.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///
    /// \sa Connect
    ///
	PvResult Disconnect();

    ///
    /// \brief Enables streaming on the device.
    ///
    /// For GigE Vision devices, this takes care of setting the TLParamsLocked so that changes
    /// to streaming related parameters are prevent in the GenICam interface of the device.
    ///
    /// For USB3 Vision devices, this takes care of the TLParamsLocked, configures the driver
    /// for streaming and sets the streaming enable bit on the device.
    ///
    /// After calling StreamEnable, the user must still execute the AcquisitionStart GenICam command
    /// of the device to instruct the device to begin acquisition.
    ///
    /// \param[in] aChannel The channel on which to enable streaming.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::GENERIC_ERROR      -if GenApi throws exception setting TLParam (GEV) or read memory faild (U3V)
    ///     - #PvResult::Code::INVALID_PARAMETER  -if command size is not correct (U3V)
    ///
    PvResult StreamEnable( uint32_t aChannel = 0 );

    ///
    /// \brief Disables streaming on the device.
    ///
    /// For GigE Vision devices, this takes care of resetting the TLParamsLocked so that changes
    /// to streaming related parameters are possible again.
    ///
    /// For USB3 Vision devices, this takes care of the TLParamsLocked and sets the streaming 
    /// enable bit on the device.
    ///
    /// StreamDisable should be called after the AcquisitionStop GenICam command has been executed
    /// and all images have been received.
    ///
    /// \param[in] aChannel The channel on which to enable streaming.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::GENERIC_ERROR      -if GenApi throws exception setting TLParam (GEV) or read memory faild (U3V)
    ///     - #PvResult::Code::INVALID_PARAMETER  -if command size is not correct (U3V)
    ///
    PvResult StreamDisable( uint32_t aChannel = 0 );

    ///
    /// \brief Test if this #PvDevice is currently connected to a GigE Vision or USB3 Vision device.
    ///
    /// \return True if this #PvDevice is currently connected to a GigE Vision or USB3 Vision device otherwise, false.
    ///
	bool IsConnected() const;

    ///
    /// \brief Test if this #PvDevice is currently connected to a Pleora powered GigE Vision or USB3 Vision device.
    ///
    /// \return True if this #PvDevice is currently connected to a Pleora powered GigE Vision or USB3 Vision device, false otherwise.
    ///
    bool IsPleoraPowered() const;

    ///
    /// \brief Save the GigE Vision or USB3 Visio device's GenICam XML file to disk.
    ///
    /// If the XML is stored in a zip file on the device, the zip file containing the XML
    /// is saved on the disk.
    ///
    /// \param aFilename  The file's name.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::NOT_SUPPORTED
    ///     - #PvResult::Code::GENERIC_ERROR
    ///
	PvResult DumpGenICamXML( const PvString &aFilename );

    ///
    /// \brief Returns a good default filename to use with DumpGenICamXML.
    ///
    /// The filename is built from the URL that was used in order to locate
    /// the GenICam XML file of the device.
    ///
    /// \param aFilename  A default filename built from the device XML location URL.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::INVALID_PARAMETER
	PvResult GetDefaultGenICamXMLFilename( PvString &aFilename );

    ///
    /// \brief Get the parameters used to control the GigE Vision or USB3 Vision device to which you are connected.
    ///
    /// Each device provides its own GenICam XML file with parameters describing its behaviour referred to as features.
    /// The PvGenParameterArray object returned by this method provides an interface to these features.
    ///
    /// \return The parameters used to control the GigE Vision device to which you are connected
    ///
	PvGenParameterArray *GetParameters();

    ///
    /// \brief Get the parameters used to define how PvDevice connects to and communicates with the device.
    ///
    /// For a listing of all the available parameters, refer to <a href="devicecommunicationGEV.html"> GEV Device Communication Parameters</a>
    ///                                                      or <a href="devicecommunicationU3V.html"> U3V Device Communication Parameters </a>
    ///
    /// \return The parameters used to define how PvDevice connects to and communicates with the device
    ///
	PvGenParameterArray *GetCommunicationParameters();

    ///
    /// \brief Reads bytes starting at a specific register address on the GigE Vision or USB3 Vision device.
    /// 
    /// See warnings about accessing the register map directly in #WriteMemory.
    ///
    /// \param[in]  aAddress       Start address of the read memory operation (mod 4).
    /// \param[out] aDestination   Buffer where to write data read from the device.
    /// \param[in]  aByteCount     Bytes to read from the device (mod 4, non 0)
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::TIMEOUT
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::NETWORK_ERROR
    ///
	PvResult ReadMemory( int64_t aAddress, unsigned char *aDestination, int64_t aByteCount );

    ///
    /// \brief Writes bytes starting at a specific register address on the GigE Vision or USB3 Vision device.
    /// 
    /// This is an advanced method and is rarely needed. In general, most users should instead 
    /// use the device's GenICam interface which can be obtained through #GetParameters.
    ///
    /// This method let advanced users control third-party GigE Vision devices with poor or incomplete GenICam 
    /// XML files. The methods presume detailed working knowledge of the GigE Vision device's register map. Since 
    /// the #WriteMemory method bypasses the GenICam interface and the SDK's cache of values, incorrect use may 
    /// cause indeterminate results.
    ///
    /// Not all GigE Vision devices are supporting the write memory operation. When using
    /// this method on a Gig Vision device not supporting write memory operations, the write is 
    /// broken down in as many write register commands as required.
    ///
    /// \param[in]  aAddress       Start address of the write memory operation (mod 4).
    /// \param[in]  aSource        Buffer holding the data to write to the device.
    /// \param[in]  aByteCount     Bytes to write to the device (mod 4, non 0)
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::TIMEOUT
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::NETWORK_ERROR
    ///
	PvResult WriteMemory( int64_t aAddress, const unsigned char *aSource, int64_t aByteCount );

    ///
    /// \brief Waits for the messaging channel to become idle.
    ///
    /// This method returns once the host-side messaging channel in-queue is empty
    /// or aTimeout (ms) has elapsed.
    ///
    /// \param[in] aTimeout Maximum time to wait, in ms.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::TIMEOUT
    ///
    PvResult WaitForMessagingChannelIdle( uint32_t aTimeout );

    ///
    /// \brief Register an event sink used for callbacks.
    ///
    /// \param[in] aEventSink  A pointer to the event sink.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::GENERIC_ERROR      - if event already registered
    ///
    PvResult RegisterEventSink( PvDeviceEventSink *aEventSink );

    ///
    /// \brief Unregister an event sink
    ///
    /// \param[in] aEventSink  A pointer to the event sink.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::NOT_FOUND
    ///
    PvResult UnregisterEventSink( PvDeviceEventSink *aEventSink );

    ///
    /// \brief Get the priority of the heartbeat thread
    ///
    /// See #SetHeartbeatThreadPriority for details about the meaning of the values returned
    /// 
    /// \return The priority of the thread
    ///
    uint32_t GetHeartbeatThreadPriority() const;

    ///
    /// \brief Set the priority of the heartbeat thread
    ///
    /// The heartbeat thread priority is critical to most applications.
    /// This thread must never starve against any other system thread to the risk of losing the device connection.
    ///
    /// This thread takes all messages queued for deferred processing by the interrupt link thread and 
    /// fires all GenApi and PvDeviceEventSink callback. All callbacks are fired/run in the context of this thread.
    ///
    /// See #GetHeartbeatThreadPriority
    /// 
    /// \param[in] aPriority   See #PvPipeline::SetBufferHandlingThreadPriority for description of the supported values
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::THREAD_ERROR
    ///
    PvResult SetHeartbeatThreadPriority( uint32_t aPriority );

    ///
    /// \brief Get the priority of the interrupt link thread
    ///
    /// See #SetInterruptLinkThreadPriority for details about the meaning of the values returned
    /// 
    /// \return The priority of the thread
    ///
    uint32_t GetInterruptLinkThreadPriority() const;

    ///
    /// \brief Set the priority of the interrupt link thread
    ///
    /// Interrupt link thread receives messaging channel packets from the device, sends acknowledge 
    /// back and queues the message in the interrupt queue. Process and callbacks are not done from this thread.
    ///
    /// This thread takes all messages queued for deferred processing by the interrupt link thread and 
    /// fires all GenApi and PvDeviceEventSink callback. All callbacks are fired/run in the context of this thread.
    ///
    /// See #GetInterruptLinkThreadPriority
    /// 
    /// \param[in] aPriority   See #PvPipeline::SetBufferHandlingThreadPriority for description of the supported values
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::THREAD_ERROR
    ///
    PvResult SetInterruptLinkThreadPriority( uint32_t aPriority );

    ///
    /// \brief Get the priority of the interrupt queue thread
    ///
    /// See #SetInterruptQueueThreadPriority for details about the meaning of the values returned
    /// 
    /// \return The priority of the thread
    ///
    uint32_t GetInterruptQueueThreadPriority() const;

    ///
    /// \brief Set the priority of the interrupt queue thread
    ///
    /// This thread is simply used to fire/invoke the OnLinkDisconnected and OnLinkReconnected PvDeviceEventSink events.
    ///
    /// This thread takes all messages queued for deferred processing by the interrupt link thread and 
    /// fires all GenApi and PvDeviceEventSink callback. All callbacks are fired/run in the context of this thread.
    ///
    /// See #GetInterruptQueueThreadPriority
    /// 
    /// \param[in] aPriority   See #PvPipeline::SetBufferHandlingThreadPriority for description of the supported values
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::THREAD_ERROR
    ///
    PvResult SetInterruptQueueThreadPriority( uint32_t aPriority );

    ///
    /// \brief Returns the payload size the device is currently configured to stream with.
    ///
    /// This method attempts to read the GenICam PayloadSize parameter of the device. If the operation fails
    /// the payload size is computed from width, height and pixel format as a fall back. If even this fails,
    /// a value that is still valid to allocate buffers is finally returned.
    ///
    /// \return The recommended payload size. 0 can only be returned if the device is not connected.
    ///
    uint32_t GetPayloadSize();

    ///
    /// \brief Returns a unique string identifier for the device.
    ///
    /// \param[out] aID Unique string identifier for the device.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///
    PvResult GetUniqueID( PvString &aID );

protected:
    ///
    /// \brief Constructor.
    ///
	PvDevice();

    PvDeviceLib::Device *mThis;

private:

	// Not implemented
	PvDevice( const PvDevice & );
	const PvDevice &operator=( const PvDevice & );

};

#endif
