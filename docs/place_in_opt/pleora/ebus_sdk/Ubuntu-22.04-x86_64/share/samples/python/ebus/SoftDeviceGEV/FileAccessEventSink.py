#!/usr/bin/env python3

'''
*****************************************************************************
*
*   Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
*
*****************************************************************************
'''

import eBUS as eb
from IFileAccessFile import FileOperationStatus
from IFileAccessFile import FileOperationSelector
from IFileAccessFile import IFileAccessFile
import Utilities as utils
from Defines import *

class FileAccessEventSink(eb.IPvSoftDeviceGEVEventSink):
    def __init__(self, register_event_sink):
        super().__init__()
        self.register_event_sink = register_event_sink

    def OnCreateCustomRegisters(self, device, factory):
        factory.AddRegister(FILESELECTORNAME, self.register_event_sink.FileSelectorAddr(), 4, eb.PvGenAccessModeReadWrite, self.register_event_sink)
        factory.AddRegister(FILEOPERATIONSELECTORNAME, self.register_event_sink.FileOperationSelectorAddr(), 4, eb.PvGenAccessModeReadWrite, self.register_event_sink)
        factory.AddRegister(FILEOPERATIONEXECUTENAME, self.register_event_sink.FileOperationExecuteAddr(), 4, eb.PvGenAccessModeReadWrite, self.register_event_sink)
        factory.AddRegister(FILEOPENMODENAME, self.register_event_sink.FileOpenModeAddr(), 4, eb.PvGenAccessModeReadWrite, self.register_event_sink)
        factory.AddByteArray(FILEACCESSBUFFERNAME, self.register_event_sink.FileAccessBufferAddr(), FILEBUFFERSIZE, eb.PvGenAccessModeReadWrite, self.register_event_sink)
        factory.AddRegister(FILEACCESSOFFSETNAME, self.register_event_sink.FileAccessOffsetAddr(), 4, eb.PvGenAccessModeReadWrite, self.register_event_sink)
        factory.AddRegister(FILEACCESSLENGTHNAME, self.register_event_sink.FileAccessLengthAddr(), 4, eb.PvGenAccessModeReadWrite, self.register_event_sink)
        factory.AddRegister(FILEOPERATIONSTATUSNAME, self.register_event_sink.FileOperationStatusAddr(), 4, eb.PvGenAccessModeReadWrite, self.register_event_sink)
        factory.AddRegister(FILEOPERATIONRESULTNAME, self.register_event_sink.FileOperationResultAddr(), 4, eb.PvGenAccessModeReadWrite, self.register_event_sink)
        factory.AddRegister(FILESIZENAME, self.register_event_sink.FileSizeAddr(), 4, eb.PvGenAccessModeReadWrite, self.register_event_sink)

    def OnCreateCustomGenApiFeatures(self, device, factory):
        lMap = device.GetRegisterMap()
        
        self.create_file_selector(lMap, factory)
        self.create_file_operation_selector(lMap, factory)
        self.create_file_operation_execute(lMap, factory)
        self.create_file_open_mode(lMap, factory)
        self.create_file_access_buffer(lMap, factory)
        self.create_file_access_offset(lMap, factory)
        self.create_file_access_length(lMap, factory)
        self.create_file_operation_status(lMap, factory)
        self.create_file_operation_result(lMap, factory)
        self.create_file_size(lMap, factory)

    def create_file_selector(self, map, factory):
        factory.SetName(FILESELECTORNAME)
        factory.SetDescription("Selects the target file in the device.")
        factory.SetToolTip("Selects the target file in the device.")
        factory.SetDisplayName("File Selector")
        factory.SetCategory(FILECATEGORYNAME)
        factory.SetVisibility(eb.PvGenVisibilityGuru)
        factory.SetStreamable(False)
        factory.AddSelected(FILEACCESSLENGTHNAME)
        factory.AddSelected(FILEACCESSOFFSETNAME)
        factory.AddSelected(FILEOPENMODENAME)
        factory.AddSelected(FILEOPERATIONEXECUTENAME)
        factory.AddSelected(FILEOPERATIONRESULTNAME)
        factory.AddSelected(FILEOPERATIONSELECTORNAME)
        factory.AddSelected(FILEOPERATIONSTATUSNAME)
        factory.AddSelected(FILESIZENAME)

        index=0
        for i in self.register_event_sink.GetFiles():
            factory.AddEnumEntry(i.DisplayName(),index)
            index += 1
        factory.CreateEnum(map.GetRegisterByAddress( self.register_event_sink.FileSelectorAddr() ))

    def create_file_operation_selector(self, map, factory):
        factory.SetName(FILEOPERATIONSELECTORNAME)
        factory.SetDescription(
            "Selects the target operation for the selected file in the device. " \
            "This Operation is executed when the FileOperationExecute feature is called.")
        factory.SetToolTip("Selects the target operation for the selected file in the device.")
        factory.SetDisplayName("File Operation Selector")
        factory.SetCategory(FILECATEGORYNAME)
        factory.SetVisibility(eb.PvGenVisibilityGuru)
        factory.SetStreamable(False)
        factory.AddSelected(FILEACCESSLENGTHNAME)
        factory.AddSelected(FILEACCESSOFFSETNAME)
        factory.AddSelected(FILEOPERATIONEXECUTENAME)
        factory.AddSelected(FILEOPERATIONRESULTNAME)
        factory.AddSelected(FILEOPERATIONSTATUSNAME)
        factory.AddEnumEntry("Open",FileOperationSelector.FOS_OPEN)
        factory.AddEnumEntry("Close",FileOperationSelector.FOS_CLOSE)
        factory.AddEnumEntry("Read",FileOperationSelector.FOS_READ)
        factory.AddEnumEntry("Write",FileOperationSelector.FOS_WRITE)
        factory.AddEnumEntry("Delete",FileOperationSelector.FOS_DELETE)
        factory.CreateEnum(map.GetRegisterByAddress( self.register_event_sink.FileOperationSelectorAddr() ))

        file_index = 0
        factory.SetName("FileOperationSelectorReadAvailableExpr")
        factory.AddVariable("FileSelector")
        lSK = "0"
        for i in self.register_event_sink.GetFiles():
            if((i.AccessMode() == eb.PvGenAccessModeReadOnly) or (i.AccessMode() == eb.PvGenAccessModeReadWrite)):
                lSK = lSK + " || (VAR_FILESELECTOR = " + str(file_index) + ")"
            file_index = file_index + 1
        factory.CreateIntSwissKnife(lSK)
        factory.SetPIsAvailableForEnumEntry(FILEOPERATIONSELECTORNAME, "Read", "FileOperationSelectorReadAvailableExpr" )

        file_index = 0
        factory.SetName("FileOperationSelectorWriteAvailableExpr")
        factory.AddVariable("FileSelector")
        lSK = "0"
        for i in self.register_event_sink.GetFiles():
            if((i.AccessMode() == eb.PvGenAccessModeWriteOnly) or (i.AccessMode() == eb.PvGenAccessModeReadWrite)):
                lSK = lSK + " || (VAR_FILESELECTOR = " + str(file_index) + ")"
            file_index = file_index + 1
        factory.CreateIntSwissKnife(lSK)
        factory.SetPIsAvailableForEnumEntry(FILEOPERATIONSELECTORNAME, "Write", "FileOperationSelectorWriteAvailableExpr" )

        file_index = 0
        factory.SetName("FileOperationSelectorDeleteAvailableExpr")
        factory.AddVariable("FileSelector")
        lSK = "0"
        for i in self.register_event_sink.GetFiles():
            if((i.AccessMode() == eb.PvGenAccessModeWriteOnly) or (i.AccessMode() == eb.PvGenAccessModeReadWrite)):
                lSK = lSK + " || (VAR_FILESELECTOR = " + str(file_index) + ")"
            file_index = file_index + 1
        factory.CreateIntSwissKnife(lSK)
        factory.SetPIsAvailableForEnumEntry(FILEOPERATIONSELECTORNAME, "Delete", "FileOperationSelectorDeleteAvailableExpr" )

    def create_file_operation_execute(self, map, factory):
        factory.SetName(FILEOPERATIONEXECUTENAME)
        factory.SetDescription("Executes the operation selected by FileOperationSelector on the selected file.")
        factory.SetToolTip("Executes the operation selected by FileOperationSelector on the selected file.")
        factory.SetDisplayName("File Operation Execute")
        factory.SetCategory(FILECATEGORYNAME)
        factory.SetVisibility(eb.PvGenVisibilityGuru)
        factory.SetStreamable(False)
        factory.SetAccessMode(eb.PvGenAccessModeWriteOnly)
        factory.SetCachable(eb.PvGenCacheWriteThrough)
        factory.CreateCommand(map.GetRegisterByAddress( self.register_event_sink.FileOperationExecuteAddr() ))

    def create_file_open_mode(self, map, factory):
        factory.SetName(FILEOPENMODENAME)
        factory.SetDescription("Selects the access mode in which a file is opened in the device.")
        factory.SetToolTip("Selects the access mode in which a file is opened in the device.")
        factory.SetDisplayName("File Open Mode")
        factory.SetCategory(FILECATEGORYNAME)
        factory.SetVisibility(eb.PvGenVisibilityGuru)
        factory.SetStreamable(False)
        factory.AddInvalidator("FileSelectorReg")
        factory.AddEnumEntry("Read", 0 )
        factory.AddEnumEntry("Write", 1 )
        factory.AddEnumEntry("ReadWrite", 2 )
        factory.CreateEnum( map.GetRegisterByAddress( self.register_event_sink.FileOpenModeAddr() ) )

        file_index = 0
        factory.SetName("FileOpenModeReadAvailableExpr")
        factory.AddVariable("FileSelector")
        lSK = "0"
        for i in self.register_event_sink.GetFiles():
            if((i.AccessMode() == eb.PvGenAccessModeReadOnly) or (i.AccessMode() == eb.PvGenAccessModeReadWrite)):
                lSK = lSK + " || (VAR_FILESELECTOR = " + str(file_index) + ")"
            file_index = file_index + 1
        factory.CreateIntSwissKnife(lSK)
        factory.SetPIsAvailableForEnumEntry(FILEOPENMODENAME, "Read", "FileOpenModeReadAvailableExpr" )

        file_index = 0
        factory.SetName("FileOpenModeWriteAvailableExpr")
        factory.AddVariable("FileSelector")
        lSK = "0"
        for i in self.register_event_sink.GetFiles():
            if((i.AccessMode() == eb.PvGenAccessModeWriteOnly) or (i.AccessMode() == eb.PvGenAccessModeReadWrite)):
                lSK = lSK + " || (VAR_FILESELECTOR = " + str(file_index) + ")"
            file_index = file_index + 1
        factory.CreateIntSwissKnife(lSK)
        factory.SetPIsAvailableForEnumEntry(FILEOPENMODENAME, "Write", "FileOpenModeWriteAvailableExpr" )

        file_index=0
        factory.SetName("FileOpenModeReadWriteAvailableExpr")
        factory.AddVariable("FileSelector")
        lSK = "0"
        for i in self.register_event_sink.GetFiles():
            if(i.AccessMode() == eb.PvGenAccessModeReadWrite):
                lSK = lSK + " || (VAR_FILESELECTOR = " + str(file_index) + ")"
            file_index = file_index + 1
        factory.CreateIntSwissKnife(lSK)
        factory.SetPIsAvailableForEnumEntry(FILEOPENMODENAME, "ReadWrite", "FileOpenModeReadWriteAvailableExpr" )

    def create_file_access_buffer(self, map, factory):
        factory.SetName(FILEACCESSBUFFERNAME)
        factory.SetDescription("Defines the intermediate access buffer that allows the exchange of data between the device file storage and the application.")
        factory.SetToolTip("Defines the intermediate access buffer that allows the exchange of data between the device file storage and the application.")
        factory.SetDisplayName("File Access Buffer")
        factory.SetCategory(FILECATEGORYNAME)
        factory.SetVisibility(eb.PvGenVisibilityGuru)
        factory.SetStreamable(False)
        factory.SetCachable(eb.PvGenCacheNone)
        factory.SetPollingTime(1000)
        factory.CreateRegister( map.GetRegisterByAddress( self.register_event_sink.FileAccessBufferAddr() ) )

    def create_file_access_offset(self, map, factory):
        factory.SetName(FILEACCESSOFFSETNAME)
        factory.SetDescription("Controls the Offset of the mapping between the device file storage and the FileAccessEventSinkBuffer.")
        factory.SetToolTip("Controls the Offset of the mapping between the device file storage and the FileAccessEventSinkBuffer.")
        factory.SetDisplayName("File Access Offset")
        factory.SetCategory(FILECATEGORYNAME)
        factory.SetVisibility(eb.PvGenVisibilityGuru)
        factory.SetStreamable(False)
        factory.SetCachable(eb.PvGenCacheNone)
        factory.SetPollingTime(1000)
        factory.SetUnit("B")
        factory.SetRepresentation(eb.PvGenRepresentationLinear)
        factory.SetPIsAvailable("FileAccessEventSinkOffsetAvailableExpr")
        factory.CreateInteger( map.GetRegisterByAddress( self.register_event_sink.FileAccessOffsetAddr() ), 0, 2**32-1 )

        # This is true for all files.
        factory.SetName("FileAccessEventSinkOffsetAvailableExpr")
        factory.AddVariable("FileOperationSelector")
        factory.CreateIntSwissKnife("(VAR_FILEOPERATIONSELECTOR = 2) || (VAR_FILEOPERATIONSELECTOR = 3)")

    def create_file_access_length(self, map, factory):
        factory.SetName(FILEACCESSLENGTHNAME)
        factory.SetDescription("Controls the Length of the mapping between the device file storage and the FileAccessEventSinkBuffer.")
        factory.SetToolTip("Controls the Length of the mapping between the device file storage and the FileAccessEventSinkBuffer.")
        factory.SetDisplayName("File Access Length")
        factory.SetCategory(FILECATEGORYNAME)
        factory.SetVisibility(eb.PvGenVisibilityGuru)
        factory.SetStreamable(False)
        factory.SetCachable(eb.PvGenCacheNone)
        factory.SetPollingTime(1000)
        factory.SetUnit("B")
        factory.SetRepresentation(eb.PvGenRepresentationLinear)
        factory.SetPIsAvailable("LengthAvailableExpr")
        factory.CreateInteger( map.GetRegisterByAddress( self.register_event_sink.FileAccessLengthAddr() ), 0, 2**32-1 )

        # Shares available expression with FileAccessEventSinkOffset.
        factory.SetName("LengthAvailableExpr")
        factory.AddVariable("FileOperationSelector")
        factory.CreateIntSwissKnife("(VAR_FILEOPERATIONSELECTOR = 2) || (VAR_FILEOPERATIONSELECTOR = 3)")

    def create_file_operation_status(self, map, factory):
        factory.SetName(FILEOPERATIONSTATUSNAME)
        factory.SetDescription("Represents the file operation execution status. " \
            "For Read or Write operations, the number of successfully read/written bytes is returned.")
        factory.SetToolTip("Represents the file operation execution status.")
        factory.SetDisplayName("File Operation Status")
        factory.SetCategory(FILECATEGORYNAME)
        factory.SetVisibility(eb.PvGenVisibilityGuru)
        factory.SetAccessMode(eb.PvGenAccessModeReadOnly)
        factory.SetStreamable(False)
        factory.SetCachable(eb.PvGenCacheNone)
        factory.SetPollingTime(1000)
        factory.AddEnumEntry( "Success", FileOperationStatus.Success )
        factory.AddEnumEntry( "Failure", FileOperationStatus.Failure )
        factory.CreateEnum( map.GetRegisterByAddress( self.register_event_sink.FileOperationStatusAddr() ) )

    def create_file_operation_result(self, map, factory):
        factory.SetName(FILEOPERATIONRESULTNAME)
        factory.SetDescription("Represents the file operation result.")
        factory.SetToolTip("Represents the file operation result.")
        factory.SetDisplayName("File Operation Result")
        factory.SetCategory(FILECATEGORYNAME)
        factory.SetVisibility(eb.PvGenVisibilityGuru)
        factory.SetAccessMode(eb.PvGenAccessModeReadOnly)
        factory.SetStreamable(False)
        factory.SetCachable(eb.PvGenCacheNone)
        factory.SetPollingTime(1000)
        factory.SetRepresentation(eb.PvGenRepresentationLinear)
        factory.CreateInteger( map.GetRegisterByAddress( self.register_event_sink.FileOperationResultAddr() ), 0, 2**32-1 )

    def create_file_size(self, map, factory):
        factory.SetName(FILESIZENAME)
        factory.SetDescription("Represents the size of the selected file in bytes.")
        factory.SetToolTip("Represents the size of the selected file in bytes.")
        factory.SetDisplayName("File Size")
        factory.SetCategory(FILECATEGORYNAME)
        factory.SetVisibility(eb.PvGenVisibilityGuru)
        factory.SetStreamable(False)
        factory.SetAccessMode(eb.PvGenAccessModeReadOnly)
        factory.SetCachable(eb.PvGenCacheNone)
        factory.SetPollingTime(1000)
        factory.SetRepresentation(eb.PvGenRepresentationLinear)
        factory.SetUnit("B")
        factory.CreateInteger( map.GetRegisterByAddress( self.register_event_sink.FileSizeAddr() ), 0, 2**32-1 )
