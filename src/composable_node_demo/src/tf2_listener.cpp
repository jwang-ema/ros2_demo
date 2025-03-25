#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/point_stamped.hpp" 
#include "tf2/exceptions.h"

#include "tf2/time.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

class TF2Listener : public rclcpp::Node
{
public:
    explicit TF2Listener(const rclcpp::NodeOptions & options): Node("tf2_listener", options), tf_buffer_(this->get_clock())
    {
        this->declare_parameter<std::string>("frame_id", "base_link");
        this->declare_parameter<std::string>("child_frame_id", "camera_link");

        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(tf_buffer_);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(1000),
            std::bind(&TF2Listener::lookup_transform, this));
    }

private:
    void lookup_transform()
    {
        try {
            geometry_msgs::msg::TransformStamped transform = 
                tf_buffer_.lookupTransform(
                    this->get_parameter("frame_id").as_string(),
                    this->get_parameter("child_frame_id").as_string(),
                    tf2::TimePointZero);

            RCLCPP_INFO(this->get_logger(), "找到base_link坐标系到camera_link坐标系的转换关系:");
            RCLCPP_INFO(this->get_logger(), "  位移: [%.2f, %.2f, %.2f]",
                transform.transform.translation.x,
                transform.transform.translation.y,
                transform.transform.translation.z);
            RCLCPP_INFO(this->get_logger(), "  旋转: [%.2f, %.2f, %.2f, %.2f]",
                transform.transform.rotation.x,
                transform.transform.rotation.y,
                transform.transform.rotation.z,
                transform.transform.rotation.w);
            
            geometry_msgs::msg::PointStamped point_base;
            point_base.header.stamp = rclcpp::Time(0);
            point_base.header.frame_id = this->get_parameter("frame_id").as_string();
            point_base.point.x = 0.0;
            point_base.point.y = 0.0;
            point_base.point.z = 0.0;

            geometry_msgs::msg::PointStamped point_camera = tf_buffer_.transform(point_base, this->get_parameter("child_frame_id").as_string());

            RCLCPP_INFO(this->get_logger(), "在base_link坐标系的原点转换到camera_link坐标系的坐标: [%.2f, %.2f, %.2f]",
                point_camera.point.x,
                point_camera.point.y,
                point_camera.point.z);
        }
        catch (const tf2::TransformException & ex) {
            RCLCPP_WARN(this->get_logger(), "无法获取坐标变换关系: %s", ex.what());
        }
    }

    tf2_ros::Buffer tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    rclcpp::TimerBase::SharedPtr timer_;
};
  

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(TF2Listener)
