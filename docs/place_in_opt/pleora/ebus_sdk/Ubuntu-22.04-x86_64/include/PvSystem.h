// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVSYSTEM_H__
#define __PVSYSTEM_H__

#include <PvSystemEventSink.h>
#include <PvNetworkAdapter.h>
#include <PvUSBHostController.h>


namespace PvSystemLib
{
    class System;
}


///
/// \struct PvSessionInfo
///
/// \brief Describes a multimedia session (typically an RTP stream) discovered via SAP.
///

typedef struct {
    PvString mName; // The textual session name
    PvString mAddress; // The session connection address (an IPv4 address)
    uint16_t mPort; // The session transport port
    PvString mLocalAddress; // The local address on which the session was discovered
} PvSessionInfo;


///
/// \class PvSystem
///
/// \brief Find interfaces (network adapters or USB host controllers) and devices reachable from this PC
///
/// #PvSystem finds all interfaces and returns #PvInterface objects. #PvInterface finds all devices on a specific 
/// interface and returns #PvDeviceInfo based objects. #PvDeviceInfo provides the information required to connect to 
/// a GigE Vision or USB3 Vision device.
///
class PV_SYSTEM_API PvSystem
{
public:

    ///
    /// \brief Constructor.
    ///
    PvSystem();

    ///
    /// \brief Destructor.
    ///
    virtual ~PvSystem();

    ///
    /// \brief Find all interfaces and GigE Vision, USB3 Vision and Pleora Protocol devices reachable from this system.
    ///
    /// This method is blocking and won't return for the duration
    /// set using #SetDetectionTimeout.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///
    PvResult Find();

    ///
    /// \brief Finds devices until a specific device is found.
    ///
    /// Performs a standard find operation and keeps listening for answers
    /// until a specific device is found.
    ///
    /// More than one device can be found and added to the interfaces during this
    /// find operation. Do not assume only the device of interest is present
    /// in the system/interface data after this method returns.
    ///
    /// If the device of interest is found, a pointer to the device info in the interface
    /// where the device was found is returned using the aDeviceInfo parameter.
    ///
    /// \param[in] aDeviceToFind String representing the device to find. Can be a MAC, IP or device name.
    /// \param[out] aDeviceInfo If the device is found, contains a pointer to a PvDeviceInfo about the device requested in aDeviceToFind.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult FindDevice( const PvString &aDeviceToFind, const PvDeviceInfo **aDeviceInfo );

    ///
    /// \brief Sets the time to search the system devices.
    ///
    /// Time the #PvSystem::Find methods wait for responses from GigE Vision or Pleora Protocol
    /// devices before returning.
    ///
    /// GigE Vision devices may wait up to 1 second before answering a discovery request and this
    /// detection timeout allows enough time for all discovery answers to come back in.
    ///
    /// USB3 Vision devices detection is instant. You can set this timeout to its minimum supported
    /// value if you are only interested in detecting USB3 Vision devices.
    ///
    /// Pleora Protocol and some GigE Vision devices answer as soon as the discovery request is received.
    /// Experimenting with this timeout and discovery stability is a way improving discovery performance.
    /// Another would be to stick with the safe, default delay and use the event sink callback to be
    /// notified as soon as a device is discovered.
    ///
    /// \param[in]  aTimeout   Time to search for devices in ms. The default value of 1500 is generally a good value in most situations.
    ///                         Values beyond 100 to 60000 are set to 100 and 60000, respectively.
    ///
    void SetDetectionTimeout( uint32_t aTimeout );

    ///
    /// \brief Get the maximum time to search for GigE Vision devices.
    ///
    /// Search here is defined as the time to wait devices to answer after 
    /// sending a discovery through each interface.
    ///
    /// Only applies to Ethernet devices as USB devices enumeration does not 
    /// include the notion of wait or timeout.
    ///
    /// See #SetDetectionTimeout.
    /// 
    /// \return See #SetDetectionTimeout.
    ///
    uint32_t GetDetectionTimeout() const;

    ///
    /// \brief Sets whether subnet broadcasts are used (or not) when detecting GigE Vision devices.
    ///
    /// \param[in] aValue True if subnet broadcasts are to be used when detecting GigE Vision devices.
    ///
    void SetSubnetBroadcastEnabled( bool aValue );

    ///
    /// \brief Gets whether subnet broadcasts are used (or not) when detecting GigE Vision devices.
    ///
    /// \return True if subnet broadcasts are to be used when detecting GigE Vision devices.
    ///
    bool GetSubnetBroadcastEnabled() const;

    ///
    /// \brief Get the version of GigE Vision spec supported by the SDK.
    ///
    /// The high 16 bits contain the major version and The low 16 bits the minor version.
    ///
    /// \return GigE Vision spec version supported by the SDK.
    ///
    uint32_t GetGEVSupportedVersion() const;

    ///
    /// \brief Get the version of USB3 Vision spec supported by the SDK.
    ///
    /// The high 16 bits contain the major version and The low 16 bits the minor version.
    ///
    /// \return USB3 Vision spec version supported by the SDK.
    ///
    uint32_t GetU3VSupportedVersion() const;

    ///
    /// \brief Register an event sink (for device finding notification).
    ///
    /// To use this method, see #PvSystemEventSink.
    ///
    /// \param[in]  aEventSink   A pointer to the event sink object.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::GENERIC_ERROR
    ///
    PvResult RegisterEventSink( PvSystemEventSink *aEventSink );

    ///
    /// \brief Unregister an event sink
    ///
    /// To use this method, see #PvSystemEventSink.
    ///
    /// \param[in]  aEventSink   A pointer to the event sink object.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND
    ///
    PvResult UnregisterEventSink( PvSystemEventSink *aEventSink );

    ///
    /// \brief Get the number of interfaces detected on the system
    ///
    /// \return The number of interfaces detected on the system; the range for the #PvSystem::GetInterface method.
    ///
    uint32_t GetInterfaceCount() const;

    ///
    /// \brief Get a #PvInterface.
    ///
    /// Interfaces are ordered in the #PvSystem array as provided by the OS.
    ///
    /// \param[in] aIndex The interface's index in the array; a number ranging from 0 to #GetInterfaceCount-1.
    ///
    /// \return A pointer to a #PvInterface object, or NULL if the index is out of bounds.
    ///
    const PvInterface *GetInterface( uint32_t aIndex ) const;

    ///
    /// \brief Returns number of device that can be enumerated from PvSystem using GetDeviceInfo.
    ///
    /// \return Device count.
    ///
    uint32_t GetDeviceCount() const;

    ///
    /// \brief Returns a pointer to a device of the PvSystem.
    ///
    /// The GetDeviceCount and GetDeviceInfo methods of PvSystem are a shortcut
    /// for users only interested in retrieving devices without having to handle the
    /// interface to device topology.
    ///
    /// \param[in] aIndex Zero-based index of the device info. Valid range is [0 .. GetDeviceCount - 1].
    ///
    /// \return Pointer to the requested PvDeviceInfo.
    ///
    const PvDeviceInfo *GetDeviceInfo( uint32_t aIndex ) const;

    ///
    /// \brief Scans for active sessions advertised via SAP. 
    /// This is typically used to discover RTP streams, and is unnecessary to call for GEV and U3V devices.
    ///
    /// \return The number of active sessions currently advertised with SAP.
    ///
    uint32_t DiscoverSessions() const;

    ///
    /// \brief Returns SDP information about an active session (unused by GEV and U3V).
    ///
    /// \param[in] aIndex Zero-based index of the session to retrieve information about. Valid range is [0 .. DiscoverSessions() - 1].
    /// \param[in, out] aSessionInfo If the session is found, fills the #PvSessionInfo with session name and connection information.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND
    ///
    PvResult GetSessionInfo( uint32_t aIndex, PvSessionInfo &aSessionInfo ) const;

protected:

private:

     // Not implemented
    PvSystem( const PvSystem & );
    const PvSystem &operator=( const PvSystem & );

    PvSystemLib::System *mThis;

};

#endif
