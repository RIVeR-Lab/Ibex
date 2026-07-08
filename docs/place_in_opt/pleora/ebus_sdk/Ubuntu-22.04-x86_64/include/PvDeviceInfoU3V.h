// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDEVICEINFOU3V_H__
#define __PVDEVICEINFOU3V_H__

#include <PvDeviceInfoUSB.h>


///
/// \class PvDeviceInfoU3V
///
/// \brief Information about a USB3 Vision device.
///
class PV_SYSTEM_API PvDeviceInfoU3V : public PvDeviceInfoUSB
{
public:

    ///
    /// \brief Constructor.
    ///
    PvDeviceInfoU3V();

    ///
    /// \brief Destructor.
    ///
	virtual ~PvDeviceInfoU3V();

    ///
    /// \brief Equal operator: copies the object.
    ///
    /// \param[in] aFrom The source for the copy.
    ///
    /// \return Reference to the object aFrom was copied to.
    ///
    PvDeviceInfoU3V &operator=( const PvDeviceInfoU3V & );

    ///
    /// \brief Get the USB3 Vision device's GenCP protocol version.
    ///
    /// The high 16 bits contain the major version and The low 16 bits the minor version.
    ///
    /// \return The USB3 Vision device's GenCP protocol version.
    ///
    uint32_t GetGenCPVersion() const;

    ///
    /// \brief Get the USB3 Vision device's USB3 Vision protocol version.
    ///
    /// The high 16 bits contain the major version and The low 16 bits the minor version.
    ///
    /// \return The USB3 Vision device's USB3 Vision protocol version.
    ///
    uint32_t GetU3VVersion() const;

    ///
    /// \brief Get the USB3 Vision GUID of the device.
    ///
    /// GUID stands for "Globally Unique Identifier". It is the USB3 Vision counterpart of 
    /// the MAC of a GigE Vision device.
    ///
    /// \return USB3 Vision GUID of the device.
    ///
    PvString GetDeviceGUID() const;

    ///
    /// \brief Returns the family name of the device.
    ///
    /// \return Family name of the device.
    ///
    PvString GetFamilyName() const;

    ///
    /// \brief Returns the U3V serial number of the device.
    ///
    /// \return U3V serial number of the device.
    ///
    PvString GetU3VSerialNumber() const;

    ///
    /// \brief Returns the U3V device path.
    ///
    /// Cryptic, internal device key used to load the driver for a device from a device info.
    ///
    /// \return U3V device key.
    ///
    PvString GetDevicePath() const;

    ///
    /// \brief Returns the U3V device key.
    ///
    /// Cryptic, internal device key used to load the driver for a device from a device info.
    ///
    /// \return U3V device key.
    ///
    PvString GetDeviceKey() const;

    ///
    /// \brief Get a message of the speed negotiated between the controller and the device (low, high, full, super) or an error message
    ///
    /// \return 
    ///    A string indicating the negotiated speed ('low', 'high', 'full', 'super') or an error message:
    ///     - 'Unknown' if the negotiation speed is not known
    ///     - 'Connect to USB 3.0 port" if a device that can only support higher speeds is connected to lower speed ports
    ///
    PvString GetSpeedMessage() const;

    ///
    /// \brief Get a message of whether device is powered properly or not
    ///
    /// This string provides information on whether the device seems to be powered properly or not
    ///
    /// \return A string indicating 'Ok' if devices seems to be powered or an error message (e.g. "Ensure correct power") otherwise
    ///
    PvString GetPowerMessage() const;

    ///
    /// \brief Returns true if the device supports USB1.0 speed.
    ///
    /// \return True if the device supports low speed.
    ///
	bool IsLowSpeedSupported() const;

    ///
    /// \brief Returns true if the device supports USB1.1 speed.
    ///
    /// \return True if the device supports full speed.
    ///
    bool IsFullSpeedSupported() const;

    ///
    /// \brief Returns true if the device supports USB2 speed.
    ///
    /// \return True if the device supports high speed.
    ///
    bool IsHighSpeedSupported() const;

    ///
    /// \brief Returns true if the device supports USB3 speed.
    ///
    /// \return True if the device supports super speed.
    ///
    bool IsSuperSpeedSupported() const;

    ///
    /// \brief Returns true if the current speed of the device is supported by the host controller.
    ///
    /// \return True if the host controller supports the current speed of the device.
    ///
    bool IsCurrentSpeedSupported() const;

    ///
    /// \brief Returns the currently effective connection speed of the USB3 Vision device.
    ///
    /// If a device supporting super speed is connected in a slower hub or controller it is
    /// not able to negotiate up to its maximum supported speed.
    ///
    /// If a device IsSuperSpeedSupported returns true and GetSpeed returns something slower
    /// look into the supported speed of the hubs chain to the controller or the controller itself.
    ///
    /// \return Effective connection speed. See #PvUSBSpeed Enum.
    ///
    PvUSBSpeed GetSpeed() const;

    ///
    /// \brief Power consumption of the device in milliamp.
    ///
    /// \return Power consumption of the device.
    ///
    uint32_t GetMaxPower() const;

    ///
    /// \brief Maximum packet size that can be used by the device in bytes.
    ///
    /// \return Maximum packet size.
    ///
    uint32_t GetMaxPacketSize() const;

    ///
    /// \brief Returns true if the currently selected driver for the device is the Pleora driver.
    ///
    /// \return True is the device is using the Pleora driver.
    ///
    bool IsPleoraDriverInstalled() const;

    ///
    /// \brief Returns true if the device U3V capabilities were retrieved and initialized
    ///
    /// \return True if the device's capabilities are accessible, false otherwise (indicating device may not be powered properly)
    ///
    bool IsInitializedCapabilities() const;

protected:

#ifndef PV_GENERATING_DOXYGEN_DOC

	PvDeviceInfoU3V( PvInterface *aInterface );

    void Init();

    void SetGenCPVersion( uint32_t aValue ) { mGenCPVersion = aValue; }
    void SetU3VVersion( uint32_t aValue ) { mU3VVersion = aValue; }

    void SetDeviceGUID( const std::string &aValue ) { *mDeviceGUID = aValue; }
    void SetFamilyName( const std::string &aValue ) { *mFamilyName = aValue; }
    void SetU3VSerialNumber( const std::string &aValue ) { *mU3VSerialNumber = aValue; }
    void SetDevicePath( const std::string &aValue ) { *mDevicePath = aValue; }
    void SetDeviceKey( const std::string &aValue ) { *mDeviceKey = aValue; }
    void SetSpeedMessage( const std::string &aValue ) { *mSpeedMessage = aValue; }
    void SetPowerMessage( const std::string &aValue ) { *mPowerMessage = aValue; }

    void SetLowSpeedSupported( bool aValue ) { mLowSpeedSupported = aValue; }
    void SetFullSpeedSupported( bool aValue ) { mFullSpeedSupported = aValue; }
    void SetHighSpeedSupported( bool aValue ) { mHighSpeedSupported = aValue; }
    void SetSuperSpeedSupported( bool aValue ) { mSuperSpeedSupported = aValue; }
    void SetCurrentSpeedSupported( bool aValue ) { mCurrentSpeedSupported = aValue; }

    void SetSpeed( PvUSBSpeed aValue ) { mSpeed = aValue; }

    void SetMaxPower( uint32_t aValue ) { mMaxPower = aValue; }
    void SetMaxPacketSize( uint32_t aValue ) { mMaxPacketSize = aValue; }

    void SetPleoraDriverInstalled( bool aValue ) { mPleoraDriverInstalled = aValue; }
    void SetInitializedCapabilities( bool aValue ) { mInitializedCapabilities = aValue; }

#endif // PV_GENERATING_DOXYGEN_DOC

private:

	 // Not implemented
    PvDeviceInfoU3V( const PvDeviceInfoU3V & );

    uint32_t mGenCPVersion;
    uint32_t mU3VVersion;

    std::string *mDeviceGUID;
    std::string *mFamilyName;
    std::string *mU3VSerialNumber;
    std::string *mDevicePath;
    std::string *mDeviceKey;
    std::string *mSpeedMessage;
    std::string *mPowerMessage;

    bool mLowSpeedSupported;
    bool mFullSpeedSupported;
    bool mHighSpeedSupported;
    bool mSuperSpeedSupported;
    bool mCurrentSpeedSupported;
    bool mInitializedCapabilities;

    PvUSBSpeed mSpeed;

    uint32_t mMaxPower;
    uint32_t mMaxPacketSize;

    bool mPleoraDriverInstalled;

};

#endif
