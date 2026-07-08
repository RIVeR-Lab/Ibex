#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2020, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************
'''

import eBUS as eb
from Defines import *


class MyRegisterEventSink(eb.IPvRegisterEventSink):
    def __init__(self):
        super().__init__()
        self._sample_enum = 0
        self._sample_strings = [ "", "", "" ]
        self._sample_booleans = [ False, False, False ]

    def PreRead(self, register):
        print(f"{register.GetName()} PreRead")
        if register.GetAddress() == SAMPLESTRINGADDR:
            register.Write( self._sample_strings[ self._sample_enum ] )
        if register.GetAddress() == SAMPLEBOOLEANADDR:
            register.Write( self._sample_booleans[ self._sample_enum ] )
        return eb.PV_OK

    def PostRead(self, register):
        print(f"{register.GetName()} PostRead")

    def PreWrite(self, register):
        print(f"{register.GetName()} PreWrite")
        return eb.PV_OK

    def PostWrite(self, register):
        # We need to reset command registers to 0 after activation for IsDone
        if register.GetAddress() == SAMPLECOMMANDADDR:
            self._sample_strings[self._sample_enum] = ""
            register.Write(0)
        if register.GetAddress() == SAMPLEENUMADDR:
            result, self._sample_enum = register.ReadInt()
        if register.GetAddress() == SAMPLESTRINGADDR:
            result, self._sample_strings[self._sample_enum] = register.ReadString()
        if register.GetAddress() == SAMPLEBOOLEANADDR:
            result, self._sample_booleans[self._sample_enum] = register.ReadInt()

        print(f"{register.GetName()} PostWrite")


    def Persist(self, register, store):
        # Manage Userset persistence for nodes which vary by a selector
        if register.GetAddress() == SAMPLESTRINGADDR:
           return self.PersistSampleEnum( register, store )
        if register.GetAddress() == SAMPLEBOOLEANADDR:
           return self.PersistSampleEnum( register, store )
        return eb.PV_NOT_IMPLEMENTED;


    def PersistSampleEnum(self, register, store):
        # Manage Userset persistence for nodes which vary by a SampleEnum
        # Preserve current mSampleEnum value
        enum_org = self._sample_enum

        for i in range(SAMPLEENUMCOUNT):
            # Select Sample Enum
            self._sample_enum = i

            # Create persistence suffix
            suffix = "EnumEntry" + str(i + 1)

            # Persist selected register (save or load depending on context)
            store.Persist(register, suffix)

        self._sample_enum = enum_org;
        return eb.PV_OK;
