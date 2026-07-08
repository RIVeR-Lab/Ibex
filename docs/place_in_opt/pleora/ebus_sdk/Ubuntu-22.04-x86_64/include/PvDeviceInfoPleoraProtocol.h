// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDEVICEINFOPLEORAPROTOCOL_H__
#define __PVDEVICEINFOPLEORAPROTOCOL_H__

#include <PvDeviceInfo.h>


///
/// \class PvDeviceInfoPleoraProtocol
///
/// \brief Information about a Pleora Protocol device.
///
/// The eBUS SDK allows enumerating devices using the Pleora protocol. However, it does not allow
/// connecting to or controlling these device.
///
class PV_SYSTEM_API PvDeviceInfoPleoraProtocol : public PvDeviceInfo
{
public:

    ///
    /// \brief Constructor.
    ///
    PvDeviceInfoPleoraProtocol();

    ///
    /// \brief Destructor.
    ///
	virtual ~PvDeviceInfoPleoraProtocol();

    ///
    /// \brief Equal operator: copies the object.
    ///
    /// \param[in] aFrom The source for the copy.
    ///
    /// \return Reference to the object aFrom was copied to.
    ///
	PvDeviceInfoPleoraProtocol &operator=( const PvDeviceInfoPleoraProtocol & );

    ///
    /// \brief Get the Pleora Protocol device's MAC address.
    ///
    /// \return The Pleora Protocol device's MAC address, in the form <i>aa</i>:<i>bb</i>:<i>cc</i>:<i>dd</i>:<i>ee</i>:<i>ff</i>, 
    ///         where <i>aa</i> through <i>ff</i> are lowercase hexadecimal numbers ranging from 0x00 to 0xff.
    ///
    PvString GetMACAddress() const;

    ///
    /// \brief Get the Pleora Protocol device's current IP address.
    ///
    /// \return The Pleora Protocol device's current IP address; the <b>GevCurrentIPAddress</b> feature. The form is <i>a</i>.<i>b</i>.<i>c</i>.<i>d</i>, 
    ///         where <i>a</i> through <i>d</i> are decimal numbers ranging from 0 to 255.
    ///
    PvString GetIPAddress() const;

    ///
    /// \brief Get the Pleora Protocol device's subnet mask.
    ///
    /// \return The Pleora Protocol device's subnet mask; the <b>GevCurrentSubnetMask</b> feature. For the form, see #GetIPAddress.
    ///
    PvString GetSubnetMask() const;

    ///
    /// \brief Get the Pleora Protocol device's default gateway.
    ///
    /// \return The Pleora Protocol device's default gateway; the <b>GevCurrentDefaultGateway</b> feature. For the form, see #GetIPAddress.
    ///
    PvString GetDefaultGateway() const;

    ///
    /// \brief Returns the Device ID of the Pleora Protocol device.
    ///
    /// \return Device ID.
    ///
    uint8_t GetDeviceID() const;

    ///
    /// \brief Returns the Module ID of the Pleora Protocol device.
    ///
    /// \return Module ID.
    ///
    uint8_t GetModuleID() const;

    ///
    /// \brief Returns the Sub ID of the Pleora Protocol device.
    ///
    /// \return Sub ID.
    ///
    uint8_t GetSubID() const;

    ///
    /// \brief Returns the Vendor ID of the Pleora Protocol device.
    ///
    /// \return Vendor ID.
    ///
    uint8_t GetVendorID() const;

    ///
    /// \brief Major version of the firmware running on the Pleora Protocol device.
    ///
    /// \return Major version of the firmware.
    ///
    uint8_t GetSoftwareMajor() const;

    ///
    /// \brief Minor version of the firmware running on the Pleora Protocol device.
    ///
    /// \return Minor version of the firmware.
    ///
    uint8_t GetSoftwareMinor() const;

protected:

#ifndef PV_GENERATING_DOXYGEN_DOC

	PvDeviceInfoPleoraProtocol( PvInterface *aInterface );

    void Init();

    void SetIPAddress( const std::string &aValue ) { *mIPAddress = aValue; }
    void SetMACAddress( const std::string &aValue ) { *mMACAddress = aValue; }
    void SetSubnetMask( const std::string &aValue ) { *mSubnetMask = aValue; }
    void SetDefaultGateway( const std::string &aValue ) { *mDefaultGateway = aValue; }

    void SetDeviceID( uint8_t aValue ) { mDeviceID = aValue; }
    void SetModuleID( uint8_t aValue ) { mModuleID = aValue; }
    void SetSubID( uint8_t aValue ) { mSubID = aValue; }
    void SetVendorID( uint8_t aValue ) { mVendorID = aValue; }
    void SetSoftwareMajor( uint8_t aValue ) { mSoftwareMajor = aValue; }
    void SetSoftwareMinor( uint8_t aValue ) { mSoftwareMinor = aValue; }

#endif // PV_GENERATING_DOXYGEN_DOC

private:

	 // Not implemented
    PvDeviceInfoPleoraProtocol( const PvDeviceInfoPleoraProtocol & );

    std::string *mIPAddress;
    std::string *mMACAddress;
    std::string *mSubnetMask;
    std::string *mDefaultGateway;

    uint8_t mDeviceID;
    uint8_t mModuleID;
    uint8_t mSubID;
    uint8_t mVendorID;
    uint8_t mSoftwareMajor;
    uint8_t mSoftwareMinor;

};

#endif
