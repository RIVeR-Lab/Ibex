// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDEVICEPLEORAPROTOCOL_H__
#define __PVDEVICEPLEORAPROTOCOL_H__

#include <PvDevice.h>

///
/// \class PvDevicePleoraProtocol
/// \brief Class simply used to configure the IP address of a Pleora Protocol device.
///
class PV_DEVICE_API PvDevicePleoraProtocol
{
public:
    ///
    /// \brief Sets the Pleora Protocol device's IP address.
    ///
    /// This static method lets you configure the Pleora Protocol device's IP address.
    ///
    /// \param[in]  aMACAddress    The Pleora Protocol device's MAC address.
    /// \param[in]  aIP            The Pleora Protocol device's new IP address.
    /// \param[in]  aSubnetMask    The Pleora Protocol device's new subnet mask.
    /// \param[in]  aGateway       The Pleora Protocol device's new gateway.
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
    PvDevicePleoraProtocol();
    PvDevicePleoraProtocol( const PvDevicePleoraProtocol & );
    const PvDevicePleoraProtocol &operator=( const PvDevicePleoraProtocol & );

};

#endif
