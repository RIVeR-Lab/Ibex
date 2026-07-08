# *****************************************************************************
#
# Copyright (c) 2020, Pleora Technologies Inc., All rights reserved.
#
# *****************************************************************************

import time
import eBUS as eb
import struct
from Defines import *

# Shows how to go through the whole register map
def dump_registers(register_map):
    # Locking the register map guarantees safe access to the registers
    if not register_map.Lock().IsOK():
        return

    for i in range(register_map.GetRegisterCount()):
        register = register_map.GetRegisterByIndex(i)
        print(f"{register.GetName()} @ 0x{register.GetAddress():X} {register.GetLength()} bytes", end='')
        if register.IsReadable():
            print(" {readable}", end='')
        if register.IsWritable():
            print(" {writable}", end='')
        print()

    # Always release a lock, failing to do so would deadlock the Software GigE Vision Device
    register_map.Release()
