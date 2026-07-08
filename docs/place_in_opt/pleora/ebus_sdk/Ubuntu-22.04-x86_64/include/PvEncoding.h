// *****************************************************************************
//
//     Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVENCODING_H__
#define __PVENCODING_H__

#ifndef EBUS_ENUM
#define EBUS_ENUM(x) typedef enum
#endif

///
/// \file PvEncoding.h
///

///
/// \enum PvEncodingEnum
/// \brief The #PvEncodingEnum enumeration indicates the coding used by the PvCompressionFilter.
///

/// \var PvEncodingUnknown
/// Value not known; indeterminate. The value is currently not supported.

/// \var PvEncodingPTC1
/// Pleora Compression Version 1 encoding.

EBUS_ENUM( PvEncodingEnum )
{
    PvEncodingUnknown = -1,
    PvEncodingPTC1 = 0

} PvEncodingEnum;


#endif // __PVENCODING_H__
