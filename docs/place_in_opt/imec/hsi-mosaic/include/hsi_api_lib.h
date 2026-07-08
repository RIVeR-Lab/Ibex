/////////////////////////////////////////////////////
//
//  Date: 6 July, 2020
//
//  Authors:
//    Wouter Charle
//
//  Copyright 2020 imec. All rights reserved.
//
////////////////////////////////////////////

#ifndef HSI_API_LIB_H
#define HSI_API_LIB_H


#if defined(_MSC_VER) || defined(__CYGWIN__)
// MSVC and CYGWIN compilers

// import definitions using SNAPSCAN_API and SNAPSCAN_API_LOCAL
# define HSI_API __declspec(dllimport)
# define HSI_API_LOCAL

// automatic linkage
# ifndef HSI_API_NO_AUTO_LINK
//  link to import library
#   pragma comment(lib,"hsi_api.lib")
# endif

#else
// not MSVC compiler
# define HSI_API
# define HSI_API_LOCAL
#endif


#endif // HSI_API_LIB_H
