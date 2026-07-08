#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Photonfocus::PF3DLib" for configuration "Release"
set_property(TARGET Photonfocus::PF3DLib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Photonfocus::PF3DLib PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libPF3DLib.so"
  IMPORTED_SONAME_RELEASE "libPF3DLib.so"
  )

list(APPEND _cmake_import_check_targets Photonfocus::PF3DLib )
list(APPEND _cmake_import_check_files_for_Photonfocus::PF3DLib "${_IMPORT_PREFIX}/lib/libPF3DLib.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
