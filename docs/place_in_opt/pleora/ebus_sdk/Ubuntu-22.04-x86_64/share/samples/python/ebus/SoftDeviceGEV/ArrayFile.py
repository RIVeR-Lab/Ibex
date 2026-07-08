#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************
'''

import eBUS as eb
from IFileAccessFile import *

class ArrayFile(IFileAccessFile):
    def __init__(self,name,max_file_size):
        super().__init__(name,eb.PvGenAccessModeWriteOnly)
        self.max_size = max_file_size
        self.is_opened = False
        self.data = [0] * self.max_size

    def Open(self,mode):
        if(self.is_opened):
            return (FileOperationStatus.Failure,0)
        self.is_opened = True
        return (FileOperationStatus.Success,0)

    def Close(self):
        if(not self.is_opened):
            return (FileOperationStatus.Failure,0)
        self.is_opened = False
        return (FileOperationStatus.Success,0)

    def Read(self,req_len,offset,file_access_buffer_reg):
        return (FileOperationStatus.Failure,0)

    def Write(self,req_len,offset,file_access_buffer_reg):
        if((not self.is_opened) or (offset >= self.max_size)):
            return (FileOperationStatus.Failure,0)
        
        adjusted_len = req_len
        if((offset + req_len) > self.max_size):
            adjusted_len = self.max_size - offset
            
        if(adjusted_len):
            result,data = file_access_buffer_reg.ReadBytes(adjusted_len,0)
            if(not result.IsOK()):
                return (FileOperationStatus.Failure,0)

            self.data[offset:] = data
            lsize = self.Size()
            self.SetSize(lsize + req_len)

        return (FileOperationStatus.Success,req_len)

    def Delete(self):
        # Simulate some kind of delete.
        # Set the size back to 0. No need to clear the whole array.
        SetSize(0)
        return (FileOperationStatus.Success,0)
