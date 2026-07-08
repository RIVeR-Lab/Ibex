if(NOT UNIX)
    include_directories(../../_libs/libtiff)
    link_directories(../../_libs/bin)
endif()

link_libraries(tiff)
