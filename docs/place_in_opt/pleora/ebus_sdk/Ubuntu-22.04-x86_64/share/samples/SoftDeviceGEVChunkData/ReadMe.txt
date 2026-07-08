Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.

===================
SoftDeviceGEV
===================

1. Description

This sample shows how to create and run a Software GigE Vision device sending data using a payload of type ChunkData. 

2. Prerequisites

This sample assumes that:
 * You have a network adapter installed on your PC with a valid IP address.
 * You have a GigE Vision controller/receiver that can receive and display images (such as eBUSPlayer or any other GigE Vision receiver that supports the GVSP protocol). The receiver should be reachable and on the same subnet as the interface from which it will be receiving.
 * You have an eBUS Edge Runtime License installed on your system

3. Description

3.1 SoftDeviceGEVChunkData.cpp

Main entry point of the sample.

3.2 MyEventSink.cpp

Implementation of the IPvSoftDeviceGEVEventSink interface. Used for event logging, creation of custom registers and creation of custom GenApi features.

3.3 MyRegisterEventSink.cpp

Implementation of the IPvRegisterEventSink interface. Used to demonstrate how to handle register events.

3.4 MySource.cpp

Implementation of the IPvStreamingChannelSource interface. Used to show how to properly implement and manage an image source in the context of using a Payload of type ChunkData.

3.5 Utilities.cpp

Stand-alone functions used throughout the sample. DumpRegister shows how to browser the IPvRegisterMap of PvSoftDeviceGEV. 


IMPORTANT: If you do not have a Pleora eBUS SDK Seat License for eBUS SDK or an eBUS Edge Runtime License installed on your system,
it will restrict your ability to customize the identity of the device and will disconnect all applications after 15 minutes.





