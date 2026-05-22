#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Photonfocus::PFCameraLib" for configuration "Release"
set_property(TARGET Photonfocus::PFCameraLib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Photonfocus::PFCameraLib PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "Photonfocus::pfDoubleRate;Photonfocus::PFPixelCollection"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libPFCameraLib.so"
  IMPORTED_SONAME_RELEASE "libPFCameraLib.so"
  )

list(APPEND _cmake_import_check_targets Photonfocus::PFCameraLib )
list(APPEND _cmake_import_check_files_for_Photonfocus::PFCameraLib "${_IMPORT_PREFIX}/lib/libPFCameraLib.so" )

# Import target "Photonfocus::PFPixelCollection" for configuration "Release"
set_property(TARGET Photonfocus::PFPixelCollection APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Photonfocus::PFPixelCollection PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libPFPixelCollection.so"
  IMPORTED_SONAME_RELEASE "libPFPixelCollection.so"
  )

list(APPEND _cmake_import_check_targets Photonfocus::PFPixelCollection )
list(APPEND _cmake_import_check_files_for_Photonfocus::PFPixelCollection "${_IMPORT_PREFIX}/lib/libPFPixelCollection.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
