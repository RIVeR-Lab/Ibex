// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENTYPES_H__
#define __PVGENTYPES_H__

#ifndef EBUS_ENUM
#define EBUS_ENUM(x) typedef enum
#endif

/// \file PvGenTypes.h
/// \ingroup GenICam features
/// \brief GenICam feature-related constants
///


/// \enum PvGenVisibility
/// \brief Feature (PvGenParameter object) visibility
///
/// Every feature has a visibility setting that suggests how
/// difficult the feature is to use and when it shouldn't be 
/// made available (visible) to the end user. The current visibility
/// might be a program-specific variable available, for example, in 
/// an options or configuration dialog. When opening the program for the 
/// first time, the visibility would typically be set to beginner.
///
/// You can test the feature-by-feature visibility using the following methods:
///     - PvGenParameter::IsVisible
///     - PvGenParameter::GetVisibility
///     - PvGenEnumEntry::IsVisible (for enumeration entries)
///     - PvGenEnumEntry::GetVisibility (for enumeration entries)
///
/// The visibility levels, from lowest to highest:
///     - Beginner (#PvGenVisibilityBeginner)
///     - Expert (#PvGenVisibilityExpert)
///     - Guru (#PvGenVisibilityGuru)
///     - Invisible (#PvGenVisibilityInvisible)
///
/// If a program's current visibility were set to Expert, the user would 
/// expect to see Beginner and Expert-level features, but not Guru
/// or Invisible-level features.

/// \var PvGenVisibilityBeginner
/// Always visible, to all users.

/// \var PvGenVisibilityExpert
/// Intended for advanced users.

/// \var PvGenVisibilityGuru
/// Intended for very advanced users.

/// \var PvGenVisibilityInvisible
/// Appears in the API, but not the GUI.

/// \var PvGenVisibilityUndefined
/// Not defined; valid visibility not provided.
EBUS_ENUM(PvGenVisibility)
{
    PvGenVisibilityBeginner = 0,
    PvGenVisibilityExpert,
    PvGenVisibilityGuru,
    PvGenVisibilityInvisible,
    PvGenVisibilityUndefined = 999

} PvGenVisibility;


/// \enum PvGenType
/// \brief Feature data type (Boolean, integer, etc.)
///
/// Each feature has a type that defines the information it contains and how it can be used.
/// The type also determines the eBUS SDK class used to represent it (and the methods that are
/// available).
///
/// To test for a feature's type, use PvGenParameter::GetType.
///

/// \var PvGenTypeInteger 
/// Type: Integer<br>
/// Class: #PvGenInteger

/// \var PvGenTypeBoolean
/// Type: Boolean<br>
/// Class: #PvGenBoolean

/// \var PvGenTypeString
/// Type: String<br>
/// Class: #PvGenString

/// \var PvGenTypeCommand
/// Type: Command<br>
/// Class: #PvGenCommand

/// \var PvGenTypeFloat
/// Type: Float<br>
/// Class: #PvGenFloat

/// \var PvGenTypeEnum
/// Type: Enumeration<br>
/// Class: #PvGenEnum

/// \var PvGenTypeUndefined
/// Type: Not defined; valid type not provided.<br>
/// Class: Not applicable.
EBUS_ENUM(PvGenType)
{
    PvGenTypeInteger = 0,
    PvGenTypeEnum,
    PvGenTypeBoolean,
    PvGenTypeString,
    PvGenTypeCommand,
    PvGenTypeFloat,
    PvGenTypeRegister,
    PvGenTypeUndefined = 999

} PvGenType;


/// \enum PvGenRepresentation
///
/// #PvGenRepresentation suggests how the UI should present a 
/// numerical feature (#PvGenInteger or #PvGenFloat).
/// 
/// To get a feature's representation, use the following methods:
///     - PvGenFloat::GetRepresentation
///     - PvGenInteger::GetRepresentation
///

/// \var PvGenRepresentationLinear
/// Linear. Use a slider.

/// \var PvGenRepresentationLogarithmic
/// Logarithmic. Use a slider. The GenICam standard doesn't specify
/// if the feature's value represents <i>x</i> or <i>y</i> in the formula
/// <i>x</i> = <i>n</i> ^ <i>y</i>. 

/// \var PvGenRepresentationBoolean
/// Boolean. The number might be presented as a check box or enable.
/// Use a similar UI as for features from the #PvGenBoolean class. 

/// \var PvGenRepresentationPureNumber
/// Pure number. Use an edit box with a decimal display.

/// \var PvGenRepresentationHexNumber
/// Hex number. Use an edit box with a hexadecimal display.

/// \var PvGenRepresentationIPV4Address
/// IPv4 address. Use an edit box with an IP address dot-decimal display.

/// \var PvGenRepresentationMACAddress
/// MAC address. Use an edit box with a MAC address (e.g. 01-23-45-67-89-AB) display.

/// \var PvGenRepresentationUndefined
/// Not defined; valid representation not provided.
EBUS_ENUM(PvGenRepresentation)
{
    PvGenRepresentationLinear = 0,            
    PvGenRepresentationLogarithmic,   
    PvGenRepresentationBoolean,       
    PvGenRepresentationPureNumber,    
    PvGenRepresentationHexNumber,
    PvGenRepresentationIPV4Address,
    PvGenRepresentationMACAddress,
    PvGenRepresentationUndefined = 999

} PvGenRepresentation;



EBUS_ENUM(PvGenNameSpace)
{
    PvGenNameSpaceStandard = 0,
    PvGenNameSpaceCustom,
    PvGenNameSpaceUndefined = 999

} PvGenNameSpace;


EBUS_ENUM(PvGenAccessMode)
{
    PvGenAccessModeReadOnly = 0,
    PvGenAccessModeReadWrite = 1,
    PvGenAccessModeWriteOnly = 2,
    PvGenAccessModeNotImplemented = 3,
    PvGenAccessModeNotAvailable = 4,
    PvGenAccessModeUndefined = 999

} PvGenAccessMode;


EBUS_ENUM(PvGenCache)
{
    PvGenCacheWriteThrough = 0,
    PvGenCacheWriteAround,
    PvGenCacheNone,
    PvGenCacheUndefined = 999

} PvGenCache;


/// \enum PvGenRefresh
/// \brief Refresh method for a GenApi browser.
///
/// GenApi browsers typically provides different methods for refresh
/// the parameters it is displaying.
///

/// \var PvGenRefreshPolling
/// The polling time provided for parameters in the XML are used to control parameter refreshing.

/// \var PvGenRefreshAuto
/// A round-robbin refresh is performed on all visible features.

/// \var PvGenRefreshManual
/// Refresh is disabled. The browser may offer a manual Refresh button that can be used to control parameter refreshing.
EBUS_ENUM(PvGenRefresh)
{
    PvGenRefreshInvalid = -1,
    PvGenRefreshPolling = 0,
    PvGenRefreshAuto,
    PvGenRefreshManual

} PvGenRefresh;


EBUS_ENUM(PvGenEndianness)
{
    PvGenEndiannessInvalid = -1,
    PvGenEndiannessLittle = 0,
    PvGenEndiannessBig

} PvGenEndianness;


EBUS_ENUM(PvGenSign)
{
    PvGenSignUndefined = -1,
    PvGenSignUnsigned = 0,
    PvGenSignSigned

} PvGenSign;


#endif

