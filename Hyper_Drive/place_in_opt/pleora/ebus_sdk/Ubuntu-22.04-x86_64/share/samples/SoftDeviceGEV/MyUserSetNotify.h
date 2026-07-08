// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvSoftDeviceGEVInterfaces.h>
#include <IPvUserSetNotify.h>
#include <PvVirtualDeviceLib.h>


///
/// \brief callback class for UserSet state changes
///
class MyUserSetNotify
    : public IPvUserSetNotify
{
public:

    MyUserSetNotify() {};
    virtual ~MyUserSetNotify() {};

    PvResult UserSetStateNotify( PvUserSetState aUserSetState, uint32_t aUserSetIndex );

private:


};
