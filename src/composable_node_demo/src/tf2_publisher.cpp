#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2_ros/transform_listener.h"
#include "geometry_msgs/msg/transform_stamped.hpp"

class TF2Publisher : public rclcpp::Node
{
public:
    explicit TF2Publisher(const rclcpp::NodeOptions & options): Node("tf2_node", options)
    {
        this->declare_parameter<std::string>("frame_id", "base_link");
        this->declare_parameter<std::string>("child_frame_id", "camera_link");
        
        broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&TF2Publisher::publish_transform, this));
    }

private:
    void publish_transform()
    {
        geometry_msgs::msg::TransformStamped transformStamped;
        transformStamped.header.stamp = this->now();

        transformStamped.header.frame_id = this->get_parameter("frame_id").as_string();
        transformStamped.child_frame_id = this->get_parameter("child_frame_id").as_string();

        RCLCPP_INFO(this->get_logger(), "Loaded frame_id: %s", this->get_parameter("frame_id").as_string().c_str());

        transformStamped.transform.translation.x = 1.0;
        transformStamped.transform.translation.y = 0.0;
        transformStamped.transform.translation.z = 0.5;
        transformStamped.transform.rotation.x = 0.0;
        transformStamped.transform.rotation.y = 0.0;
        transformStamped.transform.rotation.z = 0.0;
        transformStamped.transform.rotation.w = 1.0;

        broadcaster_->sendTransform(transformStamped);
    }

    std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
    rclcpp::TimerBase::SharedPtr timer_;
};

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(TF2Publisher)