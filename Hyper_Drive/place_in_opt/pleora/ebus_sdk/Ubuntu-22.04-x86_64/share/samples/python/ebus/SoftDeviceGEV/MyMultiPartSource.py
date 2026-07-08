#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************
'''

import time
import struct
import eBUS as eb
import numpy as np
import Utilities as utils
from Defines import *

class MyMultiPartSource(eb.IPvStreamingChannelSource):

    _DATA_TYPE_1 = eb.PvMultiPart3DImage
    _PIXEL_TYPE_1 = eb.PvPixelCoord3D_A8
    _DATA_TYPE_2 = eb.PvMultiPartConfidenceMap
    _PIXEL_TYPE_2 = eb.PvPixelConfidence8

    # The number of parts when _multipart_large_leader_trailer_enabled is set to true
    # The eBUS SDK receiver can only receive up tp 32 parts, set the value to 31 to allow one additional chunk data in this sample.
    _MULTI_PART_COUNTS_LARGE = 31
    _MULTI_PART_COUNTS_DEFAULT = 2

    def __init__(self):
        super().__init__()
        self._width = WIDTH_DEFAULT
        self._height = HEIGHT_DEFAULT
        self._pixel_type = eb.PvPixelMono8
        self._acquisition_buffer = None
        self._test_pattern_buffer = eb.PvBuffer()
        self._seed = 0
        self._frame_count = 0
        self._chunk_mode_active = False
        self._chunk_sample_enabled = False
        self._stabilizer = eb.PvFPSStabilizer()
        self._multipart_allowed = False
        self._multipart_counts = self._MULTI_PART_COUNTS_DEFAULT
        # Without Large leader trailer enabled, max supported multi-parts is 10.
        # When Large Leader Trailer is enabled, the maximum supported multi-parts number for transmission by SoftDeviceGEV is decided by the stream channel packet size,
        # and provided in the SetLargeLeaderTrailerEnabled callback.
        # If the sample is used to stream to eBUS SDK receiver, the maximum supported multiparts number to receive on eBUS SDK is 32.
        self._multipart_large_leader_trailer_enabled = False
        # The LargeLeaderTrailer should be enabled from GenICam node to inform the Transmitter/Receiver to proper adjust the leader/trailer size. 
        # If set to true in this sample without setting through GenICam Parameter the maximum supported multi-part will still be 10.
        # And you might not be able to alloc parts for the buffer.
        
        self._max_supported_multipart_counts = 10
        self._supported_pixel_types = [
            eb.PvPixelMono8
        ]

    # For GigE Vision Validation Framework and TestPayloadFormatMode GenICam XML definition
    def IsPayloadTypeSupported(self, payload_type ):
        return payload_type == eb.PvPayloadTypeMultiPart
    
    def SetMultiPartAllowed(self, allowed):
        self._multipart_allowed = allowed

    def SetLargeLeaderTrailerEnabled(self, enabled, max_multipart_count ):
        self._multipart_large_leader_trailer_enabled = enabled
        self._max_supported_multipart_counts = max_multipart_count
        if self._multipart_large_leader_trailer_enabled:
            self._multipart_counts = self._MULTI_PART_COUNTS_LARGE
            if self._multipart_counts <= 10:
                self._multipart_large_leader_trailer_enabled = False
                self._multipart_counts = 10
                print( "Multi-part large leader trailer enabled, but multi-part number is not greater than 10, please set the proper size, Large Leader Trailer will not be use.")
                return
            print( f"Multi-part large leader trailer enabled, the maximum supported part number is {self._max_supported_multipart_counts} and the multi-part number to use is {self._multipart_counts}" )
        else:
            self._multipart_counts = self._MULTI_PART_COUNTS_DEFAULT
            self._max_supported_multipart_counts = 10

        if self._multipart_counts > self._max_supported_multipart_counts:
            print(f"The number of multi-parts to use {self._multipart_counts} cannot exceed the maximum allowed value {self._max_supported_multipart_counts}, stream with the maximum allowed multi parts number instead")
            self._multipart_counts = self._max_supported_multipart_counts

    def SetTestPayloadFormatMode(self, payload_type ):
        if payload_type == eb.PvPayloadTypeMultiPart:
            print("Setting TestPayloadFormatMode to PvPayloadTypeMultiPart")
            return eb.PV_OK
        elif payload_type == eb.PvPayloadTypeNone:
            print("Disabling TestPayloadFormatMode")
            return eb.PV_OK
        else:
            return eb.PV_NOT_SUPPORTED

    def GetPixelType(self):
        return self._pixel_type

    def SetPixelType(self, pixel_type):
        self._pixel_type = pixel_type
        return eb.PV_OK

    def GetSupportedPixelType(self, index):
        if index < len(self._supported_pixel_types):
            return eb.PV_OK, self._supported_pixel_types[index]
        return eb.PV_INVALID_PARAMETER, 0

    def GetWidth(self):
        return self._width

    def GetWidthInfo(self):
        return WIDTH_MIN, WIDTH_MAX, WIDTH_INC

    def SetWidth(self, width):
        if (width < WIDTH_MIN ) or ( width > WIDTH_MAX):
            return eb.PV_INVALID_PARAMETER

        self._width = width
        return eb.PV_OK

    def GetHeight(self):
        return self._height

    def GetHeightInfo(self):
        return HEIGHT_MIN, HEIGHT_MAX, HEIGHT_INC

    def SetHeight(self, height):
        if (height < HEIGHT_MIN ) or (height > HEIGHT_MAX):
            return eb.PV_INVALID_PARAMETER

        self._height = height
        return eb.PV_OK

    def GetOffsetX(self):
        return 0

    def GetOffsetY(self):
        return 0

    def SetOffsetX(self, offset_x):
        return eb.PV_NOT_SUPPORTED

    def SetOffsetY(self, offset_y):
        return eb.PV_NOT_SUPPORTED

    def GetChunksSize(self):
        return self.GetRequiredChunkSize()

    def GetPayloadSize(self):
        payload_size = self._width * self._height
        if self._multipart_allowed:
            payload_size += ( self._MULTI_PART_COUNTS_DEFAULT - 1 ) * self._width * self._height
            if self._multipart_large_leader_trailer_enabled:
                payload_size += ( self._MULTI_PART_COUNTS_LARGE - self._MULTI_PART_COUNTS_DEFAULT ) * ( self._width * self._height )
        if self._chunk_mode_active and self._chunk_sample_enabled:
            payload_size += CHUNKSIZE
        return payload_size

    def GetScanType(self):
        return eb.PvScanTypeArea

    def GetChunkModeActive(self):
        return self._chunk_mode_active

    def SetChunkModeActive(self, enabled):
        self._chunk_mode_active = enabled
        return eb.PV_OK

    def GetSupportedChunk(self, index):
        if index == 0:
            return eb.PV_OK, CHUNKID, CHUNKCOUNTNAME
        return eb.PV_INVALID_PARAMETER, 0, ""

    def GetChunkEnable(self, chunk_id):
        if chunk_id == CHUNKID:
            return self._chunk_sample_enabled
        return False

    def SetChunkEnable(self, chunk_id, enabled):
        if chunk_id == CHUNKID:
            self._chunk_sample_enabled = enabled
            return eb.PV_OK
        return eb.PV_INVALID_PARAMETER

    def OnOpen(self, dest_ip, dest_port):
        print(f"Streaming channel opened to {dest_ip}:{dest_port}")

    def OnClose(self):
        print("Streaming channel closed")

    def OnStreamingStart(self):
        self._stabilizer.Reset()
        self.prime_test_pattern()
        print("Streaming start")

    def OnStreamingStop(self):
        print("Streaming stop")

    def AllocBuffer(self):
        buffer = eb.PvBuffer(eb.PvPayloadTypeMultiPart)
        self.AllocMultiPart(buffer)
        return buffer

    def FreeBuffer(self, pvbuffer):
        return

    def QueueBuffer(self, pvbuffer):
        # We use mAcqusitionBuffer as a 1-deep acquisition pipeline
        if not self._acquisition_buffer:
            self._acquisition_buffer = pvbuffer
            if self._multipart_allowed:
                result = self.fill_buffer_multi_part(self._acquisition_buffer)
            else:
                result = self.fill_buffer(self._acquisition_buffer)
            self._frame_count += 1
            if result != eb.PV_OK:
                print(f"Error Filling data in PvBuffer in QueueBuffer")
                self._acquisition_buffer = None
            return result

        # We already have a pvbuffer queued for acquisition
        return eb.PV_BUSY

    def RetrieveBuffer(self, not_used):
        if not self._acquisition_buffer:
            # No pvbuffer queued for acquisition
            return eb.PV_NO_AVAILABLE_DATA, None

        while not self._stabilizer.IsTimeToDisplay(DEFAULT_FPS):
            time.sleep(0.0001)

        # Remove pvbuffer from 1-deep pipeline
        pvbuffer = self._acquisition_buffer
        self._acquisition_buffer = None
        return eb.PV_OK, pvbuffer

    def AbortQueuedBuffers(self):
        pass

    def GetRequiredChunkSize(self):
        return CHUNKSIZE if (self._chunk_mode_active and self._chunk_sample_enabled) else 0
  
    def fill_buffer_multi_part(self, pvbuffer):
        result = self.AllocMultiPart(pvbuffer)
        if result != eb.PV_OK:
            return result
        src = self._test_pattern_buffer.GetDataPointer()
        dst_container = self._acquisition_buffer.GetMultiPartContainer()
        for part in range( self._multipart_counts ):
            dst_container.AttachPart(part, src)
        # Advance the test pattern
        src += 1
        if self._chunk_mode_active and self._chunk_sample_enabled and self._multipart_counts < 32:
            result = self.add_chunk_sample(dst_container.GetPart(self._multipart_counts).GetChunkData())
            if result != eb.PV_OK:
                print(f"add_chunk_sample for multipart has error {result.GetCodeString()}")
                return result
        return eb.PV_OK

    def prep_buffer(self, pvbuffer):
        if pvbuffer.GetPayloadType() != eb.PvPayloadTypeImage:
            pvbuffer.Reset( eb.PvPayloadTypeImage )
        chunksize = self.GetRequiredChunkSize()
        pvimage = pvbuffer.GetImage()
        if (pvimage.GetWidth() != self._width) \
            or (pvimage.GetHeight() != self._height) \
            or (pvimage.GetPixelType() != self.GetPixelType()) \
            or (pvimage.GetMaximumChunkLength() != chunksize):
                return pvimage.Alloc(self._width, self._height, self.GetPixelType(), 0, 0, chunksize)
        return eb.PV_OK
    
    def fill_buffer(self, pvbuffer):
        result = self.prep_buffer(pvbuffer)
        if result != eb.PV_OK:
            return result
        src = self._test_pattern_buffer.GetImage().GetDataPointer()
        dst = self._acquisition_buffer.GetImage().GetDataPointer()
        np.copyto(dst,src)
        # Since the data pointer is a numpy array we can simply ask numpy
        # to increment every value to 'advance' the test pattern. Technically
        # this isn't a 'perfect' increment for the YUV patterns.
        src += 1
        if self._chunk_mode_active and self._chunk_sample_enabled:
            self.add_chunk_sample(pvbuffer)
        return eb.PV_OK

    def AllocMultiPart(self, buffer):
        buffer.Reset(eb.PvPayloadTypeMultiPart)
        container = buffer.GetMultiPartContainer()
        container.Reset()
        if self._multipart_large_leader_trailer_enabled:
            for part in range( 10 ):
                result = container.AddImagePart(self._DATA_TYPE_1, self._width, self._height, self._PIXEL_TYPE_1)
                if not result.IsOK():
                    print(f"AddImagePart {part} has error {result.GetCodeString()}")
                    return result
            for part in range( 10, self._multipart_counts ):
                result = container.AddImagePart(self._DATA_TYPE_2, self._width, self._height, self._PIXEL_TYPE_2)
                if not result.IsOK():
                    print(f"AddImagePart {part} has error {result.GetCodeString()}")
                    return result
        else:
            result = container.AddImagePart(self._DATA_TYPE_1, self._width, self._height, self._PIXEL_TYPE_1)
            if not result.IsOK():
                print(f"AddImagePart 1 has error {result.GetCodeString()}")
                return result
            for part in range( 1, self._multipart_counts ):
                result = container.AddImagePart(self._DATA_TYPE_2, self._width, self._height, self._PIXEL_TYPE_2)
                if not result.IsOK():
                    print(f"AddImagePart {part} has error {result.GetCodeString()}")
                    return result
        if self._chunk_mode_active and self._chunk_sample_enabled:
            result = container.AddChunkPart( CHUNKSIZE, CHUNKLAYOUTID )
            if not result.IsOK():
                print(f"AddChunkPart has error {result.GetCodeString()}")
                return result
        result = container.AllocAllParts()
        if not result.IsOK():
            print(f"AllocAllParts has error {result.GetCodeString()}")
            return result
        return container.Validate()

    def fill_test_pattern_mono8(self, section):
        height = section.GetImage().GetHeight()
        width = section.GetImage().GetWidth()
        img_array = section.GetImage().GetDataPointer()       
        for y in range(height):
            base = (self._seed + y) & 0xFF
            for x in range(width) :
                img_array[y,x] = base
                base = (base + 1) & 0xFF
        self._seed += 1

    def add_chunk_sample(self, pvbuffer):
        if not self._chunk_mode_active or not self._chunk_sample_enabled:
            return eb.PV_NOT_SUPPORTED
        # pack the data into 36 bytes
        chunk_data = struct.pack("<I32s", self._frame_count, bytes(time.asctime(), 'utf-8'))
        # Add chunk data to pvbuffer
        pvbuffer.ResetChunks()
        pvbuffer.SetChunkLayoutID(CHUNKLAYOUTID)
        pvbuffer.AddChunk(CHUNKID, chunk_data)
        return eb.PV_OK

    def prime_test_pattern(self):
        self.prep_buffer(self._test_pattern_buffer)
        self.fill_test_pattern_mono8(self._test_pattern_buffer)