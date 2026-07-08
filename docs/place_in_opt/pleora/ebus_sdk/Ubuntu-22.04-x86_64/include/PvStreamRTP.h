// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVSTREAMRTP_H__
#define __PVSTREAMRTP_H__

#include <PvStream.h>

class PvSessionInfoSDP;


///
/// \class PvStreamRTP
///
/// \brief Receive data from a RTP transmitter.
///
/// This provides a means of opening a connection to the data receiver and obtaining data from it.
///
class PV_STREAM_API PvStreamRTP 
    : public PvStream
{
public:

    ///
    /// \brief Constructor.
    ///	
	PvStreamRTP();

    ///
    /// \brief Destructor
    ///
	virtual ~PvStreamRTP();

    ///
    /// \brief Opens a stream from a #PvDeviceInfo. Not supported with RTP.
    ///
    /// \param[in] aDeviceInfo The PvDeviceInfo input.
    ///
    /// \return #PvResult::Code::NOT_SUPPORTED
    ///
    PvResult Open( const PvDeviceInfo *aDeviceInfo );

    ///
    /// \brief Opens a stream from a device info string. Not supported with RTP.
    ///
    /// \param[in] aInfo The device info, formatted as a string.
    ///
    /// \return #PvResult::Code::NOT_SUPPORTED
    ///
    PvResult Open( const PvString &aInfo );

    ///
    /// \brief Tells the stream object to expect uncompressed images of a specific format.
    ///
    /// \param[in] aWidth The image's width to receive.
    /// \param[in] aHeight The image's height to receive.
    /// \param[in] aPixelType The image's pixel type to receive.
    /// \param[in] aPaddingX The image's padding to receive.
    ///
    /// \return #PvResult::Code::OK on success.
    ///
    PvResult PrepareFor( uint32_t aWidth, uint32_t aHeight, PvPixelType aPixelType, uint16_t aPaddingX = 0 );

    ///
    /// \brief Tells the stream object to expect an H.264 stream.
    ///
    /// \return #PvResult::Code::OK on success.
    ///
    PvResult PrepareForH264();

    ///
    /// \brief Resets the expected stream attributes to undefined.
    ///
    /// \return #PvResult::Code::OK on success.
    ///
    PvResult ResetPayloadType();

    ///
    /// \brief Opens the stream from a SDP session info.
    ///
    /// \param[in] aSession The SDP for a given stream.
    /// \param[in] aLocalIpAddress A local IP address to receive from.
    /// \param[in] aLocalPort A local port to receive from.
    ///
    /// \return #PvResult::Code::NOT_SUPPORTED
    ///
    PvResult Open( const PvSessionInfoSDP *aSession, const PvString &aLocalIPAddress, uint16_t aLocalPort = 0 );

    ///
    /// \brief Opens the stream object for unicast RTP receiving.
    ///
    /// \param[in] aLocalIpAddress A local IP address to receive from.
    /// \param[in] aLocalPort A local port to receive from.
    ///
    /// \return #PvResult::Code::OK on success.
    ///
    PvResult Open( const PvString &aLocalIpAddress, uint16_t aLocalPort );

    ///
    /// \brief Opens the stream object for multicast RTP receiving.
    ///
    /// \param[in] aMulticastAddress The multicast IP address of the stream.
    /// \param[in] aDataPort The stream port.
    /// \param[in] aLocalIpAddress A local IP address to receive from.
    ///
    /// \return #PvResult::Code::OK on success.
    ///
    PvResult Open( const PvString &aMulticastAddress, uint16_t aDataPort, const PvString &aLocalIpAddress );

    ///
    /// \brief Opens the stream object for TCP receiving.
    ///
    /// \param[in] aServerIPAddress The RTP server IP address.
    /// \param[in] aServerPort The RTP server port.
    ///
    /// \return #PvResult::Code::OK on success.
    ///
    PvResult OpenTCP( const PvString &aServerIpAddress, uint16_t aServerPort );

    ///
    /// \brief Receiver type.
    ///
    /// \return See #PvStreamType enum.
    ///
    PvStreamType GetType() const;

    ///
    /// \brief Returns the local socket port used by the stream.
    ///
    /// \return Stream local port.
    ///
    uint16_t GetLocalPort() const;

    ///
    /// \brief Returns the local IPaddress used by the stream.
    ///
    /// \return Stream local IP address.
    ///
    PvString GetLocalIPAddress() const;

    ///
    /// \brief Returns the multicast IP address of the stream.
    ///
    /// \return Stream multipcast IP addres if applicable.
    ///
    PvString GetMulticastIPAddress() const;

    ///
    /// \brief Returns the IP address of the server stream.
    ///
    /// Only valid in TCP mode.
    ///
    /// \return Server IP address.
    ///
    PvString GetTCPServerIPAddress() const;

    ///
    /// \brief Returns the port of the stream server.
    ///
    /// Only valid in TCP mode.
    ///
    /// \return Server port.
    ///
    uint16_t GetTCPServerPort() const;

    ///
    /// \brief Returns thread priority of the data receiver thread.
    ///
    /// \return Thread priority.
    ///
    uint32_t GetThreadPriority() const;

    ///
    /// \brief Set the priority of the thread used to run the data receiver.
    ///
    /// If you change the thread priorities, it may alterate the stability of the system. For this reason,
    /// if you report a bug to Pleora, ensure to clearly mention that you change threads priorities.
    ///
    /// See #GetThreadPriority
    /// 
    /// \param[in] aPriority See #PvPipeline::SetBufferHandlingThreadPriority for description of the supported values
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::THREAD_ERROR
    ///
    PvResult SetThreadPriority( uint32_t aPriority );

private:

private:

	 // Not implemented
	PvStreamRTP( const PvStreamRTP & );
    const PvStreamRTP &operator=( const PvStreamRTP & );
};


#endif
