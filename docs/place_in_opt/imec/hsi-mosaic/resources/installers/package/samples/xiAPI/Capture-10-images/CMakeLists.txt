cmake_minimum_required(VERSION 2.8)

project(xiAPI-capture-10-images${PROJECT_NAME_EXTENSION})

list(APPEND SOURCES
    main.cpp
    )

include_directories(../../_libs)

include(../../_libs/_cmake/add_xiapi.cmake)

include(../../_libs/_cmake/add_executable.cmake)
