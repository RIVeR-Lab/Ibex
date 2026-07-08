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
        self.chunk_mode_active = False
        self.chunk_sample_enabled = False
        self.stabilizer = eb.PvFPSStabilizer()
        self.supported_pixel_types = [
            eb.PvPixelMono8,
            eb.PvPixelRGB8,
            eb.PvPixelRGBa8,
            eb.PvPixelBGR8,
            eb.PvPixelBGRa8,
            eb.PvPixelYCbCr422_8_CbYCrY,
            eb.PvPixelYCbCr8_CbYCr
        ]
        self.test_pattern_buffer = eb.PvBuffer()
        self.channel_number = MySource.channel_count;
        MySource.channel_count = MySource.channel_count + 1;

        self.source0only_int =  50 
        self.source0only_bool = False

    def GetWidth(self):
        return self.width

    def GetWidthInfo(self):
        return WIDTH_MIN, WIDTH_MAX, WIDTH_INC

    def SetWidth(self, width):
        if (width < WIDTH_MIN ) or ( width > WIDTH_MAX):
            return eb.PV_INVALID_PARAMETER

        self.width = width
        return eb.PV_OK

    def GetHeight(self):
        return self.height

    def GetHeightInfo(self):
        return HEIGHT_MIN, HEIGHT_MAX, HEIGHT_INC

    def SetHeight(self, height):
        if (height < HEIGHT_MIN ) or (height > HEIGHT_MAX):
            return eb.PV_INVALID_PARAMETER

        self.height = height
        return eb.PV_OK

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

    def GetGevSCPSPacketSizeInfo(self):
        return eb.PV_OK, 9000

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
        self.prime_test_pattern()

    def OnStreamingStop(self):
        print("Streaming stop")

    def AllocBuffer(self):
        if self.buffer_count < BUFFERCOUNT:
            self.buffer_count += 1
            return eb.PvBuffer()

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
            self.fill_test_pattern()
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
        image = pvbuffer.GetImage()
        if image.GetWidth() != self.width \
                or (image.GetHeight() != self.height) \
                or (image.GetPixelType() != self.pixel_type) \
                or (image.GetMaximumChunkLength() != required_chunk_size ):
            image.Alloc(self.width, self.height, self.pixel_type, 0, 0, required_chunk_size)

    def fill_test_pattern(self):
        src = self.test_pattern_buffer.GetImage().GetDataPointer()
        dst = self.acquisition_buffer.GetImage().GetDataPointer()
        np.copyto(dst,src)
        # Since the data pointer is a numpy array we can simply ask numpy
        # to increment every value to 'advance' the test pattern. Technically
        # this isn't a 'perfect' increment for the YUV patterns.
        src += 1

    def prime_test_pattern(self):
        self.test_pattern_buffer.GetImage().Alloc(self.width, self.height, self.pixel_type, 0, 0, 0)
        if self.pixel_type == eb.PvPixelMono8:
            self.fill_test_pattern_mono8(self.test_pattern_buffer)
        elif self.pixel_type == eb.PvPixelBGR8 \
                or self.pixel_type == eb.PvPixelBGRa8 \
                or self.pixel_type == eb.PvPixelRGB8 \
                or self.pixel_type == eb.PvPixelRGBa8:
            self.fill_test_pattern_rgb(self.test_pattern_buffer)
        elif self.pixel_type == eb.PvPixelYCbCr8_CbYCr:
            self.fill_test_pattern_yuv444(self.test_pattern_buffer)
        elif self.pixel_type == eb.PvPixelYCbCr422_8_CbYCrY:
            self.fill_test_pattern_yuv422(self.test_pattern_buffer)

    def fill_test_pattern_mono8(self, pvbuffer):
        image = pvbuffer.GetImage()
        height = image.GetHeight()
        width = image.GetWidth()
        img_array = image.GetDataPointer()
        
        for y in range(height) :
            base = (self.seed + y) & 0xFF
            for x in range(width) :
                img_array[y,x] = base
                base = (base + 1) & 0xFF
        self.seed += 1

    def fill_test_pattern_rgb(self, pvbuffer):
        image = pvbuffer.GetImage()
        height = image.GetHeight()
        width = image.GetWidth()
        img_array = image.GetDataPointer()

        for y in range(height):
            value = self.seed + y
            for x in range(width):
                # Write current pixel
                img_array[y,x,0] = (value << 4) & 0xFF
                img_array[y,x,1] = (value << 2) & 0xFF
                img_array[y,x,2] = value & 0xFF
                value += 1
        self.seed += 1

    def fill_test_pattern_yuv444(self, pvbuffer):
        image = pvbuffer.GetImage()
        height = image.GetHeight()
        width = image.GetWidth()
        img_array = image.GetDataPointer()

        for y in range(height):
            value = self.seed + y
            for x in range(width):
                img_array[y,x,0] = (value << 1) & 0xFF
                img_array[y,x,1] = value & 0xFF
                img_array[y,x,2] = 255 - (value << 2) & 0xFF
                value += 1
        self.seed += 1

    def fill_test_pattern_yuv422(self, pvbuffer):
        image = pvbuffer.GetImage()
        height = image.GetHeight()
        width = image.GetWidth()
        img_array = image.GetDataPointer()

        for y in range(height):
            value = self.seed + y
            for x in range(width):
                img_array[y,x,0] = (value << 1) & 0xFF if (( x & 1 ) == 0) else 255 - (value << 2) & 0xFF
                img_array[y,x,1] = value & 0xFF
                value += 1
        self.seed += 1

    def add_chunk_sample(self, pvbuffer):
        if not self.chunk_mode_active or not self.chunk_sample_enabled:
            return

        # pack the data into 36 bytes
        chunk_data = struct.pack("<I32s", self.frame_count, bytes(time.asctime(), 'utf-8'))

        # Add chunk data to pvbuffer
        pvbuffer.ResetChunks()
        pvbuffer.SetChunkLayoutID(CHUNKLAYOUTID)
        pvbuffer.AddChunk(CHUNKID, chunk_data)



#
# Create the Source 0 only registers
#
    def CreateRegisters( self, register_map, factory ):
        if ( 0 == self.channel_number ):
            # Create new registers for Source0 only
            factory.AddRegister( "Source0Bool", SOURCE0_BOOL_ADDR, 4, eb.PvGenAccessModeReadWrite, self ) 
            factory.AddRegister( "Source0Int", SOURCE0_INT_ADDR, 4, eb.PvGenAccessModeReadWrite, self );
 

#
# Create the Extended Source 0 only GenICam features
#
#
    def CreateGenApiFeatures ( self, register_map, factory ):
        if ( 0 == self.channel_number ):
            # Create new features for Source0 only
            factory.SetName( "Source0OnlyBool" )
            factory.SetDescription( "Example of source only boolean." ) 
            factory.SetCategory( "Source0Only" );
            factory.SetNameSpace( eb.PvGenNameSpaceStandard )
            factory.SetTLLocked( True )
            factory.SetStreamable( True ) 
            factory.AddEnumEntry( "Off", 0 )
            factory.AddEnumEntry( "On", 1 ) 
            factory.CreateEnum( register_map.GetRegisterByAddress( SOURCE0_BOOL_ADDR ) ) 
 
            factory.SetName( "Source0OnlyInt" ) 
            factory.SetDescription( "Example of source only integer" ) 
            factory.SetCategory( "Source0Only" );
            factory.SetNameSpace( eb.PvGenNameSpaceStandard ) 
            factory.SetTLLocked( True ) 
            factory.SetStreamable( True ) 
            factory.CreateInteger( register_map.GetRegisterByAddress( SOURCE0_INT_ADDR ), 0, 256 ) 


    #
    # Fetch the variable during a Read operation
    #
    def PreRead( self, register ):
        value = 0
        print(f"{register.GetName()}  MySource PreRead" )

        address = register.GetAddress()
        if SOURCE0_BOOL_ADDR == address :
            value = self.source0only_bool
            return register.Write( value )    
        if SOURCE0_INT_ADDR == address :
            value = self.source0only_int
            return register.Write( value )    

        return eb.PV_OK

    #
    # Post-read nofitication
    #
    def PostRead( self, register ):
        print(f"{register.GetName()}  MySource PostRead" )


    #
    #  Pre-write notification 
    #  This is where a new register value is usually validated
    #
    def PreWrite( self, register ):
        print(f"{register.GetName()}  MySource PreWrite" )
        return eb.PV_OK 


    #
    # Update the variable during a Write operation
    #
    def PostWrite( self, register ):
        result, value = register.ReadInt()
        if eb.PV_OK == result:
            print(f"{register.GetName()}  MySource PostWrite" )
            address = register.GetAddress()
            if SOURCE0_BOOL_ADDR == address :
                self.source0only_bool = value
                return
            if SOURCE0_INT_ADDR == address :
                self.source0only_int = value
                return
