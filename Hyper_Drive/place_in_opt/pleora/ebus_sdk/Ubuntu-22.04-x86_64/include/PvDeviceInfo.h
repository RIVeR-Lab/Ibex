// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PV_DEVICEINFO_H__
#define __PV_DEVICEINFO_H__

#include <PvSystemLib.h>
#include <PvSystemEnums.h>


class PvInterface;
class PvDevice;

///
/// \class PvDeviceInfo
/// \brief Information identifying a device
///
/// #PvDeviceInfo provides information about a device (GigE Vision, USB3 Vision or Pleora Protocol) as found by either #PvSystem/#PvInterface
/// or device finder dialog.
///
/// To use #PvDeviceInfo, see #PvDevice.
///
class PV_SYSTEM_API PvDeviceInfo
{
public:

    ///
    /// \brief Destructor.
    ///
	virtual ~PvDeviceInfo();

    ///
    /// \brief Creates a copy of the PvDeviceInfo object.
    ///
    /// The user is responsible from freeing/deleting the object after use. The object is fully
    /// detached from PvSystem or the device finder dialog object.
    ///
    /// \return Pointer to the PvDeviceInfo copy. The user is reponsable to delete/free this pointer.
    ///
    PvDeviceInfo *Copy() const;

    ///
    /// \brief Returns the type of the device info object.
    ///
    /// \return Type of the device info object. see #PvDeviceInfoType enum.
    ///
    PvDeviceInfoType GetType() const;

    ///
    /// \brief Get the device's manufacturer name.
    ///
    /// \return The device's manufacturer name.
    ///
    PvString GetVendorName() const;

    ///
    /// \brief Get the device's model name.
    ///
    /// \return The device's model name.
    ///
    PvString GetModelName() const;

    ///
    /// \brief Get the device's version.
    ///
    /// \return The GigE Vision or USB3 Vision device's firmware version.
    ///
    PvString GetVersion() const;

    ///
    /// \brief Get the manufacturer information returned by the device
    ///
    /// \return The manufacturer information returned by the GigE Vision or USB3 Vision device.
    ///
    PvString GetManufacturerInfo() const;

    ///
    /// \brief Get the GigE Vision device's serial number.
    ///
    /// \return The GigE Vision or USB3 Vision device's serial number represented as a string.
    ///
    PvString GetSerialNumber() const;

    ///
    /// \brief Get the device's user defined name.
    ///
    /// This feature is only supported by devices having persistence memory.
    /// It can therefor be blank if not supported by the device, or simply
    /// not yet assigned.
    ///
    /// \return The device's name.
    ///
    PvString GetUserDefinedName() const;
    
    ///
    /// \brief Get a message of whether license is valid, and if not why
    ///
    /// This string provides information on validity of the license. If the license is valid, then
    /// it is simply 'valid'. Otherwise, the reason for the invalidity is provided as a string. 
    ///
    /// \return A string indicating 'valid' if license is valid, or a reason for invalid license
    ///
    PvString GetLicenseMessage() const;

    ///
    /// \brief Get a string identifying this device for display purpose.
    ///
    /// \return A string identifying the device.
    ///
    PvString GetDisplayID() const;

    ///
    /// \brief Get a string uniquely identifying this device.
    ///
    /// The content of the unique ID is not garanteed to be represent anything or to be
    /// constant between revisions of the eBUS SDK. It should only be used to opaquely,
    /// uniquely match other device unique IDs from within the same application
    /// life cycle.
    ///
    /// Do not assume the unique ID will be using a MAC address for network devices or 
    /// the USB GUID for USB devices.
    ///
    /// If you need the MAC address of a GigE Vision device, use #PvDeviceInfoGEV::GetMACAddress.
    ///
    /// If you need the USB GUID of a USB3 Vision device, use #PvDeviceInfoU3V::GetGUID.
    ///
    /// \return A unique string identifying the device.
    ///
    PvString GetUniqueID() const;

    ///
    /// \brief Get a unique string identifying this device for connection purpose.
    ///
    /// This string can be used with the factory methods of PvDevice and PvStream to
    /// create a GigE Vision device or USB3 Vision device from  generic PvDeviceInfo.
    ///
    /// \return A unique string identifying the device.
    ///
    PvString GetConnectionID() const;
    
    ///
    /// \brief Get the network adapter (#PvInterface) through which this device was reached.
    ///
    /// \return A pointer to the interface the device was found on.
    ///
    const PvInterface *GetInterface() const;

    ///
    /// \brief Test if the device can be reached as currently configured.
    ///
    /// This method compares the GigE Vision device and NIC's IP configurations to ensure they are
    /// on the same subnet. For USB3 Vision devices, the configuration is considered valid if the 
    /// device can be enumerated and the Pleora driver is associated to the device.
    ///
    /// \return True if valid.
    ///
    bool IsConfigurationValid() const;

    ///
    /// \brief Test if the eBUS SDK license is valid for this device.
    ///
    /// To connect to third-party devices, the eBUS SDK requires a valid license.
    /// If you don't have a license, contact sales.
    ///
    /// \return True if the license is valid; otherwise, false.
    ///
    bool IsLicenseValid() const;

    ///
    /// \brief Get the device class.
    ///
    /// \return The device's class; a PvDeviceClass enumeration.
    ///
    PvDeviceClass GetClass() const;

protected:

#ifndef PV_GENERATING_DOXYGEN_DOC

	PvDeviceInfo( PvDeviceInfoType, PvInterface *aInterface );
	const PvDeviceInfo &operator=( const PvDeviceInfo &aFrom );

    void Init();

    void SetLicenseValid( bool aValue ) { mLicenseValid = aValue; }
    void SetClass( PvDeviceClass aValue ) { mClass = aValue; }
    void SetConfigurationValid( bool aValue ) { mConfigurationValid = aValue; }

    void SetVendorName( const std::string &aValue ) { *mVendorName = aValue; }
    void SetModelName( const std::string &aValue ) { *mModelName = aValue; }
    void SetVersion( const std::string &aValue ) { *mVersion = aValue; }
    void SetManufacturerInfo( const std::string &aValue ) { *mManufacturerInfo = aValue; }
    void SetSerialNumber( const std::string &aValue ) { *mSerialNumber = aValue; }
    void SetUserDefinedName( const std::string &aValue ) { *mUserDefinedName = aValue; }

    void SetConnectionID( const std::string &aValue ) { *mConnectionID = aValue; }
    void SetDisplayID( const std::string &aValue ) { *mDisplayID = aValue; }
    void SetUniqueID( const std::string &aValue ) { *mUniqueID = aValue; }
    void SetCompareID( const std::string &aValue ) { *mCompareID = aValue; }
    void SetLicenseMessage( const std::string &aValue ) { *mLicenseMessage = aValue; }

    std::string *GetCompareID() { return mCompareID; }

#endif // PV_GENERATING_DOXYGEN_DOC

private:

    bool mLicenseValid;
    bool mConfigurationValid;

    const PvInterface *mInterface;
    bool mInterfaceOwned;

    std::string *mVendorName;
    std::string *mModelName;
    std::string *mVersion;
    std::string *mManufacturerInfo;
    std::string *mSerialNumber;
    std::string *mUserDefinedName;
    
    std::string *mConnectionID;
    std::string *mDisplayID;
    std::string *mUniqueID;
    std::string *mCompareID;
    std::string *mLicenseMessage;

    PvDeviceInfoType mType;      
    PvDeviceClass mClass;

	 // Not implemented
    PvDeviceInfo();
    PvDeviceInfo( const PvDeviceInfo & );

};


#endif // __PV_DEVICEINFO_H__

