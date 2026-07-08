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

class DiskFile(IFileAccessFile):
    def __init__(self,name,access_mode,is_binary):
        super().__init__(name,access_mode)
        self.is_binary = is_binary
        self.name = name
        self.fstream = None

    def Open(self,mode):
        if((not (self.fstream is None)) and not self.fstream.closed):
            return (FileOperationStatus.Failure,0)
        if (mode == FileOpenMode.FOM_READ):
            lmode = 'r'
        if (mode == FileOpenMode.FOM_WRITE):
            lmode = 'w'
        if (mode == FileOpenMode.FOM_READ_WRITE):
            lmode = 'r+'
        if(self.is_binary):
            lmode = lmode+'b'
        try:
            self.fstream = open(self.name,lmode)
        except IOError:
            print("File open failed: " + self.name)
            return (FileOperationStatus.Failure,0)
        if(self.fstream.closed):
            return (FileOperationStatus.Failure,0)

        #update the file size
        self.fstream.seek(0,2)
        self.SetSize(self.fstream.tell())

        return (FileOperationStatus.Success,0)

    def Close(self):
        if(self.fstream.closed):
            return (FileOperationStatus.Failure,0)
        self.fstream.flush()
        self.fstream.close()
        return (FileOperationStatus.Success,0)

    def Read(self,req_len,offset,file_access_buffer_reg):
        # Fail right away if file not opened.
        if(self.fstream.closed or (offset >= self.Size())):
            return (FileOperationStatus.Failure,0)

        # Move file pointer to requested offset.
        self.fstream.seek(offset)

        # Adjust requested length to remaining unread portion.
        adjusted_len = req_len
        if((offset + req_len) > self.Size()):
            adjusted_len = self.Size() - offset

        if(adjusted_len):
            data = bytearray()
            data = self.fstream.read(adjusted_len)
            result = file_access_buffer_reg.Write(data)
            if(not result.IsOK()):
                return (FileOperationStatus.Failure,0)

        return (FileOperationStatus.Success,adjusted_len)

    def Write(self,req_len,offset,file_access_buffer_reg):
        # Fail right away if file not opened.
        if(self.fstream.closed):
            return (FileOperationStatus.Failure,0)
        
        if(req_len > 0):
            result,data = file_access_buffer_reg.ReadBytes(req_len)
            if(not result.IsOK()):
                return (FileOperationStatus.Failure,0)
            
            self.fstream.write(data.tobytes())
            adjusted_len = self.PadFile(req_len)
            
            lsize = self.Size()
            self.SetSize(lsize+adjusted_len)
            
        return (FileOperationStatus.Success,req_len)

    def Delete(self):
        return (FileOperationStatus.Failure,0)
        
    def PadFile(self,req_len):
        adjusted_len = req_len
        pad_char = b'\x00' if self.is_binary else ' '
        if ((req_len % 4) == 1):
            self.fstream.write(pad_char)
            self.fstream.write(pad_char)
            self.fstream.write(pad_char)
            adjusted_len = adjusted_len + 3
        elif ((req_len % 4) == 2):
            self.fstream.write(pad_char)
            self.fstream.write(pad_char)
            adjusted_len = adjusted_len + 2
        elif ((req_len % 4) == 3):
            self.fstream.write(pad_char)
            adjusted_len = adjusted_len + 1
        return adjusted_len
