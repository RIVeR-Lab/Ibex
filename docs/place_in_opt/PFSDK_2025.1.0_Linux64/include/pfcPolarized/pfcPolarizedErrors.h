#ifndef _POLARIZEDERRORS_H
#define _POLARIZEDERRORS_H

#include "pfcPolarizedDefs.h"
#include "pfpolclib_export.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define PFPOLSDK_NOERROR									0

// Generic Error Codes
#define PFPOLSDK_ERROR_IMAGE_DIMENSIONS_OUT_OF_RANGE		0x80010000
#define PFPOLSDK_ERROR_ALLOCATE_POINTER						0x80020000
#define PFPOLSDK_ERROR_EMPTY_POINTER						0x80030000
#define PFPOLSDK_ERROR_INVALID_ARGUMENT						0x80040000
#define PFPOLSDK_ERROR_WRITE_IMAGE_TO_FILE					0x80070000
#define PFPOLSDK_ERROR_READ_IMAGE_FROM_FILE					0x80080000
#define PFPOLSDK_ERROR_INPUT_VALUE_OUT_OF_BOUNDS			0x80100000
#define PFPOLSDK_ERROR_CANNOT_OPEN_FILE						0x80110000

#define PFPOLSDK_ERROR_UNKNOWN								0xFFFF0000


PFPOLCLIB_EXPORT void pfGetErrorString(int32_t nError, char *pErrorStr, char *pErrorDescription);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif 
