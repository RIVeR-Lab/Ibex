#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************

Demonstration of a software based GEV device.
'''

import sys
import os
import time
import eBUS as eb
import lib.PvSampleUtils as psu
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), "SoftDeviceGEVChunkData"))
try:
    from MySource import MySource
    from MyEventSink import MyEventSink
    from MyRegisterEventSink import MyRegisterEventSink
    import Utilities as utils
except ImportError as e:
    print(f"Unable to import required modules: {e}")
    exit(1)

kb = psu.PvKb()

SOURCE_COUNT = 4
USERSET_COUNT = 2


# Select interface if MAC was not provided
if len(sys.argv) > 1:
    interface = sys.argv[1]
else:
    interface = psu.PvSelectInterface()
if not interface:
    print(f"No interface selected, terminating")
    exit(-1)

# Instantiate interface implementations
sources = []
for i in range(SOURCE_COUNT):
    sources.append(MySource())

register_event_sink = MyRegisterEventSink()
event_sink = MyEventSink(register_event_sink)

# Instantiate the device itself
device = eb.PvSoftDeviceGEV()

# Set device identify
info = device.GetInfo()
info.SetModelName("SoftDeviceGEVChunkData")
info.SetGenICamXMLVersion(1, 2, 3)
info.SetGenICamXMLGUIDs("BA07A10F-969E-4900-9B11-EE914F7A5D7F", "F87E8639-DE12-404E-A079-BBFF5FC9D82AA")

# Add stream, register event sink
for i in range(SOURCE_COUNT):
    device.AddStream(sources[i])
device.RegisterEventSink(event_sink)

# Configure user-set count
device.SetUserSetCount(USERSET_COUNT)
device.SetRTPProtocolEnabled(True)

# Start device
model_name = info.GetModelName()
result = device.Start(interface)
if not result.IsOK():
    print(f"Error starting {model_name}")
    if result.GetCode() == eb.PV_GENICAM_XML_ERROR:
        print(f"The error is possibly in the dynamically generated GenICam XML file: {result.GetDescription()}")
        
        lXML = device.GetGenICamXMLFile()
        print(lXML)

    exit(-1)

print(f"{model_name} started")

# Loop until keyboard hit
kb.start()
while not kb.kbhit():
    time.sleep(.1)

# Stop device
device.Stop()
print(f"{model_name} stopped")
