Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.

===================
SoftDeviceGEVTrigger
===================

1. Description

This sample shows how to create and run a Software GigE Vision device. 

2. Prerequisites

This sample assumes that:
 * You have a network adapter installed on your PC with a valid IP address.
 * You have a GigE Vision controller/receiver that can receive and display images (such as eBUSPlayer or any other GigE Vision receiver that supports the GVSP protocol). The receiver should be reachable and on the same subnet as the interface from which it will be receiving.
 * You have an eBUS Edge Runtime License installed on your system

3. Description

3.1 SoftDeviceGEVTrigger.cpp

Main entry point of the sample.

3.1 MyEventSink.cpp

Implementation of the IPvSoftDeviceGEVEventSink interface. Used for event logging, creation of custom registers and creation of custom GenApi features.

3.2 MyRegisterEventSink.cpp

Implementation of the IPvRegisterEventSink interface. Used to demonstrate how to handle register events.

3.3 MyTriggerSource.cpp

Implementation of the PvStreamingChannelSourceTrigger interface.
Used to show how to properly implement and manage an image source in the context of a Software GigE Vision Device. Instead of a real live image source, a test pattern is provided for your convenience.
Demonstrates AcquisitionStart, FrameStart and LineStart TriggerSelectors, as well as Software/Hardware TriggerSources. This can be customized per sources.

3.4 Utilities.cpp

Stand-alone functions used throughout the sample. DumpRegister shows how to browser the IPvRegisterMap of PvSoftDeviceGEV. FireTestEvents shows how to generate events on the messaging channel.


IMPORTANT: If you do not have a Pleora eBUS SDK Seat License for eBUS SDK or an eBUS Edge Runtime License installed on your system,
it will restrict your ability to customize the identity of the device and will disconnect all applications after 15 minutes.





