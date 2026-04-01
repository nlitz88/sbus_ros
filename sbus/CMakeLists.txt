cmake_minimum_required(VERSION 3.8)
project(sbus)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
find_package(rclcpp REQUIRED)
# Per https://docs.ros.org/en/jazzy/Tutorials/Intermediate/Writing-a-Composable-Node.html#cmake-changes
find_package(rclcpp_components REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(sbus_interfaces REQUIRED)

# For context on how to set up CMakeLists to build components, see
# https://docs.ros.org/en/jazzy/Tutorials/Intermediate/Writing-a-Composable-Node.html#cmake-changes

# Define how to build, install, and export the sbus joy component.
add_library(sbus_joy_component SHARED src/sbus_joy.cpp)
ament_target_dependencies(sbus_joy_component
  rclcpp
  rclcpp_components
  sensor_msgs
  sbus_interfaces
)
target_include_directories(sbus_joy_component PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_compile_features(sbus_joy_component PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
rclcpp_components_register_node(
    sbus_joy_component
    PLUGIN "sbus::SbusJoy"
    EXECUTABLE sbus_joy
)
ament_export_targets(sbus_joy_component)
install(TARGETS sbus_joy_component
        EXPORT sbus_joy_component
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
