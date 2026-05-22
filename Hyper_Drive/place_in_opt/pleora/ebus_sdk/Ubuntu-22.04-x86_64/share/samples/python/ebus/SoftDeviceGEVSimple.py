#!/usr/bin/env python3
'''
*****************************************************************************

Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.

*****************************************************************************
'''
import time
import numpy as np
import eBUS as eb
import lib.PvSampleUtils as psu
import lib.PvSampleTransmitterConfig as ptc


# This class shows how to implement a streaming channel source
class my_simple_source( eb.PvStreamingChannelSourceDefault ) :
    
    def __init__( self ) :
        super().__init__()
        self.acquisition_buffer = None
        self.fps = 30
        self.seed = 0
        self.frame_count = 0
        self.stabilizer = eb.PvFPSStabilizer()

    # Request to queue a pvbuffer for acquisition.
    # Return OK if the pvbuffer is queued or any error if no more room in acquisition queue
    def QueueBuffer( self, pvbuffer ) :
        # We use acquisition_buffer as a 1-deep acquisition pipeline
        if not self.acquisition_buffer :
            # No pvbuffer queued, accept it
            self.acquisition_buffer = pvbuffer

            # Acquire pvbuffer - could be done in another thread
            self.fill_test_pattern_mono8( pvbuffer )
            self.frame_count = self.frame_count + 1

            return eb.PV_OK

        # We already have a pvbuffer queued for acquisition
        return eb.PV_BUSY

    # Request to give back a pvbuffer ready for transmission.
    # Either block until a pvbuffer is available or return any error
    def RetrieveBuffer(self, not_used) :
        if not self.acquisition_buffer:

            # No pvbuffer queued for acquisition
            return eb.PV_NO_AVAILABLE_DATA, None

        while not self.stabilizer.IsTimeToDisplay( self.fps ) :
            time.sleep( 0.0001 )

        # Remove pvbuffer from 1-deep pipeline
        pvbuffer = self.acquisition_buffer
        self.acquisition_buffer = None

        return eb.PV_OK, pvbuffer

    # Generate a greyscale test pattern in a PvBuffer
    def fill_test_pattern_mono8( self, pvbuffer ) :
        image = pvbuffer.GetImage()
        height = image.GetHeight()
        width = image.GetWidth()

        img_array = image.GetDataPointer()
        
        for y in range(height) :
            base = (self.seed + y) & 0xFF
            for x in range(width) :
                img_array[y,x] = base
                base = (base + 1) & 0xFF

        self.seed = (self.seed + 1) & 0xFF
        

def main():
    config = ptc.PvSampleTransmitterConfig()
    config.ParseCommandLine()
    if config.MACAddress != "" :
        interface = config.MACAddress
    else :
        interface = psu.PvSelectInterface()

    if interface == '' :
        print( "No interface selected, terminating" )
        exit( -1 )

    simple_source = my_simple_source()
    device = eb.PvSoftDeviceGEV()
    device.AddStream( simple_source )

    info = device.GetInfo()
    model_name = info.GetModelName()

    result = device.Start( interface )
    if not result.IsOK() :
        print( f"Error starting {model_name}" )
        exit( -1 )

    print( f"{model_name} started. Press any key to exit." )

    kb = psu.PvKb()
    kb.start()
    while not kb.kbhit() :
        time.sleep( 0.1 )

    device.Stop()
    print( f"{model_name} stopped." )


if __name__ == '__main__':
    main()
