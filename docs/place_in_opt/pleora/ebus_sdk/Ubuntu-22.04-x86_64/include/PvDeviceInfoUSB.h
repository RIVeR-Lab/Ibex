// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDEVICEINFOUSB_H__
#define __PVDEVICEINFOUSB_H__

#include <PvDeviceInfo.h>


///
/// \class PvDeviceInfoUSB
///
/// \brief Information about a generic USB device.
///
class PV_SYSTEM_API PvDeviceInfoUSB : public PvDeviceInfo
{
public:

    ///
    /// \brief Constructor.
    ///
    PvDeviceInfoUSB();

    ///
    /// \brief Destructor.
    ///
	virtual ~PvDeviceInfoUSB();

    ///
    /// \brief Equal operator: copies the object.
    ///
    /// \param[in] aFrom The source for the copy.
    ///
    /// \return Reference to the object aFrom was copied to.
    ///
	PvDeviceInfoUSB &operator=( const PvDeviceInfoUSB & );

    ///
    /// \brief Returns the enumeration status of the USB device.
    ///
    /// \return Current enumeration status.
    ///
    PvUSBStatus GetStatus() const;

protected:

#ifndef PV_GENERATING_DOXYGEN_DOC

	PvDeviceInfoUSB( PvDeviceInfoType aType, PvInterface *aInterface );

    void SetStatus( PvUSBStatus aValue ) { mStatus = aValue; }

#endif // PV_GENERATING_DOXYGEN_DOC

private:

	 // Not implemented
    PvDeviceInfoUSB( const PvDeviceInfoUSB & );

    PvUSBStatus mStatus;

};

#endif
