// *****************************************************************************
//
//     Copyright (c) 2016, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDEVICESERIALENUMS_H__
#define __PVDEVICESERIALENUMS_H__

#ifndef EBUS_ENUM
#define EBUS_ENUM(x) typedef enum
#endif

/// \file PvDeviceSerialEnums.h
/// \brief Device serial communication constants.


///
/// \enum PvDeviceSerial
/// \brief Device serial port enumeration.
///

/// \var PvDeviceSerial0
/// Serial port 0, or UART0

/// \var PvDeviceSerial1
/// Serial port 1, or UART1

/// \var PvDeviceSerial1
/// Serial port 1, or UART1

/// \var PvDeviceSerialBulk0
/// Bulk interface 0, or BULK0. Can be UART, USRT or I2C depending on device configuration.

/// \var PvDeviceSerialBulk1
/// Bulk interface 1, or BULK1. Can be UART, USRT or I2C depending on device configuration.

/// \var PvDeviceSerialBulk2
/// Bulk interface 2, or BULK2. Can be UART, USRT or I2C depending on device configuration.

/// \var PvDeviceSerialBulk3
/// Bulk interface 3, or BULK3. Can be UART, USRT or I2C depending on device configuration.

/// \var PvDeviceSerialBulk4
/// Bulk interface 4, or BULK4. Can be UART, USRT or I2C depending on device configuration.

/// \var PvDeviceSerialBulk5
/// Bulk interface 5, or BULK5. Can be UART, USRT or I2C depending on device configuration.

/// \var PvDeviceSerialBulk6
/// Bulk interface 6, or BULK6. Can be UART, USRT or I2C depending on device configuration.

/// \var PvDeviceSerialBulk7
/// Bulk interface 7, or BULK7. Can be UART, USRT or I2C depending on device configuration.

EBUS_ENUM(PvDeviceSerial) 
{
    PvDeviceSerialInvalid = -1,
    PvDeviceSerial0 = 0,
    PvDeviceSerial1,
    PvDeviceSerialBulk0,
    PvDeviceSerialBulk1,
    PvDeviceSerialBulk2,
    PvDeviceSerialBulk3,
    PvDeviceSerialBulk4,
    PvDeviceSerialBulk5,
    PvDeviceSerialBulk6,
    PvDeviceSerialBulk7

} PvDeviceSerial;


#endif
