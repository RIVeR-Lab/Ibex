cmake_minimum_required (VERSION 3.15)

# Maps to Visual Studio solution file (ConfigAndGrabConsole.sln)
# The solution will have all targets (exe, lib, dll) as Visual Studio projects (.vcproj)
project (PFCameraLib_ConnectConfigAndGrab_Console)
 
if(NOT TARGET Photonfocus::PFCameraLib)
	find_package(PFBase CONFIG REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
endif()


add_executable(PFCameraLib_ConnectConfigAndGrab_Console ConnectConfigAndGrab_Console.cpp)
#set_target_properties(PFCameraLib_ConnectConfigAndGrab_Console PROPERTIES OUTPUT_NAME ConfigAndGrab_Console)
target_link_libraries(PFCameraLib_ConnectConfigAndGrab_Console PRIVATE Photonfocus::PFCameraLib)
target_compile_definitions(PFCameraLib_ConnectConfigAndGrab_Console PRIVATE UNICODE)
set_target_properties(PFCameraLib_ConnectConfigAndGrab_Console PROPERTIES FOLDER PFCameraLib/Examples/C++)