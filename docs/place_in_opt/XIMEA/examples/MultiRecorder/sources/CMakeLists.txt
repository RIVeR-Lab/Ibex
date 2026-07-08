cmake_minimum_required(VERSION 3.5)

project(MultiRecorderProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
elseif (APPLE)
    add_definitions(-DMACOS)
elseif (UNIX)
    add_definitions(-DLINUX)
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build-cmake)

# when sources are shipped in SP, FIS_Lib, libTIFF and TinyXml are in subdirectories
set(SUPPORT_LIBS_PATH "${PROJECT_SOURCE_DIR}")
set(SUPPORT_APICPP_PATH "${PROJECT_SOURCE_DIR}/xiApi")
set(SUPPORT_DLLS_PATH "${PROJECT_SOURCE_DIR}")
if (NOT EXISTS "${SUPPORT_LIBS_PATH}/tinyxml")
    set(SUPPORT_LIBS_PATH "${PROJECT_SOURCE_DIR}/../../Libs")
    set(SUPPORT_APICPP_PATH "${PROJECT_SOURCE_DIR}/../../m3api/xiApi")
    include_directories(../../Include)
    if(WIN32)
        set(SUPPORT_DLLS_PATH "${PROJECT_SOURCE_DIR}/../../bin")
    elseif (APPLE)
        set(SUPPORT_DLLS_PATH "${PROJECT_SOURCE_DIR}}/../../bin/OSX/")
    elseif (UNIX)
        set(SUPPORT_DLLS_PATH "${PROJECT_SOURCE_DIR}/../../../bin/X64")
    endif()
else()
    include_directories(${SUPPORT_APICPP_PATH})
endif()

# use std::string in TinyXml library
add_definitions(-DTIXML_USE_STL)
include_directories("${SUPPORT_LIBS_PATH}/tinyxml")

# increase timeouts to 30s in httpslib
add_definitions(
    -DCPPHTTPLIB_KEEPALIVE_TIMEOUT_SECOND=30
    -DCPPHTTPLIB_WRITE_TIMEOUT_SECOND=30
    -DCPPHTTPLIB_READ_TIMEOUT_SECOND=30)

add_executable(MultiRecorder
    main.cpp
    CameraControl.cpp
    CameraControl.h
    CameraServer.cpp
    CameraServer.h
    ImageExporter.cpp
    ImageExporter.h
    LiveImagePreview.cpp
    LiveImagePreview.h
    CameraClient.cpp
    CameraClient.h
    RecorderApi.cpp
    RecorderApi.h
    RecordingTask.h
    RecordingTask.cpp
    tests.h
    tests.cpp
    httplib/httplib.h
    code_samples.h code_samples.cpp
    UserImageProcessor.h UserImageProcessor.cpp
    ${SUPPORT_LIBS_PATH}/tinyxml/tinystr.cpp
    ${SUPPORT_LIBS_PATH}/tinyxml/tinyxml.h
    ${SUPPORT_LIBS_PATH}/tinyxml/tinyxml.cpp
    ${SUPPORT_LIBS_PATH}/tinyxml/tinyxmlerror.cpp
    ${SUPPORT_LIBS_PATH}/tinyxml/tinyxmlparser.cpp
    ${SUPPORT_APICPP_PATH}/xiApiEnumerators.cpp
)
target_compile_features(MultiRecorder PRIVATE cxx_std_17)

# enable recording using FIS
set(MULTIRECORDER_USE_FIS ON)
if(MULTIRECORDER_USE_FIS)
    add_definitions(-DMULTIRECORDER_FIS_ENABLE)
    target_sources(MultiRecorder PRIVATE
        ${SUPPORT_LIBS_PATH}/FIS_Lib/FIS_lib.h
        ${SUPPORT_LIBS_PATH}/FIS_Lib/FIS_lib.cpp
        ${SUPPORT_LIBS_PATH}/FIS_Lib/FIS_lib_hdd.cpp
        ${SUPPORT_LIBS_PATH}/FIS_Lib/FIS_lib_impl.cpp
        ${SUPPORT_LIBS_PATH}/FIS_Lib/FIS_lib_ram.cpp
        ${SUPPORT_LIBS_PATH}/FIS_Lib/FIS_camera_io.h
        ${SUPPORT_LIBS_PATH}/FIS_Lib/FIS_camera_io.cpp
        ${SUPPORT_LIBS_PATH}/FIS_Lib/FIS_image_metadata.h
        ${SUPPORT_LIBS_PATH}/FIS_Lib/FIS_image_metadata.cpp
    )
    # enable persistance in FIS_Lib
    add_definitions(-DFIS_PERSISTANCE)
    include_directories("${SUPPORT_LIBS_PATH}/FIS_Lib")
endif()

# copy samples
add_custom_command(TARGET MultiRecorder POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/samples
        ${EXECUTABLE_OUTPUT_PATH}/samples
)

# Enable saving to TIFF (and the requirement for libtiff)
add_definitions(-DMULTIRECORDER_TIFF_ENABLE)

# Build libTIFF as External project
include(ExternalProject)
set(LIBTIFF_SOURCE_DIR "${SUPPORT_LIBS_PATH}/libtiff_latest")
set(LIBTIFF_BINARY_DIR "${CMAKE_BINARY_DIR}/libtiff-build")

set(LIBTIFF_LIB "tiff")
if(MSVC AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    set(LIBTIFF_LIB "tiffd")
endif()

if(EXISTS "${LIBTIFF_SOURCE_DIR}/CMakeLists.txt")
    message(STATUS "Using local libtiff sources at ${LIBTIFF_SOURCE_DIR}")
    ExternalProject_Add(
        libtiff_project
        PREFIX ${LIBTIFF_BINARY_DIR}
        SOURCE_DIR ${LIBTIFF_SOURCE_DIR}
        BINARY_DIR ${LIBTIFF_BINARY_DIR}
        CMAKE_ARGS
            -DBUILD_SHARED_LIBS=OFF           # Ensure static build
            -DCMAKE_BUILD_TYPE:STRING=$<CONFIG>
            -DCMAKE_INSTALL_PREFIX=${LIBTIFF_BINARY_DIR}/install    # Install directory
            -Dtiff-contrib=OFF
            -Dtiff-docs=OFF
            -Dtiff-tests=OFF
            -Dtiff-tools=OFF
            -Djbig=OFF                        # Disable JBIG
            -Djpeg=OFF                        # Disable JPEG compression
            -Dwebp=OFF                        # Disable WEBP compression
            -Dlzma=OFF                        # Disable LZMA compression
            -Dzlib=OFF                        # Disable ZIP compression
            -Dzstd=OFF                        # Disable Zstd compression
        PATCH_COMMAND ""
        TEST_COMMAND ""
        BYPRODUCTS ${LIBTIFF_BINARY_DIR}/install/lib/libtiff.a
    )
else()
    message(STATUS "Downloading libtiff sources into ${LIBTIFF_SOURCE_DIR} ...")
    ExternalProject_Add(
        libtiff_project
        PREFIX ${LIBTIFF_BINARY_DIR}
        GIT_REPOSITORY https://gitlab.com/libtiff/libtiff.git
        GIT_TAG v4.7.0
        GIT_SHALLOW TRUE
        SOURCE_DIR ${LIBTIFF_SOURCE_DIR}   # clone into this folder
        BINARY_DIR ${LIBTIFF_BINARY_DIR}
        CMAKE_ARGS
            -DBUILD_SHARED_LIBS=OFF
            -DCMAKE_BUILD_TYPE:STRING=$<CONFIG>
            -DCMAKE_INSTALL_PREFIX=${LIBTIFF_BINARY_DIR}/install
            -Dtiff-contrib=OFF
            -Dtiff-docs=OFF
            -Dtiff-tests=OFF
            -Dtiff-tools=OFF
            -Djbig=OFF
            -Djpeg=OFF
            -Dwebp=OFF
            -Dlzma=OFF
            -Dzlib=OFF
            -Dzstd=OFF
        PATCH_COMMAND ""
        TEST_COMMAND ""
        BYPRODUCTS ${LIBTIFF_BINARY_DIR}/install/lib/libtiff.a
    )
endif()

# Make sure to add a dependency so that libTIFF builds before your application
add_dependencies(MultiRecorder libtiff_project)

# Link against the built libTIFF library
target_link_directories(MultiRecorder PRIVATE ${LIBTIFF_BINARY_DIR}/install/lib)
target_link_libraries(MultiRecorder PRIVATE ${LIBTIFF_LIB})
target_include_directories(MultiRecorder PRIVATE ${LIBTIFF_BINARY_DIR}/install/include)

if(WIN32)
    target_link_directories(MultiRecorder PRIVATE ${SUPPORT_DLLS_PATH})
    if(CMAKE_CL_64)
        target_link_libraries(MultiRecorder PRIVATE xiapi64)
    else()
        target_link_libraries(MultiRecorder PRIVATE xiapi32)
    endif()
else()
    find_library(XIAPI m3api)
    if(XIAPI)
        target_link_libraries(MultiRecorder PRIVATE ${XIAPI})
    else()
        if (APPLE)
            #target_link_directories(MultiRecorder PRIVATE ${SUPPORT_DLLS_PATH})
            #target_link_libraries(MultiRecorder "-framework m3api")
            find_library(XIAPI_MAC NAMES m3api PATHS ${SUPPORT_DLLS_PATH} REQUIRED NO_DEFAULT_PATH)
            target_link_libraries(MultiRecorder PRIVATE ${XIAPI_MAC})
        else()
            target_link_directories(MultiRecorder PRIVATE ${SUPPORT_DLLS_PATH})
            target_link_libraries(MultiRecorder PRIVATE m3api pthread dl)
        endif()
    endif()
endif()

# **** Optional OpenCV usage ****
# Link OpenCV library to the app to enable live image preview?
# When MULTIREC_OPENCV_LINKED not set, the app will try to load OpenCV dynamically, without it being used for the compilation.

#set(MULTIREC_OPENCV_LINKED on)

if(MULTIREC_OPENCV_LINKED)
    add_definitions(-DMULTIREC_OPENCV_LINKED)
    if (MSVC)
        set(OPENCV_DIR "${SUPPORT_LIBS_PATH}/opencv_3.4.1/x64")
        include_directories("${OPENCV_DIR}/../include")
        target_link_directories(MultiRecorder PRIVATE "${OPENCV_DIR}/lib")

        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_link_libraries(MultiRecorder PRIVATE opencv_world341d)
        else()
            target_link_libraries(MultiRecorder PRIVATE opencv_world341)
        endif()
    else()
        include_directories("/usr/local/include/opencv4")
        target_link_directories(MultiRecorder PRIVATE "/usr/local/lib")
        target_link_libraries(MultiRecorder PRIVATE opencv_core opencv_highgui opencv_imgproc)
    endif()
endif()
