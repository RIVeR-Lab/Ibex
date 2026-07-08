cmake_minimum_required (VERSION 3.10)

project (pfDoubleRate_Demodulate_File)
 
if(NOT TARGET Photonfocus::pfDoubleRate)
	find_package(pfDoubleRate CONFIG REQUIRED
		HINTS 
			${CMAKE_CURRENT_SOURCE_DIR}/../../../
			$ENV{PF_ROOT}/DoubleRateSDK/
	)
endif()

add_executable(pfDoubleRate_Demodulate_File pfDoubleRate_Demodulate_File.cpp)
set_target_properties(pfDoubleRate_Demodulate_File PROPERTIES FOLDER pfDoubleRate/examples/C++)
target_link_libraries(pfDoubleRate_Demodulate_File PRIVATE Photonfocus::pfDoubleRate)
target_compile_definitions(pfDoubleRate_Demodulate_File PRIVATE UNICODE _CRT_SECURE_NO_WARNINGS)
