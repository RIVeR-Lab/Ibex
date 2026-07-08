// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVSYSTEMENUMS_H__
#define __PVSYSTEMENUMS_H__

#ifndef EBUS_ENUM
#define EBUS_ENUM(x) typedef enum
#endif

EBUS_ENUM(PvDeviceClass)
{
    PvDeviceClassUnknown = 0,
    PvDeviceClassTransmitter,
    PvDeviceClassReceiver,
    PvDeviceClassTransceiver,
    PvDeviceClassPeripheral

} PvDeviceClass;


EBUS_ENUM(PvUSBStatus)
{
    PvUSBStatusNotInitialized = -1,
    PvUSBStatusConnected = 0,
    PvUSBStatusFailedEnumeration,
    PvUSBStatusGeneralFailure,
    PvUSBStatusCausedOvercurrent,
    PvUSBStatusNotEnoughPower,
    PvUSBStatusNotEnoughBandwidth,
    PvUSBStatusHubNestedTooDeeply,
    PvUSBStatusInLegacyHub,
    PvUSBStatusEnumerating,
    PvUSBStatusReset

} PvUSBStatus;


EBUS_ENUM(PvInterfaceType)
{
    PvInterfaceTypeUnknown = -1,
    PvInterfaceTypeUSBHostController = 0,
    PvInterfaceTypeNetworkAdapter = 1

} PvInterfaceType;


EBUS_ENUM(PvDeviceInfoType)
{
    PvDeviceInfoTypeUnknown = -1,
    PvDeviceInfoTypeGEV = 0,
    PvDeviceInfoTypePleoraProtocol,
    PvDeviceInfoTypeUSB,
    PvDeviceInfoTypeU3V

} PvDeviceInfoType;


EBUS_ENUM(PvUSBSpeed)
{
    PvUSBSpeedUnsupported = -1,
    PvUSBSpeedUnknown = 0,
    PvUSBSpeedLow,
    PvUSBSpeedFull,
    PvUSBSpeedHigh,
    PvUSBSpeedSuper

} PvUSBSpeed;


EBUS_ENUM(PvDeviceType)
{
    PvDeviceTypeUnknown = -1,
    PvDeviceTypeGEV = 0,
    PvDeviceTypeU3V = 1

} PvDeviceType;


#endif

