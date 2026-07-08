#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************
'''

import eBUS as eb

class EventHandler(eb.PvDeviceEventSink):

    def OnEvent(self, device, event_ID, channel, block_ID, timestamp, data):
        print(f"\nOnEvent Callback : \nReceived event ID {hex(event_ID)}    Timestamp {timestamp}")
        print(f"Channel {hex(channel)}    Block ID {block_ID}    Data Length {len(data)}")
        return (0)

    def OnEventGenICam(self, device, event_ID, channel, block_ID, timestamp, genicam_list):
        if genicam_list is not None:
            print(f"\nOnEventGenICam Callback : \nReceived event ID {hex(event_ID)}    Timestamp {timestamp}")
            print(f"Channel {hex(channel)}    Block ID {block_ID}")
            
            event_param = genicam_list.GetFirst()
            while ( event_param is not None ):
                if type(event_param) is not eb.PvGenRegister:
                    print(f"Parameter {event_param.GetName()[1]}    value: {event_param.ToString()[1]}")
                else:
                    result, data_length = event_param.GetLength()
                    data = np.zeros(data_length, dtype= np.uint8)
                    result, data = event_param.Get( data_length )
                    print(f"Parameter {event_param.GetName()[1]}    Data length: {data_length}")
                event_param = genicam_list.GetNext()