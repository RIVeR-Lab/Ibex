// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDEVICEENUMS_H__
#define __PVDEVICEENUMS_H__

#ifndef EBUS_ENUM
#define EBUS_ENUM(x) typedef enum
#endif


///
/// \file PvDeviceEnums.h
/// \brief PvDevice library enumerations.
///

///
/// \enum PvAccessType
/// \brief GigE Vision device access type
///
/// The #PvAccessType indicates whether (and to what degree) 
/// a second PC can connect to a GigE Vision device after the first one connects. The 
/// GigE Vision device stores the value in the GenICam <b>GevCCP</b> feature.
///
/// The value can be set at connection time using #PvDevice::Connect (recommended)
/// or altered using the GenICam <b>GevCCP</b> feature, programmatically or through
/// a GenICam browser dialog.
///

/// \var PvAccessUnknown
/// Value not known; indeterminate. You can't set this value
/// with #PvDevice::Connect.

/// \var PvAccessOpen
/// Open access; the GigE Vision device isn't connected to any other PC.
/// You can't set this value with #PvDevice::Connect.

/// \var PvAccessControl
/// Shared access; a main controller has read-write access to the device
/// and any other #PvDevice can have read-only access to the device.
/// \sa #PvAccessReadOnly

/// \var PvAccessReadOnly
/// Can be used with #PvDevice::Connect when requesting read-only access
/// to a device currently on #PvAccessOpen or #PvAccessControl.

/// \var PvAccessExclusive
/// Exclusive access. The controller has full read-write access and any other
/// requests but discovery or resent packets are denied.
EBUS_ENUM(PvAccessType)
{
    PvAccessUnknown = 0,
    PvAccessOpen,
    PvAccessControl,
    PvAccessExclusive,
    PvAccessReadOnly
} PvAccessType;

///
/// \enum PvActionAckStatusEnum
/// \brief Action command status enum.
///

/// \var PvActionAckStatusOK
/// The action command was received and executed with success.

/// \var PvActionAckStatusLate
/// The schedule action command was received after it was supposed to be executed.

/// \var PvActionAckStatusOverflow
/// The action command was dropped as too many action commands were already queued on the device.

/// \var PvActionAckStatusNoRefTime
/// The scheduled action command could not be synchronized as a reference time was not available.
EBUS_ENUM(PvActionAckStatusEnum)
{
    PvActionAckStatusOK = 0,
    PvActionAckStatusLate = 1,
    PvActionAckStatusOverflow = 2,
    PvActionAckStatusNoRefTime = 3

} PvActionAckStatusEnum;

#endif
