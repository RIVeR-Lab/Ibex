#!/usr/bin/env python3

'''
*****************************************************************************

    Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.

*****************************************************************************


 This sample shows how to use the PvDeviceSerialPort class to communicate with a Pleora GigE Vision or USB3 Vision device.
'''

import sys
import random
import eBUS as eb
import numpy as np
import lib.PvSampleUtils as psu


PORT     = "BULK0"
SPEED    = "Baud9600"
STOPBITS = "One"
PARITY   = "None"

TEST_COUNT = 16

#
# Shows how to use a PvDeviceSerialPort to write data to an video interface 
# and continuously read it back
# (with the video interface serial port set in loop back mode).
#
def TestSerialCommunications():
    connection_ID = psu.PvSelectDevice()
    if not connection_ID:
        print(f"No device selected." ) 
        return False 

    # Connect to the GEV or U3V Device
    print(f"Connecting to device {connection_ID}")
    result, device = eb.PvDevice.CreateAndConnect(connection_ID) 
    if not result.IsOK():
        print(f"Unable to connect to device")
        return False;

    # Create PvDevice adapter
    device_adapter = eb.PvDeviceAdapter( device );

    # Get device parameters need to control streaming
    params = device.GetParameters();

    # Configure serial port - this is done directly on the device GenICam interface, not 
    # on the serial port object! 
    if ( PORT == "UART0"):
        params.SetEnumValue( "Uart0BaudRate", SPEED );
        params.SetEnumValue( "Uart0NumOfStopBits", STOPBITS );
        params.SetEnumValue( "Uart0Parity", PARITY );

        # For this test to work without attached serial hardware we enable the port loop back
        params.SetBooleanValue( "Uart0Loopback", True );

    if (PORT == "BULK0"):
        params.SetEnumValue( "BulkSelector", "Bulk0" );
        params.SetEnumValue( "BulkMode", "UART" );
        params.SetEnumValue( "BulkBaudRate", SPEED );
        params.SetEnumValue( "BulkNumOfStopBits", STOPBITS );
        params.SetEnumValue( "BulkParity", PARITY );

        # For this test to work without attached serial hardware we enable the port loop back
        params.SetBooleanValue( "BulkLoopback", True );

    #  Open serial port
    if (PORT == "UART0"):
        port = eb.PvDeviceSerialPort() 
        result = eb.PvDeviceSerialPort.Open(port, device_adapter, eb.PvDeviceSerialUart0 );

    if (PORT == "BULK0"):
        port = eb.PvDeviceSerialPort() 
        result = port.Open( device_adapter, eb.PvDeviceSerialBulk0 );

    if not result.IsOK():
        print(f"Unable to open serial port on device: {result.GetCodeString()} {result.GetDescription()}" )
        return False;

    print(f"Serial port opened");

    size = 1;
#   Make sure the PvDeviceSerialPort receive queue is big enough to buffer all bytes 
#   Note that for every iteration in the test below, lSize is doubled so lSize << TEST_COUNT is the size (2x to give extra)
    port.SetRxBufferSize( ( size << TEST_COUNT ) * 2);

    
    for count in range(0, TEST_COUNT):
        in_buffer = np.zeros(size, dtype= np.uint8)
        out_buffer = np.zeros(size, dtype= np.uint8)

        # Fill input buffer with random data
        for i in range(0, size):
            j = random.randrange(256)
            in_buffer[i] = j

        # Send the buffer content on the serial port
        bytes_written = 0;
        result, bytes_written = port.Write( in_buffer )
        print(f"Sent {bytes_written} bytes through the serial port")
        if not result.IsOK():
            # Unable to send data over serial port!
            print(f"Error sending data over the serial port: {result.GetCodeString()}  {result.GetDescription()} ");
            break 
 

        # Wait until we have all the bytes or we timeout. The Read method only times out
        # if no data is available when the function call occurs, otherwise it returns
        # all the currently available data. It is possible we have to call Read multiple
        # times to retrieve all the data if all the expected data hasn't been received yet.
        
        # Your own code driving a serial protocol should check for a message being complete,
        # whether it is on some sort of EOF or length. You should pump out data until you
        # have what you are waiting for or reach some timeout.
        
        total_bytes_read = 0;

        while (total_bytes_read < size):
            bytes_read = 0
            
            result, out_buffer[ total_bytes_read:size ], bytes_read = port.Read( size - total_bytes_read, 500 );
            left_to_read = size - total_bytes_read
            # End of real Read code
            if ( result.GetCode() == eb.PV_TIMEOUT ):
                print(f"Timeout")
                break;

            # Increments read head
            total_bytes_read += bytes_read 

        # Validate answer
        if not total_bytes_read == bytes_written:
            # Did not receive all expected bytes
            print(f"Only received {total_bytes_read} out of {bytes_written} bytes")
        else:
            # Compare input and output buffers
            error_count = 0;
            for i in range(0, bytes_written ):
                if not in_buffer[ i ] == out_buffer[ i ]:
                    error_count = error_count + 1;

            # Display error count
            print(f"Error count: {error_count}")

        del in_buffer; 
        
        del out_buffer; 

        # Grow test case
        size *= 2;

        print(f"")


    # Close serial port
    port.Close();
    print(f"Serial port closed")

    # Delete device adapter (before freeing PvDevice!)
    del device_adapter;

    # Release the device. Use PvDevice::Free as the device was allocated using PvDevice::CreateAndConnect.
    print(f"Disconnecting and freeing device")
    eb.PvDevice.Free(device );

    return True;
 

#
# Entry code
#

#   PvDeviceSerialPort used to perform serial communication through a Pleora 
#   GigE Vision or USB3 Vision device

print(f"DeviceSerialPort Sample");
TestSerialCommunications();
print(f"")

print(f"<press a key to exit>")
kb = psu.PvKb()
kb.start()
kb.getch()
kb.stop()


