// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __EBUS_PLAYER_SHARED_H__
#define __EBUS_PLAYER_SHARED_H__


#ifdef _AFXDLL

    #ifndef VC_EXTRALEAN
    #define VC_EXTRALEAN
    #endif

    #ifdef PT_VLD
    #include <vld.h>
    #endif

    #include <afxwin.h>
    #include <afxmt.h>

    #include <stdint.h>

#endif // _AFXDLL

#ifndef WIN32

    #include <sys/time.h>
    #include <stdint.h>

    inline int64_t GetTickCount()
    {
        timeval ts;
        gettimeofday( &ts, 0 );

        return ( int64_t ) ( ts.tv_sec * 1000 + ( ts.tv_usec / 1000 ) );
    }

#endif

#endif // __EBUS_PLAYER_SHARED_H__

