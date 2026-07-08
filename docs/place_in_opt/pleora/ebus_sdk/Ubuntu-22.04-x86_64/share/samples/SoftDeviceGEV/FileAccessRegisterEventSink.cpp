// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "Defines.h"
#include "FileAccessRegisterEventSink.h"

#include <array>
#include <iostream>
#include <fstream>


std::array<uint8_t, FILE_BUFFER_SIZE> IFileAccessFile::sTempBuf;


///
/// \brief Pre-read notification - usually a good place to update the register content
///

PvResult FileAccessRegisterEventSink::PreRead( IPvRegister * const aRegister )
{
    uint32_t const lFileSelector = ReadFileSelector();
    uint32_t const lFileOperationSelector = ReadFileOperationSelector();
    uint32_t const lAddress = aRegister->GetAddress();
    if ( lAddress == FileAccessOffsetAddr() )
    {
        // FileAccessOffset[FileSelector][FileOperationSelector]
        // => write back the value of the currently selected file.
        auto const lFileAccessOffset = mFiles[ lFileSelector ]->Offset( lFileOperationSelector );
        aRegister->Write( lFileAccessOffset );
    }
    else if ( lAddress == FileAccessLengthAddr() )
    {
        // FileAccessLength[FileSelector][FileOperationSelector]
        // => write back the value of the currently selected file.
        auto const lFileAccessLength = mFiles[ lFileSelector ]->Length( lFileOperationSelector );
        aRegister->Write( lFileAccessLength );
    }
    else if ( lAddress == FileOperationStatusAddr() )
    {
        // FileAccessLength[FileSelector][FileOperationSelector]
        // => write back the value of the currently selected file.
        auto const lFileOperationStatus = mFiles[ lFileSelector ]->OperationStatus( lFileOperationSelector );
        aRegister->Write( lFileOperationStatus );
    }
    else if ( lAddress == FileOperationResultAddr() )
    {
        // FileAccessLength[FileSelector][FileOperationSelector]
        // => write back the value of the currently selected file.
        auto const lFileOperationResult = mFiles[ lFileSelector ]->OperationResult( lFileOperationSelector );
        aRegister->Write( lFileOperationResult );
    }
    else if ( lAddress == FileSizeAddr() )
    {
        // FileSize[FileSelector]
        auto const lFileSize = mFiles[ lFileSelector ]->Size();
        aRegister->Write( lFileSize );
    }

    return PvResult::Code::OK;
}


///
/// \brief Post-read nofitication
///

void FileAccessRegisterEventSink::PostRead( IPvRegister * const aRegister )
{
    PVUNREFPARAM( aRegister );
}


///
/// \brief Pre-write notification - this is where a new register value is usually validated
///

PvResult FileAccessRegisterEventSink::PreWrite( IPvRegister * const aRegister )
{
    PVUNREFPARAM( aRegister );
    return PvResult::Code::OK;
}


///
/// \brief Post-write notification: react to a register write
///

void FileAccessRegisterEventSink::PostWrite( IPvRegister * const aRegister )
{
    IFileAccessFile::FileOperationStatus lFileOperationStatus = IFileAccessFile::Success;
    uint32_t lFileOperationResult = false;

    uint32_t const lFileSelector = ReadFileSelector();
    uint32_t const lAddress = aRegister->GetAddress();
    if ( lAddress == FileSelectorAddr() )
    {
        uint32_t const lFileOpenMode = ReadFileOpenMode();
        auto const lAccessMode = mFiles[ lFileSelector ]->AccessMode();
        if ( ( lAccessMode == PvGenAccessModeReadOnly )
            && ( ( lFileOpenMode == static_cast<uint32_t>( IFileAccessFile::FOM_WRITE ) )
                || ( lFileOpenMode == static_cast<uint32_t>( IFileAccessFile::FOM_READ_WRITE ) ) ) )
        {
            // RO file.
            WriteFileOpenMode( IFileAccessFile::FOM_READ );
        }
        else if ( ( lAccessMode == PvGenAccessModeWriteOnly )
            && ( ( lFileOpenMode == static_cast<uint32_t>( IFileAccessFile::FOM_READ ) )
                || ( lFileOpenMode == static_cast<uint32_t>( IFileAccessFile::FOM_READ_WRITE ) ) ) )
        {
            // WO file.
            WriteFileOpenMode( IFileAccessFile::FOM_WRITE );
        }

        // ReadWrite files can handle any FileOpenMode.
    }
    else if ( lAddress == FileOperationExecuteAddr() )
    {
        // Perform the selected operation.
        uint32_t const lFileOperationSelector = ReadFileOperationSelector();
        if ( static_cast<uint32_t>( IFileAccessFile::FOS_OPEN ) == lFileOperationSelector )
        {
            // Open selected file.
            uint32_t const lFileOpenMode = ReadFileOpenMode();
            auto const lResult = mFiles[ lFileSelector ]->Open( static_cast<IFileAccessFile::FileOpenMode>( lFileOpenMode ) );
            lFileOperationStatus = lResult.first;
            lFileOperationResult = lResult.second;

            if ( lFileOperationStatus == IFileAccessFile::Success )
            {
                auto const lFileSize = mFiles[ lFileSelector ]->Size();
                WriteFileSize( lFileSize );
            }
        }
        else if ( static_cast<uint32_t>( IFileAccessFile::FOS_CLOSE ) == lFileOperationSelector )
        {
            // Close selected file.
            auto const lResult = mFiles[ lFileSelector ]->Close();
            lFileOperationStatus = lResult.first;
            lFileOperationResult = lResult.second;
        }
        else if ( static_cast<uint32_t>( IFileAccessFile::FOS_READ ) == lFileOperationSelector )
        {
            uint32_t const lFileAccessLength = ReadFileAccesLength();
            uint32_t const lFileAccessOffset = ReadFileAccessOffset();
            auto const lFileAccessBufferReg = mRegisterMap->GetRegisterByAddress( FileAccessBufferAddr() );

            auto const lResult = mFiles[ lFileSelector ]->Read( lFileAccessLength, lFileAccessOffset, lFileAccessBufferReg );
            lFileOperationStatus = lResult.first;
            lFileOperationResult = lResult.second;
        }
        else if ( static_cast<uint32_t>( IFileAccessFile::FOS_WRITE ) == lFileOperationSelector )
        {
            uint32_t const lFileAccessLength = ReadFileAccesLength();
            uint32_t const lFileAccessOffset = ReadFileAccessOffset();
            auto const lFileAccessBufferReg = mRegisterMap->GetRegisterByAddress( FileAccessBufferAddr() );

            auto const lResult = mFiles[ lFileSelector ]->Write( lFileAccessLength, lFileAccessOffset, lFileAccessBufferReg );
            lFileOperationStatus = lResult.first;
            lFileOperationResult = lResult.second;

            if ( lFileOperationStatus == IFileAccessFile::Success )
            {
                // Update FileSize
                uint32_t const lFileSize = ReadFileSize();
                WriteFileSize( lFileSize + lFileOperationResult );
            }
        }
        else if ( static_cast<uint32_t>( IFileAccessFile::FOS_DELETE ) == lFileOperationSelector )
        {
            auto const lResult = mFiles[ lFileSelector ]->Delete();
            lFileOperationStatus = lResult.first;
            lFileOperationResult = lResult.second;
        }

        // Save those in file, as per selectors.
        // FileOperationStatus[FileSelector][FileOperationSelector]
        // FileOperationResult[FileSelector][FileOperationSelector]
        mFiles[ lFileSelector ]->SetOperationStatus( lFileOperationSelector, static_cast<uint32_t>( lFileOperationStatus ) );
        mFiles[ lFileSelector ]->SetOperationResult( lFileOperationSelector, lFileOperationResult );
    }
    else if ( lAddress == FileAccessOffsetAddr() )
    {
        // Save value in file as per current selectors.
        // FileAccessOffset[FileSelector][FileOperationSelector]
        uint32_t const lFileOperationSelector = ReadFileOperationSelector();
        uint32_t lFileAccessOffset = 0;
        aRegister->Read( lFileAccessOffset );
        mFiles[ lFileSelector ]->SetOffset( lFileOperationSelector, lFileAccessOffset );
    }
    else if ( lAddress == FileAccessLengthAddr() )
    {
        // Save value in file as per current selectors.
        // FileAccessLength[FileSelector][FileOperationSelector]
        uint32_t const lFileOperationSelector = ReadFileOperationSelector();
        uint32_t lFileAccessLength = 0;
        aRegister->Read( lFileAccessLength );
        mFiles[ lFileSelector ]->SetLength( lFileOperationSelector, lFileAccessLength );
    }
}


///
/// Add a file to the list.
///

void FileAccessRegisterEventSink::AddFile( file_ptr aFile )
{
    mFiles.push_back( std::move( aFile ) );
}


///
/// Helper functions.
///

uint32_t FileAccessRegisterEventSink::ReadFileSelector() const
{
    uint32_t lFileSelector = 0;
    mRegisterMap->GetRegisterByAddress( FileSelectorAddr() )->Read( lFileSelector );
    return lFileSelector;
}


uint32_t FileAccessRegisterEventSink::ReadFileAccesLength() const
{
    uint32_t lFileAccessLength = 0;
    mRegisterMap->GetRegisterByAddress( FileAccessLengthAddr() )->Read( lFileAccessLength );
    return lFileAccessLength;
}


uint32_t FileAccessRegisterEventSink::ReadFileAccessOffset() const
{
    uint32_t lFileAccessOffset = 0;
    mRegisterMap->GetRegisterByAddress( FileAccessOffsetAddr() )->Read( lFileAccessOffset );
    return lFileAccessOffset;
}


uint32_t FileAccessRegisterEventSink::ReadFileOpenMode() const
{
    uint32_t lFileOpenMode = 0;
    mRegisterMap->GetRegisterByAddress( FileOpenModeAddr() )->Read( lFileOpenMode );
    return lFileOpenMode;
}


uint32_t FileAccessRegisterEventSink::ReadFileOperationSelector() const
{
    uint32_t lFileOperationSelector = 0;
    mRegisterMap->GetRegisterByAddress( FileOperationSelectorAddr() )->Read( lFileOperationSelector );
    return lFileOperationSelector;
}


uint32_t FileAccessRegisterEventSink::ReadFileSize() const
{
    uint32_t lFileSize = 0;
    mRegisterMap->GetRegisterByAddress( FileSizeAddr() )->Read( lFileSize );
    return lFileSize;
}


void FileAccessRegisterEventSink::WriteFileOpenMode(IFileAccessFile::FileOpenMode const aFileOpenMode ) const
{
   mRegisterMap->GetRegisterByAddress( FileOpenModeAddr() )->Write( static_cast<uint32_t>( aFileOpenMode ) );
}


void FileAccessRegisterEventSink::WriteFileOperationStatus( IFileAccessFile::FileOperationStatus const aFileOperationStatus ) const
{

    mRegisterMap->GetRegisterByAddress( FileOperationStatusAddr() )->Write( static_cast<uint32_t>( aFileOperationStatus ) );
}


void FileAccessRegisterEventSink::WriteFileOperationResult( uint32_t const aResult ) const
{
    mRegisterMap->GetRegisterByAddress( FileOperationResultAddr() )->Write( aResult );
}


void FileAccessRegisterEventSink::WriteFileSize( uint32_t const aFileSize ) const
{
    mRegisterMap->GetRegisterByAddress( FileSizeAddr() )->Write( aFileSize );
}
