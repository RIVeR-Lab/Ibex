cmake_minimum_required(VERSION 3.8)
project(spectrometer_drivers)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(spectrometer_interfaces REQUIRED)

# ---------------------------------------------------------------------------
# FTDI libraries (libft4222 + libftd2xx) are NOT ROS packages. They are
# installed system-wide by FTDI's install script, typically:
#   headers ->  /usr/local/include   (ftd2xx.h, libft4222.h)
#   libs    ->  /usr/local/lib       (libft4222.so, libftd2xx.so)
# If your install put them elsewhere, adjust the HINTS/PATHS below.
# ---------------------------------------------------------------------------
find_path(FTDI_INCLUDE_DIR
  NAMES libft4222.h ftd2xx.h
  HINTS /usr/local/include /usr/include
)

find_library(FT4222_LIB
  NAMES ft4222
  HINTS /usr/local/lib /usr/lib
)

# On Linux, LibFT4222 ships with D2XX built in, so a standalone libftd2xx.so
# usually does NOT exist -- the FT_* D2XX symbols live inside libft4222.
# We therefore look for libftd2xx but treat it as optional.
find_library(FTD2XX_LIB
  NAMES ftd2xx
  HINTS /usr/local/lib /usr/lib
)

if(NOT FTDI_INCLUDE_DIR OR NOT FT4222_LIB)
  message(FATAL_ERROR
    "Could not locate libft4222. "
    "Expected libft4222.h (and ftd2xx.h) plus libft4222.so. "
    "Install FTDI's LibFT4222 package (install4222.sh), or set HINTS in CMakeLists.txt. "
    "INCLUDE=${FTDI_INCLUDE_DIR} FT4222=${FT4222_LIB}")
endif()

add_executable(spectral_data_streamer src/spectral_data_streamer.cpp)

target_include_directories(spectral_data_streamer PRIVATE ${FTDI_INCLUDE_DIR})

ament_target_dependencies(spectral_data_streamer
  rclcpp
  std_msgs
  spectrometer_interfaces
)

target_link_libraries(spectral_data_streamer
  ${FT4222_LIB}
)

# Link a standalone libftd2xx only if one exists; on Linux it's normally
# bundled inside libft4222 and this variable will be empty.
if(FTD2XX_LIB)
  target_link_libraries(spectral_data_streamer ${FTD2XX_LIB})
endif()

# libft4222 (with built-in D2XX) depends on these system libraries for USB
# access and threading. Linking them explicitly avoids undefined-reference
# errors at link time.
find_package(Threads REQUIRED)
target_link_libraries(spectral_data_streamer
  Threads::Threads
  ${CMAKE_DL_LIBS}
)

install(TARGETS spectral_data_streamer
  DESTINATION lib/${PROJECT_NAME})

# ---------------------------------------------------------------------------
# Python nodes in this (ament_cmake) package.
# Requires ament_cmake_python; installs the scripts as executables so they can
# be launched with `ros2 run spectrometer_drivers combine_ibsen` etc.
# ---------------------------------------------------------------------------
find_package(ament_cmake_python REQUIRED)
find_package(rclpy REQUIRED)

# Install the python module directory (so imports resolve)
ament_python_install_package(${PROJECT_NAME})

# Install the executable python nodes
install(PROGRAMS
  ${PROJECT_NAME}/combine_ibsen.py
  ${PROJECT_NAME}/spectral_plot.py
  DESTINATION lib/${PROJECT_NAME}
)

# Install launch files if present
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
  OPTIONAL)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
