// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDEVICEINFOGEV_H__
#define __PVDEVICEINFOGEV_H__

#include <PvDeviceInfo.h>


///
/// \class PvDeviceInfoGEV
///
/// \brief Information identifying a GigE Vision device
///
class PV_SYSTEM_API PvDeviceInfoGEV : public PvDeviceInfo
{
public:

    ///
    /// \brief Constructor.
    ///
    PvDeviceInfoGEV();

    ///
    /// \brief Destructor.
    ///
	virtual ~PvDeviceInfoGEV();

    ///
    /// \brief Equal operator: copies the object.
    ///
    /// \param[in] aFrom The source for the copy.
    ///
    /// \return Reference to the object aFrom was copied to.
    ///
	PvDeviceInfoGEV &operator=( const PvDeviceInfoGEV &aFrom );

    ///
    /// \brief Get the GigE Vision device's MAC address.
    ///
    /// \return The GigE Vision device's MAC address, in the form <i>aa</i>:<i>bb</i>:<i>cc</i>:<i>dd</i>:<i>ee</i>:<i>ff</i>, 
    ///         where <i>aa</i> through <i>ff</i> are lowercase hexadecimal numbers ranging from 0x00 to 0xff.
    ///
    PvString GetMACAddress() const;

    ///
    /// \brief Get the GigE Vision device's current IP address.
    ///
    /// \return The GigE Vision device's current IP address; the <b>GevCurrentIPAddress</b> feature. The form is <i>a</i>.<i>b</i>.<i>c</i>.<i>d</i>, 
    ///         where <i>a</i> through <i>d</i> are decimal numbers ranging from 0 to 255.
    ///
    PvString GetIPAddress() const;

    ///
    /// \brief Get the GigE Vision device's default gateway.
    ///
    /// \return The GigE Vision device's default gateway; the <b>GevCurrentDefaultGateway</b> feature. For the form, see #GetIPAddress.
    /// 
    PvString GetDefaultGateway() const;

    ///
    /// \brief Get the GigE Vision device's subnet mask.
    ///
    /// \return The GigE Vision device's subnet mask; the <b>GevCurrentSubnetMask</b> feature. For the form, see #GetIPAddress.
    ///
    PvString GetSubnetMask() const;

    ///
    /// \brief Get the GigE Vision device's network protocol version.
    ///
    /// The high 16 bits contain the major version and The low 16 bits the minor version.
    ///
    /// \return The GigE Vision device's network protocol version.
    ///
    uint32_t GetGEVVersion() const;

    ///
    /// \brief Get the GigE Vision device's IP config options.
    ///
    /// \return The GigE Vision device's IP config options; The form is <i>LLA(L)</i><i>DHCP(D)</i><i>Persistent_IP(P)</i><i>PAUSE_reception(PR)</i><i>PAUSE_generation(PG)</i>,
    ///          where each represents device's possible IP configuration options.
    ///
    PvString GetIPConfigOptionsString() const;

    ///
    /// \brief Get the GigE Vision device's current IP config.
    ///
    /// \return The GigE Vision device's current IP config; The form is <i>LLA(L)</i><i>DHCP(D)</i><i>Persistent_IP(P)</i><i>PAUSE_reception(PR)</i><i>PAUSE_generation(PG)</i>,
    ///         where each represents device's currently set IP configuration.
    ///
    PvString GetIPConfigCurrentString() const;

    ///
    /// \brief Check if LLA is one of the IP config options for the GigE Vision device.
    ///
    /// \return True, if LLA is an option, False otherwise.
    ///
    bool IsLLAAvailable() const;

    ///
    /// \brief Check if DHCP is one of the IP config options for the GigE Vision device.
    ///
    /// \return True, if DHCP is an option, False otherwise.
    ///
    bool IsDHCPAvailable() const;

    ///
    /// \brief Check if Persistent_IP is one of the IP config options for the GigE Vision device.
    ///
    /// \return True, if Persistent_IP is an option, False otherwise.
    ///
    bool IsPersistentAvailable() const;

    ///
    /// \brief Check if PAUSE_reception is one of the IP config options for the GigE Vision device.
    ///
    /// \return True, if PAUSE_reception is an option, False otherwise.
    ///
    bool IsPRAvailable() const;

    ///
    /// \brief Check if PAUSE_generation is one of the IP config options for the GigE Vision device.
    ///
    /// \return True, if PAUSE_generation is an option, False otherwise.
    ///
    bool IsPGAvailable() const;

    ///
    /// \brief Check if LLA is in current IP config for the GigE Vision device.
    ///
    /// \return True, if LLA is in current IP config, False otherwise.
    ///
    bool IsLLAEnabled() const;

    ///
    /// \brief Check if DHCP is in current IP config for the GigE Vision device.
    ///
    /// \return True, if DHCP is in current IP config, False otherwise.
    ///
    bool IsDHCPEnabled() const;

    ///
    /// \brief Check if Persistent_IP is in current IP config for the GigE Vision device.
    ///
    /// \return True, if Persistent_IP is in current IP config, False otherwise.
    ///
    bool IsPersistentEnabled() const;

    ///
    /// \brief Check if PAUSE_reception is in current IP config for the GigE Vision device.
    ///
    /// \return True, if PAUSE_reception is in current IP config, False otherwise.
    ///
    bool IsPREnabled() const;

    ///
    /// \brief Check if PAUSE_generation is in current IP config for the GigE Vision device.
    ///
    /// \return True, if PAUSE_generation is in current IP config, False otherwise.
    ///
    bool IsPGEnabled() const;

protected:

#ifndef PV_GENERATING_DOXYGEN_DOC

	PvDeviceInfoGEV( PvInterface *aInterface );

    void Init();

    void SetIPAddress( const std::string &aValue ) { *mIPAddress = aValue; }
    void SetMACAddress( const std::string &aValue ) { *mMACAddress = aValue; }
    void SetDefaultGateway( const std::string &aValue ) { *mDefaultGateway = aValue; }
    void SetSubnetMask( const std::string &aValue ) { *mSubnetMask = aValue; }

    void SetGEVVersion( uint32_t aValue ) { mGEVVersion = aValue; }
    void SetIPConfigOptions( uint32_t aValue ) { mIPConfigOptions = aValue; }
    void SetIPConfigCurrent( uint32_t aValue ) { mIPConfigCurrent = aValue; }

#endif // PV_GENERATING_DOXYGEN_DOC

private:

	 // Not implemented
    PvDeviceInfoGEV( const PvDeviceInfoGEV & );

    std::string *mIPAddress;
    std::string *mMACAddress;
    std::string *mDefaultGateway;
    std::string *mSubnetMask;
    
    uint32_t mGEVVersion;
    uint32_t mIPConfigOptions;
    uint32_t mIPConfigCurrent;

};

#endif