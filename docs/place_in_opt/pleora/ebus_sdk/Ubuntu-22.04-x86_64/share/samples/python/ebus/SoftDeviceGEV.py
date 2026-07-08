#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2020, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************

Demonstration of a software based GEV device.
'''

import sys
import os
import time
import eBUS as eb
import lib.PvSampleUtils as psu
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), "SoftDeviceGEV"))
try:
    from MySource import MySource
    from MyEventSink import MyEventSink
    from MyRegisterEventSink import MyRegisterEventSink
    from MyUserSetNotify import MyUserSetNotify
    from FileAccessEventSink import FileAccessEventSink
    from FileAccessRegisterEventSink import FileAccessRegisterEventSink
    from Defines import FILESELECTORADDR
    from DiskFile import DiskFile
    from ArrayFile import ArrayFile
    import Utilities as utils
except ImportError as e:
    print(f"Unable to import required modules: {e}")
    exit(1)

kb = psu.PvKb()

SOURCE_COUNT = 4
USERSET_COUNT = 2

# To retrieve the log file using use GenICam file transfer:
# First, enable logging, and change LOG_FILE to your log file name.
# Then initialize TRANSFER_LOG_FILE to True.
TRANSFER_LOG_FILE = False
LOG_FILE = "C:\\Users\\username\\AppData\\Roaming\\example.log"

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
user_set_notify = MyUserSetNotify()

# Instantiate the device itself
device = eb.PvSoftDeviceGEV()

'''
The two files managed by this object.
FileSelector:
0: Large Binary File: a virtual file stored in a large array: WO
1: Disk file: a file that is located on disk storage: RW.
The binary file allocates 5MB of heap storage upon opening it.
Failure to do so will result in an operation failure.
The buffer is freed on file close.
Files larger than 5MB will result in write failure.

The disk file has no space limit besides the disk capacity,
and the max value of a uint32_t to hold the size.
It is open in binary mode.
For the disk file, transfer time and average speed are provided.
The transfer time and average transfer speed is calculated from open
to close operations.

All these files can be manipulated with eBUS Player's File Transfer:
(menu Tools -> File Transfer...)
'''
file_access_register_event_sink = FileAccessRegisterEventSink(FILESELECTORADDR, device.GetRegisterMap())
my_file = ArrayFile("TheNewWOArrayFile", 5 * 1024 * 1024)
file_access_register_event_sink.AddFile(my_file)
my_file = DiskFile("TheNewRWDiskFile", eb.PvGenAccessModeReadWrite, True)
file_access_register_event_sink.AddFile(my_file)
if TRANSFER_LOG_FILE:
  my_file = DiskFile(LOG_FILE, eb.PvGenAccessModeReadOnly, False)
  file_access_register_event_sink.AddFile(my_file)
file_access_event_sink = FileAccessEventSink(file_access_register_event_sink)

# Set device identify
info = device.GetInfo()
info.SetModelName("SoftDeviceGEV")
info.SetGenICamXMLVersion(1, 2, 3)
info.SetGenICamXMLGUIDs("BA07A10F-969E-4900-9B11-EE914F7A5D7F", "F87E8639-DE12-404E-A079-BBFF5FC9D82AA")

# Add stream, register event sink
for i in range(SOURCE_COUNT):
    device.AddStream(sources[i])
device.RegisterEventSink(event_sink)
device.RegisterEventSink(file_access_event_sink)

# Configure user-set count
device.SetUserSetCount(USERSET_COUNT)
device.SetUserSetNotify( user_set_notify )
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
    utils.fire_test_events(device.GetMessagingChannel())
    time.sleep(.1)

# Stop device
device.Stop()
print(f"{model_name} stopped")
