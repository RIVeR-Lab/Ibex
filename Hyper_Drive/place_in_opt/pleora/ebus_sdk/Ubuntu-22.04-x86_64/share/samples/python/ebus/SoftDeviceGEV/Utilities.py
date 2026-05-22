# *****************************************************************************
#
# Copyright (c) 2020, Pleora Technologies Inc., All rights reserved.
#
# *****************************************************************************

import time
import eBUS as eb
import struct
from Defines import *

#
# \brief This function shows how to send messaging channel events
#

def fire_test_events(message_channel):
    if message_channel == None:
        return

    if message_channel.IsOpened():
        lNow = time.perf_counter()
        if (lNow - fire_test_events.last_event) > 2.5:
            # pack the data
            event_data = struct.pack("<I32s", fire_test_events.count, bytes(time.asctime(), 'utf-8'))
            message_channel.FireEvent(EVENTDATAID, event_data)
            message_channel.FireEvent(EVENTID)

            fire_test_events.last_event = lNow
            fire_test_events.count += 1

fire_test_events.count = 0
fire_test_events.last_event = time.perf_counter()

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
