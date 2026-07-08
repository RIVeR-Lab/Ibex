// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDEVICEU3V_H__
#define __PVDEVICEU3V_H__

#include <PvDevice.h>

///
/// \class PvDeviceU3V
/// \brief Connect, configure and control a USB3 Vision device
///
class PV_DEVICE_API PvDeviceU3V : public PvDevice
{
public:
	///
	/// \brief Constructor.
	///
	PvDeviceU3V();

	///
	/// \brief Destructor
	///
	virtual ~PvDeviceU3V();

	///
	/// \brief Connect to a USB3 Vision device.
	///
	/// The #Connect method connects to a USB3 Vision device found using a #PvSystem or device finder dialog object.
	/// Only one device can be connected to at a time.
	/// 
	/// \param[in] aDeviceInfo The USB3 Vision device's connectivity information; a #PvDeviceInfo object.
	///
	/// \return 
    ///    Includes:
	/// 	- #PvResult::Code::OK
	/// 	- #PvResult::Code::INVALID_PARAMETER
	/// 	- #PvResult::Code::NO_LICENSE
	/// 	- #PvResult::Code::GENERIC_ERROR
	/// 	- #PvResult::Code::GENICAM_XML_ERROR
	///
	PvResult Connect( const PvString &aInfo );

	///
	/// \brief Connect to a USB3 Vision device.
	///
	/// The #Connect method connects to a USB3 Vision device found using a #PvSystem or device finder dialog object.
	/// Only one device can be connected to at a time.
	/// 
	/// \param[in] aInfo The USB3 Vision device's 12 hex digit GUID.
	///
	/// \return 
    ///    Includes:
	/// 	- #PvResult::Code::OK
	/// 	- #PvResult::Code::INVALID_PARAMETER
	/// 	- #PvResult::Code::NO_LICENSE
	/// 	- #PvResult::Code::GENERIC_ERROR
	/// 	- #PvResult::Code::GENICAM_XML_ERROR
	///
	PvResult Connect( const PvDeviceInfo *aDeviceInfo );

	///
	/// \brief Returns the GUID of the connected device.
	///
	/// If no device is connected, an empty string is returned.
	///
	/// \return GUID of the device.
	///
    PvString GetGUID() const;

protected:
    
private:

	 // Not implemented
	PvDeviceU3V( const PvDeviceU3V & );
	const PvDeviceU3V &operator=( const PvDeviceU3V & );

};

#endif
