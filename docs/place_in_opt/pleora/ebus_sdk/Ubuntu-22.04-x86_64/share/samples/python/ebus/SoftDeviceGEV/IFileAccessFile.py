#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************
'''

import enum
from Defines import FILEBUFFERSIZE
from abc import ABC, abstractmethod

class FileOperationSelector:
    FOS_OPEN = 0
    FOS_CLOSE = 1
    FOS_READ = 2
    FOS_WRITE = 3
    FOS_DELETE = 4
    FOS_QTY = 5

class FileOperationStatus:
    Success = 0
    Failure = 1

class FileOpenMode:
    FOM_READ = 0
    FOM_WRITE = 1
    FOM_READ_WRITE = 2

class IFileAccessFile(ABC):
    offsets = list(range(FileOperationSelector.FOS_QTY))
    lengths = list(range(FileOperationSelector.FOS_QTY))
    status = list(range(FileOperationSelector.FOS_QTY))
    results = list(range(FileOperationSelector.FOS_QTY))
    def __init__(self,name,access_mode):
        self.display_name = name
        self.access_mode = access_mode
        self.size = 0

    @abstractmethod
    def Open(self,mode):
        pass

    @abstractmethod
    def Close(self):
        pass

    @abstractmethod
    def Read(self,req_len,offset,file_access_buffer_reg):
        pass

    @abstractmethod
    def Write(self,req_len,offset,file_access_buffer_reg):
        pass

    @abstractmethod
    def Delete(self):
        pass

    def DisplayName(self):
        return self.display_name

    def AccessMode(self):
        return self.access_mode

    def Size(self):
        return self.size

    def Offset(self,file_operation_selector):
        return self.offsets[file_operation_selector]

    def Length(self,file_operation_selector):
        return self.lengths[file_operation_selector]

    def OperationStatus(self,file_operation_selector):
        return self.status[file_operation_selector]

    def OperationResult(self,file_operation_selector):
        return self.results[file_operation_selector]

    def SetOffset(self,file_operation_selector,file_access_offset):
        self.offsets[file_operation_selector] = file_access_offset

    def SetLength(self,file_operation_selector,file_access_length):
        self.lengths[file_operation_selector] = file_access_length

    def SetOperationStatus(self,file_operation_selector,file_operation_status):
        self.status[file_operation_selector] = file_operation_status

    def SetOperationResult(self,file_operation_selector,file_operation_result):
        self.results[file_operation_selector] = file_operation_result

    def SetSize(self,size):
        self.size = size

