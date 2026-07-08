// *****************************************************************************
//
// Copyright (c) 2020, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

%include "std_string.i"
%include "std_wstring.i"

#define PV_BASE_API

// Allow PvStrings to be implicitly be created from python strings.
%typemap(in) const PvString & ( PvString lTmp )
{   
    char *lBuf = nullptr;
    wchar_t *lWBuf = nullptr;
    int lAlloc = SWIG_OLDOBJ;
    void *lArgP = nullptr;
    int lRes = 0;
    if( SWIG_IsOK( ( lRes = SWIG_ConvertPtr( $input, &lArgP, $descriptor( PvString * ), 0 ) ) ) )
    {
        $1 = reinterpret_cast< PvString *>( lArgP );
    }
    else if( SWIG_IsOK( ( lRes = SWIG_AsWCharPtrAndSize( $input, &lWBuf, nullptr, &lAlloc ) ) ) )
    {
        lTmp = PvString( lWBuf );
        $1 = &lTmp;
        if( lAlloc == SWIG_NEWOBJ )
        {
            delete[] lWBuf;
        }
    }
    else if ( SWIG_IsOK( ( lRes = SWIG_AsCharPtrAndSize( $input, &lBuf, nullptr, &lAlloc ) ) ) )
    {
        lTmp = PvString( lBuf );
        $1 = &lTmp;
        if( lAlloc == SWIG_NEWOBJ )
        {
            delete[] lBuf;
        }
    }
    else
    {
        SWIG_exception_fail( SWIG_ArgError(lRes), "Argument $argnum: failed to create PvString from Python string" );
    }
}

// Allow PvStrings to work with overloaded methods
%typemap(typecheck, precedence=SWIG_TYPECHECK_STRING) PvString &
{
    static swig_type_info *lType = SWIG_TypeQuery( "PvString *" );
    if( SWIG_IsOK( SWIG_ConvertPtr( $input, 0, $descriptor( PvString * ), 0 ) ) )
    {
        $1 = 1;
    }
    else if( SWIG_IsOK( SWIG_AsCharPtrAndSize( $input, 0, nullptr, 0) ) )
    {
        $1 = 1;
    }
    else if( SWIG_IsOK( SWIG_ConvertPtr( $input, 0, $descriptor( wchar_t *), 0) ) )
    {
        $1 = 1;
    }
}

// Allow PvStrings to be returned as python strings.
// Python is unicode so do wchar for now but if need be we could do:
// $result = SWIG_FromCharPtr( $1->GetAscii() );
%typemap(out) PvString &
{  
    $result = SWIG_FromWCharPtr( $1->GetUnicode() );
}

#ifdef SWIG_EBUS_LIB

%include "PvString.h"

// Printing PvStrings is useful too.
%extend PvString
{
    const wchar_t *__str__()
    {
        return $self->GetUnicode();
    }
};

#endif