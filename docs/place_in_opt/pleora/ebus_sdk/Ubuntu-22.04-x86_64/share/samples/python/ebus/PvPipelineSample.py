#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2020, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************

Shows how to use a PvPipeline object to acquire images from a GigE Vision or
USB3 Vision device.
'''

import numpy as np
import eBUS as eb
import lib.PvSampleUtils as psu

BUFFER_COUNT=16

kb = psu.PvKb()

opencv_is_available=True
register_events=False # Change to True to see event info
try:
    # Detect if OpenCV is available
    import cv2
    opencv_version=cv2.__version__
except:
    opencv_is_available=False
    print("Warning: This sample requires python3-opencv to display a window")

class my_pipeline_event_sink(eb.PvPipelineEventSink):
    def OnBufferCreated(self, pipeline, pvbuffer):
        print(f"Buffer created.")
    def OnStart(self, pipeline):
        print("About to start.")
    def OnStop(self, pipeline):
        print("Just stopped")

def connect_to_device(connection_ID) :
    # Connect to the GigE Vision or USB3 Vision device
    print("Connecting to device.")
    result, device = eb.PvDevice.CreateAndConnect(connection_ID)
    if device == None :
        print(f"Unable to connect to device: {result.GetCodeString()} ({result.GetDescription()})")
    return device

def open_stream(connection_ID) :
    # Open stream to the GigE Vision or USB3 Vision device
    print("Opening stream from device.")
    result, stream = eb.PvStream.CreateAndOpen(connection_ID)
    if stream == None :
        print(f"Unable to stream from device. {result.GetCodeString()} ({result.GetDescription()})")
    return stream

def configure_stream(device, stream) :
    # If this is a GigE Vision device, configure GigE Vision specific streaming parameters
    if isinstance(device, eb.PvDeviceGEV) :
        # Negotiate packet size
        device.NegotiatePacketSize()
        # Configure device streaming destination
        device.SetStreamDestination(stream.GetLocalIPAddress(), stream.GetLocalPort())

def create_pipeline(device, stream):
    # Create the PvPipeline object
    pipeline = eb.PvPipeline(stream)

    if pipeline:
        # Reading payload size from device
        lSize = device.GetPayloadSize()
    
        # Set the Buffer count and the Buffer size
        pipeline.SetBufferCount(BUFFER_COUNT)
        pipeline.SetBufferSize(lSize)
    
    return pipeline

def acquire_images(device, stream, pipeline):
    # Get device parameters need to control streaming
    device_params = device.GetParameters()

    # Map the GenICam AcquisitionStart and AcquisitionStop commands
    start = device_params.Get("AcquisitionStart")
    stop = device_params.Get("AcquisitionStop")

    # Note: the pipeline must be initialized before we start acquisition
    print("Starting pipeline")
    pipeline.Start()

    # Get stream parameters
    stream_params = stream.GetParameters()

    # Map a few GenICam stream stats counters
    frame_rate = stream_params.Get("AcquisitionRate")
    bandwidth = stream_params[ "Bandwidth" ]

    # Enable streaming and send the AcquisitionStart command
    print("Enabling streaming and sending AcquisitionStart command.")
    device.StreamEnable()
    start.Execute()

    doodle = "|\\-|-/"
    doodle_index = 0
    display_image = False
    warning_issued = False
    frame_rate_val = frame_rate.GetValue()
    bandwidth_val = bandwidth.GetValue()
    errors = 0
    decompression_filter = eb.PvDecompressionFilter()

    # Acquire images until the user instructs us to stop.
    print("\n<press a key to stop streaming>")
    kb.start()
    while not kb.is_stopping():
        # Retrieve next pvbuffer
        result, pvbuffer, operational_result = pipeline.RetrieveNextBuffer(1000)
        if result.IsOK():
            if operational_result.IsOK():
                #
                # We now have a valid pvbuffer. This is where you would typically process the pvbuffer.
                # -----------------------------------------------------------------------------------------
                # ...

                result, frame_rate_val = frame_rate.GetValue()
                result, bandwidth_val = bandwidth.GetValue()

                print(f"{doodle[doodle_index]} BlockID: {pvbuffer.GetBlockID():016d}", end='')

                image = None

                lPayloadType = pvbuffer.GetPayloadType()
                if lPayloadType == eb.PvPayloadTypeImage:
                    image = pvbuffer.GetImage()
                    
                elif lPayloadType == eb.PvPayloadTypeChunkData:
                    print(f" Chunk Data payload type with {pvbuffer.GetChunkCount()} chunks", end='')

                elif lPayloadType == eb.PvPayloadTypeRawData:
                    print(f" Raw Data with {pvbuffer.GetRawData().GetPayloadLength()} bytes", end='')

                elif lPayloadType == eb.PvPayloadTypeMultiPart:
                    print(f" Multi Part with {pvbuffer.GetMultiPartContainer().GetPartCount()} parts", end='')

                elif lPayloadType == eb.PvPayloadTypePleoraCompressed:
                    if eb.PvDecompressionFilter.IsCompressed(pvbuffer):
                        result, pixel_type, width, height = eb.PvDecompressionFilter.GetOutputFormatFor(pvbuffer)
                        if result.IsOK():
                            calculated_size = eb.PvImage.GetPixelSize(pixel_type) * width * height / 8;
                            out_buffer = eb.PvBuffer()
                            result, decompressed_buffer = decompression_filter.Execute(pvbuffer, out_buffer)
                            image = decompressed_buffer.GetImage()
                            if result.IsOK():
                                decompressed_size = decompressed_buffer.GetSize()
                                compression_ratio = decompressed_size / pvbuffer.GetAcquiredSize()
                                if calculated_size != decompressed_size:
                                    errors = errors + 1
                                print(f" Pleora compressed type.   Compression ratio: {'{0:.2f}'.format(compression_ratio)}  Errors: {errors}", end='')
                            else:
                                print(f" Could not decompress (Pleora compressed)", end='')
                                errors = errors + 1
                        else:
                            print(f" Could not read header (Pleora compressed)", end='')
                            errors = errors + 1
                    else:
                        print(f" Contents do not match payload type (Pleora compressed)", end='')
                        errors = errors + 1

                else:
                    print(" Payload type not supported by this sample", end='')

                if image:
                    print(f" W: {image.GetWidth()} H: {image.GetHeight()}", end='')
                    if opencv_is_available:
                        image_data = image.GetDataPointer()
                        if image.GetPixelType() == eb.PvPixelMono8:
                            display_image = True
                        if image.GetPixelType() == eb.PvPixelRGB8:
                            image_data = cv2.cvtColor(image_data, cv2.COLOR_RGB2BGR)
                            display_image = True

                        if display_image:
                            cv2.imshow("stream",image_data)
                        else:
                            if not warning_issued:
                                # display a message that video only display for Mono8 / RGB8 images
                                print(f" ")
                                print(f" Currently only Mono8 / RGB8 images are displayed", end='\r')
                                print(f"")
                                warning_issued = True

                        if cv2.waitKey(1) & 0xFF != 0xFF:
                            break

                print(f" {frame_rate_val:.1f} FPS  {bandwidth_val / 1000000.0:.1f} Mb/s     ", end='\r')
            else:
                # Non OK operational result
                print(f"{doodle[ doodle_index ]} {operational_result.GetCodeString()}       ", end='\r')
            # Release the pvbuffer back to the pipeline
            pipeline.ReleaseBuffer(pvbuffer)
        else:
            # Retrieve pvbuffer failure
            print(f"{doodle[ doodle_index ]} {result.GetCodeString()}      ", end='\r')

        doodle_index = (doodle_index + 1) % 6
        if kb.kbhit():
            kb.getch()
            break;

    kb.stop()
    if opencv_is_available:
        cv2.destroyAllWindows()

    # Tell the device to stop sending images.
    print("\nSending AcquisitionStop command to the device")
    stop.Execute()

    # Disable streaming on the device
    print("Disable streaming on the controller.")
    device.StreamDisable()

    # Stop the pipeline
    print("Stop pipeline")
    pipeline.Stop()


print("PvPipelineSample:")

connection_ID = psu.PvSelectDevice()
if connection_ID:
    device = connect_to_device(connection_ID)
    if device:
        stream = open_stream(connection_ID)
        if stream:
            configure_stream(device, stream)
            pipeline = create_pipeline(device, stream)
            if pipeline:
                if register_events:
                    event_sink = my_pipeline_event_sink()
                    pipeline.RegisterEventSink(event_sink)
                acquire_images(device, stream, pipeline)
            
            # Close the stream
            if register_events:
                pipeline.UnregisterEventSink(event_sink)
            print("Closing stream")
            stream.Close()
            eb.PvStream.Free(stream)

        # Disconnect the device
        print("Disconnecting device")
        device.Disconnect()
        eb.PvDevice.Free(device)

print("<press a key to exit>")
kb.start()
kb.getch()
kb.stop()
