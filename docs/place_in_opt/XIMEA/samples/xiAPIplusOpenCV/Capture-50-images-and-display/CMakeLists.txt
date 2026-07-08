cmake_minimum_required(VERSION 2.8)

project(xiAPIplusOpenCV-Capture-50-images-and-display${PROJECT_NAME_EXTENSION})

list(APPEND SOURCES
    main.cpp
    ../../_libs/xiAPIplusOpenCV/xiApiPlusOcv
    )

include_directories(../../_libs)

include(../../_libs/_cmake/add_xiapi.cmake)
include(../../_libs/_cmake/add_opencv.cmake)

include(../../_libs/_cmake/add_executable.cmake)
