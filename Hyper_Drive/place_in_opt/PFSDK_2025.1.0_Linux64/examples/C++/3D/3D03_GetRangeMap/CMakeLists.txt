cmake_minimum_required (VERSION 3.15)

project (3D03_GetRangeMap CXX)

if(NOT TARGET Photonfocus::PFCameraLib)
	find_package(PFBase CONFIG REQUIRED)
endif()
if(NOT TARGET Photonfocus::PF3DLib)
	find_package(PF3DLib CONFIG REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
endif()


add_executable(3D03_GetRangeMap 
        3D03_GetRangeMap.cpp
        Configure_MVx-3D03.cpp)

set_target_properties(
    3D03_GetRangeMap
    PROPERTIES
    FOLDER "3D/C++/Samples")

target_link_libraries(3D03_GetRangeMap PRIVATE 
    Photonfocus::PFCameraLib
    Photonfocus::PF3DLib)

target_compile_definitions(3D03_GetRangeMap PRIVATE UNICODE)


