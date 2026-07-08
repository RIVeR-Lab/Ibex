#!/usr/bin/env python3

'''
*****************************************************************************

    Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.

*****************************************************************************


This sample shows you how to control features programmatically.
'''

import sys
import os
import time
import eBUS as eb
import lib.PvSampleUtils as psu

#  Have the user select a device and connect the PvDevice object
#  to the user's selection.
#  Notes: User is responsible to delete PvDevice object when it is
#  no longer needed.
def connect(connection_ID):
    # Connect to the GigE Vision or USB3 Vision device
    print(f"Connecting device")

    result, device = eb.PvDevice.CreateAndConnect(connection_ID) 
    if not result.IsOK():
        print(f"Unable to connect to device") 
        device.Free()
        return None

    return device 
 
#
# Dumps the full content of a PvGenParameterArray.
#
def dump_gen_parameter_array( param_array ):

    # Getting array size
    parameter_array_count = param_array.GetCount();
    print(f"")
    print(f"Array has {parameter_array_count} parameters")

    # Traverse through Array and print out parameters available.
    for x in range(parameter_array_count):
        # Get a parameter
        gen_parameter = param_array.Get(x)

        # Don't show invisible parameters - display everything up to Guru.
        result, lVisible = gen_parameter.IsVisible(eb.PvGenVisibilityGuru)
        if not lVisible:
            continue

        # Get and print parameter's name.
        result, category = gen_parameter.GetCategory();
        result, gen_parameter_name = gen_parameter.GetName()
        print(f"{category}:{gen_parameter_name},", end=' ')

        # Parameter available?
        result, lAvailable = gen_parameter.IsAvailable()
        if not lAvailable:
            not_available = "{Not Available}"
            print(f"{not_available}");
            continue;

        # Parameter readable?
        result, lReadable = gen_parameter.IsReadable()
        if not lReadable:
            not_readable = "{Not Readable}"
            print(f"{not_readable}")
            continue;
        
        #/ Get the parameter type
        result, gen_type = gen_parameter.GetType();
        if eb.PvGenTypeInteger == gen_type:
            result, value = gen_parameter.GetValue()
            print(f"Integer: {value}")
        elif eb.PvGenTypeEnum == gen_type:
            result, value = gen_parameter.GetValueString()
            print(f"Enum: {value}")
        elif eb.PvGenTypeBoolean == gen_type:
            result, value = gen_parameter.GetValue()
            if value:
                print(f"Boolean: TRUE")
            else:
                print(f"Boolean: FALSE")
        elif eb.PvGenTypeString == gen_type:
            result, value = gen_parameter.GetValue()
            print(f"String: {value}")
        elif eb.PvGenTypeCommand == gen_type:
            print(f"Command")
        elif eb.PvGenTypeFloat == gen_type:
            result, value = gen_parameter.GetValue()
            print(f"Float: {value}")


# Get Host's communication-related settings.

def get_host_communication_related_settings( connection_ID ):
    # Communication link can be configured before we connect to the device.
    # No need to connect to the device.
    print(f"Using non-connected PvDevice")
    device = eb.PvDeviceGEV()

    # Get the communication link parameters array
    print(f"Retrieving communication link parameters array")
    comLink = device.GetCommunicationParameters();

    # Dumping communication link parameters array content
    print(f"Dumping communication link parameters array content");
    dump_gen_parameter_array(comLink);

    device.Disconnect();

    return True;

#/
#/ Get the Device's settings
#/

def get_device_settings(connection_ID):
    # Connect to the selected device.
    device = connect(connection_ID) 
    if device == None:
        return 
    
    # Get the device's parameters array. It is built from the 
    # GenICam XML file provided by the device itself.
    print(f"Retrieving device's parameters array")
    parameters = device.GetParameters()

    # Dumping device's parameters array content.
    print(f"Dumping device's parameters array content")
    dump_gen_parameter_array(parameters)

    #Get width parameter - mandatory GigE Vision parameter, it should be there.
    width_parameter = parameters.Get( "Width" );
    if ( width_parameter == None ):
        print(f"Unable to get the width parameter.")

    # Read current width value.
    result, original_width = width_parameter.GetValue()
    if original_width == None:
        print(f"Error retrieving width from device")

    # Read max.
    result, width_max = width_parameter.GetMax()
    if width_max == None:
        print(f"Error retrieving width max from device")   
        return

    # Change width value.
    result = width_parameter.SetValue(width_max)
    if not result.IsOK():
       print(f"Error changing width on device - the device is on Read Only Mode, please change to Exclusive to change value")

    # Reset width to original value.
    result = width_parameter.SetValue(original_width)
    if not result.IsOK():
       print(f"1 Error changing width on device");   

    # Disconnect the device.
    eb.PvDevice.Free(device)
    return


#
# Get Image stream controller settings.
#
def get_image_stream_controller_settings(connection_ID):

    # Creates stream object
    print(f"Opening stream")

    result, stream = eb.PvStream.CreateAndOpen(connection_ID) 
    if not result.IsOK():
        print(f"Error creating and opening stream")
        eb.PvStream.Free(stream );

    # Get the stream parameters. These are used to configure/control
    # some stream related parameters and timings and provides
    # access to statistics from this stream.
    print(f"Retrieving stream's parameters array")
    parameters = stream.GetParameters();

    # Dumping device's parameters array content.
    print(f"Dumping stream's parameters array content")
    dump_gen_parameter_array(parameters)

    # Close and free PvStream
    eb.PvStream.Free(stream)


#
# Main function.
#
print(f"Device selection")
connection_ID = psu.PvSelectDevice()
if connection_ID:
    print(f"GenICamParameters sample")
    print(f"")
    print(f"1. Communication link parameters display")
    get_host_communication_related_settings(connection_ID) 
    print(f"")

    # Device parameters display.
    print(f"2. Device parameters display")
    print(f"")
    get_device_settings(connection_ID);

    #cout << endl;

    # Image stream parameters display.
    print(f"3. Image stream parameters display") 
    print(f"")
    get_image_stream_controller_settings(connection_ID) 

print(f"<press a key to exit>")

kb = psu.PvKb()
kb.start()
kb.getch()
kb.stop()


