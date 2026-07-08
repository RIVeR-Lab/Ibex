#!/usr/bin/env python3
'''
*****************************************************************************
*
*   Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************

Shows how to use a PvDevice object to handle events from a GigE Vision or
USB3 Vision device.
'''

import sys
import os
import numpy as np
import eBUS as eb
import lib.PvSampleUtils as psu

sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), "EventSample"))
try:
    from EventHandler import EventHandler
except ImportError as e:
    print(f"Unable to import required modules: {e}")
    exit(1)

kb = psu.PvKb()

def connect_to_device(connection_ID):
    # Connect to the GigE Vision or USB3 Vision device
    print("Connecting to device.")
    result, device = eb.PvDevice.CreateAndConnect(connection_ID)
    if device == None:
        print("Unable to connect to device: {result.GetCodeString()} ({result.GetDescription()})")
    return device

print("EventSample:")

connection_ID = psu.PvSelectDevice()
if connection_ID:
    device = connect_to_device(connection_ID)
    if device:

        event_handler = EventHandler()
        # Register EventHandler class as event sink. See EventHandler.py.
        device.RegisterEventSink( event_handler )

        print("<press a key to disconnect>")
        kb.start()
        kb.getch()
        kb.stop()

        device.UnregisterEventSink( event_handler )

        # Disconnect the device
        print("Disconnecting device")
        device.Disconnect()
        eb.PvDevice.Free(device)

print("<press a key to exit>")
kb.start()
kb.getch()
kb.stop()
