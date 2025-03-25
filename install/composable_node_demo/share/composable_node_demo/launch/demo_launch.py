from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer, PushRosNamespace, Node
from launch_ros.descriptions import ComposableNode
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    param_file = os.path.join(get_package_share_directory('composable_node_demo'), 'param', 'demo_params.yaml')

    return LaunchDescription([
        ComposableNodeContainer(
            name='composable_node_container',
            namespace='demo',
            package='rclcpp_components',
            executable='component_container',
            output='screen',
            composable_node_descriptions=[
                ComposableNode(
                    package='composable_node_demo',
                    plugin='PublisherNode',
                    name='publisher_node',
                    parameters=[param_file],
                    # remappings=[('/topic', '/demo/topic')]
                ),
                ComposableNode(
                    package='composable_node_demo',
                    plugin='SubscriberNode',
                    name='subscriber_node',
                    parameters=[param_file],
                    # remappings=[('/demo/topic', '/topic')]
                ),
                ComposableNode(
                    package='composable_node_demo',
                    plugin='ServiceNode',
                    name='service_node',
                    parameters=[param_file]
                ),
                # ComposableNode(
                #     package='composable_node_demo',
                #     plugin='ClientNode',
                #     name='client_node',
                #     parameters=[param_file]
                # ),
                ComposableNode(
                    package='composable_node_demo',
                    plugin='TF2Publisher',
                    name='tf2_publisher',
                    parameters=[param_file]
                ),
                ComposableNode(
                    package='composable_node_demo',
                    plugin='TF2Listener',
                    name='tf2_listener',
                    parameters=[param_file]
                ),
            ],
        ),
        # Node(
        #     package='composable_node_demo',
        #     executable='client_node',
        #     name='client_node',
        #     namespace='demo',
        #     parameters=[param_file],
        #     output='screen',
        #     ),  
    ])