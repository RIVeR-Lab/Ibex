// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDEVICEGEV_H__
#define __PVDEVICEGEV_H__

#include <PvDevice.h>

///
/// \class PvDeviceGEV
/// \brief Connect, configure and control a GigE Vision device
///
/// <b>To connect and configure a GigE Vision device to begin transmitting data:</b>
///     - Select the GigE Vision device to which you want to connect. See #PvSystem, #PvInterface, #PvDeviceInfo.
///     - Connect to a GigE Vision device. Use #Connect.
///     - Get the GigE Vision device's GenICam interface. Use #GetParameters.
///     - Configure the device through the GenICam interface. See #PvGenParameterArray.
///     - Configure the receiver and take note of its IP address and port.
///     - Configure your #PvDevice's streaming destination using #PvDeviceGEV::SetStreamDestination.
///     - Lock the interface for streaming. Set <b>TLParamsLocked</b> feature of the GigE Vision device's GenICam interface to 1.
///     - Configure the GigE Vision device to send images. Use <b>AcquisitionStart</b> feature of the GigE Vision device's GenICam interface.
///     - Release the interface for streaming. Set <b>TLParamsLocked</b> feature of the GigE Vision device's GenICam interface to 0.
///     - Configure the GigE Vision device to stop sending images. Use <b>AcquisitionStop</b> feature of the GigE Vision device's GenICam interface.
///
/// For a complete example that illustrates the steps above, refer to the following samples:
///     - MulticastMaster - Configure a GigE Vision device to transmit
///     - PvPipelineSample - Configure a GigE Vision device to transmit and receive the data it transmits in the same application
///
/// <h2>Recovery</h2>
/// 
/// The SDK can let you automatically recover from problems such as accidental disconnects,
/// power interrupts, and so on. To learn more, see #PvDeviceEventSink.
///
class PV_DEVICE_API PvDeviceGEV : public PvDevice
{
public:
    ///
    /// \brief Constructor.
    ///
	PvDeviceGEV();

    ///
    /// \brief Destructor
    ///
	virtual ~PvDeviceGEV();

    ///
    /// \brief Connect to a GigE Vision device.
    ///
    /// The #Connect method connects to a GigE Vision device found using a #PvSystem or device finder dialog object.
    /// Only one device can be connected to at a time.
    /// 
    /// \param[in] aDeviceInfo    The GigE Vision device's network connectivity information; a #PvDeviceInfo object.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::NO_LICENSE
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::GENICAM_XML_ERROR
    ///
	PvResult Connect( const PvDeviceInfo *aDeviceInfo );

    ///
    /// \brief Connect to a GigE Vision device (found with #PvSystem, #PvInterface, #PvDeviceInfo).
    ///
    /// The #Connect method connects to a GigE Vision device found using a #PvSystem or device finder dialog object.
    /// Only one device can be connected to at a time.
    /// 
    /// \param[in] aDeviceInfo    The GigE Vision device's network connectivity information; a #PvDeviceInfo object.
    /// \param[in] aAccessType    The GigE Vision device's availability for connections by multiple
    ///                            controllers. See #PvAccessType.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::NO_LICENSE
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::GENICAM_XML_ERROR
    ///
	PvResult Connect( const PvDeviceInfo *aDeviceInfo, PvAccessType aAccessType );

    ///
    /// \brief Connect to a GigE Vision device with the default access control mode.
    ///
    /// If you know the GigE Vision device's IP address, MAC address or DeviceUserID, you can use this method to connect to it.
    ///
    ///     - If aInfo contains an IP address, the connection attempt is direct.
    ///     - If aInfo contains a MAC address or DeviceUserID:
    ///         - An internal #PvSystem is used to find all reachable GigE Vision devices
    ///         - Found devices are browsed, looking for a match on either MAC address or DeviceUserID
    ///         - If the device is found, connection is attempted.
    ///
    /// \param[in] aInfo          Information that uniquely identifies the GigE Vision device. You may use one of the following:
    ///                               - GigE Vision device's MAC address. Use the form <tt>00:11:22:33:44:5F</tt>.
    ///                               - GigE Vision device's IP Address. Use the form <tt>192.168.1.114</tt>.
    ///                               - GigE Vision device's DeviceUserID.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::NO_LICENSE
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::GENICAM_XML_ERROR
    ///
	PvResult Connect( const PvString &aInfo );

    ///
    /// \brief Connect to a GigE Vision device.
    ///
    /// If you know the GigE Vision device's IP address, MAC address or DeviceUserID, you can use this method to connect to it.
    ///
    ///     - If aInfo contains an IP address, the connection attempt is direct.
    ///     - If aInfo contains a MAC address or DeviceUserID:
    ///         - An internal #PvSystem is used to find all reachable GigE Vision devices
    ///         - Found devices are browsed, looking for a match on either MAC address or DeviceUserID
    ///         - If the device is found, connection is attempted.
    ///
    /// \param[in] aInfo          Information that uniquely identifies the GigE Vision device. You may use one of the following:
    ///                               - GigE Vision device's MAC address. Use the form <tt>00:11:22:33:44:5F</tt>.
    ///                               - GigE Vision device's IP Address. Use the form <tt>192.168.1.114</tt>.
    ///                               - GigE Vision device's DeviceUserID.
    /// \param[in] aAccessType    The access control privilege to set at the time of connection. See #PvAccessType.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::NO_LICENSE
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::GENICAM_XML_ERROR
    ///
    PvResult Connect( const PvString &aInfo, PvAccessType aAccessType );

    ///
    /// \brief Set the UDP/IP destination to which the GigE Vision device transmits data.
    ///
    /// The destination constists of an IP address (unicast or multicast) and a port number.
    ///
    /// \param[in] aIPAddress   The receiving PC's IP address (GenICam feature name: <b>GevSCDA</b>) or a multicast IP address.
    /// \param[in] aDataPort    The receiving PC's data port (feature name: <b>GevSCPHostPort</b>) or the multicast port.
    /// \param[in] aChannel     The GigE Vision stream channel; the channel over which the GigE Vision device transmits images.
    ///                          Use 0 for device having supporting a single streaming channel.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::TIMEOUT
    ///     - #PvResult::Code::GENERIC_ERROR
    ///
    PvResult SetStreamDestination( const PvString &aIPAddress, uint16_t aDataPort, uint32_t aChannel = 0 );

    ///
    /// \brief Resets the UDP/IP streaming destination of the GigE Vision device.
    ///
    /// This method is equivalent to calling #SetStreamDestination with values ("0.0.0.0", 0, <i>aChannel</i>).
    ///
    /// \param[in] aChannel  See the <i>aChannel</i> parameter for the #SetStreamDestination method.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::TIMEOUT 
    ///     - #PvResult::Code::GENERIC_ERROR
    ///
    PvResult ResetStreamDestination( uint32_t aChannel = 0 );

    ///
    /// \brief Sets the packet size for a streaming channel.
    ///
    /// Applies a specific packet size to a streaming channel. Unlike the NegotiatePacketSize method,
    /// it does not attempt to find the best packet size, it just uses the one provided as a parameter.
    ///
    /// \param[in]  aPacketSize  The packet size to use for aChannel.
    /// \param[in]  aChannel     The streaming channel. Use 0 for most GigE Vision devices only supporting a single streaming channel.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::TIMEOUT
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::NOT_FOUND
    ///
    PvResult SetPacketSize( uint32_t aPacketSize, uint32_t aChannel = 0 );

    ///
    /// \brief Automatically finds and sets the optimal packet size.
    ///
    /// The #NegotiatePacketSize method tests the network connection for the largest as possible packet size 
    /// that the network can support between the GigE Vision transmitter and the controlling PC.
    ///
    /// Calling this method resets the streaming destination for aChannel. You should always call this
    /// method AFTER connecting, BEFORE setting the GigE Vision streaming destination using SetStreamDestination.
    ///
    /// The highest possible packet size from { MTU (if below 9000), 9000, 8000, 4000, 2000, 1500, 1000, 600 } is used. These values are
    /// used as raw seeds and are adjusted by removing 24 bytes for the Ethernet layer and then rounded down to meet
    /// the increment requirement imposed by the GevSCPSPacketSize parameter.
    ///
    /// \param[in]  aChannel            The streaming channel. Use 0 for most GigE Vision devices only supporting a single streaming channel.
    /// \param[in] aDefaultPacketSize   The packet size to use, in bytes, should the negotiation fail.
    ///                                  Use 1476 for a safe packet size; 0 to continue using the GigE Vision device's original setting in case of failure.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::TIMEOUT
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::NOT_FOUND
    ///
    PvResult NegotiatePacketSize( uint32_t aChannel = 0, uint32_t aDefaultPacketSize = 0 );

    ///
    /// \brief Read a 32-bit value from a specific register address on the GigE Vision device.
    /// 
    /// aValue is returned in the platform's expected Endianess. GigE Vision single register 
    /// operations are always performed as Big Endian and on Intel platforms are corrected to/from Little Endian.
    ///
    /// \param[in]  aAddress The GigE Vision device's register address (mod 4).
    /// \param[out] aValue   The value read from the GigE Vision device's register.
    ///
    /// \return See #WriteMemory.
    ///
	PvResult ReadRegister( int64_t aAddress, uint32_t &aValue );

    ///
    /// \brief Write a 32-bit value to a specific register address on the GigE Vision device.
    /// 
    /// See warnings about accessing the register map directly in #WriteMemory.
    ///
    /// aValue is provided in the platform's expected Endianess. GigE Vision single register 
    /// operations are always performed as Big Endian and on Intel platforms are corrected to/from Little Endian.
    ///
    /// \param[in] aAddress     The GigE Vision device's register address (mod 4).
    /// \param[in] aValue       The value to be written to the register.
    /// \param[in] aAcknowledge When sending the command, sets the acknowledge requested bit and waits for that acknowledge.
    ///                          If set to false, the acknowledge is not requested and the method returns as soon as the command has been
    ///                          pushed in the UDP socket. If not specified true is assumed.
    ///
    /// \return See #WriteMemory.
    ///
	PvResult WriteRegister( int64_t aAddress, uint32_t aValue, bool aAcknowledge = true );

    ///
    /// \brief Returns the access type of the connected device.
    ///
    /// If no device is connected, PvAccessUnknown is returned.
    ///
    /// \return Access type of the device was connected with. See #PvAccessType enum
    ///
    PvAccessType GetAccessType() const;

    ///
    /// \brief Returns the GigE Vision device class of the connected device.
    ///
    /// If no device is connected, PvDeviceClassTransmitter is returned.
    ///
    /// \return Device class of the device was connected with. See #PvDeviceClass enum
    ///
    PvDeviceClass GetClass() const;

    ///
    /// \brief Returns the IP address of the connected device.
    ///
    /// If no device is connected, an empty string is returned.
    ///
    /// \return IP address of the device.
    ///
    PvString GetIPAddress() const;

    ///
    /// \brief Returns the MAC address of the connected device.
    ///
    /// If no device is connnected, an emptyu string is returned.
    ///
    /// \return MAC address of the device.
    ///
    PvString GetMACAddress() const;

    ///
    /// \brief Returns the current access type of a device identified by its IP address.
    ///
    /// This function is static. It does not work on an specific object but will return the access type
    /// of a GigE Vision device identified by its IP address.
    ///
    /// \param[in] aDeviceIPAddress The IP address of the GigE Vision device.
    /// \param[out] aAccessType The access type of the GigE Vision device on success. See #PvAccessType enum. PvAccessUnknown on failure.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::TIMEOUT
    ///     - #PvResult::Code::GENERIC_ERROR
    ///
    static PvResult GetAccessType( const PvString &aDeviceIPAddress, PvAccessType &aAccessType );

    ///
    /// \brief Sets the GigE Vision device's IP address.
    ///
    /// This static method lets you configure the GigE Vision device's IP address. You cannot
    /// change the IP address when a PvDevice is connected.
    ///
    /// Can be used to assign a valid IP configuration to a device before connecting a #PvDevice to a
    /// GigE Vision device (using the #Connect method). The GigE Vision device to be configured is identified using its MAC address.
    ///
    /// \param[in]  aMACAddress    The GigE Vision device's MAC address.
    /// \param[in]  aIP            The GigE Vision device's new IP address.
    /// \param[in]  aSubnetMask    The GigE Vision device's new subnet mask.
    /// \param[in]  aGateway       The GigE Vision device's new gateway.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    static PvResult SetIPConfiguration( 
        const PvString &aMACAddress, 
        const PvString &aIP, 
        const PvString &aSubnetMask = PvString( "255.255.255.0" ), 
        const PvString &aGateway = PvString( "0.0.0.0" ) );

protected:
    
private:

	 // Not implemented
	PvDeviceGEV( const PvDeviceGEV & );
	const PvDeviceGEV &operator=( const PvDeviceGEV & );

};

#endif
