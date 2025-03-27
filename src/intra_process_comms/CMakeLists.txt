cmake_minimum_required(VERSION 3.8)
project(intra_process_comms)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(include)


add_executable(two_node_pipeline src/two_node_pipeline.cpp)
ament_target_dependencies(two_node_pipeline
    rclcpp
    std_msgs
)

add_executable(cyclic_pipeline src/cyclic_pipeline.cpp)
ament_target_dependencies(cyclic_pipeline
    rclcpp
    std_msgs
)

add_executable(image_pipeline_all_in_one src/image_pipeline_all_in_one.cpp)
ament_target_dependencies(image_pipeline_all_in_one
    rclcpp
    std_msgs
    sensor_msgs
    OpenCV
)

add_executable(image_pipeline_with_two_image_view src/image_pipeline_with_two_image_view.cpp)
ament_target_dependencies(image_pipeline_with_two_image_view
    rclcpp
    std_msgs
    sensor_msgs
    OpenCV
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(DIRECTORY include/intra_process_comms
    DESTINATION include/${PROJECT_NAME}
)

install(TARGETS 
    two_node_pipeline
    cyclic_pipeline
    image_pipeline_all_in_one
    image_pipeline_with_two_image_view
    DESTINATION lib/${PROJECT_NAME}
)

ament_package()
