#ifndef __WINTYPEDEFS_H
#define __WINTYPEDEFS_H

#define _WINDEF_

#define WINAPI
#define IN
#define INOUT
#define OUT
#define __cdecl
#define _cdecl
#define __declspec(x) __attribute__((x))
#define dllimport
#define dllexport visibility ("default")

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_PATH	PATH_MAX
#define _MAX_PATH	PATH_MAX

#define INVALID_HANDLE_VALUE ((void*)-1)
#define ERROR_SUCCESS 0
#ifndef FALSE
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    1
#endif

#define CONST	const
#define VOID	void

#if defined(__x86_64__) || defined(__aarch64__)
#define __int3264 long
#else
#define __int3264 int
#endif
typedef void*				HANDLE;
typedef HANDLE*				PHANDLE;
typedef HANDLE*				LPHANDLE;
typedef unsigned short		USHORT;
typedef uint32_t			DWORD;
typedef unsigned char		BYTE;
typedef BYTE				byte;
typedef unsigned short		WORD;
typedef float				FLOAT;
typedef float*				PFLOAT;
typedef int*				PBOOL;
typedef int*				LPBOOL;
typedef BYTE*				PBYTE;
typedef BYTE*				LPBYTE;
typedef int*				PINT;
typedef int*				LPINT;
typedef WORD*				PWORD;
typedef WORD*				LPWORD;
typedef int32_t*			LPLONG;
typedef DWORD*				PDWORD;
typedef DWORD*				LPDWORD;
typedef void*				LPVOID;
typedef void*				PVOID;
typedef	size_t				SIZE_T;
typedef char				CHAR;
typedef wchar_t				WCHAR;
typedef char*				PCHAR;
typedef char*				PSTR;
typedef unsigned char		UCHAR;
typedef unsigned char*		PUCHAR;
typedef short				SHORT;
typedef int32_t				LONG;
typedef uint32_t			ULONG;
typedef uint64_t			ULONGLONG;
typedef uint64_t			ULONG64;
typedef int64_t				LONGLONG;
typedef BYTE				BOOLEAN;
typedef int					INT;
typedef unsigned int		UINT;
typedef uint8_t				UINT8;
typedef uint16_t			UINT16;
typedef uint32_t			UINT32;
typedef uint64_t			UINT64;
typedef int8_t				INT8;
typedef int32_t				INT32;
typedef int64_t				INT64;
typedef unsigned int*		PUINT;
typedef const char*			LPCSTR;
typedef char*				LPSTR;
typedef wchar_t*			LPWSTR;
typedef const wchar_t*		LPCWSTR;
typedef ULONGLONG			DWORDLONG;
typedef ULONGLONG*			PDWORDLONG;
typedef unsigned __int3264	UINT_PTR;
typedef __int3264			LONG_PTR;
typedef LPCSTR				LPCTSTR;
typedef LPSTR				LPTSTR;
typedef unsigned __int3264	ULONG_PTR;

typedef struct tagSIZE
{
	LONG        cx;
	LONG        cy;
} SIZE, *PSIZE, *LPSIZE;

#endif // __WINTYPEDEFS_H
