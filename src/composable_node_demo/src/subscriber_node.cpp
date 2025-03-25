#include "rclcpp/rclcpp.hpp"
#include "interfaces/msg/num.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.h"

using sensor_msgs::msg::Image;

class SubscriberNode : public rclcpp::Node
{
public:
    explicit SubscriberNode(const rclcpp::NodeOptions & options): Node("subscriber_node", options)
    {
        this->declare_parameter<std::string>("topic_name", "num_topic");
        this->declare_parameter<std::string>("yolo_topic_name", "result_img");
        subscription_ = this->create_subscription<interfaces::msg::Num>(
            this->get_parameter("topic_name").as_string(), 10, std::bind(&SubscriberNode::topic_callback, this, std::placeholders::_1));
        
        subscription_yolo = this->create_subscription<sensor_msgs::msg::Image>(
            this->get_parameter("yolo_topic_name").as_string(), 10, std::bind(&SubscriberNode::yolo_topic_callback, this, std::placeholders::_1));
    }

private:
    void topic_callback(const interfaces::msg::Num::SharedPtr msg) const
    {
        RCLCPP_INFO(this->get_logger(), "接收消息: %ld", msg->num);
    }

    void yolo_topic_callback(const sensor_msgs::msg::Image::SharedPtr msg) const
    {
        cv::Mat yolo_detection = cv_bridge::toCvShare(msg, "bgr8")->image;
        cv::resize(yolo_detection, yolo_detection, cv::Size(1920, 1080));
        cv::imshow("service", yolo_detection);
        int key = cv::waitKey(1);
        if (key == 'q' || key == 'Q' || key == 27)
            cv::destroyAllWindows();
    }
    
    rclcpp::Subscription<interfaces::msg::Num>::SharedPtr subscription_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_yolo;
    
};

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(SubscriberNode)