#!/usr/bin/env python3

import eBUS as eb
import logging
import sys
import lib.PvSampleUtils as psu

kb = psu.PvKb()

# Example of using an event sink on the device.
class my_event_sink(eb.PvDeviceEventSink):
    def __init__(self, logger):
        super().__init__()
        self.logger = logger

    def OnLinkDisconnected(self, device):
        uniqueID = device.GetUniqueID()
        self.logger.info("Got a disconnect from %s", uniqueID)

    def OnCmdLinkRead(self, args):
        (buffer, address) = args
        self.logger.debug("OnCmdLinkRead buffer: %s  address: %d", buffer, address)

print("DeviceFinder sample")

pvsystem = eb.PvSystem()

result = pvsystem.Find()

if not result.IsOK():
    print(f"Unable to find devices: {result.GetCodeString()}")
    exit(1)

interface_count = pvsystem.GetInterfaceCount()

device_info = None

for x in range(interface_count):
    print(f"")
    print(f"Interface {x}")
    interface = pvsystem.GetInterface(x)

    if isinstance(interface, eb.PvNetworkAdapter):
        print(f"  MAC Address: {interface.GetMACAddress()}")
        for z in range(interface.GetIPAddressCount()):
            print(f"  IP Address {z}: {interface.GetIPAddress(z)}")
            print(f"  Subnet Mask {z}: {interface.GetSubnetMask(z)}")

    elif isinstance(interface, eb.PvUSBHostController):
        print(f"  Name: {interface.GetName()}")

    for y in range(interface.GetDeviceCount()):
        device_info = interface.GetDeviceInfo(y)
        print(f"")
        print(f"  Device {y}")
        print(f"    Display ID: {device_info.GetDisplayID()}")
        print(f"    Serial Number: {device_info.GetSerialNumber()}")

        if isinstance(device_info, eb.PvDeviceInfoGEV) \
                or isinstance(device_info, eb.PvDeviceInfoPleoraProtocol):
            print(f"    MAC Address: {device_info.GetMACAddress()}")
            print(f"    IP Address: {device_info.GetIPAddress()}")
        elif isinstance(device_info, eb.PvDeviceInfoU3V):
            print(f"    GUID: {device_info.GetDeviceGUID()}")
            print(f"    Speed: {device_info.GetSpeed()}")
        elif isinstance(device_info, eb.PvDeviceInfoUSB):
            print(f"    Unknown USB device?")

logger = logging.getLogger()

#Change logging level to see different logs ( INFO or DEBUG )
logging.basicConfig(stream=sys.stdout, level=logging.ERROR)

logger.info("Creating event sink")
event_sink = my_event_sink(logger)

if device_info:
    print(f"Connecting to {device_info.GetDisplayID()}")
    result, device = eb.PvDevice.CreateAndConnect(device_info)
    if result.IsOK():
        print(f"Successfully connected to {device_info.GetDisplayID()}")
        device.RegisterEventSink(event_sink)
        print(f"Disconnecting the device {device_info.GetDisplayID()}")
        eb.PvDevice.Free(device)
    else:
        print(
            f"Unable to connect to {device_info.GetDisplayID()}: {result.GetCodeString()}")
else:
    print("No device found.")

print("<press a key to exit>")
kb.start()
kb.getch()
kb.stop()
