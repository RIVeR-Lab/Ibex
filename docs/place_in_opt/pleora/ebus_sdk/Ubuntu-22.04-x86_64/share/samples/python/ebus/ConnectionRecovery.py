#!/usr/bin/env python3

'''
*****************************************************************************

    Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.

*****************************************************************************


'''

import time
import sys
import os
import eBUS as eb
import lib.PvSampleUtils as psu

kb = psu.PvKb()

#
# Application class.
#

class connection_recovery_app(eb.PvDeviceEventSink):

    def __init__( self ):
        print(f"Time to init")
        self.__connectionID = ""
        self.__connection_lost = False
        self.__device = None
        self.__stream = None
        self.__pipeline = None
        eb.PvDeviceEventSink.__init__(self)

    #
    # Main, only public function. Runs the application
    #
    def run(self):
        # Select device
        if not self._select_device():
            return False

        # Acquisition loop. Will break when user hits a key
        self._application_loop() 

        # Closes, disconnects, etc.
        self._tear_down(True)
     
        return True

    #
    # Selects a device to work with
    #
    def _select_device( self ):
        print(f"--> SelectDevice")

        # Select the device
        self.__connectionID = psu.PvSelectDevice();
        if not self.__connectionID:
            print(f"No device selected.")
            return False

        #/ IMPORTANT: 
        #/
        #/ Here we assume that the device will come back with the same IP 
        #/ address either through DHCP, static IP configuration or simple network
        #/ unplug/replug LLA. If the device can come back with a different IP address
        #/ the MAC address of the device should be used as connection ID. This will
        #/ not be as efficient but will allow reconnecting the device even if it comes
        #/ back with a different IP address. 
        #/
        #/ This does not apply to USB3 Vision devices who should always stick to the same device GUID.
        #/
        return True

    #
    # Selects, connects a device.
    def _connect_device(self):
        print(f"--> ConnectDevice {self.__connectionID}")

        # Connect to the selected Device
        result, self.__device = eb.PvDevice.CreateAndConnect( self.__connectionID );
        if not result.IsOK():
            return False

        # Register this class as an event sink for PvDevice call-backs
        self.__device.RegisterEventSink(self);

        # Clear connection lost flag as we are now connected to the device
        self.__connection_lost = False
        return True


    #
    # Opens stream, pipeline, allocates buffers
    #
    def _open_stream(self):
        print(f"--> OpenStream")

        # Creates and open the stream object based on the selected device.
        result, self.__stream = eb.PvStream.CreateAndOpen( self.__connectionID );
        if not result.IsOK():
            print(f"Unable to open the stream")
            return False;

        self.__pipeline = eb.PvPipeline( self.__stream );

        # Reading payload size from device
        size = self.__device.GetPayloadSize();

        # Create, init the PvPipeline object
        self.__pipeline.SetBufferSize(size);
        self.__pipeline.SetBufferCount( 16 );

        # The pipeline needs to be "armed", or started before  we instruct the device to send us images
        result = self.__pipeline.Start();
        if not result.IsOK():
            print(f"Unable to start pipeline");
            return False;

        #  Only for GigE Vision, if supported
        if isinstance(self.__device, eb.PvDeviceGEV):
            parameters = self.__stream.GetParameters()
            request_missing_packets_node = parameters.Get("RequestMissingPackets")
            if not request_missing_packets_node == None:
                if request_missing_packets_node.IsAvailable():
                    #  Disabling request missing packets.
                    request_missing_packets_node.SetValue( False ) 

        print(f"Connected to {self.__connectionID}")
        parameters = self.__device.GetParameters()
        
        return True

    #
    # Closes the stream, pipeline
    #
    def _close_stream(self):
 
        print(f"--> CloseStream");

        if not self.__pipeline == None :
            if (  self.__pipeline.IsStarted() ):
                if not self.__pipeline.Stop().IsOK():
                    print(f"Unable to stop the pipeline.");

            del self.__pipeline
            self.__pipeline = None;

        if not self.__stream == None:
            if ( self.__stream.IsOpen()): 
                if not self.__stream.Close().IsOK():
                   print(f"Unable to stop the stream.")

            eb.PvStream.Free( self.__stream ) 
            self.__stream = None

    #
    # Starts image acquisition
    # If the device is disconnected from the network ( Cable pull ), the OnLinkDisconnect callback
    # will execute and reconnection to the device will be attempted.
    #
    def _start_acquisition(self):
        print(f"--> StartAcquisition")
        print(f"")
        print(f"<press a key to exit>")

        # Set streaming destination (only GigE Vision devces)
        if not (self.__device == None):
            if isinstance(self.__device, eb.PvDeviceGEV):
            # Flush packet queue to make sure there is no left over from previous disconnect event
                if not self.__stream == None:
                    self.__stream.FlushPacketQueue()
                # Have to set the Device IP destination to the Stream
                result = self.__device.SetStreamDestination( self.__stream.GetLocalIPAddress(), self.__stream.GetLocalPort() )
                if not result.IsOK():
                    print(f"Setting stream destination failed")
                    return False 

        # Enables stream before sending the AcquisitionStart command.
        self.__device.StreamEnable()

        # The pipeline is already "armed", we just have to tell the device to start sending us images
        parameters = self.__device.GetParameters()
        acq_start_node = parameters.Get("AcquisitionStart") 
        result = acq_start_node.Execute() 
        if not result.IsOK():
            print(f"Unable to start acquisition")
            return False;

        return True;

    #
    # Stops acquisition
    #
    def _stop_acquisition(self):
        print(f"--> StopAcquisition")

        if not self.__device == None:
            # Tell the device to stop sending images.
            parameters = self.__device.GetParameters()
            acq_stop_node = parameters.Get("AcquisitionStop") 
            result = acq_stop_node.Execute();

            # Disable stream after sending the AcquisitionStop command.
            self.__device.StreamDisable()

            if isinstance(self.__device, eb.PvDeviceGEV):
                # Reset streaming destination (optional...)
                self.__device.ResetStreamDestination()

        return True

    #
    # Acquisition loop
    #
    def _application_loop(self):
        print(f"--> ApplicationLoop")

        doodle = "|\\-|-/"
        doodle_index = 0
        first_timeout = True

        image_count_val = 0
        frame_rate_val = 0.0
        bandwidth_val = 0.0

        kb = psu.PvKb()

        #  Acquire images until the user instructs us to stop.
        while not kb.kbhit():
            # If connection flag is up, teardown device/stream
            if ( self.__connection_lost and ( not self.__device == None ) ):
                # Device lost: no need to stop acquisition
                self._tear_down( False )

            # If the device is not connected, attempt reconnection
            if ( self.__device == None ):
                print(f"Attempt Reconnection")
                if ( self._connect_device() ):
                    print(f"Connected to device")

                    # Device is connected, open the stream
                    if self._open_stream():
                        if not self._start_acquisition():
                            self._tear_down(False)
                    else:
                        self._tear_down(False)

            # If still no device, no need to continue the loop
            if ( self.__device == None ):
                continue 

            if ( ( not self.__stream == None ) and  self.__stream.IsOpen() and 
                    ( not self.__pipeline == None ) and self.__pipeline.IsStarted() ):
                # Retrieve next buffer     
                result, pvbuffer, operational_result = self.__pipeline.RetrieveNextBuffer(1000)
            
                if result.IsOK():
                    if operational_result.IsOK():
                        #
                        # We now have a valid buffer. This is where you would typically process the buffer.
                        #---------------------------------------------------------------------------------
                        #
                        result, image_count_val = self.__stream.GetParameters().GetIntegerValue("BlockCount")
                        result, frame_rate_val = self.__stream.GetParameters().GetFloatValue("AcquisitionRate")
                        result, bandwidth_val = self.__stream.GetParameters().GetFloatValue("Bandwidth")
               
                        # If the buffer contains an image, display width and height.
                        width = 0
                        height = 0
                        if ( pvbuffer.GetPayloadType() == eb.PvPayloadType.PvPayloadTypeImage.value ):
                            # Get image specific buffer interface.
                            image = pvbuffer.GetImage()

                            # Read width, height.
                            width = image.GetWidth()
                            height = image.GetHeight()
                   
                        print(f"{doodle[ doodle_index ]}", end=" ") 
                        print(f" BlockID: {pvbuffer.GetBlockID():016d}   W: {width}  H: {height}", end=" ")
                        print(f" {frame_rate_val:.1f} FPS {bandwidth_val / 1000000.0:.1f} Mb/s \r", end=" ")

                        first_timeout = True 

                    # We have an image - do some processing (...) and VERY IMPORTANT,
                    # release the buffer back to the pipeline.
                    self.__pipeline.ReleaseBuffer(pvbuffer) 
                else:
                    # Timeout
                    if first_timeout:
                        print(f"") 
                        first_timeout = False 

                    print(f"Image timeout {doodle[doodle_index]} ")

                doodle_index = (doodle_index + 1) % 6 
            else:
                # No stream/pipeline, must be in recovery. Wait a bit...
                time.sleep(0.1) 
            
        print(f" ")

    #
    #  Disconnects the device
    #
    def _disconnect_device(self):
        print(f"--> DisconnectDevice")

        if not self.__device == None:
            if self.__device.IsConnected():
                # Unregister event sink (call-backs).
                self.__device.UnregisterEventSink( self )

            eb.PvDevice.Free(self.__device)
            self.__device = None

    #
    # Tear down: closes, disconnects, etc.
    #
    def _tear_down(self, stop_acquisition):
        print(f"--> TearDown")

        if ( stop_acquisition ):
            self._stop_acquisition()

        self._close_stream()
        self._disconnect_device()
        return eb.PV_OK

    # 
    # PvDeviceEventSink callback
    # 
    # Notification that the device just got disconnected.
    # 
    def OnLinkDisconnected( self, aDevice ):
        print(f"=====> PvDeviceEventSink::OnLinkDisconnected callback")

        # Just set flag indicating we lost the device. The main loop will tear down the 
        #  device/stream and attempt reconnection. 
        self.__connection_lost = True

        # IMPORTANT:
        # The PvDevice MUST NOT be explicitly disconnected from this callback. 
        # Here we just raise a flag that we lost the device and let the main loop
        # of the application (from the main application thread) perform the
        # disconnect.
        # 

#
# Main function
#

# Receives images using a PvPipeline, full recovery management.
print(f"***********************************************************") 
print(f"ConnectionRecovery sample- image acquisition from a device.")
print(f"*---------------------------------------------------------*")
print(f"* It is recommended to use a persistent, fixed IP address *")
print(f"* or GUID on a device when relying on automatic recovery. *")
print(f"***********************************************************")
print(f"<press a key to terminate the application>")

print(f"Starting")
lapplication = connection_recovery_app()
lRetVal = lapplication.run()
print(f"")

print("<press a key to exit>")
kb.start()
kb.getch()
kb.stop()
