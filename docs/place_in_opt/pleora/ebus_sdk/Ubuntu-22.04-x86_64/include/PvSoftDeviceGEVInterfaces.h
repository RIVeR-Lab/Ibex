// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVSOFTDEVICEGEVINTERFACES_H__
#define __PVSOFTDEVICEGEVINTERFACES_H__

#ifndef EBUS_ENUM
#define EBUS_ENUM(x) typedef enum
#endif

#include <PvVirtualDeviceLib.h>
#include <PvDeviceEnums.h>
#include <PvBuffer.h>
#include <PvGenTypes.h>


class IPvRegisterEventSink;
class IPvSoftDeviceGEV;
class IPvGenApiFactory;
class IPvRegisterFactory;
class IPvRegisterMap;
class IPvUserSetNotify;


namespace PvVirtualDeviceLib
{
    class IActionHandler;
}

EBUS_ENUM(PvScanType)
{
    PvScanTypeInvalid = -1,
    PvScanTypeArea = 0,
    PvScanTypeLine = 1,
    PvScanTypeArea3D = 2,
    PvScanTypeLine3D = 3

} PvScanType;


///
/// \class IPvSoftDeviceGEVInfo
/// \brief Interface that is used to customize the identity of a software-based GigE Vision Device.
///
/// Can be retrieved from a #PvSoftDeviceGEV to access device identity strings
/// and GenICam XML version and GUIDs.
///
/// The device identity strings can be set before or after calling #PvSoftDeviceGEV::Start. However, the GenICam
/// XML version and GUIDs must be set before calling #PvSoftDeviceGEV::Start.
///
/// Setting the GenICam XML version and GUIDs is recommended but not always required. Some non-Pleora
/// software applications will use this information to manage the cached versions of the GenICam XML file 
/// and will not download a newer XML file from the device if the GenICam XML version GUID and/or version do not change.
///
/// The recommended practice is to always update the GenICam XML GUID and version when releasing
/// a new version of your GigE Vision Device. We also recommend that all of your 
/// GigE Vision Device products use a different GenICam XML Product GUID.
///
/// Set your GenICam XML version with #SetGenICamXMLVersion.
///
/// Set your GenICam XML GUIDs with #SetGenICamXMLGUIDs.
/// 
/// This class cannot be instantiated and there is no use case where you would 
/// need to implement this interface.
///
class PV_VIRTUAL_DEVICE_API IPvSoftDeviceGEVInfo
{
public:

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~IPvSoftDeviceGEVInfo() {}

    ///
    /// \brief Returns the current manufacturer name.
    ///
    /// Maps to GigE Vision 0x0048 bootstrap register: 32 bytes, read-only.
    ///
    /// \return Manufacturer name string.
    ///
    virtual const PvString &GetManufacturerName() = 0;

    ///
    /// \brief Returns the current model name.
    ///
    /// Maps to GigE Vision 0x0068 bootstrap register: 32 bytes, read-only.
    ///
    /// \return Model name string.
    ///
    virtual const PvString &GetModelName() = 0;

    ///
    /// \brief Returns the current #PvSoftDeviceGEV version string.
    ///
    /// Maps to GigE Vision 0x0088 bootstrap register: 32 bytes, read-only.
    ///
    /// \return #PvSoftDeviceGEV version string.
    ///
    virtual const PvString &GetDeviceVersion() = 0;

    ///
    /// \brief Returns the current #PvSoftDeviceGEV firmware version string.
    ///
    /// Maps to GigE Vision 0x3FFF0100 bootstrap register: 32 bytes, read-only.
    ///
    /// \return #PvSoftDeviceGEV firmware version string.
    ///
    virtual const PvString &GetDeviceFirmwareVersion() = 0;

    ///
    /// \brief Returns the current manufacturer information string.
    ///
    /// Maps to GigE Vision 0x00A8 bootstrap register: 32 bytes, read-only.
    ///
    /// \return Manufacturer information string.
    ///
    virtual const PvString &GetManufacturerInformation() = 0;

    ///
    /// \brief Returns the current serial number string.
    ///
    /// Maps to GigE Vision 0x00D8 bootstrap register: 16 bytes, read-only.
    ///
    /// \return Serial number string.
    ///
    virtual const PvString &GetSerialNumber() = 0;

    ///
    /// \brief Updates the manufacturer name string.
    ///
    /// Maps to GigE Vision 0x0048 bootstrap register: 32 bytes, read-only.
    ///
    /// \param[in] aValue New manufacturer name, maximum 31 characters (31 + NULL).
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER if the string is too long.
    ///
    virtual PvResult SetManufacturerName( const PvString &aValue ) = 0;

    ///
    /// \brief Updates the model name string.
    ///
    /// Maps to GigE Vision 0x0068 bootstrap register: 32 bytes, read-only.
    ///
    /// \param[in] aValue New model name, maximum 31 characters (31 + NULL for 32 bytes).
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER if the string is too long.
    ///
    virtual PvResult SetModelName( const PvString &aValue ) = 0;

    ///
    /// \brief Updates the device version string.
    ///
    /// Maps to GigE Vision 0x0088 bootstrap register: 32 bytes, read-only.
    ///
    /// \param[in] aValue New device version, maximum 31 characters (31 + NULL for 32 bytes).
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER if the string is too long.
    ///
    virtual PvResult SetDeviceVersion( const PvString &aValue ) = 0;

    ///
    /// \brief Updates the device firmware version string.
    ///
    /// Maps to GigE Vision 0x3FFF0100 bootstrap register: 32 bytes, read-only.
    ///
    /// \param[in] aValue New device version, maximum 31 characters (31 + NULL for 32 bytes).
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER if the string is too long.
    ///
    virtual PvResult SetDeviceFirmwareVersion( const PvString &aValue ) = 0;

    ///
    /// \brief Updates the manufacturer information string.
    ///
    /// Maps to GigE Vision 0x00A8 bootstrap register: 48 bytes, read-only.
    ///
    /// If the #PvSoftDeviceGEV is running without a valid license, the manufacturer information
    /// is overridden to provide a warning that indicates that the device is unlicensed.
    ///
    /// \param[in] aValue New device version, maximum 47 characters (47 + NULL for 48 bytes).
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER if the string is too long.
    ///
    virtual PvResult SetManufacturerInformation( const PvString &aValue ) = 0;

    ///
    /// \brief Updates the serial number string.
    ///
    /// Maps to GigE Vision 0x00D8 bootstrap register: 16 bytes, read-only.
    ///
    /// \param[in] aValue New serial number, maximum 15 characters (15 + NULL for 16 bytes).
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER if the string is too long.
    ///
    virtual PvResult SetSerialNumber( const PvString &aValue ) = 0;

    ///
    /// \brief Returns the GenICam XML file major version.
    ///
    /// See #SetGenICamXMLVersion for more information.
    ///
    /// \return Major version.
    ///
    virtual uint16_t GetGenICamXMLVersionMajor() const = 0;

    ///
    /// \brief Returns the GenICam XML file minor version.
    ///
    /// See #SetGenICamXMLVersion for more information.
    ///
    /// \return Minor version.
    ///
    virtual uint16_t GetGenICamXMLVersionMinor() const = 0;

    ///
    /// \brief Returns the GenICam XML file subminor version.
    ///
    /// See #SetGenICamXMLVersion for more information.
    ///
    /// \return Subminor version.
    ///
    virtual uint16_t GetGenICamXMLVersionSubMinor() const = 0;

    ///
    /// \brief Returns the GenICam XML file Product GUID.
    ///
    /// See #SetGenICamXMLGUIDs for more information.
    ///
    /// \return Product GUID.
    ///
    virtual const PvString &GetGenICamXMLProductGUID() = 0;

    ///
    /// \brief Returns the GenICam XML file Version GUID.
    ///
    /// See #SetGenICamXMLGUIDs for more information.
    ///
    /// \return Version GUID.
    ///
    virtual const PvString &GetGenICamXMLVersionGUID() = 0;

    ///
    /// \brief Returns the GenICam XML Register Description model name.
    ///
    /// See #SetGenICamXMLRegDescModelName for more information.
    ///
    /// \return XML Register Description Model Name
    ///
    virtual const PvString &GetGenICamXMLRegDescModelName() { return GetModelName(); } 

    ///
    /// \brief Returns the GenICam XML Register Description vendor name.
    ///
    /// See #SetGenICamXMLRegDescVendorName for more information.
    ///
    /// \return XML Register Description Vendor Name
    ///
    virtual const PvString &GetGenICamXMLRegDescVendorName() { return GetManufacturerName(); } 

    ///
    /// \brief Sets the GenICam XML file version.
    ///
    /// See the IPvSoftDeviceGEVInfo class description for usage and recommendations.
    ///
    /// \param[in] aMajor Major version for GenICam XML file header.
    /// \param[in] aMinor Minor version for GenICam XML file header.
    /// \param[in] aSubMinor Subminor version for GenICam XML file header.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::STATE_ERROR if the GenICam XML file has already been created.
    ///
    virtual PvResult SetGenICamXMLVersion( uint16_t aMajor, uint16_t aMinor, uint16_t aSubMinor ) = 0;

    ///
    /// \brief Sets the GenICam XML file version.
    ///
    /// A GUID (globally unique identifier) is a 128-bit number that is used to identify information in computer
    /// systems. The term UUID (universally unique identifier) can also be used.
    ///
    /// Valid GUID example: CB9328DB-1273-42A3-90C7-EB3224537C39
    ///
    /// You can generate unique GUIDs using the Create GUID tool in Visual Studio, which can be accessed from the 
    /// Visual Studio Tools menu. It uses the system unique MAC address and date/time components to ensure uniqueness.
    ///
    /// See the IPvSoftDeviceGEVInfo class description for usage and recommendations.
    ///
    /// \param[in] aProduct Product GUID for GenICam XML file header.
    /// \param[in] aVersion Version GUID for GenICam XML file header.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER if one or both GUIDs are not valid.
    ///     - #PvResult::Code::STATE_ERROR if the GenICam XML file has already been created.
    ///
    virtual PvResult SetGenICamXMLGUIDs( const PvString &aProduct, const PvString &aVersion ) = 0;

    ///
    /// \brief Set the GenICam XML register description model name.
    ///
    /// Sets the model name attribute of the RegisterDescription in the device XML file
    /// If not called GenICamXML register description model name will be the same as the device model name.
    ///
    /// \param[in] aModelName GenICam XML register description model name, maximum 31 characters (31 + NULL for 32 bytes).
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER if the string is too long.
    ///
    virtual PvResult SetGenICamXMLRegDescModelName( const PvString &aModelName ){ PVUNREFPARAM( aModelName); return PvResult::Code::OK; };

    ///
    /// \brief Set the GenICam XML register description vendor name.
    ///
    /// Sets the vendor name attribute of the RegisterDescription in the device XML file
    /// If not called GenICamXML register description vendor name will be the same as the device manufacturer name.
    ///
    /// \param[in] aModelName GenICam XML register description vendor name, maximum 31 characters (31 + NULL for 32 bytes).
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER if the string is too long.
    ///
    virtual PvResult SetGenICamXMLRegDescVendorName( const PvString &aVendorName ){ PVUNREFPARAM( aVendorName ); return PvResult::Code::OK; };

};


///
/// \class IPvSoftDeviceGEVStatistics
/// \brief Interface that is used to retrieve control channel statistics from a software-based GigE Vision Device.
///
/// Can be retrieved from a properly initialized #PvSoftDeviceGEV to access
/// the statistics provided by this interface.
///
/// This class cannot be instantiated and there is no use case where you would 
/// need to implement this interface.
///
class PV_VIRTUAL_DEVICE_API IPvSoftDeviceGEVStatistics
{
public:

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~IPvSoftDeviceGEVStatistics() {}

    ///
    /// \brief Resets all counters to zero.
    ///    
    virtual void Reset() = 0;

    ///
    /// \brief GVCP read memory (READMEM_CMD) commands received.
    ///
    /// \return Read memory command count.
    ///
    virtual uint64_t GetReadMemsReceived() const = 0;

    ///
    /// \brief GVCP write memory (WRITEMEM_CMD) commands received.
    ///
    /// \return Write memory command count.
    ///
    virtual uint64_t GetWriteMemsReceived() const = 0;

    ///
    /// \brief GVCP read register (READREG_CMD) commands received.
    ///
    /// \return Read register command count.
    ///
    virtual uint64_t GetReadRegsReceived() const = 0;

    ///
    /// \brief GVCP write register (WRITEREG_CMD) commands received.
    ///
    /// \return Write register command count.
    ///
    virtual uint64_t GetWriteRegsReceived() const = 0;

    ///
    /// \brief GVCP discovery (DISCOVERY_CMD) commands received.
    ///
    /// \return Discovery command count.
    ///
    virtual uint64_t GetDiscoveriesReceived() const = 0;

    ///
    /// \brief GVCP action command (ACTION_CMD) received.
    ///
    /// \return Action command count.
    ///
    virtual uint64_t GetActionCommandsReceived() const = 0;

    ///
    /// \brief GVCP force IP configuration (FORCEIP_CMD) commands received.
    ///
    /// \return Force IP configuration command count.
    ///
    virtual uint64_t GetForceIpsReceived() const = 0;

    ///
    /// \brief GVCP packet resend (PACKETRESEND_CMD) commands received.
    ///
    /// \return Packet resend command count.
    ///
    virtual uint64_t GetPacketResendRequestsReceived() const = 0;

    ///
    /// \brief GVCP pending acknowledges (PENDING_ACK) issued.
    ///
    /// \return Pending acknowledges count.
    ///
    virtual uint64_t GetPendingAcksIssued() const = 0;

    ///
    /// \brief GVCP command retries received.
    ///
    /// \return Command retries count.
    ///
    virtual uint64_t GetRetriesReceived() const = 0;

    ///
    /// \brief GVCP command retries maximum for a single command.
    ///
    /// \return Command retries maximum count.
    ///
    virtual uint64_t GetRetriesReceivedMax() const = 0;

    ///
    /// \brief GVCP messaging channel event commands (EVENT_CMD) issued.
    ///
    /// \return Messaging channel event count.
    ///
    virtual uint64_t GetEventsIssued() const = 0;

    ///
    /// \brief GVCP messaging channel event (with data) comands (EVENTDATA_CMD) issued.
    ///
    /// \return Messaging channel event (with data) count.
    ///
    virtual uint64_t GetEventDatasIssued() const = 0;

    ///
    /// \brief GVCP retries issued for unacknowledged messaging channel event or event data commands.
    ///
    /// \return Messaging channel event retries count.
    ///
    virtual uint64_t GetEventRetriesIssued() const = 0;

    ///
    /// \return GVCP maximum retries issued for a single unacknowledged messaging channel event or event data command.
    ///
    ///
    /// \return Messaging channel event retries maximum count.
    ///
    virtual uint64_t GetEventRetriesIssuedMax() const = 0;

    ///
    /// \brief Returns the device controller session open count.
    ///
    /// \return Session open count.
    ///
    virtual uint64_t GetSessionsOpened() const = 0;

    ///
    /// \brief Returns the device controller session close count.
    ///
    /// \return Session close count.
    ///
    virtual uint64_t GetSessionsClosed() const = 0;

    ///
    /// \brief Returns the device controller session close on heartbeat failure count.
    ///
    /// \return Session close on heartbeat failure count.
    ///
    virtual uint64_t GetHeartbeatFailedCount() const = 0;

    ///
    /// \brief Returns the number of GVSP test packets issued.
    ///
    /// \return GVSP test packets count.
    ///
    virtual uint64_t GetGVSPTestPacketsIssued() const = 0;

};


///
/// \class IPvMessageChannel
/// \brief Messaging channel interface of the software-based GigE Vision Device.
///
/// Can be retrieved from a properly initialized #PvSoftDeviceGEV to monitor
/// the messaging channel state and send events to the GigE Vision controller.
///
/// This class cannot be instantiated and there is no use case where you would 
/// need to implement this interface.
///
class PV_VIRTUAL_DEVICE_API IPvMessageChannel
{
public:

    ///
    /// \brief Destructor.
    ///
    virtual ~IPvMessageChannel() {}

    ///
    /// \brief Fires a messaging channel event.
    ///
    /// Sends a messaging channel event to the messaging channel destination that is currently registered.
    /// An event ID needs to be provided.
    ///
    /// If the event is sent in the context of a streaming event, we recommend that you set the block ID 
    /// and streaming channel that the event is associated to.
    ///
    /// The default value for the block ID and channel index when an event is not associated to a streaming
    /// event is zero.
    ///
    /// \param[in] aEventID Event ID. Use standard event codes or a value between 0x9000 and 0xFFFF for custom events.
    /// \param[in] aBlockID Block ID that the event is related to or zero.
    /// \param[in] aChannelIndex Channel Index that the event is related to or 0xFFFF if no stream channel involved.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::STATE_ERROR if the messaging channel is not opened.
    ///
    virtual PvResult FireEvent( uint16_t aEventID, uint64_t aBlockID = 0, uint16_t aChannelIndex = 0xFFFF ) = 0;

    ///
    /// \brief Fires a messaging channel event.
    ///
    /// Sends a messaging channel event with data to the messaging channel destination that is currently registered.
    /// An event ID needs to be provided.
    ///
    /// If the event is sent in the context of a streaming event, we recommend that you set the block ID 
    /// and streaming channel that the event is associated to.
    ///
    /// The default value for the block ID and channel index when an event is not associated to a streaming
    /// event is zero.
    ///
    /// It is possible to map event data to the GenApi interface of the #PvSoftDeviceGEV. See #IPvGenApiFactory::MapEvent for more information.
    ///
    /// \param[in] aEventID Event ID. Use standard event codes or a value between 0x9000 and 0xFFFF for custom events.
    /// \param[in] aData Pointer to the data attached to the event.
    /// \param[in] aDataLength Length of the aData buffer. Must be 4 or higher and 536 or lower. Must also be divisible by 4.
    /// \param[in] aBlockID Block ID that the event is related to or zero.
    /// \param[in] aChannelIndex Channel index that the event is related to or 0xFFFF if no stream channel involved.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::STATE_ERROR if the messaging channel is not opened.
    ///     - #PvResult::Code::INVALID_PARAMETER if the data is NULL, data length is too small, too large, or not divisible by 4.
    ///
    virtual PvResult FireEvent( uint16_t aEventID, uint8_t *aData, uint32_t aDataLength, uint64_t aBlockID = 0, uint16_t aChannelIndex = 0xFFFF ) = 0;

    ///
    /// \brief Indicates whether the messaging channel is open.
    ///
    /// For the messaging channel to be opened, a GigE Vision controller must be connected
    /// to the #PvSoftDeviceGEV and must have set the messaging channel destination 
    /// on the MCDA (0x0B10) and MCP (0x0B00) registers.
    ///
    /// \return True if open.
    ///
    virtual bool IsOpened() = 0;

};


///
/// \class IPvSoftDeviceGEVEventSink
/// \brief Interface that is used by #PvSoftDeviceGEV to report internal events.
///
/// To receive these events, inherit one of your classes from IPvSoftDeviceGEVEventSink,
/// instantiate an object from your class, and pass a pointer to this event sink to
/// #PvSoftDeviceGEV::RegisterEventSink.
///
/// When the events occur, the corresponding method in your class will be invoked.
/// The events are usually invoked from a different thread, other than the main application thread.
///
/// It is possible to use this callback interface to create device-scope and GenApi parameters. See 
/// #IPvSoftDeviceGEVEventSink::OnCreateCustomGenApiFeatures and #IPvSoftDeviceGEVEventSink::OnCreateCustomRegisters for 
/// more information. You should create streaming channel source GenApi feature and registers for 
/// source-specific features like deinterlacing, gain, exposure time. Look at 
/// #IPvStreamingChannelSource::CreateRegisters and #IPvStreamingChannelSource::CreateGenApiFeatures 
/// to create streaming channel source scope features. Note that chunks and messaging channel GenApi parameters should always 
/// be created from IPvSoftDeviceGEVEventSink and be at the device-scope.
///
class PV_VIRTUAL_DEVICE_API IPvSoftDeviceGEVEventSink
{
public:

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~IPvSoftDeviceGEVEventSink() {}

    ///
    /// \brief Notification that a GigE Vision controller has taken ownership of the control channel of your #PvSoftDeviceGEV.
    ///
    /// \param[in] aDevice Pointer to the #PvSoftDeviceGEV object that is sending the notification.
    /// \param[in] aIPAddress IP address of the GigE Vision controller in string format, such as "192.168.138.115".
    /// \param[in] aPort UDP port of the GigE Vision controller.
    /// \param[in] aAccessType Access type secured by the GigE Vision controller.
    ///
    virtual void OnApplicationConnect( IPvSoftDeviceGEV *aDevice, const PvString &aIPAddress, uint16_t aPort, PvAccessType aAccessType ) 
    {
        PVUNREFPARAM( aDevice );
        PVUNREFPARAM( aIPAddress );
        PVUNREFPARAM( aPort );
        PVUNREFPARAM( aAccessType );
    }

    ///
    /// \brief Notification that the control channel of the #PvSoftDeviceGEV has been started.
    ///
    /// The control channel is the GVCP command server of the #PvSoftDeviceGEV.
    ///
    /// \param[in] aDevice Pointer to the #PvSoftDeviceGEV object that is sending the notification.
    /// \param[in] aMACAddress MAC address of the local network interface.
    /// \param[in] aIPAddress IP address that the control channel is bound to.
    /// \param[in] aMask Subnet mask of the IP address that the control channel is bound to.
    /// \param[in] aGateway Default gateway of the network interface that is used by the control channel.
    /// \param[in] aPort UDP port that the control channel is listening to. Hardcode this value to GigE Vision standard 3956.
    ///
    virtual void OnControlChannelStart( IPvSoftDeviceGEV *aDevice, const PvString &aMACAddress, const PvString &aIPAddress, const PvString &aMask, const PvString &aGateway, uint16_t aPort )
    {
        PVUNREFPARAM( aDevice ); 
        PVUNREFPARAM( aMACAddress );
        PVUNREFPARAM( aIPAddress );
        PVUNREFPARAM( aMask );
        PVUNREFPARAM( aGateway );
        PVUNREFPARAM( aPort );
    }

    ///
    /// \brief Notification that the GigE Vision controller has released the control channel of the #PvSoftDeviceGEV.
    ///
    /// The notification can be triggered if the control channel has been released by the #PvSoftDeviceGEV
    /// on a heartbeat refresh failure.
    ///
    /// \param[in] aDevice Pointer to the #PvSoftDeviceGEV object that is sending the notification.
    ///
    virtual void OnApplicationDisconnect( IPvSoftDeviceGEV *aDevice ) { PVUNREFPARAM( aDevice ); }

    ///
    /// \brief Notification that the control channel of the #PvSoftDeviceGEV has been stopped.
    ///
    /// The control channel is the GVCP command server of the #PvSoftDeviceGEV.
    ///
    /// \param[in] aDevice Pointer to the #PvSoftDeviceGEV object that is sending the notification.
    ///
    virtual void OnControlChannelStop( IPvSoftDeviceGEV *aDevice ) { PVUNREFPARAM( aDevice ); }

    ///
    /// \brief Notification that the #PvSoftDeviceGEV has performed a full reset.
    ///
    /// A full reset is the equivalent of a power cycle.
    ///
    /// \param[in] aDevice Pointer to the #PvSoftDeviceGEV object that is sending the notification.
    ///
    virtual void OnDeviceResetFull( IPvSoftDeviceGEV *aDevice ) { PVUNREFPARAM( aDevice ); }

    ///
    /// \brief Notification that the #PvSoftDeviceGEV has reset its network stack.
    ///
    /// A #PvSoftDeviceGEV network reset usually happens after it has received a FORCEIP_CMD
    /// command.
    ///
    /// \param[in] aDevice Pointer to the #PvSoftDeviceGEV object that is sending the notification.
    ///
    virtual void OnDeviceResetNetwork( IPvSoftDeviceGEV *aDevice ) { PVUNREFPARAM( aDevice ); }

    ///
    /// \brief Notification that the #PvSoftDeviceGEV should create its custom registers, if they are needed.
    ///
    /// Register creation must happen at a specific point in the #PvSoftDeviceGEV initialization.
    /// To create custom registers in your application, implement the IPvSoftDeviceGEVEventSink interface,
    /// register an object of this class to #PvSoftDeviceGEV::RegisterEventSink, and perform custom register
    /// creation in the #IPvSoftDeviceGEVEventSink::OnCreateCustomRegisters notification.
    ///
    /// Custom register creation occurs BEFORE the creation of custom GenApi features,
    /// inside the #PvSoftDeviceGEV::Start call.
    ///
    /// \param[in] aDevice Pointer to the #PvSoftDeviceGEV object that is sending the notification.
    /// \param[in] aFactory Register factory that is used to create the new registers. 
    ///
    virtual void OnCreateCustomRegisters( IPvSoftDeviceGEV *aDevice, IPvRegisterFactory *aFactory ) { PVUNREFPARAM( aDevice ); PVUNREFPARAM( aFactory ); }

    ///
    /// \brief Notification that the #PvSoftDeviceGEV should create its custom GenApi parameters, if they are needed.
    ///
    /// GenApi feature creation must happen at a specific point in the #PvSoftDeviceGEV initialization.
    /// To create custom GenApi features in your application, implement the IPvSoftDeviceGEVEventSink interface,
    /// register an object of this class to #PvSoftDeviceGEV::RegisterEventSink, and perform custom GenApi feature
    /// creation in the #IPvSoftDeviceGEVEventSink::OnCreateCustomGenApiFeatures notification.
    ///
    /// Custom GenApi feature creation occurs AFTER the creation of custom registers,
    /// inside the #PvSoftDeviceGEV::Start call.
    ///
    /// \param[in] aDevice #PvSoftDeviceGEV object that is sending the notification.
    /// \param[in] aFactory GenApi factory that is used to create the new GenApi features. 
    ///
    virtual void OnCreateCustomGenApiFeatures( IPvSoftDeviceGEV *aDevice, IPvGenApiFactory *aFactory ) { PVUNREFPARAM( aDevice ); PVUNREFPARAM( aFactory ); }
};


///
/// \class IPvStreamingChannelSource
/// \brief Interface that is used by #PvSoftDeviceGEV to query and report information from a streaming source implemented by your application.
///
/// For the application to provide a streaming source to your software-based GigE Vision Device,
/// inherit one of your classes from IPvStreamingChannelSource, implement all of the methods, instantiate an object 
/// from your class, and pass a pointer to your class to #PvSoftDeviceGEV::AddStream.
///
/// The GigE Vision Device will use this interface to query the application about stream
/// capabilities and to report streaming events, such as acquisition start and stop. The GigE Vision Device also uses this interface
/// to manage streaming data.
///
/// To properly report the PayloadSize GenApi parameter, your application must provide
/// the GigE Vision Device with the width, height, pixel format, and data chunk size values using
/// the GetWidth, GetHeight, GetPixelFormat, and GetChunksSize methods.
///
/// You can hardcode the supported chunk size by returning the required chunk size
/// using GetChunksSize. You could also provide more information about the supported chunk types
/// using GetSupportedChunk. Doing so will allow the GigE Vision Device to properly populate
/// the ChunkSelector of its GenApi interface to let the user decide which chunks it should
/// support. The streaming source should decide whether or not to append a specific chunk type
/// by looking at whether the master chunk active mode is enabled AND that the specific chunk type is enabled.
///
/// It is possible to create streaming channel source custom registers and GenApi parameters. See 
/// #IPvStreamingChannelSource::CreateGenApiFeatures and #IPvStreamingChannelSource::CreateRegisters for more information.
/// You should create streaming channel source GenApi feature and registers for source-specific features like
/// deinterlacing, gain, exposure time. Look at #IPvSoftDeviceGEVEventSink::OnCreateCustomRegisters and
/// #IPvSoftDeviceGEVEventSink::OnCreateCustomGenApiFeatures to create device-scope features. Note that chunks and messaging
/// channel GenApi parameters should always be created from IPvSoftDeviceGEVEventSink and be at the device-scope.
///
/// Here is what a typical streaming session would look like when it comes to buffer management:
///     - A #PvSoftDeviceGEV object is instantiated by your application
///     - A MySource object that implements the #IPvStreamingChannelSource interface is instantiated by your application
///     - #PvSoftDeviceGEV::AddStream is called by your application with a pointer to MySource
///     - #PvSoftDeviceGEV::Start is called by your application
///         - #AllocBuffer is called to allow your application to allocate streaming buffers until NULL is returned or the maximum number of buffers is reached
///         - #GetWidthInfo, #GetHeightInfo, and #GetSupportedPixelType are called on MySource to inquire about the image attributes supported by MySource
///         - #OnOpen is called on MySource when the streaming channel is opened (when a GigE Vision controller connects to your GigE Vision Device)
///             - #OnStreamingStart is called on MySource to notify the application that streaming is starting (when the GigE Vision controller calls AcquisitionStart)
///                 - Internal acquisition loop run from a GigE Vision Device thread:
///                     - Calls #QueueBuffer on MySource until it fails to provide MySource with buffers to fill with image data
///                     - Calls #RetrieveBuffer on MySource until it fails to retrieve all MySource filled buffers that are ready for transmission
///                     - End of acquisition loop
///             - #OnStreamingStop is called on MySource to notify the application that streaming is stopping (when the GigE Vision controller calls AcquisitionStop)
///                 - #AbortQueuedBuffers is called on MySource
///                 - #RetrieveBuffer is called on MySource until it fails to retrieve all MySource buffers, whether they were acquired or aborted
///         - #OnClose is called on MySource when the streaming channel is closed (when the GigE Vision controller disconnects from the GigE Vision Device)
///     - #PvSoftDeviceGEV::Stop is called explicitly by your application or by the #PvSoftDeviceGEV destructor
///
class PV_VIRTUAL_DEVICE_API IPvStreamingChannelSource
{
public:

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~IPvStreamingChannelSource() {}

    ///
    /// \brief Returns the current image width.
    ///
    /// Your application must implement this method and return the current image width when invoked.
    ///
    /// \return Current image width.
    ///
    virtual uint32_t GetWidth() const = 0;

    ///
    /// \brief Returns the current image height.
    ///
    /// Your application must implement this method and return the current image height when invoked.
    ///
    /// \return Current image height.
    ///
    virtual uint32_t GetHeight() const = 0;

    ///
    /// \brief Returns the current image offset X.
    ///
    /// Your application must implement this method and return the current image offset X when invoked.
    ///
    /// \return Current image offset X.
    ///
    virtual uint32_t GetOffsetX() const = 0;

    ///
    /// \brief Returns the current image offset Y.
    ///
    /// Your application must implement this method and return the current image offset Y when invoked.
    ///
    /// \return Current image offset Y.
    ///
    virtual uint32_t GetOffsetY() const = 0;

    ///
    /// \brief Returns the current pixel type.
    ///
    /// Your application must implement this method and return the current image pixel type when invoked.
    ///
    /// \return Current pixel type.
    ///
    virtual PvPixelType GetPixelType() const = 0;

    /// \brief Returns the stream channel width attributes.
    ///
    /// When using the GigE Vision Device as a transmitter, the Width parameter
    /// is automatically added to the device's GenICam XML file. This method is
    /// used to query the minimum, maximum, and increment attributes for the Width parameter.
    ///
    /// \param[out] aMin Width minimum.
    /// \param[out] aMax Width maximum.
    /// \param[out] aInc Width increment.
    ///
    virtual void GetWidthInfo( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) const = 0;

    /// \brief Returns the stream channel height attributes.
    ///
    /// When using the GigE Vision Device as a transmitter, the Height parameter
    /// is automatically added to the device's GenICam XML file. This method is
    /// used to query the minimum, maximum, and increment attributes for the Height parameter.
    ///
    /// \param[out] aMin Height minimum.
    /// \param[out] aMax Height maximum.
    /// \param[out] aInc Height increment.
    ///
    virtual void GetHeightInfo( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) const = 0;

    ///
    /// \brief Gets the stream channel chunk size attributes.
    ///
    /// When using data chunks, it is important to report the chunk data size when
    /// queried through this method.
    ///
    /// The chunk data size needs to be taken into account when reporting the PayloadSize
    /// GenICam parameter to the GigE Vision controller, so it can properly allocate
    /// its buffer for streaming without forced buffer re-allocation.
    ///
    /// The size reported must for each chunk adds 4 bytes for the chunk ID and another
    /// 4 bytes for the chunk length fields. If you have a chunk that is 64 bytes long and it is your
    /// only data chunk, you need to report 64 + 4 + 4 (72 bytes) when GetChunksSize is called.
    ///
    /// \return Chunk data size as currently configured.
    ///
    virtual uint32_t GetChunksSize() const = 0;

    ///
    /// \brief Stream channel payload size in bytes. Return zero to have it inferred from width, height and pixel format.
    ///
    /// If not using image streams (raw data, multi-part, etc.) use implement the GetPayloadSize method and return
    /// the payload size for the current streaming channel source configuration. If GetPayloadSize returns 0 the
    /// payload size will be inferred from the width, height, pixel format and data chunk size.
    ///
    /// When using multi-part, return the sum of the maximum size of all parts plus data chunk.
    ///
    /// When payload size is not zero, the payload size value is simply used as provided. Nothing is added
    /// to it, not even data chunk size.
    ///
    /// \return Payload size to use or 0.
    ///
    virtual uint32_t GetPayloadSize() const = 0;

    ///
    /// \brief Returns the scan type that is used by the streaming source.
    ///
    /// Return PvScanTypeArea if your image source is areascan or PvScanTypeLine if your
    /// image source is linescan.
    ///
    /// \return Scan type of the streaming source.
    ///
    virtual PvScanType GetScanType() const = 0;

    ///
    /// \brief Indicates whether the current master chunk mode is active.
    ///
    /// The application must implement this method and return the current master chunk mode state.
    /// Return false as a hardcoded value if chunks are not supported by the streaming source.
    ///
    /// \return True if chunk mode is enabled for this streaming source.
    ///
    virtual bool GetChunkModeActive() const = 0;

    ///
    /// \brief Returns the current chunk enabled state.
    ///
    /// The application must implement this method and return the current chunk enabled state
    /// for the specified chunk ID. The streaming source should maintain an enabled/disabled
    /// state for each chunk type that it supports.
    ///
    /// The application should append the chunk represented by the specified chunk ID if it is enabled
    /// and if the master chunk mode (see #GetChunkModeActive) is true.
    ///
    /// \param[in] aChunkID Chunk ID.
    ///
    /// \return True if the chunk specified by aChunk ID is enabled for this streaming source.
    ///
    virtual bool GetChunkEnable( uint32_t aChunkID ) const = 0;

    ///
    /// \brief Obtains an index of supported pixel types. Used at #PvSoftDeviceGEV::Start time to create the GenICam XML of the device.
    ///
    /// This method is used by the #PvSoftDeviceGEV to query the application
    /// about the supported pixel types for the streaming channel.
    ///
    /// The PvSoftDeviceGEV will query the application using this stream
    /// interface for all supported pixel types. It will start at index 0 and then increase
    /// until something other than #PvResult::Code::OK is returned.
    ///
    /// If the application supports Mono8, BayerRG8, and RGBa8 it needs to
    /// handle the request to GetSupportedPixelType, as follows:
    ///     - GetSupportedPixelType( 0 ) -> #PvResult::Code::OK, PvPixelMono8
    ///     - GetSupportedPixelType( 1 ) -> #PvResult::Code::OK, PvPixelBayerRG8
    ///     - GetSupportedPixelType( 2 ) -> #PvResult::Code::OK, PvPixelRGBa8
    ///     - GetSupportedPixelType( 3 ) -> #PvResult::Code::INVALID_PARAMETER
    ///
    /// \param[in] aIndex Index of the pixel type to return.
    /// \param[out] aPixelType PvPixelType requested by aIndex.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK if aIndex represents a valid index and aPixelType has been set.
    ///     - #PvResult::Code::INVALID_PARAMETER or any other error code if aIndex is not valid.
    ///
    virtual PvResult GetSupportedPixelType( int aIndex, PvPixelType &aPixelType ) const = 0;

    ///
    /// \brief Returns the indexed supported chunk type. Used at #PvSoftDeviceGEV::Start time to create the GenICam XML of the device.
    ///
    /// This method is used by the PvSoftDeviceGEV to query the application
    /// about the supported chunk types for this streaming channel.
    ///
    /// The PvSoftDeviceGEV will query the application through this stream
    /// interface for all supported chunk types. It will start at index 0 and then increase
    /// until something other than #PvResult::Code::OK is returned.
    ///
    /// The information retrieved through this method is used to populate the chunk selector, allowing
    /// the GigE Vision controller to enable or disable each chunk type individually.
    ///
    /// If the application supports ChunkSample (ID 0x4001) and ChunkHistogram (ID 0x4002) it needs to
    /// handle request to GetSupportedChunk as follows:
    ///     - GetSupportedChunk( 0 ) -> #PvResult::Code::OK, aID = 0x4001, aName = "ChunkSample"
    ///     - GetSupportedChunk( 1 ) -> #PvResult::Code::OK, aID = 0x4002, aName = "ChunkHistogram"
    ///     - GetSupportedChunk( 2 ) -> #PvResult::Code::INVALID_PARAMETER
    ///
    /// \param[in] aIndex Index of the chunk type to return.
    /// \param[out] aID ID of the chunk type requested by aIndex.
    /// \param[out] aName Name of the chunk type requested by aIndex. Should always use the "Chunk" prefix for GenICam SFNC compliance.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK if aIndex represents a valid index and aPixelType has been set.
    ///     - #PvResult::Code::INVALID_PARAMETER or any other error code if aIndex is not valid.
    ///
    virtual PvResult GetSupportedChunk( int aIndex, uint32_t &aID, PvString &aName ) const = 0;

    ///
    /// \brief Get the customized max value for GevSCPSPacketSize. Used at #PvSoftDeviceGEV::Start time to create the GenICam XML of the device.
    ///
    /// This method is optional, it is used by the PvSoftDeviceGEV to query the application
    /// about the max value to use for GenICam node GevSCPSPacketSize to fit your system needs.
    /// The default max value is 9000 bytes if choose not to implement.
    ///
    /// The max value configured here will be used for PacketSize Auto-negotiation by the receiver to determine
    /// the most suitable packet size for the system. The user is able to configure up to 65535 for certain special use case.
    /// 
    /// In order for the eBUS Receiver to negotiate the packet size based on the maximum value configured here, the eBUS Receiver
    /// version needs to be at least 6.5.
    /// 
    /// \param[out] aMax GevSCPSPacketSize maximum.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK if the default aMax have been changed.
    ///     - #PvResult::Code::NOT_IMPLEMENTED or any other error code if to keep the default settings.
    ///
    virtual PvResult GetGevSCPSPacketSizeInfo( uint32_t &aMax ) const
    {
        PVUNREFPARAM( aMax );
        return PvResult::Code::NOT_IMPLEMENTED;
    }

    ///
    /// \brief Request from the #PvSoftDeviceGEV to change the image width.
    ///
    /// The PvSoftDeviceGEV will call SetWidth when
    /// the Width GenApi parameter or the underlying register is
    /// changed by the GigE Vision controller.
    ///
    /// To accept the new width, return #PvResult::Code::OK and update
    /// your streaming code to use the new width.
    ///
    /// \param[in] aWidth New image width.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK when accepting the new width.
    ///     - #PvResult::Code::GENERIC_ERROR or another error code to refuse the new width.
    ///
    virtual PvResult SetWidth( uint32_t aWidth ) = 0;

    ///
    /// \brief Request from the PvSoftDeviceGEV to change the image height.
    ///
    /// The PvSoftDeviceGEV will call SetHeight when
    /// the Height GenApi parameter or the underlying register is
    /// changed by the GigE Vision controller.
    ///
    /// To accept the new height, return #PvResult::Code::OK and update
    /// your streaming code to use the new height.
    ///
    /// \param[in] aHeight New image height.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK when accepting the new height.
    ///     - #PvResult::Code::GENERIC_ERROR or another error code to refuse the new height.
    ///
    virtual PvResult SetHeight( uint32_t aHeight ) = 0;

    ///
    /// \brief Request from the #PvSoftDeviceGEV to change the image offset X.
    ///
    /// To accept the new offset X, return #PvResult::Code::OK and update
    /// your streaming code to use the new value.
    ///
    /// \param[in] aOffsetX New image offset X.
    ///
    /// \return 
    ///     Includes:
    ///         - #PvResult::Code::OK when accepting the new value.
    ///         - #PvResult::Code::GENERIC_ERROR or another error code to refuse the new value.
    ///
    virtual PvResult SetOffsetX( uint32_t aOffsetX ) = 0;

    ///
    /// \brief Request from the PvSoftDeviceGEV to change the image offset Y.
    ///
    /// To accept the new offset Y, return #PvResult::Code::OK and update
    /// your streaming code to use the new value.
    ///
    /// \param[in] aOffsetY New image offset Y.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK when accepting the new value.
    ///     - #PvResult::Code::GENERIC_ERROR or another error code to refuse the new value.
    ///
    virtual PvResult SetOffsetY( uint32_t aOffsetY ) = 0;

    ///
    /// \brief Request from the #PvSoftDeviceGEV to change the image pixel type.
    ///
    /// The #PvSoftDeviceGEV will call SetPixelType when
    /// the PixelType GenApi parameter or the underlying register is
    /// changed by the GigE Vision controller.
    ///
    /// To accept the new pixel type, return #PvResult::Code::OK and update
    /// your streaming code to use the new pixel type.
    ///
    /// \param[in] aPixelType New image pixel type.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK when accepting the new pixel type.
    ///     - #PvResult::Code::GENERIC_ERROR or another error code to refuse the new pixel type.
    ///
    virtual PvResult SetPixelType( PvPixelType aPixelType ) = 0;

    ///
    /// \brief Set the master chunk mode active state.
    ///
    /// The streaming source should append a specific chunk type to its streaming data
    /// when both the #GetChunkModeActive (master chunk mode) and #GetChunkEnable for this
    /// specific chunk type are true.
    ///
    /// \param[in] aEnabled True to enable chunk mode for this streaming channel, false to disable it.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK to accept the new setting.
    ///     - #PvResult::Code::GENERIC_ERROR or another error code to refuse the new setting.
    ///
    virtual PvResult SetChunkModeActive( bool aEnabled ) = 0;

    ///
    /// \brief Sets the chunk enabled state for a specific chunk type.
    ///
    /// The streaming source should append a specific chunk type to its streaming data
    /// when both the #GetChunkModeActive (master chunk mode) and #GetChunkEnable for this
    /// specific chunk type are true.
    ///
    /// \param[in] aChunkID Chunk ID of the chunk type to enable or disable.
    /// \param[in] aEnabled True to enable the chunk type, false to disable it.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK to accept the new setting.
    ///     - #PvResult::Code::GENERIC_ERROR or another error code to refuse the new setting.
    ///
    virtual PvResult SetChunkEnable( uint32_t aChunkID, bool aEnabled ) = 0;

    ///
    /// \brief Notification from the #PvSoftDeviceGEV that this streaming channel has been opened.
    ///
    /// \param[in] aDestIP IP address of the stream destination as a string in the "192.168.138.115" format (SCDA).
    /// \param[in] aDestPort UDP port of the stream destination (SCP).
    ///
    virtual void OnOpen( const PvString &aDestIP, uint16_t aDestPort ) = 0;

    ///
    /// \brief Notification from the PvSoftDeviceGEV that this streaming channel has been closed.
    ///
    virtual void OnClose() = 0;

    ///
    /// \brief Notification that the GigE Vision controller has requested that the device start streaming on this channel.
    ///
    virtual void OnStreamingStart() = 0;

    ///
    /// \brief Notification that the GigE Vision controller has requested that the device stop streaming on this channel.
    ///
    virtual void OnStreamingStop() = 0;

    ///
    /// \brief Request by the #PvSoftDeviceGEV to allocate a PvBuffer.
    ///
    /// On initialization, the #PvSoftDeviceGEV will call this method, asking the application to
    /// allocate buffers that will be used for image acquisition and transmission. The application should return 
    /// buffers until it has reached the desired number of buffers it wants to use for streaming and then
    /// return NULL.
    ///
    /// The more buffers you use, the more buffers you can have queued in the application for image acquisition
    /// and the more buffers you can leave in the transmission output queue for packet resend. The
    /// downside of using more buffers is increased memory usage.
    ///
    /// Buffers returned on AllocBuffer calls should be unique: you should not return the same
    /// buffer more than once.
    ///
    /// A maximum of 64 buffers can be used by one streaming channel of the #PvSoftDeviceGEV. If your
    /// application does not return NULL on the 64th buffer, AllocBuffer will not be called a 64th time.
    ///
    /// \return Pointer to a new PvBuffer that can be used by the #PvSoftDeviceGEV's acquisition manager.
    ///
    virtual PvBuffer *AllocBuffer() = 0;

    ///
    /// \brief Request by the #PvSoftDeviceGEV to free or release a buffer allocated with AllocBuffer.
    ///
    /// You should not delete or free any PvBuffer provided by the AllocBuffer method. FreeBuffer will be called
    /// for each buffer created with AllocBuffer when #PvSoftDeviceGEV::Stop is called.
    ///
    /// \param[in] aBuffer Buffer to free.
    ///
    virtual void FreeBuffer( PvBuffer *aBuffer ) = 0;

    ///
    /// \brief Request by the #PvSoftDeviceGEV to queue a buffer for acquisition.
    ///
    /// If the application can accept the buffer and queue it for image acquisition, it 
    /// returns success. If the acquisition queue or pipeline is full, return a PvResult
    /// error code.
    ///
    /// \param[in] aBuffer Buffer to queue for image acquisition.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK to accept the buffer.
    ///     - Any error code to refuse the buffer.
    ///
    virtual PvResult QueueBuffer( PvBuffer *aBuffer ) = 0;

    ///
    /// \fn bool IPvStreamingChannelSource::PreQueueBufferProc()
    ///
    /// This is an advanced feature used mostly internally. For expert users only.
    /// It is higly recommended to not override but use the default implementation.
    ///
    /// \return True if the processing loop can proceed with queueing/retrieving next buffer.
    virtual bool PreQueueBufferProc() { return true; }
    
    ///
    /// \fn void IPvStreamingChannelSource::PostRetrieveBufferProc()
    ///
    /// This is an advanced feature used mostly internally. For expert users only.
    /// It is higly recommended to not override but use the default implementation.
    virtual void PostRetrieveBufferProc() {}

    ///
    /// \brief Request by the #PvSoftDeviceGEV to retrieve a buffer from the streaming source.
    ///
    /// During acquisition, buffers retrieved with this method will contain valid image data and are
    /// going to be transmitted using the GVSP protocol. This method is also used to retrieve queued
    /// buffers after calling AbortQueuedBuffers.
    ///
    /// If no image is immediately available, we recommend that you wait until an image is available
    /// in this method to allow the acquisition thread to wait efficiently for a new image.
    /// If getting a new image can take a long time, your application should wait small chunks of time (10, 20, or 50 ms)
    /// to prevent deadlocking the acquisition thread and to allow the #PvSoftDeviceGEV to
    /// stop streaming gracefully.
    ///
    /// \param[in] aBuffer A pointer to a pointer to a buffer: set to a pointer to the PvBuffer being retrieved.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK if a buffer has been retrieved.
    ///     - Any error code if there are currently no buffers available.
    ///
    virtual PvResult RetrieveBuffer( PvBuffer **aBuffer ) = 0;

    ///
    /// \brief Request by the #PvSoftDeviceGEV to abort all buffers queued for acquisition.
    ///
    /// This method should block until acquisition has been aborted or canceled for all buffers
    /// provided by QueueBuffer that have not yet been retrieved with RetrievedBuffer.
    ///
    /// When this method returns, it should be possible to call RetrieveBuffer for all aborted
    /// buffers.
    ///
    virtual void AbortQueuedBuffers() = 0;

    ///
    /// \brief When called, the streaming channel source should create its custom registers if they are needed.
    ///
    /// To create source-specific custom registers in your application, implement the CreateRegisters method of the
    /// IPvStreamingChannelSource interface.
    ///
    /// Custom source-specific register creation occurs BEFORE custom source-specific GenApi features creation,
    /// inside the #PvSoftDeviceGEV::Start call.
    ///
    /// If more than one streaming channel source is created from the same class, you must make sure its registers
    /// are still unique: RegisterXYZ in Source0 and Source1 should not have the same address. It is also recommended
    /// to assign them unique names like RegisterXYZ0 and RegisterXYZ1.
    ///
    /// \param[in] aRegisterMap The register map of the device.
    /// \param[in] aFactory Register factory used to create the new registers. 
    ///
    virtual void CreateRegisters( IPvRegisterMap *aRegisterMap, IPvRegisterFactory *aFactory )
    {
        PVUNREFPARAM( aRegisterMap );
        PVUNREFPARAM( aFactory );
    }

    ///
    /// \brief When called, the streaming channel source should create its custom GenApi parameters if they are needed.
    ///
    /// Create GenApi features from this callback if the feature is associated with a streaming channel source. For global-device,
    /// non streaming channel source features use #IPvSoftDeviceGEVEventSink::OnCreateCustomGenApiFeatures instead.
    ///
    /// In a multi-source application, GenApi features created by a streaming channel source will only be available when
    /// the SourceSelector is set to the streaming channel source that created the feature.
    ///
    /// If FeatureXYX is created by Source0 and Source1 but not by Source2, the feature will only be available in the GenApi
    /// interface when SourceSelector is set to Source0 or Source1.
    ///
    ///     - MySource0 has Feature0, Feature1, Feature2
    ///     - MySource1 has Feature0, Feature3, Feature4
    ///     - In a GenApi browser with SourceSelector=Source0, Feature0, Feature1, Feature2 are available. Feature3 and Feature4 are disabled.
    ///     - In a GenApi browser with SourceSelector=Source1, Feature0, Feature3, Feature4 are available. Feature1 and Feature2 are disabled.
    ///
    /// If integers with the same name are created for more than one source but with source-specific minimum, maximum and increment
    /// attributes, these attributes will be handled by GenICam SwissKnife operators in the GenICam XML so they are dynamically
    /// adjusted based on the value of SourceSelector. The same applies to float maximum and minimum.
    ///
    ///     - MyInteger is created for Source0 with min:0 max:100 increment:4
    ///     - MyInteger is created for Source1 with min:4 max:200 increment:1
    ///     - In a GenApi browser, MyInteger will report min:0 max:100 increment:4 with Source0 is selected
    ///     - In a GenApi browser, MyInteger will report min:4 max:200 increment:1 with Source1 is selected
    ///
    /// If enumerations with the same name are created for more than one source but with different enum entries, they will be
    /// added to the GenICam XML file of the device in way that the correct enum entries should be available for the enum features
    /// based on the current SourceSelector value:
    ///
    ///     - MyEnum is created with EE0, EE1, EE2 for Source0
    ///     - MyEnum is created with EE0, EE3, EE4 for Source1
    ///     - In a GenApi browser with SourceSelector=Source0, you would see EE0, EE1, EE2 in the combo box
    ///     - In a GenApi browser with SourceSelector=Source1, you would see EE0, EE3, EE4 in the combo box
    ///
    /// Creating features of the same name but different types is not supported. Attempting to do so should have the #PvSoftDeviceGEV::Start
    /// call failing with an appropriate error description.
    ///
    /// GenApi features created for a streaming channel source should use registers unique to this streaming channel source. If you have
    /// a source class MySource that creates registers and features, it should still create unique register having their own unique addresses.
    /// They should however use the same parameter name. The GenApi parameters created by MySource should use the right register for the 
    /// source instance. The complexity of overlapping per-source feature name is handled by PvSoftDeviceGEV when creating the GenICam XML
    /// file from #PvSoftDeviceGEV::Start. 
    ///
    ///     - For Source0:
    ///         - MySource creates MySourceRegister0 @ 0x20000000 + 0x1000 * source index + 0
    ///         - MySource creates MySourceParameter named "MySourceParameter" for Source0 using MySourceRegister0
    ///     - For Source1:
    ///         - MySource creates MySourceRegister1 @ 0x20000000 + 0x1000 * source index + 4
    ///         - MySource creates MySourceParameter named "MySourceParameter" for Source1 using MySourceRegister1
    ///
    /// The exception to this rule is selectors: if selectors are created at the streaming source level
    /// they must all use the same register, sharing the same register address.
    ///
    /// Custom source-specific GenApi features creation occurs AFTER custom source-specific registers creation,
    /// inside the #PvSoftDeviceGEV::Start call.
    ///
    /// \param[in] aRegisterMap The register map of the device.
    /// \param[in] aFactory GenApi factory used to create the new GenApi features. 
    ///
    virtual void CreateGenApiFeatures( IPvRegisterMap *aRegisterMap, IPvGenApiFactory *aFactory ) 
    {
        PVUNREFPARAM( aRegisterMap );
        PVUNREFPARAM( aFactory );
    }

    ///
    /// \brief When called, the streaming source has to return whether the specificed payload type is supported or not.
    ///
    /// Return true if the streaming source exlusively or non-exclusively can stream buffers of the 
    /// specified payload type. A default implementation is provided for this method which returns false.
    ///
    /// This method must be implemented and returning true for aPayloadType being equal to PvPayloadTypeMultiPart
    /// when multi-part is supported by the streaming source.
    ///
    /// See #SetTestPayloadFormatMode and #SetMultiPartAllowed for more information on propertly supporting the multi-part payload type.
    ///
    /// \param aPayloadType [in] The payload type for which support is queried.
    ///
    /// \return Return true if the payload type is supported, false if not.
    ///
    virtual bool IsPayloadTypeSupported( PvPayloadType aPayloadType ) { PVUNREFPARAM( aPayloadType ); return false; }

    ///
    /// \brief Sets whether the streaming source is allowed or not, but not forced, to transmit multi-part data.
    ///
    /// Streaming sources supporting multi-part data transmission must support non-multi-part streaming by default.
    /// All devices starting in a mode where multi-part is not allowed. The GigE Vision controller must unlock
    /// the multi-part capability of a GVSP streaming channel using either the SCCx and SCCFGx bootstrap registers 
    /// or the GevSCCFGMultiPartEnabled GenApi parameter.
    ///
    /// The GevSCCFGMultiPartEnabled GenApi parameter is automatically added to the GenICam XML file
    /// of a PvSoftDeviceGEV device when #IsPayloadTypeSupported returns true when prompted with
    /// PvPayloadTypeMultiPart.
    ///
    /// See #IsPayloadTypeSupported and #SetTestPayloadFormatMode for more information on propertly supporting the multi-part payload type.
    ///
    /// \param aAllowed [in] True if multi-part is allowed.
    ///
    virtual void SetMultiPartAllowed( bool aAllowed ) { PVUNREFPARAM( aAllowed ); }

    ///
    /// \brief Sets whether the streaming source which transmit multi-part data uses large data leader and trailer packets.
    ///
    /// Streaming sources using multi-part data transmission without large data leader and trailer enabled only supports up to 10 multi-parts
    /// due to the data leader and trailer packet size limitation. Large data leader and data trailer packets can be enabled by configuring 
    /// the GevSCCFGLargeLeaderTrailerEnabled GenApi parameter to allow more than 10 parts. 
    /// The large leader and trailer packet size is then set to the value allowed by the SCPSx register.
    /// 
    /// The GevSCCFGLargeLeaderTrailerEnabled GenApi parameter is automatically added to the GenICam XML file
    /// of a PvSoftDeviceGEV device when #IsPayloadTypeSupported returns true when prompted with
    /// PvPayloadTypeMultiPart. This parameter will only be available when GevSCCFGMultiPartEnabled is set to true.
    ///
    /// \param aEnabled [in] True if Large Leader Trailer is enabled.
    /// \param aMaxMultiPartCount [in] The number of maximum allowed Multi parts calculated from the value allowed by the SCPSx register.
    ///
    virtual void SetLargeLeaderTrailerEnabled( bool aEnabled, uint32_t aMaxMultiPartCount )
    {
        PVUNREFPARAM( aEnabled );
        PVUNREFPARAM( aMaxMultiPartCount );
    }

    ///
    /// \brief When called, the streaming source has to ensure it can stream buffers of the specified payload type.
    ///
    /// The GigE Vision Validation Framework has to be able to set the device in a functional streaming mode
    /// for some payload types (currently only multi-part with more to come) in order to properly validate the device.
    ///
    /// This method must be implemented and for aPayloadType being PvPayloadTypeMultiPart
    /// when multi-part is supported by the streaming source. The streaming source must be able to configure itself
    /// in a mode where it will be able to stream data for certification with the GigE Vision Validation Framework.
    ///
    /// Of course, this method will only be called on a streaming source for payload type that have
    /// been reported as supported with #IsPayloadTypeSupported.
    ///
    /// How to prepare your multi-source device source for GigE Vision Validation Framework validation:
    ///
    ///     - Implement #IsPayloadTypeSupported and return true if aPayloadType is PvPayloadTypeMultiPart.
    ///     - Implement #SetTestPayloadFormatMode
    ///         - If aPayloadType is PvPayloadTypeMultiPart 
    ///             - Setup your streaming source for autonomous multi-part streaming if needed
    ///             - Return #PvResult::Code::OK
    ///         - Else if aPayloadType is PvPayloadTypeNone
    ///             - Disable test mode, go back to streaming source normal operation mode
    ///             - Return #PvResult::Code::OK
    ///         - Else
    ///             - Return any #PvResult error code like #PvResult::Code::NOT_SUPPORTED
    ///
    /// See the SoftDeviceGEVMultiPart C++ sample for an example of how to implement this method, #SetMultiPartAllowed, and #IsPayloadTypeSupported. 
    ///
    /// \param[in] aPayloadType Payload type to prepare test for. PvPayloadTypeNone sets the device back in normal operation mode.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - Any error code can be returned to indicate failure.
    ///
    virtual PvResult SetTestPayloadFormatMode( PvPayloadType aPayloadType ) { PVUNREFPARAM( aPayloadType ); return PvResult::Code::NOT_SUPPORTED; }
};


///
/// \class IPvRegisterInfo
/// \brief Interface to access basic information about a #PvSoftDeviceGEV register.
///
/// This interface can be used to query basic information about a register.
///
/// This class cannot be instantiated and there is no use case where you would 
/// need to implement this interface.
///
class PV_VIRTUAL_DEVICE_API IPvRegisterInfo
{
public:

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~IPvRegisterInfo() {}

    ///
    /// \brief Returns the register name.
    ///
    /// All #PvSoftDeviceGEV registers are named. This method
    /// allows the user to retrieve this name.
    ///
    /// The GigE Vision standard does not require that you name your registers.
    /// The name is used internally to provide contextual information
    /// about the register in logs or user interfaces.
    ///
    /// \return Register name.
    ///
    virtual const PvString &GetName() = 0;

    ///
    /// \brief Returns the register address.
    ///
    /// \return Register address.
    ///
    virtual uint32_t GetAddress() const = 0;

    ///
    /// \brief Returns the length of the register, in bytes.
    ///
    /// \return Register length.
    ///
    virtual size_t GetLength() const = 0;

    ///
    /// \brief Returns the contextual pointer provided at register creation.
    ///
    /// \return Const contextual pointer.
    ///
    virtual const void *GetContext() const = 0;

    ///
    /// \brief Returns the contextual pointer provided at register creation.
    ///
    /// \return Contextual pointer.
    ///
    virtual void *GetContext() = 0;

    ///
    /// \brief Indicates whether the register is writable.
    ///
    /// Whether the register is writable (or not) is only enforced when
    /// the register is accessed from the GigE Vision controller.
    ///
    /// Using the internal APIs it is possible to write to read-only
    /// registers. However, take care when doing so: If you do not fully control the
    /// custom register, it could cause issues (and is not
    /// recommended).
    ///
    /// \return True if the register is writable.
    ///
    virtual bool IsWritable() const = 0;

    ///
    /// \brief Indicates whether the register is readable.
    ///
    /// \return True if the register is readable.
    ///
    virtual bool IsReadable() const = 0;

    ///
    /// \brief Indicates whether the register is an array of bytes or plain data type.
    ///
    /// \return True if the register is array of bytes.
    ///
    virtual bool IsArrayOfBytes() const = 0;
};


///
/// \class IPvRegister
/// \brief Interface that is used to access the registers of the software-based GigE Vision Device.
///
/// This interface is retrieved through the IPvRegisterMap interface and
/// can be used to read from or write to registers. It is also possible to register
/// event sinks to registers using this interface.
///
/// This interface cannot be instantiated and there is no use case where you
/// would need to implement this interface.
///
class PV_VIRTUAL_DEVICE_API IPvRegister
    : public IPvRegisterInfo
{
public:

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~IPvRegister() {}

    ///
    /// \brief Reads a register to an array of bytes.
    ///
    /// \param[in] aData Pointer to the data where the data is read to.
    /// \param[in] aByteCount Length of the read operation.
    /// \param[in] aOffset Offset (in bytes) of the read operation in the register.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult Read( uint8_t *aData, uint32_t aByteCount, uint32_t aOffset = 0 ) = 0;

    ///
    /// \brief Writes an array of bytes to a register.
    ///
    /// \param[in] aData Pointer to the data to write to the register.
    /// \param[in] aByteCount Length of write operation.
    /// \param[in] aOffset Offset (in bytes) of the write operation in the register.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult Write( const uint8_t *aData, uint32_t aByteCount, uint32_t aOffset = 0 ) = 0;

    ///
    /// \brief Reads the register content to a 4-byte unsigned integer.
    ///
    /// Convenience method. Works the same as the byte-array Read but reads to
    /// a 4-byte unsigned integer directly.
    ///
    /// \param[out] aValue Output of the read operation.
    /// \param[in] aOffset Offset of the read operation in the register.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult Read( uint32_t &aValue, uint32_t aOffset = 0 ) = 0;

    ///
    /// \brief Writes the register content from a 4-byte unsigned integer.
    ///
    /// Convenience method. Works the same as the byte-array Write but writes from
    /// a 4-byte unsigned integer directly.
    ///
    /// \param[in] aValue Value to write to the register.
    /// \param[in] aOffset Offset of the write operation in the register.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult Write( uint32_t aValue, uint32_t aOffset = 0 ) = 0;

    ///
    /// \brief Reads the register content to a string.
    ///
    /// Convenience method. Works the same as the byte-array Read but reads to
    /// a string directly.
    ///
    /// \param[out] aValue Output of the read operation.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult Read( PvString &aValue ) = 0;

    ///
    /// \brief Writes the register content from a string.
    ///
    /// Convenience method. Works the same as the byte-array Write but writes from
    /// a string directly.
    ///
    /// \param[in] aValue Value to write to the register.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult Write( const PvString &aValue ) = 0;

    ///
    /// \brief Reads the register content to a 4-byte float.
    ///
    /// Convenience method. Works the same as the byte-array Read but reads to
    /// a float directly.
    ///
    /// \param[out] aValue Output of the read operation.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult ReadFloat( float &aValue ) = 0;

    ///
    /// \brief Writes the register content from a float.
    ///
    /// Convenience method. Works the same as the byte-array Write but writes from
    /// a float directly.
    ///
    /// \param[in] aValue Value to write to the register.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult WriteFloat( float aValue ) = 0;

    ///
    /// \brief Reads the register content to a double precision floating point.
    ///
    /// Convenience method. Works the same as the byte-array Read but reads to
    /// a double precision float directly.
    ///
    /// \param[out] aValue Output of the read operation.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult ReadDouble( double &aValue ) = 0;

    ///
    /// \brief Writes the register content from a double precision floating point.
    ///
    /// Convenience method. Works the same as the byte-array Write but writes from
    /// a double precision float directly.
    ///
    /// \param[in] aValue Value to write to the register.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult WriteDouble( double aValue ) = 0;

    ///
    /// \brief Registers a #IPvRegisterEventSink for register callbacks.
    ///
    /// \sa IPvRegisterEventSink
    ///
    /// \param[in] aEventSink Pointer to the event sink.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if the event sink is NULL or was previously registered.
    ///
    virtual PvResult AddEventSink( IPvRegisterEventSink *aEventSink ) = 0;

    ///
    /// \brief Removes an event sink registered with AddEventSink.
    ///
    /// \param[in] aEventSink Pointer to the event sink that is being removed.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if the event sink is NULL or was not previously registered.
    ///
    virtual PvResult RemoveEventSink( IPvRegisterEventSink *aEventSink ) = 0;
};


///
/// \class IPvRegisterStore
/// \brief Class used with register persistence in the context of user-sets.
///
/// The register store is where user-set data is stored. On Windows it is the Registry,
/// on Linux user-profile files in the home directory. On embedded applications it could
/// also be any static storage like flash memory.
///
/// \sa #IPvRegisterEventSink::Persist
/// \sa #IPvSoftDeviceGEV::SetUserSetCount
///
class PV_VIRTUAL_DEVICE_API IPvRegisterStore
{
public:

    virtual ~IPvRegisterStore() {}

    ///
    /// \brief Persists a register to the register store using a name suffix.
    ///
    /// Use this method from the IPvRegisterEventSink::Persist callback when walking registers through
    /// all possible selector(s) values. This method saves data to the store when saving a user-set
    /// and loads data from the store when loading a user-set.
    ///
    /// \sa #IPvRegisterEventSink::Persist
    /// \sa #IPvSoftDeviceGEV::SetUserSetCount
    ///
    /// \param[in] aRegister Register to save to or load from the store.
    /// \param[in] aNameSuffix Name suffix to use when saving to or loading from the store.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - Any other error code on failure.
    ///
    virtual PvResult Persist( IPvRegister *aRegister, const PvString &aNameSuffix ) = 0;
};


///
/// \class IPvRegisterEventSink
/// \brief Interface that is used to report the register activity of the software-based GigE Vision Device.
///
/// To receive register access callbacks, inherit one of your classes from #IPvRegisterEventSink
/// and pass a pointer to an object of that class to #IPvRegister::AddEventSink for the register that you 
/// want to get callbacks from.
///
/// It is possible to have more than one event sink registered for callbacks to the same register.
/// It is possible (and can be convenient) to use the same event sink for more than one register.
/// When an event sink is used for more than one register, all #IPvRegisterEventSink callbacks 
/// will have the register of interest provided as the first parameter, to provide contextual differentiation.
///
///
class PV_VIRTUAL_DEVICE_API IPvRegisterEventSink
{
public:

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~IPvRegisterEventSink() {}

    ///
    /// \brief Pre-read register notification.
    ///
    /// This notification is invoked before a register read operation on the #PvSoftDeviceGEV.
    /// The notification handler is a good place to update the content of the register
    /// with dynamic information.
    ///
    /// As an example, the #PvSoftDeviceGEV uses this notification to update
    /// the timestamp register every time it is read.
    ///
    /// Returning an error from this notification will cause the read operation to
    /// fail up to where it was initiated, which could be from an internal #IPvRegister
    /// read operation or from a READREG_CMD or READMEM_CMD GVCP remote operation.
    ///
    /// \param[in] aRegister Register to which the notification applies.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - Any PvResult error code on failure.
    ///
    virtual PvResult PreRead( IPvRegister *aRegister ) { PVUNREFPARAM( aRegister ); return PvResult::Code::OK; }

    ///
    /// \brief Post-read register notification.
    ///
    /// This notification is invoked after a register read operation on the #PvSoftDeviceGEV.
    ///
    /// \param[in] aRegister Register to which the notification applies.
    ///
    virtual void PostRead( IPvRegister *aRegister ) { PVUNREFPARAM( aRegister ); }

    ///
    /// \brief Pre-write register notification.
    ///
    /// This notification is invoked before a register write operation on the #PvSoftDeviceGEV.
    /// The notification handler is a good place to validate a new value before it is 
    /// committed to the register.
    ///
    /// On receiving this notification, the value of the register can be retrieved using
    /// the IPvRegister interface. On failure, the register will revert back to its original value.
    ///
    /// Returning an error from this notification will cause the write operation to
    /// fail up to where it was initiated, which could be from an internal #IPvRegister
    /// write operation or from a WRITEREG_CMD or WRITEMEM_CMD GVCP remote operation.
    ///
    /// We recommend that you do not react to new register values in this notification handler.
    /// It is possible that other event sinks for the register could refuse the write, 
    /// which will result in the operation failing and will cause the register to revert back to
    /// its original value. Use the #PostWrite notification instead.
    ///
    /// \param[in] aRegister Register to which the notification applies.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - Any PvResult error code on failure.
    ///
    virtual PvResult PreWrite( IPvRegister *aRegister ) { PVUNREFPARAM( aRegister ); return PvResult::Code::OK; }

    ///
    /// \brief Post-write register notification.
    ///
    /// This notification is invoked after a register write operation on the #PvSoftDeviceGEV. It can
    /// be used to react to the value of a register being changed.
    ///
    /// As an example, #PvSoftDeviceGEV uses this notification internally 
    /// to open or close the streaming channel on SCDA or SCP changes.
    ///
    /// \param[in] aRegister Register to which the notification applies.
    ///
    virtual void PostWrite( IPvRegister *aRegister ) { PVUNREFPARAM( aRegister ); }

    ///
    /// \brief Peristence request for selected registers for user-sets.
    ///
    /// This interface callback is invoked when saving to or loading from registers in the context of user-sets.
    ///
    /// Most registers can be saved to device user-sets by only setting the Streamable attribute
    /// of their attached GenApi feature when creating the GenICam XML file.
    ///
    /// Some registers have GenApi selector(s) and thus contain more than one value. These registers
    /// need to be "walked" through all possible selector(s) values when persisted. Here the word
    /// "persisted" is used for both Save and Load use-cases.
    ///
    /// These registers must implement this Persist method and call #IPvRegisterStore::Persist for
    /// each possible selector(s) combination.
    ///
    /// \sa #IPvSoftDeviceGEV::SetUserSetCount
    ///
    /// \param[in] aRegister Register to persist.
    /// \param[in] aStore Register store to use for persistence.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK if the method successfully handled persistence for the register.
    ///     - #PvResult::Code::NOT_IMPLEMENTED if the register does not need advanced persistence. Returned by default implementation.
    ///     - Any other PvResult error code on failure.
    ///
    virtual PvResult Persist( IPvRegister *aRegister, IPvRegisterStore *aStore ) { PVUNREFPARAM( aRegister ); PVUNREFPARAM( aStore ); return PvResult::Code::NOT_IMPLEMENTED; }
};


///
/// \class IPvRegisterFactory
/// \brief Class used for register creation.
///
/// The #AddRegister method can be used to add customer registers to the #PvSoftDeviceGEV.
/// Note however that custom registers can only be added from the
/// #IPvSoftDeviceGEVEventSink::OnCreateCustomRegisters notification. A pointer to this register 
/// factory interface is provided with the notification.
///
class PV_VIRTUAL_DEVICE_API IPvRegisterFactory
{
public:

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~IPvRegisterFactory() {}

    ///
    /// \brief Adds a new register (Integer, Float) to the #PvSoftDeviceGEV register map.
    ///
    /// \param[in] aName The name of the new register to create.
    /// \param[in] aAddress The address for the new register. It must be unique and must not overlap previously-allocated register space.
    /// \param[in] aLength Register length, in bytes. Must be 4 or 8.
    /// \param[in] aAccessMode Access mode of the register: PvGenAccessModeReadOnly, PvGenAccessModeReadWrite, or PvGenAccessModeWriteOnly.
    /// \param[in] aRegisterEventSink An optional event sink to register on creation.
    /// \param[in] aContext A contextual pointer that is specific to the register. It can be retrieved using #IPvRegisterInfo::GetContext.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult AddRegister( const PvString &aName, uint32_t aAddress, uint32_t aLength, PvGenAccessMode aAccessMode,
        IPvRegisterEventSink *aRegisterEventSink = NULL, void *aContext = NULL ) = 0;

    ///
    /// \brief Adds a new array of bytes (String, Register) to the #PvSoftDeviceGEV register map.
    ///
    /// \param[in] aName The name of the new register to create.
    /// \param[in] aAddress The address for the new register. It must be unique and must not overlap previously-allocated register space.
    /// \param[in] aLength Register length, in bytes. Must be divisible by 4.
    /// \param[in] aAccessMode Access mode of the register: PvGenAccessModeReadOnly, PvGenAccessModeReadWrite, or PvGenAccessModeWriteOnly.
    /// \param[in] aRegisterEventSink An optional event sink to register on creation.
    /// \param[in] aContext A contextual pointer that is specific to the register. It can be retrieved using #IPvRegisterInfo::GetContext.
    ///
    /// \return
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult AddByteArray( const PvString &aName, uint32_t aAddress, uint32_t aLength, PvGenAccessMode aAccessMode,
        IPvRegisterEventSink *aRegisterEventSink = NULL, void *aContext = NULL ) = 0;
};


///
/// \class IPvRegisterMap
/// \brief Register map interface for a software-based GigE Vision Device.
///
/// The interface to the #PvSoftDeviceGEV register map can be retrieved using
/// #IPvSoftDeviceGEV::GetRegisterMap any time after the device has been started.
///
/// This interface can be used to browse the register map using the #GetRegisterCount
/// and #GetRegisterByIndex methods. If you know the address of the register that you are looking
/// for, you can access it directly with #GetRegisterByAddress.
///
class PV_VIRTUAL_DEVICE_API IPvRegisterMap
{
public:

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~IPvRegisterMap() {}

    ///
    /// \brief Obtains a count of how many registers are in the #PvSoftDeviceGEV register map.
    ///
    /// Typically used to enumerate all registers. Call #GetRegisterByIndex
    /// with aIndex iterating from 0 to #GetRegisterCount - 1, inclusively.
    ///
    /// \return Register count.
    ///
    virtual size_t GetRegisterCount() = 0;

    ///
    /// \brief Obtains a register from the register map based on the register's index.
    ///
    /// \param[in] aIndex 0-based index.
    ///
    /// \return Register interface or NULL on failure.
    ///
    virtual IPvRegister *GetRegisterByIndex( size_t aIndex ) = 0;

    ///
    /// \brief Obtains a register from the register map based on the register's address.
    ///
    /// \param[in] aAddress Register address.
    ///
    /// \return Register interface or NULL on failure.
    ///
    virtual IPvRegister *GetRegisterByAddress( uint32_t aAddress ) = 0;

    ///
    /// \brief Locks the register map for safe register access.
    ///
    /// We recommend that you lock the register map before accessing the registers using the
    /// IPvRegisterMap interface. The lock should not be held for longer than necessary
    /// and should be released as soon as possible.
    ///
    /// Failure to lock the register map before accessing its content could mean that an
    /// IPvRegister obtained through #GetRegisterByIndex or #GetRegisterByAddress could be
    /// deleted while using it in parallel with a #PvSoftDeviceGEV reset.
    ///
    /// Locking the register map prevents register map additions or removals while it is being
    /// accessed. However, it does not prevent register access, which can safely occur 
    /// in parallel, because each individual register is thread-safe on read/write operations.
    ///
    /// It is very important that you call Release after a #Lock() success. Failure to do so will
    /// likely result in a #PvSoftDeviceGEV deadlock on reset or shutdown.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::STATE_ERROR if the #PvSoftDeviceGEV is not running.
    ///
    virtual PvResult Lock() = 0;

    ///
    /// \brief Locks the register map for safe register access but fails on timeout.
    ///
    /// See #Lock() for more information.
    ///
    /// \param[in] aTimeout Timeout, in ms, to wait for the lock.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::STATE_ERROR if the device is not running.
    ///     - #PvResult::Code::TIMEOUT if the lock could not be acquired before aTimeout ran out.
    ///
    virtual PvResult Lock( uint32_t aTimeout ) = 0;

    ///
    /// \brief Releases a lock acquired with #Lock.
    ///
    /// Not calling this method after calling #Lock will most likely result in a #PvSoftDeviceGEV
    /// deadlock on reset or shutdown.
    ///
    virtual void Release() = 0;
};


///
/// \class IPvGenApiFactory
/// \brief GenApi factory used to add custom GenApi parameters to a software-based GigE Vision Device GenICam XML file.
///
/// GenApi features can only be added to the device's GenICam XML file using the
/// #IPvSoftDeviceGEVEventSink::OnCreateCustomGenApiFeatures callback. See #IPvSoftDeviceGEVEventSink
/// and #PvSoftDeviceGEV for more information.
///
/// To add a feature, set all of its attributes in the factory and then call one of the Create methods. After calling
/// a Create method, ALL feature attributes in the factory are reset to their default value.
///
/// The following sequence will work and is valid:
///     - Set Feature A name.
///     - Set Feature A category.
///     - Create Feature A.
///     - Reset feature attributes of the factory.
///     - Set Feature B name.
///     - Set Feature B category.
///     - Create Feature B.
///
/// The following sequence will fail because Feature B would not have a category defined:
///     - Set Feature A name.
///     - Set Feature A category.
///     - Create Feature A.
///     - Reset feature attributes of the factory.
///     - Set Feature B name.
///     - Create Feature B.
///
class PV_VIRTUAL_DEVICE_API IPvGenApiFactory
{
public:

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~IPvGenApiFactory() {}
    
    ///
    /// \brief Sets the name of the next feature to be added to the GenICam XML file.
    ///
    /// The name must not contain any special characters. It can only contain letters,
    /// digits, and underscores. The feature name must begin with a letter.
    ///
    /// \param[in] aName Feature name.
    ///
    virtual void SetName( const PvString &aName ) = 0;

    ///
    /// \brief Sets the display name of the next feature to be added to the GenICam XML.
    ///
    /// The display name can be used by GenApi browsers in order to present the features
    /// in a simpler user-interface. Display names do not have to be unique and do can
    /// contain spaces.
    ///
    /// Features for which display names are not provided will have a display name
    /// automatically generated for them.
    ///
    /// Automatically generated display names created by adding spaces and removing
    /// the last sub-category prefix from the name of the feature:
    ///
    ///     - DeviceType from DeviceControl category: Type
    ///     - WidthMax from ImageFormatControl category: Width Max
    ///     - EventTestTimestamp from EventControl\\EventTestData category: Test Timestamp
    ///
    /// These simple rules should be enough for most features. If you need more control
    /// on the display name of a feature, just call this method to set a custom display name
    /// before using the factory to create the feature.
    ///
    /// \param[in] aDisplayName Feature name.
    ///
    virtual void SetDisplayName( const PvString &aDisplayName ) = 0;

    ///
    /// \brief Sets the category of the next feature to be added to the GenICam XML file.
    ///
    /// The category must not contain any special characters. It can only contain letters,
    /// digits, and underscores. The category must start with a letter.
    ///
    /// Multiple category levels must be separated with a backslash which
    /// would be expressed in a C/C++ string as "Level1\\Level2".
    ///
    /// Features with no category are not going to be visible in a GenApi browser. These
    /// features are usually referred to private, support or hidden features.
    ///
    /// \param[in] aCategory Feature category.
    ///
    virtual void SetCategory( const PvString &aCategory ) = 0;


    virtual void SetCategory( const PvString &aCategory, const PvString &aCategoryDisplayName ) = 0;

    ///
    /// \brief Sets the description of the next feature to be added to the GenICam XML file.
    ///
    /// \param[in] aDescription Feature description.
    ///
    virtual void SetDescription( const PvString &aDescription ) = 0;

    ///
    /// \brief Sets the tooltip of the next feature to be added to the GenICam XML file.
    ///
    /// \param[in] aToolTip Feature tooltip.
    ///
    virtual void SetToolTip( const PvString &aToolTip ) = 0;

    ///
    /// \brief Sets the access mode of the next feature to be added to the GenICam XML file.
    ///
    /// The access mode can be different than the register that it is bound to. However, it
    /// does not make sense for it to be more restrictive. As an example, a register could be
    /// read-write and the GenApi feature be read-only.
    ///
    /// \param[in] aAccessMode Feature access mode.
    ///
    virtual void SetAccessMode( PvGenAccessMode aAccessMode ) = 0;

    ///
    /// \brief Sets the numeric representation of the next feature to be added to the GenICam XML file.
    ///
    /// \param[in] aRepresentation Feature representation.
    ///
    virtual void SetRepresentation( PvGenRepresentation aRepresentation ) = 0;

    ///
    /// \brief Sets the visibility of the next feature to be added to the GenICam XML.
    ///
    /// \param[in] aVisibility Feature visibility.
    ///
    virtual void SetVisibility( PvGenVisibility aVisibility ) = 0;

    ///
    /// \brief Sets the cache behavior of the next feature to be added to the GenICam XML file.
    ///
    /// Possible values:
    ///     - PvGenCacheWriteThrough: The cached value of the register behind the feature is updated to the new value on write operations.
    ///     - PvGenCacheWriteAround: The cached value of the register behind the feature is invalidated on write operations and refreshed with the value read from #PvSoftDeviceGEV on the next read operation.
    ///     - PvGenCacheNone: The register value behind the feature is not cached and is refreshed every #SetPollingTime ms cycle.
    ///
    /// \param[in] aCache Feature cachable.
    ///
    virtual void SetCachable( PvGenCache aCache ) = 0;

    ///
    /// \brief Sets the polling time, in ms, of the next feature to be added to the GenICam XML file.
    ///
    /// The polling time provides a hint of how often no-cache features should be updated
    /// from GenApi browsers.
    ///
    /// \param[in] aPollingTime Feature polling time.
    ///
    virtual void SetPollingTime( uint32_t aPollingTime ) = 0;

    ///
    /// \brief Sets the namespace of the next feature to be added to the GenICam XML file.
    ///
    /// Possible values:
    /// - PvGenNameSpaceStandard for GenICam SFNC features.
    /// - PvGenNameSpaceCustom for custom features.
    ///
    /// \param[in] aNameSpace Feature namespace.
    ///
    virtual void SetNameSpace( PvGenNameSpace aNameSpace ) = 0;

    ///
    /// \brief Sets the TLParamsLocked behavior of the next feature to be added to the GenICam XML file.
    ///
    /// A feature with its TLLocked attribute set to True will be forced to read-only
    /// when the #PvSoftDeviceGEV is streaming.
    ///
    /// As an example, the Width and Height features use TLLocked (transport layer locked) to prevent
    /// the user from changing their values when the #PvSoftDeviceGEV is streaming.
    ///
    /// \param[in] aLocked Feature TLLocked attribute.
    ///
    virtual void SetTLLocked( bool aLocked ) = 0;

    ///
    /// \brief Sets the streamable behaviour of the next feature to be added to the GenICam XML.
    ///
    /// A feature with its streamable attribute set to true will be be considered when creating
    /// a persistence file of the GenApi interface of the device.
    ///
    /// \param[in] aStreamable Feature streamable attribute.
    ///
    virtual void SetStreamable( bool aStreamable ) = 0;

    ///
    /// \brief Sets the units for an integer or float feature.
    ///
    /// This string-based unit is only used for display purpose, it carries not functional signification.
    ///
    /// Applies to integer or float feature, SwissKnife or Converter.
    ///
    /// \param[in] aUnit Feature unit attribute.
    ///
    virtual void SetUnit( const PvString &aUnit ) = 0;

    ///
    /// \brief Sets the pValue attribute of the next feature to be added to the GenICam XML.
    ///
    /// Most GenApi features are linked to a register. With pValue you replace the register
    /// by the link or pointer to another feature.
    ///
    /// When defining a register as pValue, do not provide a register when calling the factory
    /// create method, use NULL instead.
    ///
    /// For ADVANCED GenApi support use cases.
    ///
    /// \param aFeatureName [in] Feature name to use for pValue.
    ///
    virtual void SetPValue( const PvString &aFeatureName ) = 0;

    ///
    /// \brief Sets the pIsAvaialble attribute of the next feature to be added to the GenICam XML.
    ///
    /// This attribute can be used to make a feature dynamically available or unavailable. It has
    /// to be the name of another feature.
    ///
    /// If you need an more complex expression to control the available state of a feature,
    /// create a private integer SwissKnife that returns non-zero (1 or true) when the feature
    /// should be available.
    ///
    /// See #SetCategory for more information on private features. See #CreateIntSwissKnife for more
    /// information on SwissKnifes.
    ///
    /// When pIsAvailable evaluates to true the feature is available as defined: read-write, read-only
    /// or write-only. When pIsAvailable evaluates to false the feature is not readable and not writable.
    /// The eBUS GenApi browser shows a feature that state as {Not available}.
    ///
    /// For ADVANCED GenApi support use cases.
    ///
    /// \param aFeatureName [in] Feature name to use for pIsImplemented.
    ///
    virtual void SetPIsAvailable( const PvString &aFeatureName ) = 0;
    virtual void SetPIsLocked( const PvString &aFeatureName ) = 0;

    ///
    /// \brief Maps the next feature to be added to the GenICam XML file to a chunk, identified by its ID.
    ///
    /// Sets the chunk ID and mapping (address, length, and endianness) of the next feature to be added 
    /// to the GenICam XML file.
    ///
    /// By default all features map to a #PvSoftDeviceGEV register provided in the Create calls, unless
    /// #MapChunk or #MapEvent are called when defining the feature attributes.
    ///
    /// \param[in] aChunkID Chunk ID to map.
    /// \param[in] aAddress Data offset of the feature in the chunk, in bytes.
    /// \param[in] aLength Data length of the feature in the chunk, in bytes.
    /// \param[in] aEndianness Data endianness.
    ///
    virtual void MapChunk( uint32_t aChunkID, uint32_t aAddress, size_t aLength, PvGenEndianness aEndianness = PvGenEndiannessLittle ) = 0;

    ///
    /// \brief Maps the next feature to be added to the GenICam XML file to a messaging channel event, identified by its ID.
    ///
    /// Sets the messaging channel event ID and mapping (address, length, and endianness) of the next feature to be added 
    /// to the GenICam XML file.
    ///
    /// By default all features map to a #PvSoftDeviceGEV register provided in the Create calls, unless
    /// #MapChunk or #MapEvent are called when defining the feature attributes.
    ///
    /// \param[in] aEventID Event ID to map.
    /// \param[in] aAddress Data offset of the feature in the event data, in bytes.
    /// \param[in] aLength Data length of the feature in the event data, in bytes.
    /// \param[in] aEndianness Data endianness.
    /// \param[in] aAdjustAddress Applies an offset on the provided address.
    ///
    virtual void MapEvent( uint32_t aEventID, uint32_t aAddress, size_t aLength, PvGenEndianness aEndianness = PvGenEndiannessLittle, bool aAdjustAddress = true ) = 0;

    ///
    /// \brief Adds a selector to the next feature to be added to the GenICam XML file.
    ///
    /// A selector, as specified by the GenICam SFNC, is used to index which instance of the feature is accessed in 
    /// situations where multiple instances of a feature exist (For instance, the analog gain for each 
    /// separate channel of the red/green/blue component of a color camera). 
    ///
    /// Feature A can be the selector for Features B and C. We could also say that
    /// B and C are indexed by A. In that case, Feature A needs to be identified as the selector
    /// for Feature B and Feature C.
    ///
    /// Example:
    ///     - Create Feature A, our selector
    ///     - Call #IPvGenApiFactory::AddSelected with Feature B name
    ///     - Call #IPvGenApiFactory::AddSelected with Feature C name
    ///     - Create Feature B (selected by A)
    ///     - Create Feature C (selected by A)
    ///
    /// A feature can have more than one selector. A feature with one selector
    /// could be thought of as a one-dimensional vector. A feature with two selector
    /// would be like a table with columns and rows, etc.
    ///
    /// \param[in] aFeatureName Name of the feature that is selecting the new feature.
    ///
    virtual void AddSelected( const PvString &aFeatureName ) = 0;

    ///
    /// \brief Add an invalidator to the next feature to be added to the GenICam XML file.
    ///
    /// Add one invalidator to a feature. For example, Feature A being invalidated by Feature B
    /// has the cache of Feature A being invalidated when a new value is written to Feature B.
    ///
    /// \param[in] aFeatureName Name of the feature that is invalidating the new feature.
    ///
    virtual void AddInvalidator( const PvString &aFeatureName ) = 0;

    ///
    /// \brief Add an enumeration entry to the next feature to be added to the GenICam XML file.
    ///
    /// When #CreateEnum is called, it will be populated with the enumeration entries added
    /// using this method. Call #AddEnumEntry for each enumeration entry of the new feature.
    ///
    /// If one of the Create methods other than #CreateEnum is called, the enumeration entries
    /// added to the factory are ignored.
    ///
    /// \param[in] aName Name of the new enumeration entry.
    /// \param[in] aValue Value of the new enumeration entry.
    ///
    virtual void AddEnumEntry( const PvString &aName, uint32_t aValue ) = 0;

    ///
    /// \brief Add an enumeration entry to the next feature to be added to the GenICam XML file.
    ///
    /// When #CreateEnum is called, it will be populated with the enumeration entries added
    /// using this method. Call #AddEnumEntry for each enumeration entry of the new feature.
    ///
    /// If one of the Create methods other than #CreateEnum is called, the enumeration entries
    /// added to the factory are ignored.
    ///
    /// \param[in] aName Name of the new enumeration entry.
    /// \param[in] aValue Value of the new enumeration entry.
    /// \param[in] aDisplayName Custom display name for the new enum entry. If an empty string is provided a default display name is generated for the enum entry.
    /// \param[in] aNameSpace Namespace for the new enum entry. See #SetNameSpace for more information on GenICam XML namespaces.
    ///
    virtual void AddEnumEntry( const PvString &aName, uint32_t aValue, const PvString &aDisplayName, PvGenNameSpace aNameSpace = PvGenNameSpaceCustom ) = 0;

    ///
    /// \brief Adds a variable that will be declared for the next SwissKnife of Converter to be added to the GenICam XML file.
    ///
    /// In order to refer to a feature in a SwissKnife or Converter formula, it first needs to be defined
    /// as a variable.
    ///
    /// Variables are defined using the exact case sensitive feature name. They are then used in the formulas using the same
    /// exact feature name.
    ///
    /// A SwissKnife can have one or more variables.
    ///
    /// A Converter can have zero or more variables. Important: the main feature a Converter operates on (TO/FROM)
    /// does not have to be defined as a variable.
    ///
    /// Variables are ignored if the next feature to be added to the GenICam XML file is not
    /// a SwissKnife or Converter.
    ///
    /// For ADVANCED GenApi support use cases.
    ///
    /// \param aFeatureName [in] Feature name to use as variable.
    ///
    virtual void AddVariable( const PvString &aFeatureName ) = 0;

    ///
    /// \brief Adds an integer feature to the GenICam XML file using the current state of the factory for its attributes.
    ///
    /// After calling this method, all feature creation attributes are reset to their default values.
    ///
    /// \param[in] aRegister Register that the new integer feature maps to or NULL if creating a chunk or messaging channel event data feature.
    /// \param[in] aMin Minimum of the new integer feature.
    /// \param[in] aMax Maximum of the new integer feature.
    /// \param[in] aInc Increment of the new integer feature.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///          - If aRegister is NULL and none of chunk ID, messaging channel event or pValue has been defined.
    ///          - If aMin and aMax are not compatible with each other.
    ///     - #PvResult::Code::NOT_INITIALIZED if #SetName has not been called for the new feature.
    ///
    virtual PvResult CreateInteger( IPvRegister *aRegister, int64_t aMin, int64_t aMax, int64_t aInc = 1 ) = 0;
    virtual PvResult CreateInteger( IPvRegister *aRegister = NULL ) = 0;

    ///
    /// \brief Adds a float feature to the GenICam XML file using the current state of the factory for its attributes.
    ///
    /// After calling this method, all feature creation attributes are reset to their default values.
    ///
    /// \param[in] aRegister Register that the new float feature maps to or NULL if creating a chunk or messaging channel event data feature.
    /// \param[in] aMin Minimum of the new float feature.
    /// \param[in] aMax Maximum of the new float feature.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///          - If aRegister is NULL and none of chunk ID, messaging channel event or pValue has been defined.
    ///          - If aMin and aMax are not compatible with each other.
    ///     - #PvResult::Code::NOT_INITIALIZED if #SetName has not be called for the new feature.
    ///
    virtual PvResult CreateFloat( IPvRegister *aRegister, double aMin, double aMax ) = 0;
    virtual PvResult CreateFloat( IPvRegister *aRegister = NULL ) = 0;

    ///
    /// \brief Adds a string feature to the GenICam XML file using the current state of the factory for its attributes.
    ///
    /// After calling this method, all feature creation attributes are reset to their default values.
    ///
    /// \param[in] aRegister Register that the new string feature maps to or NULL if creating a chunk or messaging channel event data feature.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///          - If aRegister is NULL and none of chunk ID, messaging channel event or pValue has been defined.
    ///     - #PvResult::Code::NOT_INITIALIZED if #SetName has not been called for the new feature.
    ///
    virtual PvResult CreateString( IPvRegister *aRegister = NULL ) = 0;
    virtual PvResult CreateRegister( IPvRegister *aRegister = NULL ) = 0;

    ///
    /// \brief Adds a Boolean feature to the GenICam XML file using the current state of the factory for its attributes.
    ///
    /// After calling this method, all feature creation attributes are reset to their default values.
    ///
    /// \param[in] aRegister Register that the new Boolean feature maps to or NULL if creating a chunk or messaging channel event data feature.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///          - If aRegister is NULL and none of chunk ID, messaging channel event or pValue has been defined.
    ///     - #PvResult::Code::NOT_INITIALIZED if #SetName has not been called for the new feature.
    ///
    virtual PvResult CreateBoolean( IPvRegister *aRegister = NULL ) = 0;

    ///
    /// \brief Adds a command feature to the GenICam XML file using the current state of the factory for its attributes.
    ///
    /// After calling this method, all feature creation attributes are reset to their default values.
    ///
    /// \param[in] aRegister Register that the new command feature maps to.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///          - If aRegister is NULL and pValue has not been defined.
    ///     - #PvResult::Code::NOT_INITIALIZED if #SetName has not been called for the new feature.
    ///
    virtual PvResult CreateCommand( IPvRegister *aRegister = NULL ) = 0;

    ///
    /// \brief Adds an enumeration feature to the GenICam XML file using the current state of the factory for its attributes.
    ///
    /// After calling this method, all feature creation attributes are reset to their default values.
    ///
    /// \param[in] aRegister Register that the new enumeration feature maps to or NULL if creating a chunk or messaging channel event data feature.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///          - If aRegister is NULL and none of chunk ID, messaging channel event or pValue has been defined.
    ///          - If no enumeration entries have been defined for the new feature.
    ///     - #PvResult::Code::NOT_INITIALIZED if #SetName has not be called for the new feature.
    ///
    virtual PvResult CreateEnum( IPvRegister *aRegister = NULL ) = 0;

    ///
    /// \brief Adds an integer SwissKnife to the GenICam XML file using the current state of the factory for its attributes.
    ///
    /// An integer SwissKnife is a GenApi read-only feature that resolves a formula whenever it is read. The formula
    /// is defined as a string and evaluates to an integer. It can refer to other features as long as they have been
    /// declared with #AddVariable. Variables can be integer, float (truncated to integer), enum (integer value)
    /// boolean (1 or 0 integer value) or command as long as they are readable.
    ///
    /// Example of a simple SwissKnife adding the value of two features:
    /// \code{.cpp}
    /// lFactory->SetName( "MySwissKnife" );
    /// lFactory->SetCategory( "MyCategory" );
    /// lFactory->AddVariable( "ValueA" );
    /// lFactory->AddVariable( "ValueB" );
    /// lFactory->CreateIntSwissKnife( "ValueA + ValueB" );
    /// \endcode
    ///
    /// When MySwissKnife is read, the formula is evaluated. If ValueA is 4 and ValueB is 3, MySwissKnife
    /// will read as 7: 4 + 3.
    ///
    /// Formula examples:
    ///     - Simple constant: 42
    ///     - Simple feature: ValueA
    ///     - Simple mathematical expression: 32 + 10
    ///     - Simple mathematical expression with features: ValueA + (10 * ValueB)
    ///     - Bitwise operations: (ValueA << 16) | (ValueB >> 16)
    ///     - 'if' statement: (ValueA = 4) ? 8 : 16
    ///     - Switch-case statement with default: (ValueA = 0) ? 0x0000B824 : (ValueA = 1) ? 0x0000B82C : (ValueA = 2) ? 0x0000B884 : (ValueA = 3) ? 0x0000B88C : 0xFFFFFFFF
    ///
    /// After calling this method, all feature creation attributes are reset to their default values.
    ///
    /// GenApi formulas are compatible with expressions defined for the Kirill Zaborsk's math parser library, 
    /// the CCalc library from Walery Studennikov. More information is available at http://kirya.narod.ru/mathparser.html.
    ///
    /// For ADVANCED GenApi support use cases.
    ///
    /// \param aFormula [in] SwissKnife formula.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if the formula is empty.
    ///     - #PvResult::Code::NOT_INITIALIZED if #SetName has not be called for the new feature.
    ///
    virtual PvResult CreateIntSwissKnife( const PvString &aFormula ) = 0;

    ///
    /// \brief Adds a float SwissKnife to the GenICam XML file using the current state of the factory for its attributes.
    ///
    /// A float SwissKnife is a GenApi read-only feature that resolves a formula whenever it is read. The formula
    /// is defined as a string and evaluates to a float. It can refer to other features as long as they have been
    /// declared with #AddVariable. Variables can be integer (converter to float), float, enum (integer value converted to float)
    /// boolean (1 or 0 integer value converted to float) or command as long as they are readable.
    ///
    /// Example of a simple SwissKnife adding the value of two features:
    /// \code{.cpp}
    /// lFactory->SetName( "MySwissKnife" );
    /// lFactory->SetCategory( "MyCategory" );
    /// lFactory->AddVariable( "ValueA" );
    /// lFactory->AddVariable( "ValueB" );
    /// lFactory->CreateFloatSwissKnife( "ValueA + ValueB" );
    /// \endcode
    ///
    /// When MySwissKnife is read, the formula is evaluated. If ValueA is 4.1 and ValueB is 3.2, MySwissKnife
    /// will read as 7.3: 4.1 + 3.2.
    ///
    /// Formula examples:
    ///     - Simple constant: 42.4
    ///     - Simple feature: ValueA
    ///     - Simple mathematical expression: 32.34 + 10.714
    ///     - Simple mathematical expression with features: ValueA + (10.1 * ValueB)
    ///     - Advanced mathematical expression: SIN(0.814)
    ///     - Advanced mathematical expression with feature: SQRT(ValueA)
    ///     - 'if' statement: (ValueA > 4) ? 8.12 : 16.991
    ///     - Bitwise operations are not supported with float SwissKnife.
    ///
    /// After calling this method, all feature creation attributes are reset to their default values.
    ///
    /// GenApi formulas are compatible with expressions defined for the Kirill Zaborsk's math parser library, 
    /// the CCalc library from Walery Studennikov. More information is available at http://kirya.narod.ru/mathparser.html.
    ///
    /// For ADVANCED GenApi support use cases.
    ///
    /// \param aFormula [in] SwissKnife formula.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if the formula is empty.
    ///     - #PvResult::Code::NOT_INITIALIZED if #SetName has not be called for the new feature.
    ///
    virtual PvResult CreateFloatSwissKnife( const PvString &aFormula ) = 0;

    ///
    /// \brief Adds an integer Converter to the GenICam XML file using the current state of the factory for its attributes.
    ///
    /// A converter is a read-write GenApi construct that allows reading from and writing to a referenced feature using
    /// SwissKnife-like formulas.
    ///
    /// The referenced feature is provided through aValueFeatureName and represents the feature on which the 
    /// expressions operates. It is referred to as FROM and TO in the  converter formulas and does not have
    /// to be declared as a variable.
    ///
    /// The formula used to read the referenced feature is provided with aFromFormula. The raw value read from the
    /// reference is available as the special TO variable.
    ///
    /// The formula used to write to the referenced feature is provided with aToFormula. The raw value written
    /// to the converter is available as the special FROM variable.
    ///
    /// If the converter formulas need to reference additional features they should first be declared
    /// using the #AddVariable method.
    ///
    /// Example of a converter handling conversion between milliseconds and nanoseconds:
    /// \code{.cpp}
    /// lFactory->SetName( "MyNanosecondFeature" );
    /// lFactory->SetCategory( "MyCategory" );
    /// lFactory->SetUnit( "ms" );
    /// lFactory->CreateIntConverter( "MyMillisecondFeature", "TO * 1000", "FROM / 1000" );
    /// \endcode
    ///
    /// In the example above:
    ///     - We create a MyNanosecondFeature.
    ///     - The referenced feature is MyMillisecondFeature.
    ///     - The converter read formula is TO * 1000, where TO is the value read from MyMillisecondFeature.
    ///         - Reading from the MyNanosecondFeature converter:
    ///             - Reads the referenced feature, MyMillisecondFeature (4)
    ///             - Replaces TO in the 'from' formula with the value read from MyMillisecondFeature (4 * 1000)
    ///             - Evaluates the formula (4000)
    ///             - Completes the read operation returning 4000
    ///     - The converter write formula is is FROM / 1000, where FROM is the value written to the converter.
    ///         - Writing to the MyNanosecondFeature converter:
    ///             - Value written to the converter (2000)
    ///             - Replaces FROM in the 'to' formula with the value written to the converter (2000 / 1000)
    ///             - Evaluates the formula (2)
    ///             - Writes 2 to MyMillisecondFeature
    ///
    /// See #CreateFloatSwissKnife for more information on formulas.
    ///
    /// For ADVANCED GenApi support use cases.
    ///
    /// After calling this method, all feature creation attributes are reset to their default values.
    ///
    /// \param aValueFeatureName [in] Main feature the converter operates on, referred as TO and FROM in the formulas.
    /// \param aFromFormula [in] Formula used when reading from the converter.
    /// \param aToFormula [in] Formula used when writing to the converter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if value feature name or either formula is empty.
    ///     - #PvResult::Code::NOT_INITIALIZED if #SetName has not be called for the new feature.
    ///
    virtual PvResult CreateIntConverter( const PvString &aValueFeatureName, const PvString &aFromFormula, const PvString &aToFormula ) = 0;

    ///
    /// \brief Adds a float Converter to the GenICam XML file using the current state of the factory for its attributes.
    ///
    /// A converter is a read-write GenApi construct that allows reading from and writing to a referenced feature using
    /// SwissKnife-like formulas.
    ///
    /// The referenced feature is provided through aValueFeatureName and represents the feature on which the 
    /// expressions operates. It is referred to as FROM and TO in the  converter formulas and does not have
    /// to be declared as a variable.
    ///
    /// The formula used to read the referenced feature is provided with aFromFormula. The raw value read from the
    /// reference is available as the special TO variable.
    ///
    /// The formula used to write to the referenced feature is provided with aToFormula. The raw value written
    /// to the converter is available as the special FROM variable.
    ///
    /// If the converter formulas need to reference additional features they should first be declared
    /// using the #AddVariable method.
    ///
    /// Example of a converter handling conversion between inches and centimeters:
    /// \code{.cpp}
    /// lFactory->SetName( "MyCentimeterFeature" );
    /// lFactory->SetCategory( "MyCategory" );
    /// lFactory->SetUnit( "cm" );
    /// lFactory->CreateFloatConverter( "MyInchesFeature", "TO * 2.54", "FROM * 0.3937" );
    /// \endcode
    ///
    /// In the example above:
    ///    - We create a MyCentimeterFeature.
    ///    - The referenced feature is MyInchesFeature.
    ///    - The converter read formula is TO * 2.54, where TO is the value read from MyInchesFeature.
    ///         - Reading from the MyCentimeterFeature converter:
    ///             - Reads the referenced feature, MyInchesFeature (4)
    ///             - Replaces TO in the 'from' formula with the value read from MyInchesFeature (4 * 2.54)
    ///             - Evaluates the formula (10.16)
    ///             - Completes the read operation returning 10.16
    ///             - The converter write formula is is FROM * 0.3937, where FROM is the value written to the converter.
    ///         - Writing to the MyCentimeterFeature converter:
    ///             - Value written to the converter (2)
    ///             - Replaces FROM in the 'to' formula with the value written to the converter (2 * 0.3937)
    ///             - Evaluates the formula (0.7874)
    ///             - Writes 0.7874 to MyInchesFeature
    ///
    /// See #CreateFloatSwissKnife for more information on formulas.
    ///
    /// For ADVANCED GenApi support use cases.
    ///
    /// After calling this method, all feature creation attributes are reset to their default values.
    ///
    /// \param aValueFeatureName [in] Main feature the converter operates on, referred as TO and FROM in the formulas.
    /// \param aFromFormula [in] Formula used when reading from the converter.
    /// \param aToFormula [in] Formula used when writing to the converter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if value feature name or either formula is empty.
    ///     - #PvResult::Code::NOT_INITIALIZED if #SetName has not be called for the new feature.
    ///
    virtual PvResult CreateFloatConverter( const PvString &aValueFeatureName, const PvString &aFromFormula, const PvString &aToFormula ) = 0;

    ///
    /// \brief Adds a a custom invalidator to a feature managed by #PvSoftDeviceGEV.
    ///
    /// This methods allows defining extra custom invalidators for features managed by #PvSoftDeviceGEV. As an example, if you
    /// call this method to add an ComponentSelector invalidator to the #PvSoftDeviceGEV managed (or owned) PixelFormat feature,
    /// the invalidator is added to the invalidators of PixelFormat when creating the GenICam XML of the device. With this example,
    /// the GenApi cache of PixelFormat would be invalidated when the value of ComponentSelector changes client-side.
    ///
    /// For ADVANCED GenApi support use cases.
    ///
    /// \param aStandardFeatureName [in] Name of the feature to add the invalidator to.
    /// \param aInvalidatorFeatureName [in] Name of the feature which changes causes aStandardFeatureName to be invalidated.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult AddInvalidatorTo( const PvString &aStandardFeatureName, const PvString &aInvalidatorFeatureName ) = 0;

    ///
    /// \brief Forces a pIsAvailable reference for a feature managed by #PvSoftDeviceGEV.
    ///
    /// The pIsAvailable would typically be an IntSwissKnife defined as a global feature in the
    /// #IPvSoftDeviceGEVEventSink::OnCreateCustomGenApiFeatures callback. The feature identified by
    /// aStandardFeatureName would only be available when the IntSwissKnife referred to by aPIsAvailableFeatureName
    /// evaluates to non-zero.
    ///
    /// With most use-cases #SetPIsAvailableFor should be called in the same context.
    ///
    /// For ADVANCED GenApi support use cases.
    ///
    /// \param aStandardFeatureName [in] Name of the feature to add the pIsAvailable for.
    /// \param aPIsAvailableFeatureName [in] Feature used to evaluate if the feature is available or not.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult SetPIsAvailableFor( const PvString &aStandardFeatureName, const PvString &aPIsAvailableFeatureName ) = 0;

    ///
    /// \brief Forces a pIsAvailable reference for a the enum entry of an enum feature managed by #PvSoftDeviceGEV.
    ///
    /// The pIsAvailable would typically be an IntSwissKnife defined as a global feature in the
    /// #IPvSoftDeviceGEVEventSink::OnCreateCustomGenApiFeatures callback. aEnumEntry of the enum feature identified by
    /// aStandardFeatureName would only be available when the IntSwissKnife referred to by aPIsAvailableFeatureName
    /// evaluates to non-zero.
    ///
    /// With most use-cases #SetPIsAvailableFor should be called in the same context.
    ///
    /// For ADVANCED GenApi support use cases.
    ///
    /// \param aStandardFeatureName [in] Name of the enum feature the enum entry belongs to.
    /// \param aEnumEntryName [in] Name of the enum entry to add the pIsAvailable for.
    /// \param aPIsAvailableFeatureName [in] Feature used to evaluate if the enum entry is available or not.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult SetPIsAvailableForEnumEntry( const PvString &aStandardFeatureName, const PvString &aEnumEntryName, const PvString &aPIsAvailableFeatureName ) = 0;
    virtual PvResult SetPIsLockedFor( const PvString &aStandardFeatureName, const PvString &aPIsLockedFeatureName ) = 0;

    ///
    /// \brief Forces a pValue reference for a feature managed by #PvSoftDeviceGEV.
    ///
    /// For ADVANCED GenApi support use cases.
    ///
    /// \param aStandardFeatureName [in] Name of the feature to force pValue for.
    /// \param aPValueFeatureName [in] Name of the feature used provide the value of the aStandardFeatureName feature.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult SetPValueFor( const PvString &aStandardFeatureName, const PvString &aPValueFeatureName ) = 0;

    ///
    /// \brief Forces a pMin reference for an integer or float feature managed by #PvSoftDeviceGEV.
    ///
    /// For ADVANCED GenApi support use cases.
    ///
    /// \param aStandardFeatureName [in] Name of the feature to force pMin for.
    /// \param aPMinFeatureName [in] Name of the feature used provide the minimum of the aStandardFeatureName feature.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult SetPMinFor( const PvString &aStandardFeatureName, const PvString &aPMinFeatureName ) = 0;

    ///
    /// \brief Forces a pMax reference for an integer or float feature managed by #PvSoftDeviceGEV.
    ///
    /// For ADVANCED GenApi support use cases.
    ///
    /// \param aStandardFeatureName [in] Name of the feature to force pMax for.
    /// \param aPMaxFeatureName [in] Name of the feature used provide the maximum of the aStandardFeatureName feature.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult SetPMaxFor( const PvString &aStandardFeatureName, const PvString &aPMaxFeatureName ) = 0;

    ///
    /// \brief Forces a pInc reference for an integer feature managed by #PvSoftDeviceGEV.
    ///
    /// For ADVANCED GenApi support use cases.
    ///
    /// \param aStandardFeatureName [in] Name of the feature to force pInc for.
    /// \param aPIncFeatureName [in] Name of the feature used provide the maximum of the aStandardFeatureName feature.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///
    virtual PvResult SetPIncFor( const PvString &aStandardFeatureName, const PvString &aPIncFeatureName ) = 0;

    ///
    /// \brief Sets node to use for integer or float dynamic minimum attribute.
    ///
    /// Sets the pMin attribute of an integer or float to point to another GenApi node like
    /// a Swiss Knife. When pMin is set for a new feature, it has precedence on any defined
    /// constant minimum.
    ///
    /// \param aFeatureName [in] Name of the feature the integer or float minimum attribute points to.
    ///
    virtual void SetPMin( const PvString &aFeatureName ) = 0;

    ///
    /// \brief Sets node to use for integer or float dynamic maximum attribute.
    ///
    /// Sets the pMax attribute of an integer or float to point to another GenApi node like
    /// a Swiss Knife. When pMax is set for a new feature, it has precedence on any defined
    /// constant maximum.
    ///
    /// \param aFeatureName [in] Name of the feature the integer or float maximum attribute points to.
    ///
    virtual void SetPMax( const PvString &aFeatureName ) = 0;

    ///
    /// \brief Sets node to use for integer dynamic increment attribute.
    ///
    /// Sets the pInc attribute of an integer to point to another GenApi node like
    /// a Swiss Knife. When pInc is set for a new feature, it has precedence on any defined
    /// constant increment.
    ///
    /// \param aFeatureName [in] Name of the feature the integer increment attribute points to.
    ///
    virtual void SetPInc( const PvString &aFeatureName ) = 0;
};



///
/// \class IPvSoftDeviceGEV
/// \brief Interface to the main class of the software-based GigE Vision Device.
///
class PV_VIRTUAL_DEVICE_API IPvSoftDeviceGEV
{
public:

    ///
    /// \brief Virtual destructor.
    ///
    virtual ~IPvSoftDeviceGEV() {}

    ///
    /// \brief Adds a new stream to the #PvSoftDeviceGEV.
    ///
    /// All streams must be added before starting the #PvSoftDeviceGEV.
    ///
    /// \param[in] aSource Interface to a user-provided source.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if aSource is NULL.
    ///     - #PvResult::Code::STATE_ERROR if the #PvSoftDeviceGEV is already running.
    ///     - #PvResult::Code::NOT_SUPPORTED if the maximum stream count has already been reached.
    ///
    virtual PvResult AddStream( IPvStreamingChannelSource * aSource ) = 0;

    ///
    /// \brief Sets the number of user-sets supported by the #PvSoftDeviceGEV.
    ///
    /// The maximum supported user-set count is 256.
    ///
    /// Can only be changed before starting the device.
    ///
    /// User-sets are all about device-side persistence. The status of the device can be saved
    /// or retrieved from some static store on the device. User-sets are first and foremost managed at the 
    /// register level. The SFNC allowing defines the UserSetFeatureSelector which is populated
    /// with feature names. This creates an imperfect but mostly acceptable one-to-one link between
    /// Streamable (persistent) GenApi features and the register providing its implementation.
    ///
    /// Features marked as Streamable (persistent) when creating the GenICam XML file
    /// of the device are used for device-side persistence through user-sets.
    ///
    /// Persistent features have the content of the register they directly map to
    /// saved to the persistent store of the device. The persistent store is implemented
    /// through the Registry on Windows and profile home files on Linux. Hardware implementations
    /// could rely on Flash memory to store user-sets.
    ///
    /// Most registers are simply persisted as-is: their binary content is simply saved to or loaded 
    /// from the store.
    ///
    /// Some registers have only one address but can contain many values when indexed through
    /// device-managed selectors. If those registers are Streamable (persistent) the application
    /// must handle the IPvRegisterEventSink::Persist event for the registers and for each
    /// possible selector combination call #IPvRegisterStore::Persist. For the application the
    /// same code is used for both save and load operations, thus the generic "persist" naming.
    ///
    /// The SoftDeviceGEV sample shows basic use of user-sets.
    ///
    /// @cond
    /// The SoftDeviceGEV3d sample shows how to handle complex multi-selector registers in
    /// the context of user-sets.
    /// @endcond
    /// 
    /// Selected registers handling in pseudo-code:
    ///     - Save current selector of interest values
    ///     - Set selectors to 1st combination
    ///     - Call #IPvRegisterStore::Persist with register and unique name suffix representing selector of interest values
    ///     - Set selectors to 2nd combination
    ///     - Call #IPvRegisterStore::Persist with register and unique name suffix representing selector of interest values
    ///     - ...
    ///     - Restore selector values
    ///
    /// The #IPvRegisterStore::Persist method loads or saves the register content in the store using the
    /// feature name and provided suffix.
    ///
    /// On startup the default user-set is always updated in the store from the fresh device status. If the UserSetDefault 
    /// GenApi feature is assigned to a user-set other than Default, the user-set is automatically loaded on device startup.
    ///
    /// The UserSetDefault GenApi feature is special: it is not saved to a user-set but is immediately and automatically
    /// persistent to the store whenever changed.
    ///
    /// User-sets can be saved and loaded explicitly using the UserSetSelector, UserSetSave, and UserSetLoad
    /// GenApi features.
    ///
    /// The UserSetFeatureSelector and UserSetFeatureEnable GenApi features can be used to control which features
    /// are saved to the user-sets. All features defined as Streamable when creating the GenICam XML file of the
    /// device should be available under the UserSetFeatureSelector enum feature.
    ///
    /// \param[in] aCount Count of user-sets supported, in addition to the default user-set.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if aSource is greather than the maximum user-set count.
    ///     - #PvResult::Code::STATE_ERROR if the #PvSoftDeviceGEV is already running.
    ///
    virtual PvResult SetUserSetCount( uint32_t aCount ) = 0;

    ///
    /// \brief Binds in a class which will contain a callback which will be called when the 
    ///  state of a UsersetLoad/Save changes.
    ///
    /// \return
    ///    Includes
    ///     - #PvResult::Code::OK on success
    ///
    virtual PvResult SetUserSetNotify ( IPvUserSetNotify *aUserSetNotify ) = 0;

    ///
    /// \brief Enables experimental TCP transport.
    ///
    /// PvSoftDeviceGEV supports optional, experimental TCP transport for GigE Vision devices.
    /// It affects GVCP control, GVCP messaging channel and GVSP streaming.
    ///
    /// When TCP is enabled the device still respond to UDP requests as usual. Additional TCP
    /// sockets are opened to optionally accept TCP connections from the host.
    ///
    /// As long as TCP is not fully ratified by the GigE Vision specification, enabling
    /// TCP could and most likely will break GigE Vision compliance. It also means only the 
    /// eBUS PvDeviceGEV and PvStreamGEV can be used to interact with the device when using 
    /// TCP transport.
    ///
    /// \param[in] aEnabled Toggles TCP support.
    ///
    /// \return #PvResult::Code::OK
    ///
    virtual PvResult SetTCPTransportEnabled( bool aEnabled ) = 0;

    ///
    /// \brief Enables the option of using RTP for streaming.
    ///
    /// PvSoftDeviceGEV supports optional RTP protocol for GigE Vision devices.
    ///
    /// When RTP is enabled it can be selected as the protocol used to data transmission
    /// at the stream channel level of the GenApi interface of the device.
    ///
    /// RTP supported is not covered by the GigE Vision specification. A GigE Vision Device
    /// actively streaming using RTP goes beyond the bounds covered by the GigE Vision standard.
    ///
    /// The RTP implementation is however compliant with the RTP RFCs covering the transport
    /// of images (RFC 4175) and H.264 data (RFC 6184).
    ///
    /// When a stream transmits using the RTP protocol, the stream is announced on a multicast
    /// channel (224.2.127.254:9875) using the SAP protocol with a SDP descriptor as required
    /// by the DEFSTAN 0082 specification.
    ///
    /// \param[in] aEnabled Toggles RTP support.
    ///
    /// \return #PvResult::Code::OK
    ///
    virtual PvResult SetRTPProtocolEnabled( bool aEnabled ) = 0;

    ///
    /// \brief 
    ///
    /// \return PvResult.
    ///
    virtual PvResult SetActionHandler( PvVirtualDeviceLib::IActionHandler *aActionHandler ) = 0;

    ///
    /// \brief Registers an event sink to receive notifications from the #PvSoftDeviceGEV.
    ///
    /// This method can be called before the #PvSoftDeviceGEV is started.
    ///
    /// \sa IPvVirtualDeviceEventSink
    ///
    /// \param[in] aEventSink Pointer to a #PvSoftDeviceGEV event sink.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if aEventSink is NULL or already registered.
    ///
    virtual PvResult RegisterEventSink( IPvSoftDeviceGEVEventSink *aEventSink ) = 0;

    ///
    /// \brief Unregisters a previously registered event sink.
    ///
    /// This method can be called before the #PvSoftDeviceGEV is started and after it has been stopped.
    ///
    /// \param[in] aEventSink Pointer to a #PvSoftDeviceGEV event sink.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::INVALID_PARAMETER if aEventSink is NULL.
    ///
    virtual PvResult UnregisterEventSink( IPvSoftDeviceGEVEventSink *aEventSink ) = 0;

    ///
    /// \brief Starts the #PvSoftDeviceGEV.
    ///
    /// Both GVCP and GVSP are operated from a background thread. Even though the Start method
    /// returns, the device is running in your process until you call Stop or until the 
    /// object is destroyed.
    ///
    /// \param[in] aMACAddress MAC address of the interface to bind the #PvSoftDeviceGEV to.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::STATE_ERROR if the #PvSoftDeviceGEV is already running.
    ///     - #PvResult::Code::INVALID_PARAMETER if the MAC address is not valid.
    ///
    virtual PvResult Start( const PvString &aIpAddress ) = 0;

    ///
    /// \brief Stops the #PvSoftDeviceGEV.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success.
    ///     - #PvResult::Code::STATE_ERROR if the #PvSoftDeviceGEV is not running.
    ///
    virtual PvResult Stop() = 0;

    ///
    /// \brief Obtains the information interface of the #PvSoftDeviceGEV.
    ///
    /// Only available after the #PvSoftDeviceGEV has been started.
    ///
    /// #IPvSoftDeviceGEVInfo is used to set the information strings in the read-only 
    /// bootstrap registers of the #PvSoftDeviceGEV, such as Model Name, Vendor Name, etc.
    ///
    /// \return Information interface of the #PvSoftDeviceGEV or NULL if the device is not running.
    ///
    virtual IPvSoftDeviceGEVInfo *GetInfo() = 0;

    ///
    /// \brief Obtains the register map interface of the #PvSoftDeviceGEV.
    ///
    /// Only available after the #PvSoftDeviceGEV has been started.
    ///
    /// #IPvRegisterMap can be used to directly access the register map of the 
    /// #PvSoftDeviceGEV. It is also possible to subscribe to register
    /// read/write events using #IPvSoftDeviceGEVEventSink.
    ///
    /// See #IPvSoftDeviceGEVEventSink::OnCreateCustomRegisters for information about adding custom
    /// registers to a #PvSoftDeviceGEV.
    ///
    /// \return Register map interface of the #PvSoftDeviceGEV or NULL if the device is not running.
    ///
    virtual IPvRegisterMap *GetRegisterMap() = 0;

    ///
    /// \brief Obtains the statistics interface of the #PvSoftDeviceGEV.
    ///
    /// Only available after the #PvSoftDeviceGEV has been started.
    ///
    /// \return Statistics interface of the #PvSoftDeviceGEV or NULL if the device is not running.
    ///
    virtual IPvSoftDeviceGEVStatistics *GetStatistics() = 0;

    ///
    /// \brief Obtains the messaging channel interface of the #PvSoftDeviceGEV.
    ///
    /// The #IPvMessageChannel interface can be used to send events on the messaging
    /// channel when a GigE Vision controller is connected to the #PvSoftDeviceGEV and has properly set up
    /// a destination for messaging channel events.
    ///
    /// \sa #IPvMessageChannel
    ///
    /// Only available after the #PvSoftDeviceGEV has been started.
    ///
    /// \return Messaging channel interface of the #PvSoftDeviceGEV or NULL if the device is not running.
    ///
    virtual IPvMessageChannel *GetMessagingChannel() = 0;
    //virtual IPvTrigger *GetTrigger( uint32_t aChannel ) = 0;

    ///
    /// \brief Returns the GenICam XML file of the device.
    ///
    /// Can be used to retrieve the GenICam XML file of the device after the Start
    /// method has been called. Can be used on success or failure of the Start method.
    ///
    /// \param aString [out] String where the XML file is copied.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK on success
    ///     - #PvResult::Code::STATE_ERROR if the GenICam XML file is not available (likely Start has not been called yet)
    ///
    virtual PvResult GetGenICamXMLFile( PvString &aString ) const = 0;
};

#endif
