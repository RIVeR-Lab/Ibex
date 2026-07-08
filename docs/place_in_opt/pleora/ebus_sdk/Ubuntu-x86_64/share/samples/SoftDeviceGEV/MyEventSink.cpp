// *****************************************************************************
//
// Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "Defines.h"
#include "MyEventSink.h"
#include "Utilities.h"

#include <limits>


///
/// \brief Constructor
///

MyEventSink::MyEventSink( IPvRegisterEventSink *aRegisterEventSink )
    : mRegisterEventSink( aRegisterEventSink )
{
}


///
/// \brief Destructor
///

MyEventSink::~MyEventSink()
{

}


///
/// \brief Application connection notification
///

void MyEventSink::OnApplicationConnect( IPvSoftDeviceGEV *aDevice, const PvString &aIPAddress, uint16_t aPort, PvAccessType aAccessType )
{
    PVUNREFPARAM( aDevice );
    PVUNREFPARAM( aAccessType );
    std::cout << "Application connected from " << aIPAddress.GetAscii() << ":" << aPort << std::endl;
}


///
/// \brief Application disconnection notification
///

void MyEventSink::OnApplicationDisconnect( IPvSoftDeviceGEV *aDevice )
{
    PVUNREFPARAM( aDevice );
    std::cout << "Application disconnected" << std::endl;
}


///
/// \brief Control channel start notification
///

void MyEventSink::OnControlChannelStart( IPvSoftDeviceGEV *aDevice, const PvString &aMACAddress, const PvString &aIPAddress, const PvString &aMask, const PvString &aGateway, uint16_t aPort )
{
    PVUNREFPARAM( aDevice );
    std::cout << "Control channel started on [" << aMACAddress.GetAscii() << "] ";
    std::cout << aIPAddress.GetAscii() << ":" << aPort << " ";
    std::cout << "Mask:" << aMask.GetAscii() << ", ";
    std::cout << "Gateway:" << aGateway.GetAscii() << std::endl;

    DumpRegisters( aDevice->GetRegisterMap() );
}


///
/// \brief Control channel stop notification
///

void MyEventSink::OnControlChannelStop( IPvSoftDeviceGEV *aDevice )
{
    PVUNREFPARAM( aDevice );
    std::cout << "Control channel stopped" << std::endl;
}


///
/// \brief Device reset notification
///

void MyEventSink::OnDeviceResetFull( IPvSoftDeviceGEV *aDevice )
{
    PVUNREFPARAM( aDevice );
    std::cout << "Device reset" << std::endl;
}


///
/// \brief Network reset notification
///

void MyEventSink::OnDeviceResetNetwork( IPvSoftDeviceGEV *aDevice )
{
    PVUNREFPARAM( aDevice );
    std::cout << "Network reset" << std::endl;
}


///
/// \brief Callback used to initiate custom registers creation
///

void MyEventSink::OnCreateCustomRegisters( IPvSoftDeviceGEV *aDevice, IPvRegisterFactory *aFactory )
{
    PVUNREFPARAM( aDevice );

    aFactory->AddRegister( SAMPLEINTEGERNAME, SAMPLEINTEGERADDR, 4, PvGenAccessModeReadWrite, mRegisterEventSink );
    aFactory->AddRegister( SAMPLEFLOATNAME, SAMPLEFLOATADDR, 4, PvGenAccessModeReadWrite, mRegisterEventSink );
    aFactory->AddRegister( SAMPLESTRINGNAME, SAMPLESTRINGADDR, 16, PvGenAccessModeReadWrite, mRegisterEventSink );
    aFactory->AddRegister( SAMPLEBOOLEANNAME, SAMPLEBOOLEANADDR, 4, PvGenAccessModeReadWrite, mRegisterEventSink );
    aFactory->AddRegister( SAMPLECOMMANDNAME, SAMPLECOMMANDADDR, 4, PvGenAccessModeReadWrite, mRegisterEventSink );
    aFactory->AddRegister( SAMPLEENUMNAME, SAMPLEENUMADDR, 4, PvGenAccessModeReadWrite, mRegisterEventSink );
}


///
/// \brief Callback used to initiate GenApi features creation
///

void MyEventSink::OnCreateCustomGenApiFeatures( IPvSoftDeviceGEV *aDevice, IPvGenApiFactory *aFactory )
{
    IPvRegisterMap *lMap = aDevice->GetRegisterMap();

    CreateSampleParameters( lMap, aFactory );
    CreateChunkParameters( aFactory );
    CreateEventParameters( aFactory );
}


///
/// \brief Shows how to create custom GenApi parameters
///

void MyEventSink::CreateSampleParameters( IPvRegisterMap *aMap, IPvGenApiFactory *aFactory )
{
    // Creates sample enumeration feature mapped to SAMPLEENUMADDR register.
    // Enumeration entries of EnumEntry1 (0), EnumEntry2 (1) and EnumEntry3 (2) are created.
    // This feature is also defined as a selector of the sample string and sample Boolean.
    aFactory->SetName( SAMPLEENUMNAME );
    aFactory->SetDescription( SAMPLEENUMDESCRIPTION );
    aFactory->SetToolTip( SAMPLEENUMTOOLTIP );
    aFactory->SetCategory( SAMPLECATEGORY );
    aFactory->AddEnumEntry( "EnumEntry1", 0 );
    aFactory->AddEnumEntry( "EnumEntry2", 1 );
    aFactory->AddEnumEntry( "EnumEntry3", 2 );
    aFactory->AddSelected( SAMPLESTRINGNAME );
    aFactory->AddSelected( SAMPLEBOOLEANNAME );
    aFactory->CreateEnum( aMap->GetRegisterByAddress( SAMPLEENUMADDR ) );

    // Creates sample string feature mapped to SAMPLESTRINGADDR register.
    // This feature is selected by sample enumeration.
    // This feature has an invalidator to the sample command: its cached value will be invalidated (reset) whenever the command is executed.
    aFactory->SetName( SAMPLESTRINGNAME );
    aFactory->SetDescription( SAMPLESTRINGDESCRIPTION );
    aFactory->SetToolTip( SAMPLESTRINGTOOLTIP );
    aFactory->SetCategory( SAMPLECATEGORY );
    aFactory->AddInvalidator( SAMPLECOMMANDNAME );
    aFactory->CreateString( aMap->GetRegisterByAddress( SAMPLESTRINGADDR ) );

    // Creates sample Boolean feature mapped to SAMPLEBOOLEANADDR register.
    // This feature is selected by sample enumeration.
    aFactory->SetName( SAMPLEBOOLEANNAME );
    aFactory->SetDescription( SAMPLEBOOLEANDESCRIPTION );
    aFactory->SetToolTip( SAMPLEBOOLEANTOOLTIP );
    aFactory->SetCategory( SAMPLECATEGORY );
    aFactory->CreateBoolean( aMap->GetRegisterByAddress( SAMPLEBOOLEANADDR ) );

    // Creates sample command feature mapped to SAMPLECOMMANDADDR register.
    // The sample string has this command defined as its invalidator.
    // Executing this command resets the controller-side GenApi cached value of the sample string.
    aFactory->SetName( SAMPLECOMMANDNAME );
    aFactory->SetDescription( SAMPLECOMMANDDESCRIPTION );
    aFactory->SetToolTip( SAMPLECOMMANDTOOLTIP );
    aFactory->SetCategory( SAMPLECATEGORY );
    aFactory->CreateCommand( aMap->GetRegisterByAddress( SAMPLECOMMANDADDR ) );

    // Creates sample integer feature mapped to SAMPLEINTEGERADDR register.
    // Minimum is defined as -10000, maximum 100000, increment as 1 and unit as milliseconds.
    aFactory->SetName( SAMPLEINTEGERNAME );
    aFactory->SetDescription( SAMPLEINTEGERDESCRIPTION );
    aFactory->SetToolTip( SAMPLEINTEGERTOOLTIP );
    aFactory->SetCategory( SAMPLECATEGORY );
    aFactory->SetRepresentation( PvGenRepresentationLinear );
    aFactory->SetUnit( SAMPLEINTEGERUNITS );
    aFactory->CreateInteger( aMap->GetRegisterByAddress( SAMPLEINTEGERADDR ), -10000, 10000, 1 );

    // Creates sample float feature mapped to SAMPLEFLOATADDR register.
    // Minimum is defined as -100.0 and maximum as 100.0 and units as inches.
    aFactory->SetName( SAMPLEFLOATNAME );
    aFactory->SetDescription( SAMPLEFLOATDESCRIPTION );
    aFactory->SetToolTip( SAMPLEFLOATTOOLTIP );
    aFactory->SetCategory( SAMPLECATEGORY );
    aFactory->SetRepresentation( PvGenRepresentationPureNumber );
    aFactory->SetUnit( SAMPLEFLOATUNITS );
    aFactory->CreateFloat( aMap->GetRegisterByAddress( SAMPLEFLOATADDR ), -100.0, 100.0 );

    // Creates sample pValue feature which simply links to another feature.
    // This feature is linked to sample integer.
    // We use "pValue" as display name as the automatically generated one is not as good.
    aFactory->SetName( SAMPLEPVALUE );
    aFactory->SetDisplayName( SAMPLEPVALUEDISPLAYNAME );
    aFactory->SetDescription( SAMPLEPVALUEDESCRIPTION );
    aFactory->SetToolTip( SAMPLEPVALUETOOLTIP );
    aFactory->SetCategory( SAMPLECATEGORY );
    aFactory->SetPValue( SAMPLEINTEGERNAME );
    aFactory->SetUnit( SAMPLEPVALUEUNITS );
    aFactory->CreateInteger();

    // Creates an integer SwissKnife, a read-only expression.
    // It only has one variable, the sample integer.
    // The SwissKnife expression converts the millisecond sample integer to nanoseconds with a * 1000 formula".
    aFactory->SetName( SAMPLEINTSWISSKNIFENAME );
    aFactory->SetDescription( SAMPLEINTSWISSKNIFEDESCRIPTION );
    aFactory->SetToolTip( SAMPLEINTSWISSKNIFETOOLTIP );
    aFactory->SetCategory( SAMPLECATEGORY );
    aFactory->AddVariable( SAMPLEINTEGERNAME );
    aFactory->SetUnit( SAMPLEINTSWISSKNIFEUNITS );
    aFactory->CreateIntSwissKnife( SAMPLEINTEGERNAME " * 1000" );

    // Creates a float SwissKnife, a read-only expression.
    // It only has one variable, the sample float.
    // The SwissKnife expression converts the inches sample float to centimeters with a * 2.54 formula.
    aFactory->SetName( SAMPLEFLOATSWISSKNIFENAME );
    aFactory->SetDescription( SAMPLEFLOATSWISSKNIFEDESCRIPTION );
    aFactory->SetToolTip( SAMPLEFLOATSWISSKNIFETOOLTIP );
    aFactory->SetCategory( SAMPLECATEGORY );
    aFactory->AddVariable( SAMPLEFLOATNAME );
    aFactory->SetUnit( SAMPLEFLOATSWISSKNIFEUNITS );
    aFactory->CreateFloatSwissKnife( SAMPLEFLOATNAME " * 2.54" );

    // Create integer converter (read-write to/from expressions)
    // The main feature the converter acts uppon is the sample integer and handles millisecond to nanosecond conversion.
    // No additional variables are required, no need to call AddVariable on the factory.
    aFactory->SetName( SAMPLEINTCONVERTERNAME );
    aFactory->SetDescription( SAMPLEINTCONVERTERDESCRIPTION );
    aFactory->SetToolTip( SAMPLEINTCONVERTERTOOLTIP );
    aFactory->SetCategory( SAMPLECATEGORY );
    aFactory->SetUnit( SAMPLEINTCONVERTERUNITS );
    aFactory->CreateIntConverter( SAMPLEINTEGERNAME, "TO * 1000", "FROM / 1000" );

    // Create float converter (read-write to/from expressions)
    // The main feature the converter acts uppon is the sample float and handles inches to centimeter conversion.
    // No additional variables are required, no need to call AddVariable on the factory.
    aFactory->SetName( SAMPLEFLOATCONVERTERNAME );
    aFactory->SetDescription( SAMPLEFLOATCONVERTERDESCRIPTION );
    aFactory->SetToolTip( SAMPLEFLOATCONVERTERTOOLTIP );
    aFactory->SetCategory( SAMPLECATEGORY );
    aFactory->SetUnit( SAMPLEFLOATCONVETERUNITS );
    aFactory->CreateFloatConverter( SAMPLEFLOATNAME, "TO * 2.54", "FROM * 0.3937" );

    // Create hidden SwissKnife that returns non-zero (true, or 1) if the sample integer is above 5.
    // The feature will not show up in GUI browsers as it does not have a category.
    // We typically refer to these features as support or private features.
    // These features typically do not have description or ToolTips as they do not show up in a GenApi user interface.
    aFactory->SetName( SAMPLEHIDDENSWISSKNIFENAME );
    aFactory->AddVariable( SAMPLEINTEGERNAME );
    aFactory->CreateIntSwissKnife( SAMPLEINTEGERNAME " > 5" );

    // Create integer: link to sample integer but only available when the hidden SwissKnife is non-zero.
    // This feature is an integer that has a pValue link to our sample enumeration, displaying its integer value.
    // We use this feature to demonstrate the pIsAvailable attribute: it links to our hidden SwissKnife and controls when it is available.
    // When the hidden SwissKnife is true (sample integer > 5) this feature will be readable and writable.
    // When the hidden SwissKnife is false (sample integer <= 5) this feature will be "not available".
    // We use "pIsAvailable" as display name as the automatically generated one is not as good.
    aFactory->SetName( SAMPLEPISAVAILABLENAME );
    aFactory->SetDisplayName( SAMPLEPISAVAILABLEDISPLAYNAME );
    aFactory->SetDescription( SAMPLEPISAVAILABLEDESCRIPTION );
    aFactory->SetToolTip( SAMPLEPISAVAILABLETOOLTIP );
    aFactory->SetCategory( SAMPLECATEGORY );
    aFactory->SetPIsAvailable( SAMPLEHIDDENSWISSKNIFENAME );
    aFactory->SetPValue( SAMPLEENUMNAME );
    aFactory->CreateInteger();
}


///
/// \brief Shows how to create custom GenApi parameters for chunk data mapping
///

void MyEventSink::CreateChunkParameters( IPvGenApiFactory *aFactory )
{
    // Create GenApi feature used to map the chunk data count field
    aFactory->SetName( CHUNKCOUNTNAME );
    aFactory->SetDescription( CHUNKCOUNTDESCRIPTION );
    aFactory->SetToolTip( CHUNKCOUNTTOOLTIP );
    aFactory->SetCategory( CHUNKCATEGORY );
    aFactory->MapChunk( CHUNKID, 0, 4, PvGenEndiannessLittle );
    aFactory->CreateInteger( NULL, 0, std::numeric_limits<uint32_t>::max() );

    // Create GenApi feature used to map the chunk data time field
    aFactory->SetName( CHUNKTIMENAME );
    aFactory->SetDescription( CHUNKTIMEDESCRIPTION );
    aFactory->SetToolTip( CHUNKTIMETOOLTIP );
    aFactory->SetCategory( CHUNKCATEGORY );
    aFactory->MapChunk( CHUNKID, 4, 32, PvGenEndiannessLittle );
    aFactory->CreateString( NULL );
}


///
/// \brief Shows how to create custom GenApi parameters for event data mapping
///

void MyEventSink::CreateEventParameters( IPvGenApiFactory *aFactory )
{
    // Create GenApi feature used to map the event data count field
    aFactory->SetName( EVENTCOUNTNAME );
    aFactory->SetDescription( EVENTCOUNTDESCRIPTION );
    aFactory->SetToolTip( EVENTCOUNTTOOLTIP );
    aFactory->SetCategory( EVENTCATEGORY );
    aFactory->MapEvent( EVENTDATAID, 24, 4 );
    aFactory->CreateInteger( NULL, 0, std::numeric_limits<uint32_t>::max() );

    // Create GenApi feature used to map the event data time field
    aFactory->SetName( EVENTTIMENAME );
    aFactory->SetDescription( EVENTTIMEDESCRIPTION );
    aFactory->SetToolTip( EVENTTIMETOOLTIP );
    aFactory->SetCategory( EVENTCATEGORY );
    aFactory->MapEvent( EVENTDATAID, 28, 32 );
    aFactory->CreateString( NULL );
}

