cmake_minimum_required(VERSION 2.8)

project(xiAPI-capture-10-images-to-file${PROJECT_NAME_EXTENSION})

list(APPEND SOURCES
    main
    ../../_libs/xiAPI/xiAPI_tiff
    )

include_directories(../../_libs)

include(../../_libs/_cmake/add_xiapi.cmake)
include(../../_libs/_cmake/add_libtiff.cmake)

include(../../_libs/_cmake/add_executable.cmake)
