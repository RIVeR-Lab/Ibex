// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#if !defined( PV_CODEC_NO_DECLSPEC )
    #if defined( PV_CODEC_EXPORTS )

        #if !defined( PT_LIB_STATIC ) && defined( WIN32 )
            #define PV_CODEC_API __declspec( dllexport )
        #else
            #define PV_CODEC_API
        #endif

    #else

        #if !defined( PT_LIB_STATIC ) && defined( WIN32 )
            #define PV_CODEC_API __declspec( dllimport )
        #else
            #define PV_CODEC_API
        #endif

        #define PT_LIB_NAME "PvCodec"
        #include <PvLinkLib.h>

    #endif
#endif

#if !defined( PV_CODEC_API )
    #define PV_CODEC_API
#endif

#include <PvResult.h>

