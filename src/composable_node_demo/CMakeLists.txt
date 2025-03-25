cmake_minimum_required(VERSION 3.5)
project(composable_node_demo)



# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2 REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(interfaces REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)
find_package(interfaces REQUIRED)

# Include directories
include_directories(include)

include_directories(/home/jwang/jwang/data/onnxruntime/include)
link_directories(/home/jwang/jwang/data/onnxruntime/lib)


# Create a library for the composable nodes
add_library(publisher_node_component SHARED src/publisher_node.cpp)
ament_target_dependencies(publisher_node_component rclcpp std_msgs tf2_ros tf2_geometry_msgs geometry_msgs rclcpp_components interfaces)

rclcpp_components_register_node(
    publisher_node_component
    PLUGIN "PublisherNode"
    EXECUTABLE publisher_node
)

add_library(subscriber_node_component SHARED src/subscriber_node.cpp)
ament_target_dependencies(subscriber_node_component rclcpp std_msgs sensor_msgs OpenCV cv_bridge rclcpp_components interfaces)

rclcpp_components_register_node(
    subscriber_node_component
    PLUGIN "SubscriberNode"
    EXECUTABLE subscriber_node
)

add_library(service_node_component SHARED src/service_node.cpp)
ament_target_dependencies(service_node_component rclcpp cv_bridge sensor_msgs interfaces OpenCV rclcpp_components)
target_link_libraries(service_node_component onnxruntime)

rclcpp_components_register_node(
    service_node_component
    PLUGIN "ServiceNode"
    EXECUTABLE service_node
)

add_library(client_node_component SHARED src/client_node.cpp)
ament_target_dependencies(client_node_component rclcpp cv_bridge sensor_msgs interfaces OpenCV rclcpp_components)

rclcpp_components_register_node(
    client_node_component
    PLUGIN "ClientNode"
    EXECUTABLE client_node
)

add_library(tf2_publisher_component SHARED src/tf2_publisher.cpp)
ament_target_dependencies(tf2_publisher_component rclcpp tf2_ros geometry_msgs rclcpp_components)

rclcpp_components_register_node(
    tf2_publisher_component
    PLUGIN "TF2Publisher"
    EXECUTABLE tf2_publisher
)

add_library(tf2_listener_component SHARED src/tf2_listener.cpp)
ament_target_dependencies(tf2_listener_component rclcpp tf2_ros geometry_msgs tf2_geometry_msgs tf2 rclcpp_components)

rclcpp_components_register_node(
    tf2_listener_component
    PLUGIN "TF2Listener"
    EXECUTABLE tf2_listener
)

ament_export_targets(export_publisher_node_component)
install(TARGETS publisher_node_component
        EXPORT export_publisher_node_component
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

ament_export_targets(export_subscriber_node_component)
install(TARGETS subscriber_node_component
        EXPORT export_subscriber_node_component
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

ament_export_targets(export_service_node_component)
install(TARGETS service_node_component
        EXPORT export_service_node_component
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

ament_export_targets(export_client_node_component)
install(TARGETS client_node_component
        EXPORT export_client_node_component
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

ament_export_targets(export_tf2_publisher_component)
install(TARGETS tf2_publisher_component
        EXPORT export_tf2_publisher_component
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

ament_export_targets(export_tf2_listener_component)
install(TARGETS tf2_listener_component
        EXPORT export_tf2_listener_component
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

# Install launch files
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

# Install parameter files
install(DIRECTORY param
  DESTINATION share/${PROJECT_NAME}
)

# add_executable(client_node src/client_node.cpp)
# ament_target_dependencies(client_node rclcpp sensor_msgs interfaces OpenCV cv_bridge)

# install(TARGETS client_node DESTINATION lib/${PROJECT_NAME})

# Export the package
ament_package()