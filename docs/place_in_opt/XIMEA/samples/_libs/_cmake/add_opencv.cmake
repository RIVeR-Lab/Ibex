include_directories(../../_libs/xiAPIplusOpenCV)
include_directories($(OPENCV_DIR)/../../include)

if(UNIX)
    link_libraries(opencv_core)
    link_libraries(opencv_highgui)
    link_libraries(opencv_imgproc)
else()
    link_libraries(opencv_world310d)
endif()

link_directories($(OPENCV_DIR)/lib)

