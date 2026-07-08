cmake_minimum_required (VERSION 3.15)


project (PFCameraLib_Console_SoftwareTrigger_Asynchronous)
 
if(NOT TARGET Photonfocus::PFCameraLib)
	find_package(PFBase CONFIG REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
endif()

find_package(Threads REQUIRED)

add_executable(PFCameraLib_Console_SoftwareTrigger_Asynchronous Console_SoftwareTrigger_Asynchronous.cpp)

set_target_properties(PFCameraLib_Console_SoftwareTrigger_Asynchronous PROPERTIES FOLDER PFCameraLib/Examples/C++)
target_link_libraries(PFCameraLib_Console_SoftwareTrigger_Asynchronous PRIVATE Photonfocus::pfcTypes Photonfocus::PFCameraLib Threads::Threads)
target_compile_definitions(PFCameraLib_Console_SoftwareTrigger_Asynchronous PRIVATE UNICODE)