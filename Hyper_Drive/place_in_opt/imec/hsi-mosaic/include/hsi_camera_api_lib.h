/////////////////////////////////////////////////////
//
//  Date: 9 April, 2018
//
//  Authors:
//    Wouter Charle
//
//  Copyright 2020 imec. All rights reserved.
//
////////////////////////////////////////////

#ifndef HSI_CAMERA_API_LIB_H
#define HSI_CAMERA_API_LIB_H


#if defined(_MSC_VER) || defined(__CYGWIN__)
// MSVC and CYGWIN compilers

// import definitions using HSI_CAMERA_API and HSI_CAMERA_API_LOCAL
# define HSI_CAMERA_API __declspec(dllimport)
# define HSI_CAMERA_API_LOCAL

// automatic linkage
# ifndef HSI_CAMERA_NO_AUTO_LINK
//  link to import library
#   pragma comment(lib,"hsi_camera_api.lib")
# endif

#else
// not MSVC compiler
# define HSI_CAMERA_API
# define HSI_CAMERA_API_LOCAL
#endif


#endif // HSI_CAMERA_API_LIB_H
