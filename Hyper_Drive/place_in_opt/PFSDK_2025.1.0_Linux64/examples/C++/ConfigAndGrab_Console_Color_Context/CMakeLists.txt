cmake_minimum_required (VERSION 3.15)

# Maps to Visual Studio solution file (ConfigAndGrabConsole.sln)
# The solution will have all targets (exe, lib, dll) as Visual Studio projects (.vcproj)
project (PFCameraLib_ConfigAndGrabConsole_Color_Context)
 
if(NOT TARGET Photonfocus::PFCameraLib)
	find_package(PFBase CONFIG REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
endif()

add_executable(${PROJECT_NAME} ConfigAndGrab_Console_Color_Context.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER PFCameraLib/Examples/C++)
target_link_libraries(${PROJECT_NAME} PRIVATE Photonfocus::pfcTypes Photonfocus::PFCameraLib)
target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE)