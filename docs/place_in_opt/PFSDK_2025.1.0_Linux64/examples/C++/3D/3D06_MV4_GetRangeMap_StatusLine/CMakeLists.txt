cmake_minimum_required (VERSION 3.15)

project (3D06_MV4_GetRangeMap_StatusLine CXX)

if(NOT TARGET Photonfocus::PFCameraLib)
	find_package(PFBase CONFIG REQUIRED)
endif()

if(NOT TARGET Photonfocus::PF3DLib)
	find_package(PF3DLib CONFIG REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
endif()


add_executable(3D06_MV4_GetRangeMap_StatusLine 
        3D06_MV4_GetRangeMap_StatusLine.cpp
        Configure_MV4-3D06.cpp)

set_target_properties(
    3D06_MV4_GetRangeMap_StatusLine
    PROPERTIES
    FOLDER "3D/C++/Samples")

target_link_libraries(3D06_MV4_GetRangeMap_StatusLine PRIVATE 
    Photonfocus::PFCameraLib
    Photonfocus::PF3DLib)

target_compile_definitions(3D06_MV4_GetRangeMap_StatusLine PRIVATE UNICODE)


