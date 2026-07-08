// *****************************************************************************
//
//     Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#ifndef EBUS_ENUM
#define EBUS_ENUM(x) typedef enum
#endif

#include <PvDeviceLib.h>

///
/// \file IPvUserSetNotify.h
/// \brief An interface to register a state change callback for UserSets

/// \enum PvUserSetState
/// \brief UserSetState enumeration type.

/// \var PvUserSetStateSaveStart
/// The device is about to start a UserSetSave

/// \var PvUserSetStateSaveCompleted 
/// The device has just completed a UserSetSave

/// \var PvUserSetStateLoadStart
/// The device is about to start a UserSetLoad

/// \var PvUserSetStateLoadCompleted 
/// The device has just completed a UserSetLoad

EBUS_ENUM(PvUserSetState)
{ 
    PvUserSetStateSaveStart, 
    PvUserSetStateSaveCompleted,
    PvUserSetStateLoadStart,
    PvUserSetStateLoadCompleted
} PvUserSetState;

///
/// \brief An interface to register a state change callback for UserSets
///
class IPvUserSetNotify
{
public:   
    virtual ~IPvUserSetNotify() {};



    /// \brief  The callback that is issued when a UserSetState is reached.
    ///         The DefaultUserSet is index 0 and the rest start from 1.
    ///
    /// \param[in] aUserSetState The UserSet state the device has entered
    /// \param[in] aUserSetIndex The numeric value of the UserSet being affected
    ///                          DefaultUserSet is 0 and the rest start from 1.
    ///
    virtual PvResult UserSetStateNotify( PvUserSetState aUserSetState, uint32_t aUserSetIndex ) = 0;
};

