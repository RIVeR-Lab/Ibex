cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)


project(xiAPI-capture-10-concatenated-images-to-file${PROJECT_NAME_EXTENSION})

list(APPEND SOURCES
    main.cpp
    ../../_libs/GenTL/GenTLStream.cpp
	../../_libs/GenTL/xi_GenTL.cpp
	../../_libs/GenTL/xi_exception.cpp
	../../_libs/GenTL/xi_ImgBuffer.cpp
        ../../_libs/xiAPI/xiAPI_tiff
    )

include_directories(../../_libs/GenTL)
include_directories(../../_libs)
include_directories(../../_libs/libtiff/libtiff)

include(../../_libs/_cmake/add_xiapi.cmake)
include(../../_libs/_cmake/add_libtiff.cmake)

include(../../_libs/_cmake/add_executable.cmake)
