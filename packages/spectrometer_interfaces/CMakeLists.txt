cmake_minimum_required(VERSION 3.8)
project(spectrometer_interfaces)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Spectra.msg"
  "msg/SpectraArray.msg"
  "srv/StartCollect.srv"
  "srv/EndCollect.srv"
  "srv/RequestOnce.srv"
  "srv/Light.srv"
  "srv/Integration.srv"
  DEPENDENCIES std_msgs
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
