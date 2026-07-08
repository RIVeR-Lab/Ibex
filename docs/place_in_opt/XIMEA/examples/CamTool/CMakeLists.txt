cmake_minimum_required(VERSION 3.16)
project(plugins VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui PrintSupport Widgets Xml)

if((QT_VERSION_MAJOR GREATER 5))
    qt_standard_project_setup()
else()
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED YES)
endif()

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

# Destination directory for all targets
if(WIN32)
    add_compile_definitions(WIN32)
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        set(ARCH_DIR win/x86)
    else()
        set(ARCH_DIR win/x64)
    endif()
elseif(APPLE)
    set(ARCH_DIR macx)
elseif(LINUX)
    if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64")
        set(ARCH_DIR linux/jetson-tx2)
        add_compile_definitions(XI_OS_LINUX64)
    elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        set(ARCH_DIR linux/x86)
        add_compile_definitions(XI_OS_LINUX32)
    else()
        set(ARCH_DIR linux/x64)
        add_compile_definitions(XI_OS_LINUX64)
    endif()
endif()

set(SAMPLE_ROOT "${CMAKE_CURRENT_LIST_DIR}/xvpSample")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DESTDIR "${SAMPLE_ROOT}/bin/${ARCH_DIR}/debug")
else()
    set(DESTDIR "${SAMPLE_ROOT}/bin/${ARCH_DIR}/release")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DESTDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DESTDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DESTDIR})

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

if(MSVC)
    # Prevent MSVC builder to make additional subdir for Debug and Release
    foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${DESTDIR})
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${DESTDIR})
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${DESTDIR})
    endforeach()
endif()

#----xiAPI----
if(WIN32)
    set(XIMEA_DIR "C:/XIMEA")
    include_directories("${XIMEA_DIR}/API/xiAPI")
elseif(APPLE)
    set(XIMEA_DIR "/library/Frameworks/m3api.framework")
    include_directories("${XIMEA_DIR}/Headers")
elseif(LINUX)
    set(XIMEA_DIR "/opt/XIMEA")
    include_directories("${XIMEA_DIR}/include")
endif()

#----CamTool libraries----
include_directories(
    ${SAMPLE_ROOT}/include/xiCore
    ${SAMPLE_ROOT}/include/xiCoreGui
    ${SAMPLE_ROOT}/include/qcustomplot
)

if(WIN32)
    add_compile_definitions(WIN32)
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        link_directories("${SAMPLE_ROOT}/lib/win/x86")
    else()
        link_directories("${SAMPLE_ROOT}/lib/win/x64")
    endif()
elseif(LINUX)
    link_directories("${XIMEA_DIR}/CamTool")
endif()

#----Sample plugin----
add_subdirectory(xvpSample)
