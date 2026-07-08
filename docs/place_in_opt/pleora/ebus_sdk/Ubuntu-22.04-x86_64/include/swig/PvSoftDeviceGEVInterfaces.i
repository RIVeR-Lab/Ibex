// *****************************************************************************
//
// Copyright (c) 2020, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************
%{

#include <IPvUserSetNotify.h>
#include <PvSoftDeviceGEV.h>
#include <PvFPSStabilizer.h>
#include <PvSampleTransmitterConfig.h>
#include <PvStreamingChannelSourceDefault.h>

%}

%include "stdint.i"

// typemaps for GetWidthInfo, GetHeightInfo
%typemap(in,numinputs=0) ( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) (uint32_t lMin, uint32_t lMax, uint32_t lInc )
{
    $1 = &lMin;
    $2 = &lMax;
    $3 = &lInc;
}
%typemap(argout) ( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc )
{
    $result = PyTuple_New(3);
    PyTuple_SetItem( $result, 0, SWIG_From_unsigned_SS_int( *($1) ) );
    PyTuple_SetItem( $result, 1, SWIG_From_unsigned_SS_int( *($2) ) );
    PyTuple_SetItem( $result, 2, SWIG_From_unsigned_SS_int( *($3) ) );
}
%typemap(directorargout) ( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc )
{
    PyObject *lOut = PyTuple_GetItem($result, 0);
    if( nullptr == lOut )
    {
        Swig::DirectorException::raise("$symname failed to return a 3-tuple");
    }
    int lErr = SWIG_AsVal_unsigned_SS_int( lOut, &($1) );
    if( !SWIG_IsOK(lErr) )
    {
        Swig::DirectorTypeMismatchException::raise(SWIG_ErrorType(SWIG_ArgError(lErr)), "in output value of type '""uint32_t""'");
    }
    lOut = PyTuple_GetItem($result, 1);
    if( nullptr == lOut )
    {
        Swig::DirectorException::raise("$symname failed to return a 3-tuple");
    }
    lErr = SWIG_AsVal_unsigned_SS_int( lOut, &($2) );
    if( !SWIG_IsOK(lErr) )
    {
        Swig::DirectorTypeMismatchException::raise(SWIG_ErrorType(SWIG_ArgError(lErr)), "in output value of type '""uint32_t""'");
    }
    lOut = PyTuple_GetItem($result, 2);
    if( nullptr == lOut )
    {
        Swig::DirectorException::raise("$symname failed to return a 3-tuple");
    }
    lErr = SWIG_AsVal_unsigned_SS_int( lOut, &($3) );
    if( !SWIG_IsOK(lErr) )
    {
        Swig::DirectorTypeMismatchException::raise(SWIG_ErrorType(SWIG_ArgError(lErr)), "in output value of type '""uint32_t""'");
    }
}

// Typemaps for GetSupportedChunk
%typemap(in) ( int aIndex, uint32_t &aID, PvString &aName ) (uint32_t lID, PvString lName)
{
    int lErr = SWIG_AsVal_int( $input, &($1));
    if( ! SWIG_IsOK(lErr) )
    {
        SWIG_exception_fail(SWIG_ArgError(lErr), "method '$symname' expected argument 1 of type int");
    }
    $2 = &lID;
    $3 = &lName;
}
%typemap(argout) ( int aIndex, uint32_t &aID, PvString &aName )
{
    PyObject *lRes = $result;
    $result = PyTuple_New(3);
    PyTuple_SetItem( $result, 0, lRes );
    PyTuple_SetItem( $result, 1, SWIG_From_unsigned_SS_int( *($2) ) );
    PyTuple_SetItem( $result, 2, SWIG_FromCharPtr( ($3)->GetAscii() ) );
}
%typemap(directorin) ( int aIndex, uint32_t &aID, PvString &aName )
{
    $input = SWIG_From_int( $1 );
}
// This behaves a bit differently. We return a tuple (result,id,name) but
// because SWIG coalesces typemap arguments it assumes we've only returned
// one extra item which it stores in $result. That will correspond to the
// aID argument. In this case we need to 'cheat' and use the literal 'result'
// which is the original tuple returned to access the aName.
%typemap(directorargout) ( int aIndex, uint32_t &aID, PvString &aName )
{
    int lErr = SWIG_AsVal_unsigned_SS_int( $result, &($2) );
    if( !SWIG_IsOK(lErr) )
    {
        Swig::DirectorTypeMismatchException::raise(SWIG_ErrorType(SWIG_ArgError(lErr)), "in output value of type '""uint32_t""'");
    }
    PyObject *lNameObj = PyTuple_GetItem(result, 2);
    if( nullptr == lNameObj )
    {
        Swig::DirectorException::raise("$symname failed to return a 3-tuple");
    }
    char *lBuf = nullptr;
    wchar_t *lWBuf = nullptr;
    int lAlloc = SWIG_OLDOBJ;
    if( SWIG_IsOK( ( lErr = SWIG_AsWCharPtrAndSize( lNameObj, &lWBuf, nullptr, &lAlloc ) ) ) )
    {
        $3 = PvString( lWBuf );
        if( lAlloc == SWIG_NEWOBJ )
        {
            delete[] lWBuf;
        }
    }
    else if ( SWIG_IsOK( ( lErr = SWIG_AsCharPtrAndSize( lNameObj, &lBuf, nullptr, &lAlloc ) ) ) )
    {
        $3 = PvString( lBuf );
        if( lAlloc == SWIG_NEWOBJ )
        {
            delete[] lBuf;
        }
    }
    else
    {
        Swig::DirectorTypeMismatchException::raise(SWIG_ErrorType(SWIG_ArgError(lErr)), "in output value of type str");
    }
}

// GetGevSCPSPacketSizeInfo
// directorout is needed since the PvResult.i typemap doesn't handle tuple with PvResult as the first item.
// and the c++ side needs to check the PvResult returned to decide the next step.
// directargout is also needed to return the aMax value to the C++ side based on the PvResult status.
// As the PvResult.i mentioned we also need to handle PvResult as integer and class at the same time....
// TO_DO there might be a better way to handle this, leave this here by now to revisit in the future. EBUS-5600
%typemap(in,numinputs=0) uint32_t &aMax ( uint32_t lTmp )
{
    $1 = &lTmp;
}
%typemap(argout) ( uint32_t &aMax )
{
    PyObject *lRes = $result;
    $result = PyTuple_New(2);
    PyTuple_SetItem( $result, 0, lRes );
    PyTuple_SetItem( $result, 1, SWIG_From_unsigned_SS_int( *($1) ) );
}
%typemap(directorout) PvResult GetGevSCPSPacketSizeInfo
{
    PyObject *lOut = PyTuple_GetItem($input, 0);
    if( nullptr == lOut )
    {
        Swig::DirectorException::raise("$symname failed to return a 2-tuple");
    }
    int lResult = 0;
    int lErr = SWIG_AsVal_int( lOut, &lResult );
    if( SWIG_IsOK( lErr ) )
    {
        $result.SetCode( (uint32_t)lResult );
    }
    else
    {
        void *lPvResult;
        lErr = SWIG_ConvertPtr( lOut, &lPvResult, $descriptor( PvResult * ), 0 );
        if( !SWIG_IsOK( lErr ) )
        {
            Swig::DirectorTypeMismatchException::raise(SWIG_ErrorType(SWIG_ArgError(lErr)), "in method $symname 1 with output value of type '""PvResult""'");
        }
        $result = *(reinterpret_cast< PvResult * >( lPvResult ) );
        if (SWIG_IsNewObj( lErr ))
        {
            delete reinterpret_cast< PvResult * >( lPvResult );
        }
    }

}
%typemap(directorargout) uint32_t &aMax
{
    PyObject *lOut = PyTuple_GetItem($result, 1);
    if( nullptr == lOut )
    {
        Swig::DirectorException::raise("$symname failed to return a 2-tuple");
    }
    int lErr = SWIG_AsVal_unsigned_SS_int( lOut, &($1) );
    if( !SWIG_IsOK(lErr) )
    {
        Swig::DirectorTypeMismatchException::raise(SWIG_ErrorType(SWIG_ArgError(lErr)), "in output value of type '""uint32_t""'");
    }
}

// Typemap for FireEvent
%typemap(in) (uint8_t *aData, uint32_t aDataLength)
{
  if (!PyBytes_Check($input)) {
    SWIG_exception_fail(SWIG_TypeError, "in method '$symname', argument $argnum of type '$type'");
  }
  $1 = (uint8_t *)PyBytes_AsString( $input );
  $2 = (uint32_t)PyBytes_Size($input);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_STRING) (uint8_t *aData, uint32_t aDataLength)
{
    $1 = PyBytes_Check($input);
}

// Definitions below this do not need to be shared with the python plugin
#ifdef SWIG_EBUS_LIB

// For IPvRegister::Write of raw data
%typemap(in) (const uint8_t *aData, uint32_t aByteCount)
{
    if (!PyBytes_Check($input)) {
        SWIG_exception_fail(SWIG_TypeError, "in method '$symname', argument $argnum of type '$type'");
    }
    $1 = (uint8_t *)PyBytes_AsString( $input );
    $2 = (uint32_t)PyBytes_Size($input);
}
%typemap(typecheck, precedence=SWIG_TYPECHECK_STRING) (const uint8_t *aData, uint32_t aByteCount)
{
    $1 = PyBytes_Check($input);
}

#define PV_VIRTUAL_DEVICE_API

%typemap(constcode) int {
    PyObject *val = PyLong_FromLong(($type)($value));
    SWIG_Python_SetConstant(d, "$1", val);
    const char *name = "$typemap(enum_realname,$1_type)";
    PyObject *e = PyDict_GetItemString(d, name);
    if (!e) PyDict_SetItemString(d, name, e = PyDict_New());
    PyDict_SetItemString(e, "$value", val);
}
        
%include "PvDeviceEnums.h"

#if 0
// Technically this allows RetrieveBuffer to take no argument but...it breaks
// the directorargout for some reason. For now it looks like RetrieveBuffer
// takes a 'dummy' argument.
%typemap(in, numinputs=0) PvBuffer ** (PvBuffer *lBuf)
{
    $1 = &lBuf;
}
%typemap(argout) PvBuffer **
{
    $result = SWIG_NewPointerObj( $1, $descriptor( PvBuffer **), 0 );
}
#endif
%typemap(directorargout) PvBuffer **
{
    void *lBuffer;
    int lRes = SWIG_ConvertPtr( $result, &lBuffer, $descriptor( PvBuffer * ), 0 );
    if( !SWIG_IsOK( lRes ) )
    {
        Swig::DirectorTypeMismatchException::raise(SWIG_ErrorType(SWIG_ArgError(lRes)), "in output value of type 'PvBuffer *'");
    }
    *($1) = reinterpret_cast<PvBuffer *>( lBuffer );
}

%feature("director") IPvStreamingChannelSource;
%feature("director") IPvSoftDeviceGEVEventSink;
%feature("director") IPvRegisterEventSink;
%feature("director") IPvUserSetNotify;

// Typemaps for ReadFloat
%typemap(in,numinputs=0) float & ( float lTmp )
{
    $1 = &lTmp;
}
%typemap(argout) float &
{
    PyObject *lStr = Py_None;
    PyObject *lRes = $result;
    $result = PyTuple_New(2);
    PyTuple_SetItem( $result, 0, lRes );
    if ( result.IsOK() )
    {
        lStr = PyFloat_FromDouble( *$1 );
    }
    PyTuple_SetItem( $result, 1, lStr );
}

// Typemaps for ReadDouble
%typemap(in,numinputs=0) double & ( double lTmp )
{
    $1 = &lTmp;
}
%typemap(argout) double &
{
    PyObject *lStr = Py_None;
    PyObject *lRes = $result;
    $result = PyTuple_New(2);
    PyTuple_SetItem( $result, 0, lRes );
    if ( result.IsOK() )
    {
        lStr = PyFloat_FromDouble( *$1 );
    }
    PyTuple_SetItem( $result, 1, lStr );
}


// extend Read to ReadString and ReadInt and ReadBytes
%ignore IPvRegister::Read( uint32_t &aValue, uint32_t aOffset = 0 );
%ignore IPvRegister::Read( PvString &aValue );
%ignore IPvRegister::Read( uint8_t *aData, uint32_t aByteCount, uint32_t aOffset = 0 );
%extend IPvRegister
{
    PvResult ReadString( PvString &aValue )
    {
        PvResult lRes = $self->Read( aValue );
        return lRes;
    }

    PvResult ReadInt( uint32_t &aValue, uint32_t aOffset = 0 )
    {
        PvResult lRes = $self->Read( aValue, aOffset );
        return lRes;
    }

    PvResult ReadBytes( uint8_t *aData, uint32_t aByteCount, uint32_t aOffset = 0 )
    {
        PvResult lRes = $self->Read( aData, aByteCount, aOffset );
        return lRes;
    }
};

// Typemaps for GetGenICamXMLFile and ReadString
%typemap(in,numinputs=0) PvString & ( PvString lTmp )
{
    $1 = &lTmp;
}
%typemap(argout) PvString &
{
    PyObject *lStr = Py_None;
    PyObject *lRes = $result;
    $result = PyTuple_New(2);
    PyTuple_SetItem( $result, 0, lRes );
    if ( result.IsOK() )
    {
        lStr = SWIG_FromWCharPtr( $1->GetUnicode() );
    }
    PyTuple_SetItem( $result, 1, lStr );
}

// Typemaps for ReadInt
%typemap(in,numinputs=0) uint32_t & ( uint32_t lTmp )
{
    $1 = &lTmp;
}
%typemap(argout) uint32_t &
{
    PyObject *lInt = Py_None;
    PyObject *lRes = $result;
    $result = PyTuple_New(2);
    PyTuple_SetItem( $result, 0, lRes );
    if ( result.IsOK() )
    {
        lInt = PyLong_FromLong(*$1);
    }
    PyTuple_SetItem( $result, 1, lInt );
}


// IPvRegister::ReadBytes needs to suppress the data input in order to return it.
// A typecheck is mandatory to let Swig generate dispatching rules since the ReadBytes has an
// optional argument which has been overloaded by Swig, and we are typemapping the arguments
// after.
%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) ( uint8_t *aData, uint32_t aByteCount )
{
    if (!PyInt_Check($input))
    {
        const char* typestring = pytype_string($input);
        PyErr_Format(PyExc_TypeError,
                    "Int dimension expected.  '%s' given.",
                    typestring);
    }
}
%apply( uint8_t *ARGOUT_ARRAY1, int DIM1 )
{
    ( uint8_t *aData, uint32_t aByteCount )
}


%typemap(constcode) int {
    PyObject *val = PyLong_FromLong(($type)($value));
    SWIG_Python_SetConstant(d, "$1", val);
    const char *name = "$typemap(enum_realname,$1_type)";
    PyObject *e = PyDict_GetItemString(d, name);
    if (!e) PyDict_SetItemString(d, name, e = PyDict_New());
    PyDict_SetItemString(e, "$value", val);
}

%include "IPvUserSetNotify.h"
%include "PvSoftDeviceGEVInterfaces.h"
%include "PvSoftDeviceGEV.h"

%typemap(out) PvResult GetGenICamXMLFile;
%typemap(in,numinputs=0) float &;
%typemap(argout) float &;
%typemap(in,numinputs=0) double &;
%typemap(argout) double &;
%typemap(in,numinputs=0) PvString &;
%typemap(argout) PvString &;
%typemap(in,numinputs=0) uint32_t &;
%typemap(argout) uint32_t &;
%clear( uint8_t *aData, uint32_t aByteCount );

#define PV_APPUTILS_API
%include "PvFPSStabilizer.h"

%feature("director") PvStreamingChannelSourceDefault;
%include "PvStreamingChannelSourceDefault.h"

%typemap(directorargout) PvBuffer **;

#endif // SWIG_EBUS_LIB
