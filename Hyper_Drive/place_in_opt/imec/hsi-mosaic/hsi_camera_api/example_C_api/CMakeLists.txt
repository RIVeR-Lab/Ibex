cmake_minimum_required(VERSION 3.10)

message("*** Setting up hsi_camera_api_example")

project (hsi_camera_api_example)

message("PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
message("CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
message("CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})

if(UNIX)
  set(MY_DEBUG_OPTIONS -g -std=c++17 -Wall -Wno-deprecated-declarations -Wno-unused-local-typedefs -Wno-unknown-pragmas -fpermissive)
  set(MY_RELEASE_OPTIONS -O3 -std=c++17 -Wall -Wno-deprecated-declarations -Wno-unused-local-typedefs -Wno-unknown-pragmas -Wno-unused-variable -fpermissive)

  set(LIB_DIR "./lib/")
  set(BIN_DIR "./bin/")
endif()
if(WIN32)
  set(MY_DEBUG_OPTIONS /std:c++17 /utf-8 /TP /MDd /D_DEBUG /GS)
  set(MY_RELEASE_OPTIONS /std:c++17 /utf-8 /TP /MD /MP /GS)  

  set(LIB_DIR "${PROJECT_SOURCE_DIR}/../../lib/")
  set(BIN_DIR "${PROJECT_SOURCE_DIR}/../../bin/")
endif()

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  "${BIN_DIR}" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  "${LIB_DIR}" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  "${LIB_DIR}" )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${BIN_DIR}" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${LIB_DIR}" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${LIB_DIR}" )

add_compile_options(
  "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>"
  "$<$<CONFIG:RELEASE>:${MY_RELEASE_OPTIONS}>")

add_executable(${PROJECT_NAME}
    main.cpp 
)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_d" VERSION 1.0)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    hsi_api
    hsi_camera_api
)

target_link_directories(${PROJECT_NAME}
    PUBLIC
if(UNIX)
    ${PROJECT_SOURCE_DIR}/../../bin
endif()    
if(WIN32)
    ${PROJECT_SOURCE_DIR}/../../lib
endif()
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/../../include
)


