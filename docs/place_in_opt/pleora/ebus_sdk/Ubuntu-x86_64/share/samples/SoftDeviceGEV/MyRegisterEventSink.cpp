// *****************************************************************************
//
// Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "Defines.h"
#include "MyRegisterEventSink.h"


///
/// \brief Constructor
///

MyRegisterEventSink::MyRegisterEventSink()
{

}


///
/// \brief Destructor
///

MyRegisterEventSink::~MyRegisterEventSink()
{

}


///
/// \brief Pre-read notification - usually a good place to update the register content
///

PvResult MyRegisterEventSink::PreRead( IPvRegister *aRegister )
{
    std::cout << aRegister->GetName().GetAscii() << " PreRead" << std::endl;
    return PvResult::Code::OK;
}


///
/// \brief Post-read nofitication
///

void MyRegisterEventSink::PostRead( IPvRegister *aRegister )
{
    std::cout << aRegister->GetName().GetAscii() << " PostRead" << std::endl;
}


///
/// \brief Pre-write notification - this is where a new register value is usually validated
///

PvResult MyRegisterEventSink::PreWrite( IPvRegister *aRegister )
{
    std::cout << aRegister->GetName().GetAscii() << " PreWrite" << std::endl;
    return PvResult::Code::OK;
}


///
/// \brief Post-write notification: react to a register write
///

void MyRegisterEventSink::PostWrite( IPvRegister *aRegister )
{
    // We need to reset command registers to 0 after activation for IsDone
    if ( aRegister->GetAddress() == SAMPLECOMMANDADDR )
    {
        aRegister->Write( 0 );
    }

    std::cout << aRegister->GetName().GetAscii() << " PostWrite" << std::endl;
}

