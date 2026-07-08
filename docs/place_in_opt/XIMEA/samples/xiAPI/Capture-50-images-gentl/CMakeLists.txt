cmake_minimum_required(VERSION 2.8)

project(xiAPI-capture-50-images-gentl${PROJECT_NAME_EXTENSION})

list(APPEND SOURCES
    main.cpp
    ../../_libs/GenTL/GenTLStream.cpp
	../../_libs/GenTL/xi_GenTL.cpp
	../../_libs/GenTL/xi_exception.cpp
	../../_libs/GenTL/xi_ImgBuffer.cpp
    )

include_directories(../../_libs/GenTL)
include_directories(../../_libs)

include(../../_libs/_cmake/add_xiapi.cmake)

include(../../_libs/_cmake/add_executable.cmake)
