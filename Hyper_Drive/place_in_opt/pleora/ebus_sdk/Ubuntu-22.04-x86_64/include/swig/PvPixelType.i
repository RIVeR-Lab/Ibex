// *****************************************************************************
//
// Copyright (c) 2020, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

%{

#include <sstream>
#include <PvPixelType.h>

%}

%include "exception.i"
%include "typemaps.i"

#define PV_BUFFER_API

// PvPixelType is a bit tricky because it's technically an enum but we have
// quite a few methods that take one as a reference to return the value. For
// example PvResult GetSupportedPixelType(int, PvPixelType &). One way to deal
// with this in python is to simply return a tuple with a PvResult and a PvPixelType.
%typemap(in) ( int aIndex, PvPixelType &aPixelType ) (PvPixelType lTmpPixelType)
{
    lTmpPixelType = PvPixelUndefined;
    int lErr = SWIG_AsVal_int( $input, &($1) );
    if( !SWIG_IsOK( lErr ) )
    {
        SWIG_exception( SWIG_RuntimeError, "Invalid index" );
    }
    $2 = &lTmpPixelType;
}
%typemap(argout) ( int aIndex, PvPixelType &aPixelType )
{
    PyTuple_SetItem( $result, 1, SWIG_From_unsigned_SS_int( *($2) ) );
}
%typemap(out) PvResult GetSupportedPixelType
{
    $result = PyTuple_New(2);
    PyTuple_SetItem( $result, 0, SWIG_NewPointerObj( new PvResult($1), $descriptor( PvResult * ), SWIG_POINTER_OWN ) );
}
// To add insult to injury, we need to support this model with directors as well. A python
// class can inherit from something that implements GetSupportedPixelType so we need to corral
// all the tuples and differentiate between integers and pointers and stuff.
%typemap(directorin) ( int aIndex, PvPixelType &aPixelType )
{
    $input = SWIG_From_int( $1 );
}
// A directorargout typemap is assuming that you will be following the 'return a tuple' pattern
// so we need to make sure we can decode what comes out. See also the director typemaps for
// PvResult.
%typemap(directorargout) ( int aIndex, PvPixelType &aPixelType )
{
    int lPixelType;
    int lErr = SWIG_AsVal_int( $result, &lPixelType );
    if( !SWIG_IsOK( lErr ) )
    {
        Swig::DirectorTypeMismatchException::raise(SWIG_ErrorType(SWIG_ArgError(lErr)), "in output value of type '""PvPixel""'");
    }
    $2 = static_cast<PvPixelType>(lPixelType);
}

#ifdef SWIG_EBUS_LIB

%typemap(constcode) int {
    PyObject *val = PyLong_FromLong(($type)($value));
    SWIG_Python_SetConstant(d, "$1", val);
    if ( strncmp("PVPIXEL", "$1_name", 7) != 0 && "$1_name" != "PVBITSPERPIXELMASK" )
    {
        const char *name = "$typemap(enum_realname,$1_type)";
        PyObject *e = PyDict_GetItemString(d, name);
        if (!e) PyDict_SetItemString(d, name, e = PyDict_New());
        PyDict_SetItemString(e, "$value", val);
    }
}

%include "PvPixelType.h"

#else

%apply unsigned int { PvPixelType };

#endif
