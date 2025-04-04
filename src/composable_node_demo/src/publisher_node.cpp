#include <rclcpp/rclcpp.hpp>
#include "interfaces/msg/num.hpp"
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <chrono>

using namespace std::chrono_literals;

// sudo tc qdisc add dev lo root netem loss 5%
// sudo tc qdisc delete dev lo root netem loss 5%

class PublisherNode : public rclcpp::Node
{
public:
    explicit PublisherNode(const rclcpp::NodeOptions & options): Node("publisher_node", options), count_(0)
    {
        this->declare_parameter<std::string>("topic_name", "num_topic");
        this->declare_parameter<double>("publish_rate", 10.0);
        
        auto qos = rclcpp::QoS(
            rclcpp::QoSInitialization(
                history_,
                depth_
        ));
        qos.reliability(reliability_policy_);
        
        publisher_ = this->create_publisher<interfaces::msg::Num>(this->get_parameter("topic_name").as_string(), qos);
        timer_ = this->create_wall_timer(
            std::chrono::duration<double>(1.0 / this->get_parameter("publish_rate").as_double()),
            std::bind(&PublisherNode::publish_message, this));
    }

private:
    void publish_message()
    {
        auto message = interfaces::msg::Num();
        message.num = this->count_++; 
        publisher_->publish(message);
        RCLCPP_INFO(this->get_logger(), "发布消息: %ld", message.num);
    }

    rclcpp::Publisher<interfaces::msg::Num>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    size_t count_;
    size_t depth_ = 10;
    rmw_qos_reliability_policy_t reliability_policy_ = RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
    rmw_qos_history_policy_t history_ = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
};

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(PublisherNode)