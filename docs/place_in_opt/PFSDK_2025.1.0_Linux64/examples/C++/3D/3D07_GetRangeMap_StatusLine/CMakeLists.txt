cmake_minimum_required (VERSION 3.15)

project (3D07_GetRangeMap_StatusLine CXX)

if(NOT TARGET Photonfocus::PFCameraLib)
    if(WIN32)
        find_package(PFBase CONFIG REQUIRED)
    else()
        find_package(PFBase CONFIG REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../../) 
    endif()
endif()

if(NOT TARGET Photonfocus::PF3DLib)
    if(WIN32)
        find_package(PF3DLib CONFIG REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
    else()
        find_package(PF3DLib CONFIG REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../../)
    endif()
endif()

add_executable(3D07_GetRangeMap_StatusLine 
        3D07_GetRangeMap_StatusLine.cpp
        Configure_MVx-D1280-L01-3D07.cpp)

set_target_properties(3D07_GetRangeMap_StatusLine PROPERTIES FOLDER "3D/C++/Samples")

target_link_libraries(3D07_GetRangeMap_StatusLine PRIVATE 
    Photonfocus::PFCameraLib
    Photonfocus::PF3DLib)

target_compile_definitions(3D07_GetRangeMap_StatusLine PRIVATE UNICODE)
