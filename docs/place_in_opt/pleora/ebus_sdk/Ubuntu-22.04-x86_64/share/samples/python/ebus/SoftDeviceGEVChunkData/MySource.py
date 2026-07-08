#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2020, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************
'''

import time
import struct
import eBUS as eb
import numpy as np
import Utilities as utils
from Defines import *

class MySource(eb.IPvRegisterEventSink, eb.IPvStreamingChannelSource):
    channel_count = 0;

    def __init__(self):
        # Since this class uses multiple inheritence, the __init__ method
        # of each base class must be called explicitly.
        eb.IPvRegisterEventSink.__init__( self )
        eb.IPvStreamingChannelSource.__init__( self )

        self.width = WIDTH_DEFAULT
        self.height = HEIGHT_DEFAULT
        self.pixel_type = eb.PvPixelMono8
        self.buffer_count = 0
        self.acquisition_buffer = None
        self.seed = 0
        self.frame_count = 0
        self.chunk_mode_active = True 
        self.chunk_sample_enabled = True
        self.stabilizer = eb.PvFPSStabilizer()
        self.supported_pixel_types = [
            eb.PvPixelMono8 
        ]
        self.channel_number = MySource.channel_count
        MySource.channel_count = MySource.channel_count + 1

    def GetWidth(self):
        return self.width

    def SetWidth(self, aWidth):
        if ( aWidth < WIDTH_MIN ) or ( aWidth > WIDTH_MAX ):
            return eb.PV_INVALID_PARAMETER
        self.width = aWidth
        return eb.PV_OK

    def GetWidthInfo(self):
        return WIDTH_MIN, WIDTH_MAX, WIDTH_INC

    def GetHeight(self):
        return self.height

    def SetHeight(self, aHeight):
        if ( aHeight < HEIGHT_MIN ) or ( aHeight > HEIGHT_MAX ):
            return eb.PV_INVALID_PARAMETER
        self.height = aHeight
        return eb.PV_OK

    def GetHeightInfo(self):
        return HEIGHT_MIN, HEIGHT_MAX, HEIGHT_INC

    def GetOffsetX(self):
        return 0

    def GetOffsetY(self):
        return 0

    def SetOffsetX(self, offset_x):
        return eb.PV_NOT_SUPPORTED

    def SetOffsetY(self, offset_y):
        return eb.PV_NOT_SUPPORTED

    def GetPixelType(self):
        return self.pixel_type

    def SetPixelType(self, pixel_type):
        self.pixel_type = pixel_type
        return eb.PV_OK

    def GetSupportedPixelType(self, index):
        if index < len(self.supported_pixel_types):
            return eb.PV_OK, self.supported_pixel_types[index]
        return eb.PV_INVALID_PARAMETER, 0

    def GetChunksSize(self):
        return self.GetRequiredChunkSize()

    def GetPayloadSize(self):
        return 0

    def GetScanType(self):
        return eb.PvScanTypeArea

    def GetChunkModeActive(self):
        return self.chunk_mode_active

    def SetChunkModeActive(self, enabled):
        self.chunk_mode_active = enabled
        return eb.PV_OK

    def GetSupportedChunk(self, index):
        if index == 0:
            return eb.PV_OK, CHUNKID, "Sample"
        return eb.PV_INVALID_PARAMETER, 0, ""

    def GetChunkEnable(self, chunk_id):
        if chunk_id == CHUNKID:
            return self.chunk_sample_enabled
        return False

    def SetChunkEnable(self, chunk_id, enabled):
        if chunk_id == CHUNKID:
            self.chunk_sample_enabled = enabled
            return eb.PV_OK
        return eb.PV_INVALID_PARAMETER

    def OnOpen(self, dest_ip, dest_port):
        print(f"Streaming channel opened to {dest_ip}:{dest_port}")

    def OnClose(self):
        print("Streaming channel closed")

    def OnStreamingStart(self):
        print("Streaming start")
        self.stabilizer.Reset()

    def OnStreamingStop(self):
        print("Streaming stop")

    def AllocBuffer(self):
        if self.buffer_count < BUFFERCOUNT:
            self.buffer_count += 1
            return eb.PvBuffer(eb.PvPayloadTypeChunkData)

        return None

    def FreeBuffer(self, pvbuffer):
        self.buffer_count -= 1

    def QueueBuffer(self, pvbuffer):
        # We use mAcqusitionBuffer as a 1-deep acquisition pipeline
        if not self.acquisition_buffer:
            # No pvbuffer queued, accept it
            self.acquisition_buffer = pvbuffer
            # Acquire pvbuffer - could be done in another thread
            self.resize_buffer_if_needed(self.acquisition_buffer)
            self.add_chunk_sample(self.acquisition_buffer)
            self.frame_count += 1
            return eb.PV_OK

        # We already have a pvbuffer queued for acquisition
        return eb.PV_BUSY

    def RetrieveBuffer(self, not_used):
        if not self.acquisition_buffer:
            # No pvbuffer queued for acquisition
            return eb.PV_NO_AVAILABLE_DATA, None

        while not self.stabilizer.IsTimeToDisplay(DEFAULT_FPS):
            time.sleep(0.0001)

        # Remove pvbuffer from 1-deep pipeline
        pvbuffer = self.acquisition_buffer
        self.acquisition_buffer = None
        return eb.PV_OK, pvbuffer

    def AbortQueuedBuffers(self):
        pass

    def GetRequiredChunkSize(self):
        return CHUNKSIZE if (self.chunk_mode_active and self.chunk_sample_enabled) else 0

    def resize_buffer_if_needed(self, pvbuffer):
        required_chunk_size = self.GetRequiredChunkSize()
        chunk_data  = pvbuffer.GetChunkData()
        if (chunk_data.GetChunkDataSize() != required_chunk_size): 
           chunk_data.Alloc(required_chunk_size)

    def add_chunk_sample(self, pvbuffer):
        pvbuffer.ResetChunks()
        if not self.chunk_mode_active or not self.chunk_sample_enabled:
            return

        # pack the data into 36 bytes
        chunk_data = struct.pack("<I32s", self.frame_count, bytes(time.asctime(), 'utf-8'))

        # Add chunk data to pvbuffer
        pvbuffer.SetChunkLayoutID(CHUNKLAYOUTID)
        pvbuffer.AddChunk(CHUNKID, chunk_data)


