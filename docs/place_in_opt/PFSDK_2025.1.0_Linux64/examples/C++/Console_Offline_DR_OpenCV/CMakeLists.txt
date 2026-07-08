cmake_minimum_required (VERSION 3.15)

# Maps to Visual Studio solution file (ConfigAndGrabConsole.sln)
# The solution will have all targets (exe, lib, dll) as Visual Studio projects (.vcproj)
project (PFCameraLib_Console_Offline_DR_OpenCV)
 
if(NOT TARGET Photonfocus::PFCameraLib)
	find_package(PFBase CONFIG REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
endif()

set(OpenCV_STATIC OFF)
find_package(OpenCV REQUIRED)
message("-- Found OpenCV version: ${OpenCV_VERSION}")
message("-- OpenCV Include dirs: ${OpenCV_INCLUDE_DIRS}")

add_executable(${PROJECT_NAME} Console_Offline_DR_OpenCV.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER PFCameraLib/Examples/C++)
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Photonfocus::pfcTypes Photonfocus::PFCameraLib ${OpenCV_LIBS})
target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE)




