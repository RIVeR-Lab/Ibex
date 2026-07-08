cmake_minimum_required (VERSION 3.15)

# Maps to Visual Studio solution file (ConfigAndGrabConsole.sln)
# The solution will have all targets (exe, lib, dll) as Visual Studio projects (.vcproj)
project (PFCameraLib_ConfigAndGrabConsole_SoftwareTrigger)
 
if(NOT TARGET Photonfocus::PFCameraLib)
	find_package(PFBase CONFIG REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
endif()

find_package(Threads REQUIRED)

add_executable(PFCameraLib_ConfigAndGrabConsole_SoftwareTrigger ConfigAndGrab_Console_SoftwareTrigger.cpp)
#set_target_properties(PFCameraLib_ConfigAndGrabConsole PROPERTIES OUTPUT_NAME "ConfigAndGrabConsole")
set_target_properties(PFCameraLib_ConfigAndGrabConsole_SoftwareTrigger PROPERTIES FOLDER PFCameraLib/Examples/C++)
target_link_libraries(PFCameraLib_ConfigAndGrabConsole_SoftwareTrigger PRIVATE Photonfocus::pfcTypes Photonfocus::PFCameraLib Threads::Threads)
target_compile_definitions(PFCameraLib_ConfigAndGrabConsole_SoftwareTrigger PRIVATE UNICODE)