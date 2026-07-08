// *****************************************************************************
//
//     Copyright (c) 2016, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __USERSETSMANAGER_H__
#define __USERSETSMANAGER_H__

#include <PvResult.h>

#include <vector>


class PvDevice;
class PvGenParameterArray;


typedef std::vector<PvString> UserSetVector;


class UserSetsManager
{
public:

    UserSetsManager( PvDevice *aDevice );
    virtual ~UserSetsManager();

    bool IsConfigurationChanged();

    int GetCount() const;
    bool IsLoadAvailable( int aIndex );
    bool IsSaveAvailable( int aIndex );

    const PvString &GetUserSetName( int aIndex );
    
    PvResult Load( int aIndex );
    PvResult Save( int aIndex );
    
    PvResult SetDefault( int aIndex );
    int GetDefaultIndex();

protected:

    void RefreshParameters();
    void DisableUpdates();
    void EnableUpdates();
    void ResetInitialConfiguration();
    PvString &RemoveVolatileParameters( PvString &aConfiguration );

private:

    // We do not own these pointers
    PvDevice *mDevice;
    PvGenParameterArray *mParameters;
    PvGenParameterArray *mCommunicationParameters;

    PvString mInitialConfiguration;
    UserSetVector mUserSets;

};


#endif // __USERSETSMANAGER_H__

