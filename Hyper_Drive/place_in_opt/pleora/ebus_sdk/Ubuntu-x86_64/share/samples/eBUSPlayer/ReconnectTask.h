// *****************************************************************************
//
//     Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __RECONNECT_TASK_H__
#define __RECONNECT_TASK_H__

#include "Task.h"
#include "Player.h"

#include <assert.h>


class ReconnectTask : public Task
{
#ifdef _AFXDLL
    DECLARE_DYNAMIC( ReconnectTask )
#endif // _AFXDLL

public:

    ReconnectTask( Player *aPlayer, const PvString &aConnectionID )
        : mPlayer( aPlayer )
        , mConnectionID( aConnectionID )
    {
        assert( mPlayer != NULL );
    }

    virtual void Execute()
    {
        mResult = mPlayer->ReconnectTaskHandler( GetProgress(), mConnectionID );
    }

private:

    Player *mPlayer;

    PvString mConnectionID;

};


#endif // __RECONNECT_TASK_H__

