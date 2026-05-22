#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Photonfocus::PFPolarizedLib" for configuration "Release"
set_property(TARGET Photonfocus::PFPolarizedLib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Photonfocus::PFPolarizedLib PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libPFPolarizedLib.so"
  IMPORTED_SONAME_RELEASE "libPFPolarizedLib.so"
  )

list(APPEND _cmake_import_check_targets Photonfocus::PFPolarizedLib )
list(APPEND _cmake_import_check_files_for_Photonfocus::PFPolarizedLib "${_IMPORT_PREFIX}/lib/libPFPolarizedLib.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
