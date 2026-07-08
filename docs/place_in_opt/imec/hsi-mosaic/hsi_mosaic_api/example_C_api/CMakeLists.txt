cmake_minimum_required(VERSION 3.10)

message("*** Setting up hsi_mosaic_api_example")

project (hsi_mosaic_api_example)

message("CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
message("CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
message("PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})

add_definitions(
	-DIMEC_LINUX
)

if(UNIX)
  set(MY_DEBUG_OPTIONS -g -std=c++17 -Wall -Wno-deprecated-declarations -Wno-unused-local-typedefs -Wno-unknown-pragmas -fpermissive)
  set(MY_RELEASE_OPTIONS -O3 -std=c++17 -Wall -Wno-deprecated-declarations -Wno-unused-local-typedefs -Wno-unknown-pragmas -Wno-unused-variable -fpermissive)

  set(LIB_DIR "./lib/")
  set(BIN_DIR "./bin/")
endif()
if(WIN32)
  set(MY_DEBUG_OPTIONS /std:c++17 /utf-8 /TP /MDd /D_DEBUG /GS)
  set(MY_RELEASE_OPTIONS /std:c++17 /utf-8 /TP /MD /MP /GS)  

  set(LIB_DIR "${CMAKE_BINARY_DIR}/../../../lib/")
  set(BIN_DIR "${CMAKE_BINARY_DIR}/../../../bin/")
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

message("HSI_MOSAIC: " ${HSI_MOSAIC})

add_executable(${PROJECT_NAME}
    main.cpp
)

target_include_directories(${PROJECT_NAME} 
	PUBLIC
	.
	..
	${PROJECT_SOURCE_DIR}/../../include)
if(UNIX)
target_link_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/../../bin
)
endif()

if(WIN32)
target_link_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/../../lib
)
endif()

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    hsi_api
    hsi_mosaic_api
)
