// *****************************************************************************
//
//     Copyright (c) 2016, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "UserSetsManager.h"

#include <assert.h>
#include <PvDeviceGEV.h>
#include <PvConfigurationWriter.h>


#define USERSET_ANSWER_TIMEOUT ( "AnswerTimeout" )
#define USERSET_ANSWER_TIMEOUT_ENTRY 10000
#define USERSET_DEFAULT_ANSWER_TIMEOUT 100
#define USERSET_COUNT_BEFORE_ENTRY 17
#define USERSET_PARAMETERS_COUNT 2
#define USERSET_SELECTOR ( "UserSetSelector" )
#define USERSET_DEFAULT ( "UserSetDefault" )
#define USERSET_DEFAULT_SELECTOR ( "UserSetDefaultSelector" )
#define USERSET_LOAD ( "UserSetLoad" )
#define USERSET_SAVE ( "UserSetSave" )


///
/// \brief Constructor.
///

UserSetsManager::UserSetsManager( PvDevice *aDevice ) 
    : mDevice( aDevice ),
    mParameters( aDevice->GetParameters() )
    , mCommunicationParameters( aDevice->GetCommunicationParameters() )
{
    assert( mDevice != NULL );

    DisableUpdates();

    // Save the initial configuration for the save dialog condition
    PvConfigurationWriter lWriter;
    lWriter.Store( mParameters );
    lWriter.SaveToString( mInitialConfiguration );
    RemoveVolatileParameters( mInitialConfiguration );

    // Add the user set names into the UserSetVector
    PvString lUserSet;
    int64_t lCount = 0;
    const PvGenEnumEntry *lEntry = NULL;
    PvGenEnum *lSelector = mParameters->GetEnum( USERSET_SELECTOR );
    if ( lSelector != NULL )
    {
        lSelector->GetEntriesCount( lCount );
    }
    for ( int i = 0; i < lCount; i++ )
    {
        lSelector->GetEntryByIndex( i, &lEntry );
        lEntry->GetName( lUserSet );
        mUserSets.push_back( lUserSet );
    }

    EnableUpdates();
}


///
/// \brief Destructor.
///

UserSetsManager::~UserSetsManager()
{
}


///
/// \brief Looks if the device configuration changed.
///

bool UserSetsManager::IsConfigurationChanged()
{
    RefreshParameters();

    PvString lConfiguration;
    PvConfigurationWriter lWriter;

    // Save the actual configuration
    lWriter.Store( mParameters );
    lWriter.SaveToString( lConfiguration );
    RemoveVolatileParameters( lConfiguration );

    return ( mInitialConfiguration != lConfiguration );
}


///
/// \brief Looks if the save command is available.
///

bool UserSetsManager::IsSaveAvailable( int aIndex )
{
    DisableUpdates();

    // Set the user set selector to the correct index
    PvString lUserSet = mUserSets.at( aIndex );
    PvGenEnum *lSelector = mParameters->GetEnum( USERSET_SELECTOR );
    if ( lSelector == NULL )
    {
        return false;
    }
    lSelector->SetValue( lUserSet );

    // Look at the availability
    PvGenParameter *lCommand = mParameters->Get( USERSET_SAVE );
    if ( lCommand == NULL )
    {
        return false;
    }
    bool lIsAvailable = lCommand->IsAvailable();

    EnableUpdates();

    return lIsAvailable;
}


///
/// \brief Looks if the load command is available.
///

bool UserSetsManager::IsLoadAvailable( int aIndex )
{
    DisableUpdates();

    // Set the user set selector to the correct index
    PvString lUserSet = mUserSets.at( aIndex );
    PvGenEnum *lSelector = mParameters->GetEnum( USERSET_SELECTOR );
    if ( lSelector == NULL )
    {
        return false;
    }
    lSelector->SetValue( lUserSet );
    
    // Look at the availability
    PvGenParameter *lCommand = mParameters->Get( USERSET_LOAD );
    if ( lCommand == NULL )
    {
        return false;
    }
    bool lIsAvailable = lCommand->IsAvailable();
    
    EnableUpdates();

    return lIsAvailable;
}


///
/// \brief Returns the user set associated to the index.
///

const PvString &UserSetsManager::GetUserSetName( int aIndex )
{
    PvString *aUserSet;
    aUserSet = &mUserSets.at( aIndex );

    return *aUserSet;
}


///
/// \brief Loads the current user set.
///

PvResult UserSetsManager::Load( int aIndex )
{
    DisableUpdates();

    // Raise the AnswerTimeout so the device isn't disconnected during the load
    int64_t lOldAnswerTimeout = USERSET_DEFAULT_ANSWER_TIMEOUT;
    PvGenInteger *lAnswerTimeout = mCommunicationParameters->GetInteger( USERSET_ANSWER_TIMEOUT );
    if ( lAnswerTimeout == NULL )
    {
        return PvResult::Code::GENERIC_ERROR;
    }
    lAnswerTimeout->GetValue( lOldAnswerTimeout );
    lAnswerTimeout->SetValue( USERSET_ANSWER_TIMEOUT_ENTRY );

    mCommunicationParameters->SetEnumValue( USERSET_ANSWER_TIMEOUT, 10000 );

    // Set the user set selector at the correct index
    PvString lUserSet = mUserSets.at( aIndex );
    PvGenEnum *lSelector = mParameters->GetEnum( USERSET_SELECTOR );
    if ( lSelector == NULL )
    {
        return PvResult::Code::GENERIC_ERROR;
    }
    lSelector->SetValue( lUserSet );

    // Load the user set
    PvGenCommand *lLoad = mParameters->GetCommand( USERSET_LOAD );
    if ( lLoad == NULL )
    {
        return PvResult::Code::GENERIC_ERROR;
    }
    PvResult lResult = lLoad->Execute();

    // Store the new parameters as the initial configuration
    ResetInitialConfiguration();

    if ( lAnswerTimeout == NULL)
    {
        return PvResult::Code::GENERIC_ERROR;
    }
    lAnswerTimeout->SetValue( lOldAnswerTimeout );
    EnableUpdates();

    return lResult;
}


///
/// \brief Saves the current user set.
///

PvResult UserSetsManager::Save( int aIndex )
{
    DisableUpdates();

    // Raise the AnswerTimeout so the device isn't disconnected during the save
    int64_t lOldAnswerTimeout = USERSET_DEFAULT_ANSWER_TIMEOUT;
    PvGenInteger *lAnswerTimeout = mCommunicationParameters->GetInteger( USERSET_ANSWER_TIMEOUT );
    if ( lAnswerTimeout == NULL )
    {
        return PvResult::Code::GENERIC_ERROR;
    }
    lAnswerTimeout->GetValue( lOldAnswerTimeout ) ;
    lAnswerTimeout->SetValue( USERSET_ANSWER_TIMEOUT_ENTRY );

    // Set the user set selector at the correct index
    PvString lUserSet = mUserSets.at( aIndex );
    PvGenEnum* lSelector = mParameters->GetEnum( USERSET_SELECTOR );
    if ( lSelector == NULL )
    {
        return PvResult::Code::GENERIC_ERROR;
    }
    lSelector->SetValue( lUserSet );

    // Save the user set
    PvGenCommand *lSave = mParameters->GetCommand( USERSET_SAVE );
    if ( lSelector == NULL )
    {
        return PvResult::Code::GENERIC_ERROR;
    }
    PvResult lResult = lSave->Execute();

    // Store the new parameters as the initial configuration
    ResetInitialConfiguration();

    if ( lAnswerTimeout == NULL )
    {
        return PvResult::Code::GENERIC_ERROR;
    }
    lAnswerTimeout->SetValue( lOldAnswerTimeout );
    EnableUpdates();

    return lResult;
}


///
/// \brief Selects the default user set.
///

PvResult UserSetsManager::SetDefault( int aIndex )
{
    PvGenEnum *lDefaultSelector = mParameters->GetEnum( USERSET_DEFAULT );
    // For older protocol versions, the parameter's name is "UserSetDefaultSelector" instead
    if ( lDefaultSelector == NULL )
    {
        lDefaultSelector = mParameters->GetEnum( USERSET_DEFAULT_SELECTOR );
    }
    PvResult lResult = lDefaultSelector->SetValue( aIndex );

    return lResult;
}


///
/// \brief Returns the default user set.
///

int UserSetsManager::GetDefaultIndex()
{
    PvString lDefault;
    PvResult lResult = mParameters->GetEnumValue( USERSET_DEFAULT, lDefault );
    // For older protocol versions, the parameter's name is "UserSetDefaultSelector" instead
    if ( lResult == PvResult::Code::NOT_FOUND )
    {
        lResult = mParameters->GetEnumValue( USERSET_DEFAULT_SELECTOR, lDefault );
        if ( lResult == PvResult::Code::NOT_FOUND )
        {
            return -1;
        }
    }

    int lIndex = 0;
    while ( mUserSets.at( lIndex ) != lDefault )
    {
        lIndex++;
    }

    return lIndex;
}


///
/// \brief Returns the number of user sets.
///

int UserSetsManager::GetCount() const
{
    int lCount = static_cast<int>( mUserSets.size() );

    return lCount;
}


///
/// \brief Refreshes the device parameters into the UserSetsManager object.
///

void UserSetsManager::RefreshParameters()
{
    // Retrieve the device parameters
    mParameters = mDevice->GetParameters();

    // Retrieve the communication controls
    mCommunicationParameters = mDevice->GetCommunicationParameters();
}


///
/// \brief Disables the parameters updates.
///

void UserSetsManager::DisableUpdates()
{
    uint32_t lCount = mParameters->GetCount();
    for ( uint32_t i = 0; i < lCount; i++ )
    {
        mParameters->Get( i )->SetUpdatesEnabled ( false );
    }
}


///
/// \brief Disables the parameters updates.
///

void UserSetsManager::EnableUpdates()
{
    uint32_t lCount = mParameters->GetCount();
    for ( uint32_t i = 0; i < lCount; i++ )
    {
        mParameters->Get( i )->SetUpdatesEnabled ( true );
    }
}


///
/// \brief Resets the initial configuration string.
///

void UserSetsManager::ResetInitialConfiguration()
{
    RefreshParameters();
    PvConfigurationWriter lWriter;
    lWriter.Store( mParameters );
    lWriter.SaveToString ( mInitialConfiguration );
    RemoveVolatileParameters( mInitialConfiguration );
}


///
/// \brief Helper function. Remove the user set parameters from a configuration PvString.
///

PvString& UserSetsManager::RemoveVolatileParameters( PvString& aConfiguration )
{
    std::string lParameterToRemove = aConfiguration.GetAscii();
    size_t lBegin = 0;
    size_t lEnd = 0;
    const char *lUserSetParameters[ USERSET_PARAMETERS_COUNT ] =
        { USERSET_SELECTOR, USERSET_DEFAULT };
    for ( int i = 0; i < USERSET_PARAMETERS_COUNT; i++ )
    {
        lBegin = lParameterToRemove.find( lUserSetParameters[ i ] );
        lEnd = lParameterToRemove.find( "\n", lBegin ) + 1;
        if ( lBegin != std::string::npos )
        {
            lBegin -= USERSET_COUNT_BEFORE_ENTRY;
            lParameterToRemove.erase( lBegin, lEnd - lBegin );
        }
    }
    aConfiguration = lParameterToRemove.c_str();

    return aConfiguration;
}

