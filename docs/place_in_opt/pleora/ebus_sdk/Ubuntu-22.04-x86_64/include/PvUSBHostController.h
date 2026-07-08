// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVUSBHOSTCONTROLLER_H__
#define __PVUSBHOSTCONTROLLER_H__

#include <PvInterface.h>
#include <PvDeviceInfoUSB.h>
#include <PvDeviceInfoU3V.h>


///
/// \class PvUSBHostController
///
/// \brief Represents one USB host controller on a system (the PC)
///
/// To use this class, see #PvSystem.
///
class PV_SYSTEM_API PvUSBHostController : public PvInterface
{
public:

    ///
    /// \brief Constructor.
    ///
    PvUSBHostController();

    ///
    /// \brief Destructor.
    ///
    virtual ~PvUSBHostController();

    ///
    /// \brief Equal operator: copies the object.
    ///
    /// \param[in] aFrom The source for the copy.
    ///
    /// \return Reference to the object aFrom was copied to.
    ///
    PvUSBHostController&operator=( const PvUSBHostController &aFrom );

    ///
    /// \brief Vendor ID of the USB host controller.
    ///
    /// \return Vendor ID.
    ///
    uint32_t GetVendorID() const;

    ///
    /// \brief Device ID of the USB host controller.
    ///
    /// \return Device ID.
    ///
    uint32_t GetDeviceID() const;

    ///
    /// \brief Subsystem ID of the USB host controller.
    ///
    /// \return Subsystem ID.
    ///
    uint32_t GetSubsystemID() const;

    ///
    /// \brief Revision of the USB host controller.
    ///
    /// \return Revision of the USB host controller.
    ///
    uint32_t GetRevision() const;

    ///
    /// \brief Speed of the USB host controller.
    ///
    /// Even if a USB host controller supports super speed it does not guarantee a device
    /// will be connected as super speed. Bad cables or hubs not supporting super speed
    /// could force the controller and device to end up being connected at a slower speed.
    /// 
    /// \return Speed of the USB host controller.
    ///
    PvUSBSpeed GetSpeed() const;

protected:

    ///
    /// \brief Constructor.
    ///
    /// \param[in] aFinderReporter A finder reporter.
    ///
    PvUSBHostController( PvSystemLib::IFinderReporter *aFinderReporter );

    ///
    /// \brief Internal/private method for object initialization.
    ///
    void Init();

    void SetVendorID( uint32_t aValue ) { mVendorID = aValue; }
    void SetDeviceID( uint32_t aValue ) { mDeviceID = aValue; }
    void SetSubsystemID( uint32_t aValue ) { mSubsystemID = aValue; }
    void SetRevision( uint32_t aValue ) { mRevision = aValue; }
    void SetSpeed( PvUSBSpeed aValue ) { mSpeed = aValue; }

private:

	 // Not implemented
	PvUSBHostController( const PvUSBHostController & );

    uint32_t mVendorID;
    uint32_t mDeviceID;
    uint32_t mSubsystemID;
    uint32_t mRevision;
    PvUSBSpeed mSpeed;

};

#endif
