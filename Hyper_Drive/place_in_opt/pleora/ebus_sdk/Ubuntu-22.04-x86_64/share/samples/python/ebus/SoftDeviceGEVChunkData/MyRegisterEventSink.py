#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************
'''

import eBUS as eb
class MyRegisterEventSink(eb.IPvRegisterEventSink):
    def __init__(self):
        super().__init__()

    def PreRead(self, register):
        print(f"{register.GetName()} PreRead")
        return eb.PV_OK

    def PostRead(self, register):
        print(f"{register.GetName()} PostRead")

    def PreWrite(self, register):
        print(f"{register.GetName()} PreWrite")
        return eb.PV_OK

    def PostWrite(self, register):
        print(f"{register.GetName()} PostWrite")
