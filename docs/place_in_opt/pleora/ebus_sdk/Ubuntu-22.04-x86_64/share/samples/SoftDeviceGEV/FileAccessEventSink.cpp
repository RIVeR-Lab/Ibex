// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "FileAccessEventSink.h"

#include <limits>
#include <sstream>


static const char *sFileCategoryName = "FileAccessControl";
static const char *sFileSelectorName = "FileSelector";
static const char *sFileOperationSelectorName = "FileOperationSelector";
static const char *sFileOperationExecuteName = "FileOperationExecute";
static const char *sFileOpenModeName = "FileOpenMode";
static const char *sFileAccessBufferName = "FileAccessBuffer";
static const char *sFileAccessOffsetName = "FileAccessOffset";
static const char *sFileAccessLengthName = "FileAccessLength";
static const char *sFileOperationStatusName = "FileOperationStatus";
static const char *sFileOperationResultName = "FileOperationResult";
static const char *sFileSizeName = "FileSize";


///
/// \brief Callback used to initiate custom registers creation
///

void FileAccessEventSink::OnCreateCustomRegisters( IPvSoftDeviceGEV * const aDevice, IPvRegisterFactory * const aFactory )
{
    PVUNREFPARAM( aDevice );

    if ( mRegisterEventSink->cbegin() == mRegisterEventSink->cend() )
    {
        return;
    }

    aFactory->AddRegister( sFileSelectorName, mRegisterEventSink->FileSelectorAddr(), 4, PvGenAccessModeReadWrite, mRegisterEventSink.get() );
    aFactory->AddRegister( sFileOperationSelectorName, mRegisterEventSink->FileOperationSelectorAddr(), 4, PvGenAccessModeReadWrite, mRegisterEventSink.get() );
    aFactory->AddRegister( sFileOperationExecuteName, mRegisterEventSink->FileOperationExecuteAddr(), 4, PvGenAccessModeReadWrite, mRegisterEventSink.get() );
    aFactory->AddRegister( sFileOpenModeName, mRegisterEventSink->FileOpenModeAddr(), 4, PvGenAccessModeReadWrite, mRegisterEventSink.get() );
    aFactory->AddByteArray( sFileAccessBufferName, mRegisterEventSink->FileAccessBufferAddr(), mRegisterEventSink->FileAccessBufferSize(), PvGenAccessModeReadWrite, mRegisterEventSink.get() );
    aFactory->AddRegister( sFileAccessOffsetName, mRegisterEventSink->FileAccessOffsetAddr(), 4, PvGenAccessModeReadWrite, mRegisterEventSink.get() );
    aFactory->AddRegister( sFileAccessLengthName, mRegisterEventSink->FileAccessLengthAddr(), 4, PvGenAccessModeReadWrite, mRegisterEventSink.get() );
    aFactory->AddRegister( sFileOperationStatusName, mRegisterEventSink->FileOperationStatusAddr(), 4, PvGenAccessModeReadOnly, mRegisterEventSink.get() );
    aFactory->AddRegister( sFileOperationResultName, mRegisterEventSink->FileOperationResultAddr(), 4, PvGenAccessModeReadOnly, mRegisterEventSink.get() );
    aFactory->AddRegister( sFileSizeName, mRegisterEventSink->FileSizeAddr(), 4, PvGenAccessModeReadOnly, mRegisterEventSink.get() );
 }


///
/// \brief Callback used to initiate GenApi features creation
///

void FileAccessEventSink::OnCreateCustomGenApiFeatures( IPvSoftDeviceGEV * const aDevice, IPvGenApiFactory * const aFactory )
{
    if ( mRegisterEventSink->cbegin() == mRegisterEventSink->cend() )
    {
        return;
    }
    
    auto const lRegisterMap = aDevice->GetRegisterMap();
    AddFileSelector( lRegisterMap, aFactory );
    AddFileOperationSelector( lRegisterMap, aFactory );
    AddFileOperationExecute( lRegisterMap, aFactory );
    AddFileOpenMode( lRegisterMap, aFactory );
    AddFileAccessBuffer( lRegisterMap, aFactory );
    AddFileAccessOffset( lRegisterMap, aFactory );
    AddFileOperationStatus( lRegisterMap, aFactory );
    AddFileOperationResult( lRegisterMap, aFactory );
    AddFileSize( lRegisterMap, aFactory );
}


///
/// Helper functions.
///

void FileAccessEventSink::AddFileSelector( IPvRegisterMap * const aMap, IPvGenApiFactory * const aFactory )
{
    aFactory->SetName( sFileSelectorName );
    static const auto lFileSelectorTooltip = "Selects the target file in the device.";
    aFactory->SetDescription( lFileSelectorTooltip );
    aFactory->SetToolTip( lFileSelectorTooltip );
    aFactory->SetDisplayName( "File Selector" );
    aFactory->SetCategory( sFileCategoryName );
    aFactory->SetVisibility( PvGenVisibilityGuru );
    aFactory->SetStreamable( false );
    aFactory->AddSelected( sFileAccessLengthName );
    aFactory->AddSelected( sFileAccessOffsetName );
    aFactory->AddSelected( sFileOpenModeName );
    aFactory->AddSelected( sFileOperationExecuteName );
    aFactory->AddSelected( sFileOperationResultName );
    aFactory->AddSelected( sFileOperationSelectorName );
    aFactory->AddSelected( sFileOperationStatusName );
    aFactory->AddSelected( sFileSizeName );

    auto lEnumIx = 0;
    for ( auto lIt = mRegisterEventSink->cbegin(); lIt != mRegisterEventSink->cend(); ++lIt )
    {
        aFactory->AddEnumEntry( ( *lIt )->DisplayName().c_str(), lEnumIx );
        ++lEnumIx;
    }

    aFactory->CreateEnum( aMap->GetRegisterByAddress( mRegisterEventSink->FileSelectorAddr() ) );
}


void FileAccessEventSink::AddFileOperationSelector( IPvRegisterMap * const aMap, IPvGenApiFactory * const aFactory )
{
    aFactory->SetName( sFileOperationSelectorName );
    aFactory->SetDescription( 
        "Selects the target operation for the selected file in the device. "
        "This Operation is executed when the FileOperationExecute feature is called." );
    static const auto lFileOperationTooltip = "Selects the target operation for the selected file in the device.";
    aFactory->SetToolTip( lFileOperationTooltip );
    aFactory->SetDisplayName( "File Operation Selector" );
    aFactory->SetCategory( sFileCategoryName );
    aFactory->SetVisibility( PvGenVisibilityGuru );
    aFactory->SetStreamable( false );
    aFactory->AddSelected( sFileAccessLengthName );
    aFactory->AddSelected( sFileAccessOffsetName );
    aFactory->AddSelected( sFileOperationExecuteName );
    aFactory->AddSelected( sFileOperationResultName );
    aFactory->AddSelected( sFileOperationStatusName );
    aFactory->AddEnumEntry( "Open", static_cast<uint32_t>( IFileAccessFile::FOS_OPEN ) );
    aFactory->AddEnumEntry( "Close", static_cast<uint32_t>( IFileAccessFile::FOS_CLOSE ) );
    aFactory->AddEnumEntry( "Read", static_cast<uint32_t>( IFileAccessFile::FOS_READ ) );
    aFactory->AddEnumEntry( "Write", static_cast<uint32_t>( IFileAccessFile::FOS_WRITE ) );
    aFactory->AddEnumEntry( "Delete", static_cast<uint32_t>( IFileAccessFile::FOS_DELETE ) );
    aFactory->CreateEnum( aMap->GetRegisterByAddress( mRegisterEventSink->FileOperationSelectorAddr() ) );

    // Parse file list and create available expressions for Read, Write.
    {
        auto lFileIx = 0;
        aFactory->SetName( "FileOperationSelectorReadAvailableExpr" );
        aFactory->AddVariable( "FileSelector" );
        std::stringstream lSK;
        lSK << "0";
        for ( auto lIt = mRegisterEventSink->cbegin(); lIt != mRegisterEventSink->cend(); ++lIt )
        {
            if ( ( ( *lIt )->AccessMode() == PvGenAccessModeReadOnly ) || ( ( *lIt )->AccessMode() == PvGenAccessModeReadWrite ) )
            {
                lSK << " || (VAR_FILESELECTOR = " << lFileIx << ")";
            }
            ++lFileIx;
        }

        aFactory->CreateIntSwissKnife( lSK.str().c_str() );
        aFactory->SetPIsAvailableForEnumEntry( sFileOperationSelectorName, "Read", "FileOperationSelectorReadAvailableExpr" );
    }

    {
        auto lFileIx = 0;
        aFactory->SetName( "FileOperationSelectorWriteAvailableExpr" );
        aFactory->AddVariable( "FileSelector" );
        std::stringstream lSK;
        lSK << "0";
        for ( auto lIt = mRegisterEventSink->cbegin(); lIt != mRegisterEventSink->cend(); ++lIt )
        {
            if ( ( ( *lIt )->AccessMode() == PvGenAccessModeWriteOnly ) || ( ( *lIt )->AccessMode() == PvGenAccessModeReadWrite ) )
            {
                lSK << " || (VAR_FILESELECTOR = " << lFileIx << ")";
            }
            ++lFileIx;
        }

        aFactory->CreateIntSwissKnife( lSK.str().c_str() );
        aFactory->SetPIsAvailableForEnumEntry( sFileOperationSelectorName, "Write", "FileOperationSelectorWriteAvailableExpr" );
    }

    {
        auto lFileIx = 0;
        aFactory->SetName( "FileOperationSelectorDeleteAvailableExpr" );
        aFactory->AddVariable( "FileSelector" );
        std::stringstream lSK;
        lSK << "0";
        for ( auto lIt = mRegisterEventSink->cbegin(); lIt != mRegisterEventSink->cend(); ++lIt )
        {
            if ( ( ( *lIt )->AccessMode() == PvGenAccessModeWriteOnly ) || ( ( *lIt )->AccessMode() == PvGenAccessModeReadWrite ) )
            {
                lSK << " || (VAR_FILESELECTOR = " << lFileIx << ")";
            }
            ++lFileIx;
        }

        aFactory->CreateIntSwissKnife( lSK.str().c_str() );
        aFactory->SetPIsAvailableForEnumEntry( sFileOperationSelectorName, "Delete", "FileOperationSelectorDeleteAvailableExpr" );
    }
}


void FileAccessEventSink::AddFileOperationExecute(IPvRegisterMap * const aMap, IPvGenApiFactory * const aFactory)
{
    aFactory->SetName( sFileOperationExecuteName );
    static const auto lFileOperationExecuteTooltip =
        "Executes the operation selected by FileOperationSelector on the selected file.";
    aFactory->SetDescription( lFileOperationExecuteTooltip );
    aFactory->SetToolTip( lFileOperationExecuteTooltip );
    aFactory->SetDisplayName( "File Operation Execute" );
    aFactory->SetCategory( sFileCategoryName );
    aFactory->SetVisibility( PvGenVisibilityGuru );
    aFactory->SetStreamable( false );
    aFactory->SetAccessMode( PvGenAccessModeWriteOnly );
    aFactory->SetCachable( PvGenCacheWriteThrough );
    aFactory->CreateCommand( aMap->GetRegisterByAddress( mRegisterEventSink->FileOperationExecuteAddr() ) );
}


void FileAccessEventSink::AddFileOpenMode( IPvRegisterMap * const aMap, IPvGenApiFactory * const aFactory )
{
    aFactory->SetName( sFileOpenModeName );
    static const auto lFileOpenModeTooltip = "Selects the access mode in which a file is opened in the device.";
    aFactory->SetDescription( lFileOpenModeTooltip );
    aFactory->SetToolTip( lFileOpenModeTooltip );
    aFactory->SetDisplayName( "File Open Mode" );
    aFactory->SetCategory( sFileCategoryName );
    aFactory->SetVisibility( PvGenVisibilityGuru );
    aFactory->SetStreamable( false );
    aFactory->AddInvalidator( "FileSelectorReg" );
    aFactory->AddEnumEntry( "Read", static_cast<uint32_t>( IFileAccessFile::FOM_READ ) );
    aFactory->AddEnumEntry( "Write", static_cast<uint32_t>( IFileAccessFile::FOM_WRITE ) );
    aFactory->AddEnumEntry( "ReadWrite", static_cast<uint32_t>( IFileAccessFile::FOM_READ_WRITE ) );
    aFactory->CreateEnum( aMap->GetRegisterByAddress( mRegisterEventSink->FileOpenModeAddr() ) );

    // Parse file list and create available expressions for Read, Write, ReadWrite.
    {
        auto lFileIx = 0;
        aFactory->SetName( "FileOpenModeReadAvailableExpr" );
        aFactory->AddVariable( "FileSelector" );
        std::stringstream lSK;
        lSK << "0";
        for ( auto lIt = mRegisterEventSink->cbegin(); lIt != mRegisterEventSink->cend(); ++lIt )
        {
            if ( ( ( *lIt )->AccessMode() == PvGenAccessModeReadOnly ) || ( ( *lIt )->AccessMode() == PvGenAccessModeReadWrite ) )
            {
                lSK << " || (VAR_FILESELECTOR = " << lFileIx << ")";
            }
            ++lFileIx;
        }

        aFactory->CreateIntSwissKnife( lSK.str().c_str() );
        aFactory->SetPIsAvailableForEnumEntry( sFileOpenModeName, "Read", "FileOpenModeReadAvailableExpr" );
    }

    {
        auto lFileIx = 0;
        aFactory->SetName( "FileOpenModeWriteAvailableExpr" );
        aFactory->AddVariable( "FileSelector" );
        std::stringstream lSK;
        lSK << "0";
        for ( auto lIt = mRegisterEventSink->cbegin(); lIt != mRegisterEventSink->cend(); ++lIt )
        {
            if ( ( ( *lIt )->AccessMode() == PvGenAccessModeWriteOnly ) || ( ( *lIt )->AccessMode() == PvGenAccessModeReadWrite ) )
            {
                lSK << " || (VAR_FILESELECTOR = " << lFileIx << ")";
            }
            ++lFileIx;
        }

        aFactory->CreateIntSwissKnife( lSK.str().c_str() );
        aFactory->SetPIsAvailableForEnumEntry( sFileOpenModeName, "Write", "FileOpenModeWriteAvailableExpr" );
    }

    {
        auto lFileIx = 0;
        aFactory->SetName( "FileOpenModeReadWriteAvailableExpr" );
        aFactory->AddVariable( "FileSelector" );
        std::stringstream lSK;
        lSK << "0";
        for ( auto lIt = mRegisterEventSink->cbegin(); lIt != mRegisterEventSink->cend(); ++lIt )
        {
            if ( ( *lIt )->AccessMode() == PvGenAccessModeReadWrite )
            {
                lSK << " || (VAR_FILESELECTOR = " << lFileIx << ")";
            }

            ++lFileIx;
        }

        aFactory->CreateIntSwissKnife( lSK.str().c_str() );
        aFactory->SetPIsAvailableForEnumEntry( sFileOpenModeName, "ReadWrite", "FileOpenModeReadWriteAvailableExpr" );
    }
}


void FileAccessEventSink::AddFileAccessBuffer( IPvRegisterMap * const aMap, IPvGenApiFactory * const aFactory )
{
    aFactory->SetName( sFileAccessBufferName );
    static const auto lFileAccessEventSinkBufferTooltip =
        "Defines the intermediate access buffer that allows the exchange of data between the device file storage and the application.";
    aFactory->SetDescription( lFileAccessEventSinkBufferTooltip );
    aFactory->SetToolTip( lFileAccessEventSinkBufferTooltip );
    aFactory->SetDisplayName( "File Access Buffer" );
    aFactory->SetCategory( sFileCategoryName );
    aFactory->SetVisibility( PvGenVisibilityGuru );
    aFactory->SetStreamable( false );
    aFactory->SetCachable( PvGenCacheNone );
    aFactory->SetPollingTime( 1000 );
    // IRegister and IString have the same throughput over the link.
    aFactory->CreateRegister( aMap->GetRegisterByAddress( mRegisterEventSink->FileAccessBufferAddr() ) );
    //aFactory->CreateString( aMap->GetRegisterByAddress( mRegisterEventSink->FileAccessBufferAddr() ) );
}


void FileAccessEventSink::AddFileAccessOffset( IPvRegisterMap * const aMap, IPvGenApiFactory * const aFactory )
{
    aFactory->SetName( sFileAccessOffsetName );
    static const auto lFileAccessEventSinkOffsetTooltip =
        "Controls the Offset of the mapping between the device file storage and the FileAccessEventSinkBuffer.";
    aFactory->SetDescription( lFileAccessEventSinkOffsetTooltip );
    aFactory->SetToolTip( lFileAccessEventSinkOffsetTooltip );
    aFactory->SetDisplayName( "File Access Offset" );
    aFactory->SetCategory( sFileCategoryName );
    aFactory->SetVisibility( PvGenVisibilityGuru );
    aFactory->SetStreamable( false );
    aFactory->SetCachable( PvGenCacheNone );
    aFactory->SetPollingTime( 1000 );
    aFactory->SetUnit( "B" );
    aFactory->SetRepresentation( PvGenRepresentationLinear );
    aFactory->SetPIsAvailable( "FileAccessEventSinkOffsetAvailableExpr" );
    aFactory->CreateInteger( aMap->GetRegisterByAddress( mRegisterEventSink->FileAccessOffsetAddr() ), INT64_C( 0 ), std::numeric_limits<int64_t>::max() );

    // This is true for all files.
    aFactory->SetName( "FileAccessEventSinkOffsetAvailableExpr" );
    aFactory->AddVariable( "FileOperationSelector" );
    std::stringstream lSK;
    lSK << "(VAR_FILEOPERATIONSELECTOR = 2) || (VAR_FILEOPERATIONSELECTOR = 3)";
    aFactory->CreateIntSwissKnife( lSK.str().c_str() );

    aFactory->SetName( sFileAccessLengthName );
    static const auto lFileAccessEventSinkLengthTooltip =
        "Controls the Length of the mapping between the device file storage and the FileAccessEventSinkBuffer.";
    aFactory->SetDescription( lFileAccessEventSinkLengthTooltip );
    aFactory->SetToolTip( lFileAccessEventSinkLengthTooltip );
    aFactory->SetDisplayName( "File Access Length" );
    aFactory->SetCategory( sFileCategoryName );
    aFactory->SetVisibility( PvGenVisibilityGuru );
    aFactory->SetStreamable( false );
    aFactory->SetCachable( PvGenCacheNone );
    aFactory->SetPollingTime( 1000 );
    aFactory->SetUnit( "B" );
    aFactory->SetRepresentation( PvGenRepresentationLinear );
    aFactory->SetPIsAvailable( "LengthAvailableExpr" );
    aFactory->CreateInteger( aMap->GetRegisterByAddress( mRegisterEventSink->FileAccessLengthAddr() ), INT64_C( 0 ), std::numeric_limits<int64_t>::max() );

    // Shares available expression with FileAccessEventSinkOffset.
    aFactory->SetName( "LengthAvailableExpr" );
    aFactory->AddVariable( "FileOperationSelector" );
    aFactory->CreateIntSwissKnife( lSK.str().c_str() );
}


void FileAccessEventSink::AddFileOperationStatus( IPvRegisterMap * const aMap, IPvGenApiFactory * const aFactory )
{
    aFactory->SetName( sFileOperationStatusName );
    aFactory->SetDescription( "Represents the file operation execution status. "
        "For Read or Write operations, the number of successfully read/written bytes is returned." );
    aFactory->SetToolTip( "Represents the file operation execution status." );
    aFactory->SetDisplayName( "File Operation Status" );
    aFactory->SetCategory( sFileCategoryName );
    aFactory->SetVisibility( PvGenVisibilityGuru );
    aFactory->SetAccessMode( PvGenAccessModeReadOnly );
    aFactory->SetStreamable( false );
    aFactory->SetCachable( PvGenCacheNone );
    aFactory->SetPollingTime( 1000 );
    aFactory->AddEnumEntry( "Success", static_cast<uint32_t>( IFileAccessFile::Success ) );
    aFactory->AddEnumEntry( "Failure", static_cast<uint32_t>( IFileAccessFile::Failure ) );
    aFactory->CreateEnum( aMap->GetRegisterByAddress( mRegisterEventSink->FileOperationStatusAddr() ) );
}


void FileAccessEventSink::AddFileOperationResult( IPvRegisterMap * const aMap, IPvGenApiFactory * const aFactory )
{
    aFactory->SetName( sFileOperationResultName );
    static const auto lFileOperationResultTooltip = "Represents the file operation result.";
    aFactory->SetDescription( lFileOperationResultTooltip );
    aFactory->SetToolTip( lFileOperationResultTooltip );
    aFactory->SetDisplayName( "File Operation Result" );
    aFactory->SetCategory( sFileCategoryName );
    aFactory->SetVisibility( PvGenVisibilityGuru );
    aFactory->SetAccessMode( PvGenAccessModeReadOnly );
    aFactory->SetStreamable( false );
    aFactory->SetCachable( PvGenCacheNone );
    aFactory->SetPollingTime( 1000 );
    aFactory->SetRepresentation( PvGenRepresentationLinear );
    aFactory->CreateInteger( aMap->GetRegisterByAddress( mRegisterEventSink->FileOperationResultAddr() ), INT64_C( 0 ), std::numeric_limits<int64_t>::max() );
}


void FileAccessEventSink::AddFileSize( IPvRegisterMap * const aMap, IPvGenApiFactory * const aFactory )
{
    aFactory->SetName( sFileSizeName );
    static const auto lFileSizeTooltip = "Represents the size of the selected file in bytes.";
    aFactory->SetDescription( lFileSizeTooltip );
    aFactory->SetToolTip( lFileSizeTooltip );
    aFactory->SetDisplayName( "File Size" );
    aFactory->SetCategory( sFileCategoryName );
    aFactory->SetVisibility( PvGenVisibilityGuru );
    aFactory->SetStreamable( false );
    aFactory->SetAccessMode( PvGenAccessModeReadOnly );
    aFactory->SetCachable( PvGenCacheNone );
    aFactory->SetPollingTime( 1000 );
    aFactory->SetRepresentation( PvGenRepresentationLinear );
    aFactory->SetUnit( "B" );
    aFactory->CreateInteger( aMap->GetRegisterByAddress( mRegisterEventSink->FileSizeAddr() ), INT64_C( 0 ), std::numeric_limits<int64_t>::max() );
}
