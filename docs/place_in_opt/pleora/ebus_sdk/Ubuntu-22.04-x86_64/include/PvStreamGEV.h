// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVSTREAMGEV_H__
#define __PVSTREAMGEV_H__

#ifndef EBUS_ENUM
#define EBUS_ENUM(x) typedef enum
#endif

#include <PvStream.h>


EBUS_ENUM(PvDataReceiverType)
{
    PvDataReceiverTypeUnknown = -1,
    PvDataReceiverTypeEBusDriver = 0,
    PvDataReceiverTypeUserMode = 1,
    PvDataReceiverTypeTCP = 2

} PvDataReceiverType;


///
/// \class PvStreamGEV
///
/// \brief Receive data from a GigE Vision.
///
/// This provides a means of opening a connection to the data receiver and obtaining data from it.
///
class PV_STREAM_API PvStreamGEV : public PvStream
{
public:

    ///
    /// \brief Constructor.
    ///	
	PvStreamGEV();

    ///
    /// \brief Destructor
    ///
	virtual ~PvStreamGEV();

    ///
    /// \brief Begin receiving blocks (images or raw data) <b>unicasted</b> from a GigE Vision transmitter.
    ///
    /// See equivalent Open method with aInfo PvString as first parameter for more information.
    ///
    /// \param[in] aDeviceInfo    Device information as retrieved from a device finder.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::INVALID_PARAMETER    
    ///     - #PvResult::Code::NOT_FOUND
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::BAD_VERSION
    ///
    PvResult Open( const PvDeviceInfo *aDeviceInfo );

    ///
    /// \brief Begin receiving blocks (images or raw data) <b>unicasted</b> from a GigE Vision transmitter.
    ///
    /// See equivalent Open method with aInfo PvString as first parameter for more information.
    ///
    /// \param[in] aDeviceInfo      Device information as retrieved from a device finder.
    /// \param[in] aLocalPort       The local port upon which to begin receiving incoming data. A value of 0 (default) indicates that the local port will be assigned automatically. Call #GetLocalPort afterwards to obtain the port number that was assigned by the system. Set the streaming channel destination address (SCDA) on your GigE Vision transmitter accordingly by calling #PvDeviceGEV::SetStreamDestination.
    /// \param[in] aChannel         The GigE Vision transmitter's streaming channel (default 0).
    /// \param[in] aLocalIPAddress  Deprecated and ignored.
    /// \param[in] aBuffersCapacity The maximum number of buffers that can be stored simultaneously in the data receiver.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::INVALID_PARAMETER    
    ///     - #PvResult::Code::NOT_FOUND
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::BAD_VERSION
    ///
    PvResult Open( const PvDeviceInfo *aDeviceInfo, uint16_t aLocalPort, uint16_t aChannel = 0, const PvString & aLocalIpAddress = PvString(), uint32_t aBuffersCapacity = 64 );

    ///
    /// \brief Begin receiving blocks (images or raw data) <b>multicasted</b> from a GigE Vision transmitter.
    ///
    /// See equivalent Open method with aInfo PvString as first parameter for more information.
    ///
    /// \param[in] aDeviceInfo     Device information as retrieved from a device finder.
    /// \param[in] aMulticastAddr   The multicast IP address to which the receiver subscribes and the GigE Vision transmitter sends data. (Set the streaming channel destination address (SCDA) on your GigE Vision transmitter accordingly by calling #PvDeviceGEV::SetStreamDestination.)
    /// \param[in] aDataPort        The multicast port to which the receiver subscribes and the GigE Vision transmitter sends data. (Set the streaming channel port (SCP) on your GigE Vision transmitter accordingly by calling #PvDeviceGEV::SetStreamDestination.)
    /// \param[in] aChannel         The GigE Vision transmitter's streaming channel (default 0).
    /// \param[in] aLocalIPAddress Deprecated and ignored.
    /// \param[in] aBuffersCapacity The maximum number of buffers that can be stored simultaneously in the data receiver.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::INVALID_PARAMETER    
    ///     - #PvResult::Code::NOT_FOUND
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::BAD_VERSION
    ///
    PvResult Open( const PvDeviceInfo *aDeviceInfo, const PvString & aMulticastAddr, uint16_t aDataPort, uint16_t aChannel = 0, const PvString & aLocalIPAddress = PvString(), uint32_t aBuffersCapacity = 64 );

    ///
    /// \brief Begin receiving blocks (images or raw data) <b>unicasted</b> from a GigE Vision transmitter.
    ///
    /// Note that this overload of Open is for receiving blocks that are unicasted from a GigE Vision transmitter to an interface on the same subnet.
    /// To receive multicasted data, refer to the other overload of the other (multicast) Open method.
    /// 
    /// \param[in] aInfo            The GigE Vision transmitter's IP address.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::INVALID_PARAMETER    
    ///     - #PvResult::Code::NOT_FOUND
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::BAD_VERSION
    ///
    PvResult Open( const PvString &aInfo );

    ///
    /// \brief Begin receiving blocks (images or raw data) <b>unicasted</b> from a GigE Vision transmitter.
    ///
    /// Note that this overload of Open is for receiving blocks that are unicasted from a GigE Vision transmitter to an interface on the same subnet.
    /// To receive multicasted data, refer to the other overload of the other (multicast) Open method.
    /// 
    /// \param[in] aInfo            The GigE Vision transmitter's IP address.
    /// \param[in] aLocalPort       The local port upon which to begin receiving incoming data. A value of 0 (default) indicates that the local port will be assigned automatically. Call #GetLocalPort afterwards to obtain the port number that was assigned by the system. Set the streaming channel destination address (SCDA) on your GigE Vision transmitter accordingly by calling #PvDeviceGEV::SetStreamDestination.
    /// \param[in] aChannel         The GigE Vision transmitter's streaming channel (default 0).
    /// \param[in] aLocalIPAddress  The local IP address upon which to begin receiving incoming data. An empty string (default) indicates that the local IP address will be assigned automatically. Call #GetLocalIPAddress afterwards to obtain the local IP address that was assigned by the system. Set the streaming channel port number (SCP) on your GigE Vision transmitter accordingly by calling #PvDeviceGEV::SetStreamDestination.
    /// \param[in] aBuffersCapacity The maximum number of buffers that can be stored simultaneously in the data receiver.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::INVALID_PARAMETER    
    ///     - #PvResult::Code::NOT_FOUND
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::BAD_VERSION
    ///
    PvResult Open( const PvString &aInfo, uint16_t aLocalPort, uint16_t aChannel = 0, const PvString & aLocalIpAddress = PvString(), uint32_t aBuffersCapacity = 64 );

    ///
    /// \brief Begin receiving blocks (images or raw data) <b>multicasted</b> from a GigE Vision transmitter.
    ///
    /// Note that this overload of Open is for receiving blocks that are multicasted from a GigE Vision transmitter to a given multicast group.
    /// To receive unicasted data, refer to the other overload of the Open method.
    /// 
    /// Multicasting is the delivery of data to a group of receivers simultaneously in a single transmission.
    ///
    /// The following example illustrates how to multicast data from one transmitter to a given set of receivers:
    ///     - Connect receivers and transmitter to an IGMP compliant switch.
    ///     - The GigE Vision transmitter begins transmitting to a multicast group (i.e. 239.192.1.1:1042).
    ///     - The GigE Vision transmitter has a control channel listening for device discovery requests and packet resend requests on a given IP address (i.e. 169.254.1.1).
    ///     - Receiver applications discover the device and subscribe to the multicast group (call <b>Open( "169.254.1.1", "239.192.1.1", 1042 )</b>)
    ///
    /// Why does the GigE Vision device address (<i>aIPAddress</i>) need to be specified?
    /// In this example, the data is transmitted using the GVSP protocol to a multicast group (239.192.1.1:1042).
    /// The receiving application contacts the device (IP address 169.254.1.1) using the GVCP protocol to ensure that it is reachable.
    /// In addition to this, packet resend requests are directed from receiver to GigE Vision device using the GVCP control channel.
    /// This is by design as per the GigEVision standard.
    ///
    /// Multicasting uses the IGMP (Internet Group Management Protocol) and requires an IGMP-compliant switch.
    /// The switch maintains a list of subscribed peers associated with a given multicast group.
    /// Multicast group IP addresses take the form <tt>239.192.<i>xx</i>.<i>yy</i></tt>. 
    /// It is also possible to use multicasting with a non-IGMP compliant switch or network. In that case, the multicast stream is simply
    /// broadcasted to every connected entity on the network. It can be OK in controlled environment, but use with caution!
    /// To learn more about IGMPv3, see <i>RFC 3376</i> at www.ietf.org.
    ///
    /// For a complete illustration of how to use this method to receive data multicasted from a GigE Vision transmitter, refer to the following two samples:
    /// - MulticastMaster - Connects to a GigE Vision device as a controler and tells it to begin streaming to a multicast address.
    /// - MulticastSlave - Illustrates the use of PvStream multicast Open method by receiving the data the GigE Vision device is transmitting.
    ///
    /// \param[in] aInfo            The GigE Vision transmitter's IP address.
    /// \param[in] aMulticastAddr   The multicast IP address to which the receiver subscribes and the GigE Vision transmitter sends data. (Set the streaming channel destination address (SCDA) on your GigE Vision transmitter accordingly by calling #PvDeviceGEV::SetStreamDestination.)
    /// \param[in] aDataPort        The multicast port to which the receiver subscribes and the GigE Vision transmitter sends data. (Set the streaming channel port (SCP) on your GigE Vision transmitter accordingly by calling #PvDeviceGEV::SetStreamDestination.)
    /// \param[in] aChannel         The GigE Vision transmitter's streaming channel (default 0).
    /// \param[in] aLocalIPAddress  The local IP address upon which the stream object will begin receiving incoming data. An empty string (default) indicates that the local ip address will be assigned automatically.
    /// \param[in] aBuffersCapacity The maximum number of buffers that can be stored simultaneously in the data receiver.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::INVALID_PARAMETER    
    ///     - #PvResult::Code::NOT_FOUND
    ///     - #PvResult::Code::GENERIC_ERROR
    ///     - #PvResult::Code::BAD_VERSION
    ///
    PvResult Open( const PvString &aInfo, const PvString & aMulticastAddr, uint16_t aDataPort, uint16_t aChannel = 0, const PvString & aLocalIPAddress = PvString(), uint32_t aBuffersCapacity = 64 );

    ///
    /// \brief Returns whether the object is a GigE Vision or USB3 Vision stream receiver.
    ///
    /// \return Specific object type.
    ///
    virtual PvStreamType GetType() const;

    ///
    /// \brief Returns the type of GigE Vision data receiving used by the stream receiver.
    ///
    /// \return Data receiver type. See #PvDataReceiverType Enum.
    ///
    PvDataReceiverType GetDataReceiverType() const;

    ///
    /// \brief Flushes all non-processed UDP packets from the data receiver.
    ///
    /// Should be called before resuming streaming after a pause
    /// to ensure that old packets are not processed
    /// as new incoming data.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///
    PvResult FlushPacketQueue();

    ///
    /// \brief Wait for the first packet of the block to start the stream capture <a href="streamparameters.html#StartStreamOnlyOnPacketIdZero">StartStreamOnlyOnPacketIdZero</a> property.
    ///
    /// \return The value of the <a href="streamparameters.html#StartStreamOnlyOnPacketIdZero">StartStreamOnlyOnPacketIdZero</a> property.
    ///
    bool GetWaitForFirstPacketOfBlockToStart() const;

    ///
    /// \brief Set the stream's <a href="streamparameters.html#StartStreamOnlyOnPacketIdZero">StartStreamOnlyOnPacketIdZero</a> property.
    ///
    /// \param[in]  aWaitForFirstPacketOfBlockToStart   The value being set.
    /// 
    /// \return #PvResult::Code::OK or #PvResult::Code::NOT_SUPPORTED or #PvResult::Code::NOT_CONNECTED
    ///
    PvResult SetWaitForFirstPacketOfBlockToStart( bool aWaitForFirstPacketOfBlockToStart );

    ///
    /// \brief Get the stream's <a href="streamparametersGEV.html#DataPort">DataPort</a> property.
    ///
    /// \return The stream's <a href="streamparametersGEV.html#DataPort">DataPort</a> property.
    ///
    uint16_t GetLocalPort() const;

    ///
    /// \brief Get the stream's <a href="streamparametersGEV.html#LocalIPAddress">LocalIPAddress</a> property.
    ///
    /// \return The stream's <a href="streamparametersGEV.html#LocalIPAddress">LocalIPAddress</a> property.
    ///
    PvString GetLocalIPAddress() const;

    /// 
    /// \brief Get the stream's <a href="streamparametersGEV.html#MulticastIPAddress">MulticastIPAddress</a> property.
    ///
    /// \return The stream's <a href="streamparametersGEV.html#MulticastIPAddress">MulticastIPAddress</a> property.
    ///
    PvString GetMulticastIPAddress() const;

    ///
    /// \brief Get the IP address of the device this stream receives data from.
    ///
    /// \return IP address of the streaming device.
    ///
    PvString GetDeviceIPAddress() const;

    ///
    /// \brief Returns the local UDP port requested when opening the stream.
    ///
    /// 0 is returned if the user did not open the stream requesting a specific local UDP port if if the stream is not opened.
    ///
    /// \return Local UDP port requested when opening the stream.
    ///
    uint16_t GetSpecificLocalPort() const;

    ///
    /// \brief Get the priority of the thread used to run the data receiver user mode
    ///
    /// See #SetUserModeDataReceiverThreadPriority for details about the meaning of the values returned
    /// 
    /// \return The priority of the thread
    ///
    uint32_t GetUserModeDataReceiverThreadPriority() const;

    ///
    /// \brief Set the priority of the thread used to run the data receiver user mode
    ///
    /// Only applicable if using a user-mode data receiver. The thread priority of a 
    /// driver-based data receiver cannot be controlled. This thread receives and processes 
    /// all packets received from the device.
    ///
    /// If you change the thread priorities, it may alterate the stability of the system. For this reason,
    /// if you report a bug to Pleora, ensure to clearly mention that you change threads priorities.
    ///
    /// See #GetUserModeDataReceiverThreadPriority
    /// 
    /// \param[in] aPriority   See #PvPipeline::SetBufferHandlingThreadPriority for description of the supported values
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::THREAD_ERROR
    ///
    PvResult SetUserModeDataReceiverThreadPriority( uint32_t aPriority );

    ///
    /// \brief Read the current Receive Buffer Size set by the PvStreamGEV that the socket uses.
    ///
    /// The API is ONLY supported in User Mode when eBUS UniversalProForEthernet Driver is not used.
    ///
    /// See #SetUserModeBufferSizeRx for details.
    ///
    /// \param[out] aSize  The Receive Buffer Size used in the socket opened by the current PvStreamGEV instance.
    ///
    /// \return
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::NOT_SUPPORTED
    ///
    PvResult GetUserModeSocketRxBufferSize( uint32_t &aBufferSize );

    ///
    /// \brief Set the Receive Buffer Size through socket option SO_RCVBUF by PvStreamGEV.
    ///
    /// The API is ONLY supported in User Mode when eBUS UniversalProForEthernet Driver is not used.
    ///
    /// The API allows the user to configure the SO_RCVBUF - the size of the buffer the kernel allocates to hold the data
    /// arriving into the given socket between the time when it arrives over the network and when it is read by eBUS SDK User Mode Data Receiver.
    ///
    /// In case the method was called when eBUS UniversalProForEthernet Driver is used, the value will be ignored.
    ///
    /// This API must be called before PvStreamGEV::Open(), otherwise you will receive NETWORK_CONFIG_ERROR.
    /// The actual value set can be confirmed by #GetUserModeSocketRxBufferSize after PvStream::Open() returns successfully.
    /// And it's recommended to always call #GetUserModeSocketRxBufferSize to validate the value set.
    ///
    /// To configure and confirm the value used by the socket, you should perform this sequence:
    ///     - #PvStreamGEV.SetUserModeSocketRxBufferSize
    ///     - #PvStreamGEV.Open
    ///     - #PvStreamGEV.GetUserModeSocketRxBufferSize
    /// 
    /// *
    /// On Linux System:
    /// 
    /// The value allowed is limited by rmem_max defined on your Linux System.
    /// If the value set exceeds rmem_max, rmem_max will be used instead, and the method will return INVALID_PARAMETER.
    ///
    /// The rmem_max value on your Linux system could be configured by script set_socket_buffer_size.sh provided by eBUS SDK.
    ///
    /// In case the value #PvStreamGEV.GetUserModeBufferSizeRx retrieved is not what you expected, check the rmem_max setting first.
    ///
    /// Note: The kernel doubles the value set by #SetUserModeSocketRxBufferSize (to allow space for bookkeeping overhead) if possible.
    /// The retrived value from #GetUserModeSocketRxBufferSize could be doubled, but should not exceed rmem_max * 2.
    /// *
    ///
    /// Note: the set value is not permanent or global, it is only for the current PVStreamGEV instance.
    ///
    /// \param[in] aSize  The Receive Buffer Size to use with socket option SO_RCVBUF.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NETWORK_CONFIG_ERROR
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetUserModeSocketRxBufferSize( uint32_t aSize );

    ///
    /// \brief Set ther Receive Packet Buffer Size used by PvStreamGEV.
    ///
    /// The API is ONLY supported in User Mode when eBUS UniversalProForEthernet Driver is not used.
    ///
    /// In case the method was called on Linux system when eBUS UniversalProForEthernet Driver is used, the value will be ignored.
    ///
    /// This API allows the user to increase the max jumbo packet size eBUS SDK supports.
    /// The minimum value can be set is 17000, the default value of eBUS SDK, to support all physical NICs. 
    /// The maximum value can be set is 65500, to support Virtual NICs.
    /// eBUS will set the value to min or max for you if the aSize value is out of range.
    ///
    /// Note: the set value is not permanent or global, it is only for the current PVStreamGEV instance.
    /// 
    /// This API must be called before PvStreamGEV::Open(), otherwise you will receive NETWORK_CONFIG_ERROR.
    ///
    /// To configure the value, you should perform this sequence:
    ///     - #PvStreamGEV.SetUserModeRxPacketBufferSize
    ///     - #PvStreamGEV.Open
    ///
    /// \param[in] aSize  The Receive Packet Buffer Size to use when using User Mode DataReceiver. Minimum supported value is 17000 bytes, Maximum is 65500 bytes.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NETWORK_CONFIG_ERROR
    ///
    PvResult SetUserModeRxPacketBufferSize( uint32_t aSize );

    ///
    /// \brief Test if the specified GigE Vision device is currently reachable through an eBUS driver
    ///
    /// \param[in]   aIPAddress     The GigE Vision transmitter's IP address.
    /// \param[out]  aInstalled     True if the device is reachable through an eBUS driver; otherwise, false.
    /// \param[in]  aLocalIPAddress Deprecated and ignored.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    static PvResult IsDriverInstalled( PvString &aIPAddress, bool &aInstalled, const PvString & aLocalIPAddress = PvString() );

protected:

private:

	 // Not implemented
	PvStreamGEV( const PvStreamGEV & );
    const PvStreamGEV &operator=( const PvStreamGEV & );
   

};

#endif
