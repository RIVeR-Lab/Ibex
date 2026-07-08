#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************
'''

import eBUS as eb

#
#  callback for UserSet state changes
#
class MyUserSetNotify(eb.IPvUserSetNotify):
    def __init__(self):
        super().__init__()

    def UserSetStateNotify(self, userSetState, userSetIndex):
        if eb.PvUserSetStateSaveStart == userSetState:
            print(f"Userset {userSetIndex} : SaveStart" )
        
        if eb.PvUserSetStateSaveCompleted == userSetState:
            print(f"Userset {userSetIndex} : SaveCompleted" )
       
        if eb.PvUserSetStateLoadStart == userSetState:
            print(f"Userset {userSetIndex} : LoadStart" )
      
        if eb.PvUserSetStateLoadCompleted == userSetState:
            print(f"Userset {userSetIndex} : LoadCompleted" )
        return eb.PV_OK

