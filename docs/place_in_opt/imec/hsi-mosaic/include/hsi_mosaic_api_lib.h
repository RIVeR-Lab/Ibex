/////////////////////////////////////////////////////
//
//  Date: 3 November, 2020
//
//  Authors:
//    Wouter Charle
//
//  Copyright 2020 imec. All rights reserved.
//
////////////////////////////////////////////

#ifndef HSI_MOSAIC_API_LIB_H
#define HSI_MOSAIC_API_LIB_H


#if defined(_MSC_VER) || defined(__CYGWIN__)
// MSVC and CYGWIN compilers

// import definitions using HSI_MOSAIC_API and HSI_MOSAIC_API_LOCAL
# define HSI_MOSAIC_API __declspec(dllimport)
# define HSI_MOSAIC_API_LOCAL

// automatic linkage
# ifndef HSI_MOSAIC_NO_AUTO_LINK
//  link to import library
#   pragma comment(lib,"hsi_mosaic_api.lib")
# endif

#else
// not MSVC compiler
# define HSI_MOSAIC_API
# define HSI_MOSAIC_API_LOCAL
#endif


#endif // HSI_MOSAIC_API_LIB_H
