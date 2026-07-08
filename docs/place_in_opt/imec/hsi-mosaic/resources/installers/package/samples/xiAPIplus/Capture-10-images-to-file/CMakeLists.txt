cmake_minimum_required(VERSION 2.8)

project(xiAPIplus-capture-10-images-to-file${PROJECT_NAME_EXTENSION})

list(APPEND SOURCES
    main.cpp
    ../../_libs/xiAPIplus/xiAPIplus_core
    ../../_libs/xiAPIplus/xiAPIplus_tiff
    )

include_directories(../../_libs)

include(../../_libs/_cmake/add_xiapi.cmake)
include(../../_libs/_cmake/add_libtiff.cmake)

include(../../_libs/_cmake/add_executable.cmake)
