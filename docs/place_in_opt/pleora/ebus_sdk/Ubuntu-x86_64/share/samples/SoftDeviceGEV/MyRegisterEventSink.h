// *****************************************************************************
//
// Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvSoftDeviceGEVInterfaces.h>


///
/// \brief Register event sink: used to demonstrate how to handle register read/write events
///

class MyRegisterEventSink
    : public IPvRegisterEventSink
{
public:

    MyRegisterEventSink();
    virtual ~MyRegisterEventSink();

    // IPvRegisterEventSink implementation
    PvResult PreRead( IPvRegister *aRegister );
    void PostRead( IPvRegister *aRegister );
    PvResult PreWrite( IPvRegister *aRegister );
    void PostWrite( IPvRegister *aRegister );

};

