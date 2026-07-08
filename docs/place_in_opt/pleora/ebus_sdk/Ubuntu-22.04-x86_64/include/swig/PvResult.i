// *****************************************************************************
//
// Copyright (c) 2020, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

%{

#include <sstream>
#include <PvResult.h>

%}

%include "exception.i"
%include "typemaps.i"

#define PV_BASE_API

/*
PvResults get passed around as classes or integers. C++ handles that conversion
automatically with default constructors. Python not so much. This checks any
PvResult returns coming back from python and decides if they're integers or
classes and does the 'right thing'.

NOTE: Do we need to check other integral types?
NOTE: If you try to return a PvResult by value rather than reference within directors
      SWIG will barf up a memory leak error because of not having a destructor for PvResult.
*/
%typemap(directorout) PvResult
{
    int lResult = 0;
    int lErr = SWIG_AsVal_int( $input, &lResult );
    if( SWIG_IsOK( lErr ) )
    {
        $result.SetCode( (uint32_t)lResult );
    }
    else
    {
        void *lPvResult;
        lErr = SWIG_ConvertPtr( $input, &lPvResult, $descriptor( PvResult * ), 0 );
        if( !SWIG_IsOK( lErr ) )
        {
            Swig::DirectorTypeMismatchException::raise(SWIG_ErrorType(SWIG_ArgError(lErr)), "in method $symname with output value of type '""PvResult""'");
        }
        $result = *(reinterpret_cast< PvResult * >( lPvResult ) );
        if (SWIG_IsNewObj( lErr ))
        {
            delete reinterpret_cast< PvResult * >( lPvResult );
        }
    }
}

#ifdef SWIG_EBUS_LIB

%include "PvResult.h"

#endif
