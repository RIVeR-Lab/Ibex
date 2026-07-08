#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2020, Pleora Technologies Inc., All rights reserved.
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

    def OnCreateCustomRegisters(self, device, factory):
        factory.AddRegister(SAMPLEINTEGERNAME, SAMPLEINTEGERADDR, 4, eb.PvGenAccessModeReadWrite, self.register_event_sink)
        factory.AddRegister(SAMPLEFLOATNAME, SAMPLEFLOATADDR, 4, eb.PvGenAccessModeReadWrite, self.register_event_sink)
        factory.AddByteArray(SAMPLESTRINGNAME, SAMPLESTRINGADDR, 16, eb.PvGenAccessModeReadWrite, self.register_event_sink)
        factory.AddRegister(SAMPLEBOOLEANNAME, SAMPLEBOOLEANADDR, 4, eb.PvGenAccessModeReadWrite, self.register_event_sink)
        factory.AddRegister(SAMPLECOMMANDNAME, SAMPLECOMMANDADDR, 4, eb.PvGenAccessModeReadWrite, self.register_event_sink)
        factory.AddRegister(SAMPLEENUMNAME, SAMPLEENUMADDR, 4, eb.PvGenAccessModeReadWrite, self.register_event_sink)

    def OnCreateCustomGenApiFeatures(self, device, factory):
        lMap = device.GetRegisterMap()

        self.create_sample_parameters(lMap, factory)
        self.create_chunk_parameters(factory)
        self.create_event_parameters(factory)

    def create_sample_parameters(self, map, factory):
        # Creates sample enumeration feature mapped to SAMPLEENUMADDR register.
        # Enumeration entries of EnumEntry1 (0), EnumEntry2 (1) and EnumEntry3 (2) are created.
        # This feature is also defined as a selector of the sample string and sample Boolean.
        factory.SetName(SAMPLEENUMNAME)
        factory.SetDescription(SAMPLEENUMDESCRIPTION)
        factory.SetToolTip(SAMPLEENUMTOOLTIP)
        factory.SetCategory(SAMPLECATEGORY)
        factory.AddEnumEntry("EnumEntry1", 0)
        factory.AddEnumEntry("EnumEntry2", 1)
        factory.AddEnumEntry("EnumEntry3", 2)
        factory.AddSelected(SAMPLESTRINGNAME)
        factory.AddSelected(SAMPLEBOOLEANNAME)
        factory.CreateEnum(map.GetRegisterByAddress( SAMPLEENUMADDR ))

        # Creates sample string feature mapped to SAMPLESTRINGADDR register.
        # This feature is selected by sample enumeration.
        # This feature has an invalidator to the sample command: its cached value will be invalidated (reset) whenever the command is executed.
        factory.SetName(SAMPLESTRINGNAME)
        factory.SetDescription(SAMPLESTRINGDESCRIPTION)
        factory.SetToolTip(SAMPLESTRINGTOOLTIP)
        factory.SetCategory(SAMPLECATEGORY)
        factory.AddInvalidator(SAMPLEENUMNAME)
        factory.AddInvalidator(SAMPLECOMMANDNAME)
        factory.CreateString(map.GetRegisterByAddress( SAMPLESTRINGADDR ))

        # Creates sample Boolean feature mapped to SAMPLEBOOLEANADDR register.
        # This feature is selected by sample enumeration.
        factory.SetName(SAMPLEBOOLEANNAME)
        factory.SetDescription(SAMPLEBOOLEANDESCRIPTION)
        factory.SetToolTip(SAMPLEBOOLEANTOOLTIP)
        factory.SetCategory(SAMPLECATEGORY)
        factory.AddInvalidator(SAMPLEENUMNAME)
        factory.CreateBoolean(map.GetRegisterByAddress( SAMPLEBOOLEANADDR ))

        # Creates sample command feature mapped to SAMPLECOMMANDADDR register.
        # The sample string has this command defined as its invalidator.
        # Executing this command resets the controller-side GenApi cached value of the sample string.
        factory.SetName(SAMPLECOMMANDNAME)
        factory.SetDescription(SAMPLECOMMANDDESCRIPTION)
        factory.SetToolTip(SAMPLECOMMANDTOOLTIP)
        factory.SetCategory(SAMPLECATEGORY)
        factory.SetStreamable(False)
        factory.CreateCommand(map.GetRegisterByAddress( SAMPLECOMMANDADDR ))

        # Creates sample integer feature mapped to SAMPLEINTEGERADDR register.
        # Minimum is defined as -10000, maximum 100000, increment as 1 and unit as milliseconds.
        factory.SetName(SAMPLEINTEGERNAME)
        factory.SetDescription(SAMPLEINTEGERDESCRIPTION)
        factory.SetToolTip(SAMPLEINTEGERTOOLTIP)
        factory.SetCategory(SAMPLECATEGORY)
        factory.SetRepresentation(eb.PvGenRepresentationLinear)
        factory.SetUnit(SAMPLEINTEGERUNITS)
        factory.CreateInteger(map.GetRegisterByAddress( SAMPLEINTEGERADDR ), -10000, 10000, 1)

        # Creates sample float feature mapped to SAMPLEFLOATADDR register.
        # Minimum is defined as -100.0 and maximum as 100.0 and units as inches.
        factory.SetName(SAMPLEFLOATNAME)
        factory.SetDescription(SAMPLEFLOATDESCRIPTION)
        factory.SetToolTip(SAMPLEFLOATTOOLTIP)
        factory.SetCategory(SAMPLECATEGORY)
        factory.SetRepresentation(eb.PvGenRepresentationPureNumber)
        factory.SetUnit(SAMPLEFLOATUNITS)
        factory.CreateFloat(map.GetRegisterByAddress( SAMPLEFLOATADDR ), -100.0, 100.0)

        # Creates sample pValue feature which simply links to another feature.
        # This feature is linked to sample integer.
        # We use "pValue" as display name as the automatically generated one is not as good.
        factory.SetName(SAMPLEPVALUE)
        factory.SetDisplayName(SAMPLEPVALUEDISPLAYNAME)
        factory.SetDescription(SAMPLEPVALUEDESCRIPTION)
        factory.SetToolTip(SAMPLEPVALUETOOLTIP)
        factory.SetCategory(SAMPLECATEGORY)
        factory.SetPValue(SAMPLEINTEGERNAME)
        factory.SetUnit(SAMPLEPVALUEUNITS)
        factory.CreateInteger()

        # Creates an integer SwissKnife, a read-only expression.
        # It only has one variable, the sample integer.
        # The SwissKnife expression converts the millisecond sample integer to nanoseconds with a * 1000 formula".
        factory.SetName(SAMPLEINTSWISSKNIFENAME)
        factory.SetDescription(SAMPLEINTSWISSKNIFEDESCRIPTION)
        factory.SetToolTip(SAMPLEINTSWISSKNIFETOOLTIP)
        factory.SetCategory(SAMPLECATEGORY)
        factory.AddVariable(SAMPLEINTEGERNAME)
        factory.SetUnit(SAMPLEINTSWISSKNIFEUNITS)
        factory.CreateIntSwissKnife(SAMPLEINTEGERNAME + " * 1000")

        # Creates a float SwissKnife, a read-only expression.
        # It only has one variable, the sample float.
        # The SwissKnife expression converts the inches sample float to centimeters with a * 2.54 formula.
        factory.SetName(SAMPLEFLOATSWISSKNIFENAME)
        factory.SetDescription(SAMPLEFLOATSWISSKNIFEDESCRIPTION)
        factory.SetToolTip(SAMPLEFLOATSWISSKNIFETOOLTIP)
        factory.SetCategory(SAMPLECATEGORY)
        factory.AddVariable(SAMPLEFLOATNAME)
        factory.SetUnit(SAMPLEFLOATSWISSKNIFEUNITS)
        factory.CreateFloatSwissKnife(SAMPLEFLOATNAME + " * 2.54")

        # Create integer converter (read-write to/from expressions)
        # The main feature the converter acts uppon is the sample integer and handles millisecond to nanosecond conversion.
        # No additional variables are required, no need to call AddVariable on the factory.
        factory.SetName(SAMPLEINTCONVERTERNAME)
        factory.SetDescription(SAMPLEINTCONVERTERDESCRIPTION)
        factory.SetToolTip(SAMPLEINTCONVERTERTOOLTIP)
        factory.SetCategory(SAMPLECATEGORY)
        factory.SetUnit(SAMPLEINTCONVERTERUNITS)
        factory.CreateIntConverter(SAMPLEINTEGERNAME, "TO * 1000", "FROM / 1000")

        # Create float converter (read-write to/from expressions)
        # The main feature the converter acts uppon is the sample float and handles inches to centimeter conversion.
        # No additional variables are required, no need to call AddVariable on the factory.
        factory.SetName(SAMPLEFLOATCONVERTERNAME)
        factory.SetDescription(SAMPLEFLOATCONVERTERDESCRIPTION)
        factory.SetToolTip(SAMPLEFLOATCONVERTERTOOLTIP)
        factory.SetCategory(SAMPLECATEGORY)
        factory.SetUnit(SAMPLEFLOATCONVETERUNITS)
        factory.CreateFloatConverter(SAMPLEFLOATNAME, "TO * 2.54", "FROM * 0.3937")

        # Create hidden SwissKnife that returns non-zero (true, or 1) if the sample integer is above 5.
        # The feature will not show up in GUI browsers as it does not have a category.
        # We typically refer to these features as support or private features.
        # These features typically do not have description or ToolTips as they do not show up in a GenApi user interface.
        factory.SetName(SAMPLEHIDDENSWISSKNIFENAME)
        factory.AddVariable(SAMPLEINTEGERNAME)
        factory.CreateIntSwissKnife(SAMPLEINTEGERNAME + " > 5")

        # Create integer: link to sample integer but only available when the hidden SwissKnife is non-zero.
        # This feature is an integer that has a pValue link to our sample enumeration, displaying its integer value.
        # We use this feature to demonstrate the pIsAvailable attribute: it links to our hidden SwissKnife and controls when it is available.
        # When the hidden SwissKnife is true (sample integer > 5) this feature will be readable and writable.
        # When the hidden SwissKnife is false (sample integer <= 5) this feature will be "not available".
        # We use "pIsAvailable" as display name as the automatically generated one is not as good.
        factory.SetName(SAMPLEPISAVAILABLENAME)
        factory.SetDisplayName(SAMPLEPISAVAILABLEDISPLAYNAME)
        factory.SetDescription(SAMPLEPISAVAILABLEDESCRIPTION)
        factory.SetToolTip(SAMPLEPISAVAILABLETOOLTIP)
        factory.SetCategory(SAMPLECATEGORY)
        factory.SetPIsAvailable(SAMPLEHIDDENSWISSKNIFENAME)
        factory.SetPValue(SAMPLEENUMNAME)
        factory.CreateInteger()

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

    def create_event_parameters(self, factory):
        # Create GenApi feature used to map the event data count field
        factory.SetName(EVENTCOUNTNAME)
        factory.SetDescription(EVENTCOUNTDESCRIPTION)
        factory.SetToolTip(EVENTCOUNTTOOLTIP)
        factory.SetCategory(EVENTCATEGORY)
        factory.MapEvent(EVENTDATAID, 0, 4)
        factory.CreateInteger(None, 0, 2**32-1)

        # Create GenApi feature used to map the event data time field
        factory.SetName(EVENTTIMENAME)
        factory.SetDescription(EVENTTIMEDESCRIPTION)
        factory.SetToolTip(EVENTTIMETOOLTIP)
        factory.SetCategory(EVENTCATEGORY)
        factory.MapEvent(EVENTDATAID, 4, 32)
        factory.CreateString(None)

