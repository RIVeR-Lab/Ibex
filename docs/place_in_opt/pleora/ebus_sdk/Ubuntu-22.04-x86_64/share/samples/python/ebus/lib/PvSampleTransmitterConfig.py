'''
*****************************************************************************

Copyright (c) 2020, Pleora Technologies Inc., All rights reserved.

*****************************************************************************
'''
import argparse
import eBUS as eb
import lib.PvSampleUtils as psu

# Application config
class PvSampleTransmitterConfig :

    def __init__( self ) :
        # Initialize the argument parser
        self.__init_parser()

        # Only used to enumerate network interfaces, no need to call Find
        lSystem = eb.PvSystem()

        # Find default source address
        lFound = 0
        for i in range( lSystem.GetInterfaceCount() ) :
            lNetworkAdapter = lSystem.GetInterface( i )
            if not isinstance(lNetworkAdapter, eb.PvNetworkAdapter ) :
                continue

            lIPCount = lNetworkAdapter.GetIPAddressCount()
            
            for j in range( lIPCount ) :
                lIP = lNetworkAdapter.GetIPAddress( j )
                if lIP.GetAscii() != "0.0.0.0" :
                    lFound = lFound + 1
                    if lFound == 1 :
                        self.SourceAddress = lIP
                    break
        if lFound == 0:
            print( "No valid network interfaces found." )
        elif lFound > 1 :
            # Multiple valid IP addresses found. Not guessing.
            self.SourceAddress = ""
        
    def ParseCommandLine( self ) :
        self.mArgParser.parse_args(namespace=self)

    def __init_parser( self ):
        self.mArgParser = argparse.ArgumentParser(
            description="Optional command line arguments:",
            formatter_class=argparse.ArgumentDefaultsHelpFormatter)

        self.mArgParser.add_argument(
            "-m", "--MACaddress",
            help="Hardware address of interface on which to launch transmitter in the form '12:34:56:78:9a:bc'.",
            dest="MACAddress",
            required=False,
            default="")

        self.mArgParser.add_argument(
            "--packetsize",
            help='Maximimum size of streaming packets. For best results, set "Jumbo Frames" property on your NIC and increase this value accordingly.',
            dest="PacketSize",
            required=False,
            type=int,
            default=1440)

        self.mArgParser.add_argument(
            "--destinationaddress",
            help="Destination address in the form 123.456.789.101.",
            dest="DestinationAddress",
            required=False,
            default="239.192.1.1")

        self.mArgParser.add_argument(
            "--destinationport",
            help="Destination port.",
            dest="DestinationPort",
            type=int,
            required=False,
            default=1042)

        self.mArgParser.add_argument(
            "--sourceaddress",
            help="Source address in the form 123.456.789.101.",
            dest="SourceAddress",
            required=False,
            default="first valid address encountered while enumerating interfaces if there is only one")

        self.mArgParser.add_argument(
            "--sourceport",
            help="Source port. By default a port is automatically assigned when the socket is opened.",
            dest="",
            type=int,
            required=False,
            default=0)

        self.mArgParser.add_argument(
            "--buffercount",
            help="Number of transmit buffers to use. Increase this number when sending smaller images at high frame rates.",
            dest="BufferCount",
            type=int,
            required=False,
            default=4)

        self.mArgParser.add_argument(
            "--silent",
            help="Don't wait for a key press. By default, the system waits for a key press before it begins transmitting.",
            dest="Silent",
            required=False,
            default=False)

        self.mArgParser.add_argument(
            "-f", "--fps",
            help="Frames per second.",
            dest="FPS",
            type=int,
            default=30)
