// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVINTERFACE_H__
#define __PVINTERFACE_H__

#include <PvSystemLib.h>
#include <PvDeviceInfo.h>\


namespace PvSystemLib
{
    class System;
    class NetworkAdapter;
    class USBHostController;

    class DeviceInfoVector;
    class IFinderReporter;
}


///
/// \class PvInterface
///
/// \brief Represents one interface (network adapter or USB host controller) on a system (the PC)
///
/// To use this class, see #PvSystem.
///
class PV_SYSTEM_API PvInterface
{
public:

    ///
    /// \brief Destructor.
    ///
    virtual ~PvInterface();

    ///
    /// \brief Creates a copy of the PvInterface object.
    ///
    /// The user is responsible from freeing/deleting the object after use. The object is fully
    /// detached from PvSystem or the device finder dialog object.
    ///
    /// \return Pointer to the PvInterface copy. The user is reponsable to delete/free this pointer.
    ///
    PvInterface *Copy() const;

    ///
    /// \brief Get the interface type.
    ///
    /// \return Type of interface (network interface or USB host controller). See #PvInterfaceType Enum.
    ///
    PvInterfaceType GetType() const;

    ///
    /// \brief Get the description of the NIC, as reported by the OS.
    ///
    /// \return The description of the NIC, as reported by the OS. The value is intended
    ///         only for human reading.
    ///
    PvString GetName() const;

    ///
    /// \brief Get a string that can be used in a user interface to identify the interface.
    ///
    /// \return The interface's display ID.
    ///
    PvString GetDisplayID() const;

    ///
    /// \brief Get a string uniquely identifying the interface.
    ///
    /// The content of the unique ID is not garanteed to be represent anything or to be
    /// constant between revisions of the eBUS SDK. It should only be used to opaquely,
    /// uniquely match other interface unique IDs from within the same application
    /// life cycle.
    ///
    /// Do not assume the unique ID will be using a MAC address for network adapters or 
    /// anything specifc for USB host controllers.
    ///
    /// If you need the MAC address of a network adapter, use #PvNetworkAdapter::GetMACAddress.
    ///
    /// If you need specific information about a USB host controller, use the PvUSBHostController class.
    ///
    /// \return The interface's unique ID.
    ///
    PvString GetUniqueID() const;

    ///
    /// \brief Get a string that can be used to sort interfaces.
    ///
    /// The content of the compare ID is not garanteed to be represent anything or to be
    /// constant between revisions of the eBUS SDK.
    ///
    /// It should only be used to opaquely, uniquely compare other interface sorting IDs
    /// to allow consistent sorting and ordering of interfaces, typically in a user 
    /// interface context.
    ///
    /// \return String that can be used to sort interfaces.
    ///
    PvString GetCompareID() const;

    ///
    /// \brief Get the number of found devices on this #PvInterface.
    ///
    /// \return The number of devices found through this #PvInterface. 
    ///    
    uint32_t GetDeviceCount() const;

    ///
    /// \brief Get information identifying a device.
    ///
    /// Returns information identifying a device discovered on this interface (network adapter or USB host controller).
    /// This information can be used to connect and control it with a #PvDevice object. 
    /// 
    /// To connect, use #PvDevice::CreateAndConnect (supports both GigE Vision and USB3 Vision devices), #PvDeviceGEV::Connect or #PvDeviceU3V::Connect.
    ///
    /// \param[in] aIndex  The device's array position. The zero-based
    ///                     parameter accepts 0 through #GetDeviceCount-1. 
    ///
    /// \return Information identifying a device; a #PvDeviceInfo object. 
    ///         If <i>aIndex</i> is out of bounds, the return is NULL.
    ///
    const PvDeviceInfo *GetDeviceInfo( uint32_t aIndex ) const;

    ///
    /// \brief Static method used for sorting interfaces
    ///
    /// \param[in] a1 A first interface.
    /// \param[in] a2 A second interface.
    ///
    /// \return
    ///     - true if the value of the first character that doesn't match is lower in a1's ID or all compared characters match but the compared ID is shorter.
    ///     - false otherwise.
    ///
    static bool Compare( const PvInterface *a1, const PvInterface *a2 );

    /// \brief Get the eBUS driver version installed on this #PvInterface.
    /// 
    /// \return
    ///     - The eBUS driver version, if installed. The numbers are, in order: the major, minor, subminor and build number (format: #.#.#.#.####).
    ///     - An empty string if the eBUS driver is not currently installed on the interface.
    PvString GetPleoraDriverVersion() const;

protected:

#ifndef PV_GENERATING_DOXYGEN_DOC

    PvInterface( PvSystemLib::IFinderReporter *aFinderReporter, PvInterfaceType aType );
	PvInterface&operator=( const PvInterface &aFrom );

    void SetName( const std::string &aValue ) { *mName = aValue; }
    void SetDisplayID( const std::string &aValue ) { *mDisplayID = aValue; }
    void SetUniqueID( const std::string &aValue ) { *mUniqueID = aValue; }
    void SetCompareID( const std::string &aValue ) { *mCompareID = aValue; }
    void SetDriverVersion( const std::string &aValue ) { *mDriverVersion = aValue; }

    void SetConfigurationValid( bool aValue ) { mConfigurationValid = aValue; }

    PvSystemLib::DeviceInfoVector *GetDevices() { return mDevices; }

#endif // PV_GENERATING_DOXYGEN_DOC

    friend class PvSystemLib::System;
    friend class PvSystemLib::NetworkAdapter;
    friend class PvSystemLib::USBHostController;

private:

	 // Not implemented
	PvInterface( const PvInterface & );

    PvInterfaceType mType;

    std::string *mName;
    std::string *mDisplayID;
    std::string *mUniqueID;
    std::string *mCompareID;
    std::string *mDriverVersion;

    bool mConfigurationValid;

    PvSystemLib::DeviceInfoVector *mDevices;

};

#endif
