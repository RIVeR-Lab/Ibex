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
    mSampleEnum = 0;
    mSampleStrings.assign( SAMPLEENUMCOUNT, "" );
    mSampleBooleans.assign( SAMPLEENUMCOUNT, 0 );
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
    if ( aRegister->GetAddress() == SAMPLESTRINGADDR )
    {
        aRegister->Write( mSampleStrings[ mSampleEnum ] );
    }
    if ( aRegister->GetAddress() == SAMPLEBOOLEANADDR )
    {
        aRegister->Write( mSampleBooleans[ mSampleEnum ] );
    }
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
        mSampleStrings[ mSampleEnum ] = "";
        aRegister->Write( 0 );
    }
    if ( aRegister->GetAddress() == SAMPLEENUMADDR )
    {
        aRegister->Read( mSampleEnum );
    }
    if ( aRegister->GetAddress() == SAMPLESTRINGADDR )
    {
        aRegister->Read( mSampleStrings[ mSampleEnum ] );
    }
    if ( aRegister->GetAddress() == SAMPLEBOOLEANADDR )
    {
        aRegister->Read( mSampleBooleans[ mSampleEnum ] );
    }

    std::cout << aRegister->GetName().GetAscii() << " PostWrite" << std::endl;
}


///
/// \brief Manage Userset persistence for nodes which vary by a selector
///
PvResult MyRegisterEventSink::Persist( IPvRegister *aRegister, IPvRegisterStore *aStore )
{
    switch ( aRegister->GetAddress() )
    {
    case SAMPLESTRINGADDR:
    case SAMPLEBOOLEANADDR:
        return PersistSampleEnum( aRegister, aStore );
    }
    return PvResult::Code::NOT_IMPLEMENTED;
}


//
/// \brief Manage Userset persistence for nodes which vary by SampleEnum
///
/// Since these nodes have a single address we need to explicitly save/load all
/// possible values over the selector range
///
PvResult MyRegisterEventSink::PersistSampleEnum( IPvRegister *aRegister, IPvRegisterStore *aStore ) 
{
    // Preserve current mSampleEnum value
    uint32_t lEnumOrg = mSampleEnum;

    for ( int i = 0; i < SAMPLEENUMCOUNT; i++ )
    {
       // Select Sample Enum
       mSampleEnum = i;

       // Create persistence suffix
       long long lEnumSuffix = i + 1;
       std::string lSuffix = std::string("EnumEntry") + std::to_string(lEnumSuffix) ;

       // Persist selected register (save or load depending on context)
       aStore->Persist( aRegister, lSuffix.c_str() );
    }

    mSampleEnum = lEnumOrg;
    return PvResult::Code::OK;
 
}
