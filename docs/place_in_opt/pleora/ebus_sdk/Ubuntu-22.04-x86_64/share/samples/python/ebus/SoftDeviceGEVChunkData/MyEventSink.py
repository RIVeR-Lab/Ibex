#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************
'''

import eBUS as eb
import Utilities as utils
from Defines import *

class MyEventSink(eb.IPvSoftDeviceGEVEventSink):
    def __init__(self, register_event_sink):
        super().__init__()
        self.register_event_sink = register_event_sink

    def OnApplicationConnect(self, device, IP_address, port, access_type):
        print(f"Application connected from {IP_address}:{port}")

    def OnApplicationDisconnect(self, device):
        print("Application disconnected")

    def OnControlChannelStart(self, device, MAC_address, IP_address, mask, gateway, port):
        print(f"Control channel started on [{MAC_address}] {IP_address}:{port} Mask:{mask} Gateway:{gateway}")
        utils.dump_registers(device.GetRegisterMap())

    def OnControlChannelStop(self, device):
        print("Control channel stopped")

    def OnDeviceResetFull(self, device):
        print("Device reset")

    def OnDeviceResetNetwork(self, device):
        print("Network reset")

    def OnCreateCustomGenApiFeatures(self, device, factory):
        lMap = device.GetRegisterMap()

        self.create_chunk_parameters(factory)

    def create_chunk_parameters(self, factory):
        # Create GenApi feature used to map the chunk data count field
        factory.SetName(CHUNKCOUNTNAME)
        factory.SetDescription(CHUNKCOUNTDESCRIPTION)
        factory.SetToolTip(CHUNKCOUNTTOOLTIP)
        factory.SetCategory(CHUNKCATEGORY)
        factory.MapChunk(CHUNKID, 0, 4, eb.PvGenEndiannessLittle)
        factory.CreateInteger(None, 0, 2**32-1)

        # Create GenApi feature used to map the chunk data time field
        factory.SetName(CHUNKTIMENAME)
        factory.SetDescription(CHUNKTIMEDESCRIPTION)
        factory.SetToolTip(CHUNKTIMETOOLTIP)
        factory.SetCategory(CHUNKCATEGORY)
        factory.MapChunk(CHUNKID, 4, 32, eb.PvGenEndiannessLittle)
        factory.CreateString(None)


