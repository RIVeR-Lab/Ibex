cmake_minimum_required (VERSION 3.15)

# Maps to Visual Studio solution file (ConfigAndGrab_Console_Events.sln)
# The solution will have all targets (exe, lib, dll) as Visual Studio projects (.vcproj)
project (PFCameraLib_ConfigAndGrab_Console_Events)

if(NOT TARGET Photonfocus::PFCameraLib)
	find_package(PFBase CONFIG REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
endif()

add_executable(PFCameraLib_ConfigAndGrab_Console_Events ConfigAndGrab_Console_Events.cpp)
#set_target_properties(PFCameraLib_ConfigAndGrab_Console_Events PROPERTIES OUTPUT_NAME ConfigAndGrab_Console_Events)
target_link_libraries(PFCameraLib_ConfigAndGrab_Console_Events PRIVATE Photonfocus::PFCameraLib)
target_compile_definitions(PFCameraLib_ConfigAndGrab_Console_Events PRIVATE UNICODE)
set_target_properties(PFCameraLib_ConfigAndGrab_Console_Events PROPERTIES FOLDER PFCameraLib/Examples/C++)