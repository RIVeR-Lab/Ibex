#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************
'''
import eBUS as eb
from Defines import FILESELECTORADDR
from Defines import FILEBUFFERSIZE
from IFileAccessFile import *

class FileAccessRegisterEventSink(eb.IPvRegisterEventSink):
    file_list = []

    def __init__(self, register_base_address, map):
        super().__init__()
        self.register_base_address = register_base_address
        self.map = map

    def PreRead(self, register):
        file_selector = self.__ReadFileSelector()
        file_operation_selector = self.__ReadFileOperationSelector()
        address = register.GetAddress()
        if(address == self.FileAccessOffsetAddr()):
            # FileAccessOffset[FileSelector][FileOperationSelector]
            # => write back the value of the currently selected file.
            file_access_offset = self.file_list[file_selector].Offset(file_operation_selector)
            register.Write(file_access_offset)
        elif(address == self.FileAccessLengthAddr()):
            # FileAccessLength[FileSelector][FileOperationSelector]
            # => write back the value of the currently selected file.
            file_access_length = self.file_list[file_selector].Length(file_operation_selector)
            register.Write(file_access_length)
        elif(address == self.FileOperationStatusAddr()):
            # FileAccessLength[FileSelector][FileOperationSelector]
            # => write back the value of the currently selected file.
            file_operation_status = self.file_list[file_selector].OperationStatus(file_operation_selector)
            register.Write(file_operation_status)
        elif(address == self.FileOperationResultAddr()):
            # FileAccessLength[FileSelector][FileOperationSelector]
            # => write back the value of the currently selected file.
            file_operation_result = self.file_list[file_selector].OperationResult(file_operation_selector)
            register.Write(file_operation_result)
        elif(address == self.FileSizeAddr()):
            # FileSize[FileSelector]
            file_size = self.file_list[file_selector].Size()
            register.Write(file_size)

        return eb.PV_OK

    def PostRead(self, register):
        return

    def PreWrite(self, register):
        return eb.PV_OK

    def PostWrite(self, register):
        file_operation_status = FileOperationStatus.Success
        file_operation_result = False
        file_selector = self.__ReadFileSelector()
        address=register.GetAddress()
        
        if(address == self.FileSelectorAddr()):
            file_open_mode = self.__ReadFileOpenMode()
            access_mode=self.file_list[file_selector].AccessMode()
            if((access_mode == eb.PvGenAccessModeReadOnly) and ((file_open_mode==FileOpenMode.FOM_WRITE) or (file_open_mode==FileOpenMode.FOM_READ_WRITE))):
                # RO file
                self.__WriteFileOpenMode(FileOpenMode.FOM_READ)
            elif((access_mode == eb.PvGenAccessModeWriteOnly) and ((file_open_mode==FileOpenMode.FOM_READ) or (file_open_mode==FileOpenMode.FOM_READ_WRITE))):
                # WO file
                self.__WriteFileOpenMode(FileOpenMode.FOM_WRITE)
            # ReadWrite files can handle any FileOpenMode.
        
        elif(address == self.FileOperationExecuteAddr()):
            # Perform the selected operation.
            file_operation_selector = self.__ReadFileOperationSelector()
            if(FileOperationSelector.FOS_OPEN == file_operation_selector):
                # Open selected file.
                file_open_mode = self.__ReadFileOpenMode()
                file_operation_status, file_operation_result = self.file_list[file_selector].Open(file_open_mode)
                if(file_operation_status == FileOperationStatus.Success):
                    file_size = self.file_list[file_selector].Size()
                    self.__WriteFileSize(file_size)
                    
            elif(FileOperationSelector.FOS_CLOSE == file_operation_selector):
                # Close selected file.
                file_operation_status, file_operation_result = self.file_list[file_selector].Close()
                
            elif(FileOperationSelector.FOS_READ == file_operation_selector):
                file_access_length = self.__ReadFileAccessLength()
                file_access_offset = self.__ReadFileAccessOffset()
                file_access_buffer_reg = self.map.GetRegisterByAddress(self.FileAccessBufferAddr())
                
                file_operation_status, file_operation_result = self.file_list[file_selector].Read(file_access_length, file_access_offset, file_access_buffer_reg)

            elif(FileOperationSelector.FOS_WRITE == file_operation_selector):
                file_access_length = self.__ReadFileAccessLength()
                file_access_offset = self.__ReadFileAccessOffset()
                file_access_buffer_reg = self.map.GetRegisterByAddress(self.FileAccessBufferAddr())
                
                file_operation_status, file_operation_result = self.file_list[file_selector].Write(file_access_length, file_access_offset, file_access_buffer_reg)
                if(file_operation_status == FileOperationStatus.Success):
                    # Update FileSize
                    file_size = self.__ReadFileSize()
                    self.__WriteFileSize(file_size + file_operation_result)
                    
            elif(FileOperationSelector.FOS_DELETE == file_operation_selector):
                file_operation_status, file_operation_result = self.file_list[file_selector].Delete()
            
            # Save those in file, as per selectors.
            # FileOperationStatus[FileSelector][FileOperationSelector]
            # FileOperationResult[FileSelector][FileOperationSelector]
            self.file_list[ file_selector ].SetOperationStatus( file_operation_selector, file_operation_status );
            self.file_list[ file_selector ].SetOperationResult( file_operation_selector, file_operation_result );

        elif(address == self.FileAccessOffsetAddr()):
            # Save value in file as per current selectors.
            # FileAccessOffset[FileSelector][FileOperationSelector]
            file_operation_selector = self.__ReadFileOperationSelector()
            file_access_offset = 0
            result, file_access_offset = register.ReadInt()
            self.file_list[file_selector].SetOffset(file_operation_selector, file_access_offset)

        elif(address == self.FileAccessLengthAddr()):
            # Save value in file as per current selectors.
            # FileAccessLength[FileSelector][FileOperationSelector]
            file_operation_selector = self.__ReadFileOperationSelector()
            file_access_length = 0
            result, file_access_length = register.ReadInt()
            self.file_list[file_selector].SetLength(file_operation_selector, file_access_length)

    def FileSelectorAddr(self):
        return FILESELECTORADDR

    def FileOperationSelectorAddr(self):
        return FILESELECTORADDR + 0x04

    def FileOperationExecuteAddr(self):
        return FILESELECTORADDR + 0x08

    def FileOpenModeAddr(self):
        return FILESELECTORADDR + 0x0C

    def FileAccessBufferAddr(self):
        return FILESELECTORADDR + 0x10

    def FileAccessBufferSize(self):
        return FILEBUFFERSIZE

    def FileAccessOffsetAddr(self):
        return FILESELECTORADDR + 0x10 + FILEBUFFERSIZE

    def FileAccessLengthAddr(self):
        return FILESELECTORADDR + 0x14 + FILEBUFFERSIZE

    def FileOperationStatusAddr(self):
        return FILESELECTORADDR + 0x18 + FILEBUFFERSIZE

    def FileOperationResultAddr(self):
        return FILESELECTORADDR + 0x1C + FILEBUFFERSIZE

    def FileSizeAddr(self):
        return FILESELECTORADDR + 0x20 + FILEBUFFERSIZE

    def AddFile(self,file):
        self.file_list.append(file)

    def GetFiles(self):
        return self.file_list

    def __ReadFileSelector(self):
        result, value = self.map.GetRegisterByAddress( self.FileSelectorAddr() ).ReadInt()
        return value

    def __ReadFileAccessLength(self):
        result, value = self.map.GetRegisterByAddress( self.FileAccessLengthAddr() ).ReadInt()
        return value

    def __ReadFileAccessOffset(self):
        result, value = self.map.GetRegisterByAddress( self.FileAccessOffsetAddr() ).ReadInt()
        return value

    def __ReadFileOpenMode(self):
        result, value = self.map.GetRegisterByAddress( self.FileOpenModeAddr() ).ReadInt()
        return value

    def __ReadFileOperationSelector(self):
        result, value = self.map.GetRegisterByAddress( self.FileOperationSelectorAddr() ).ReadInt()
        return value

    def __ReadFileSize(self):
        result, value = self.map.GetRegisterByAddress( self.FileSizeAddr() ).ReadInt()
        return value

    def __WriteFileOpenMode(self,mode):
        self.map.GetRegisterByAddress( self.FileOpenModeAddr() ).Write(mode)

    def __WriteFileOperationStatus(self,status):
        self.map.GetRegisterByAddress( self.FileOperationStatusAddr() ).Write(status)

    def __WriteFileOperationResult(self,result):
        self.map.GetRegisterByAddress( self.FileOperationResultAddr() ).Write(result)

    def __WriteFileSize(self,size):
        self.map.GetRegisterByAddress( self.FileSizeAddr() ).Write(size)
