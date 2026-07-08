#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "lodepng::lodepng" for configuration "Release"
set_property(TARGET lodepng::lodepng APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(lodepng::lodepng PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/liblodepng.a"
  )

list(APPEND _cmake_import_check_targets lodepng::lodepng )
list(APPEND _cmake_import_check_files_for_lodepng::lodepng "${_IMPORT_PREFIX}/lib/liblodepng.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
