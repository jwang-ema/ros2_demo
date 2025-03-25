// #include "rclcpp/rclcpp.hpp"
// #include "sensor_msgs/msg/image.hpp"
// #include "cv_bridge/cv_bridge.h"
// #include "opencv2/opencv.hpp"
// #include "interfaces/srv/myimage.hpp"

// using sensor_msgs::msg::Image;

// using interfaces::srv::Myimage;
// using namespace std::chrono_literals;

// class ClientNode : public rclcpp::Node {
// public:
//     ClientNode(): Node("client_node"){
//         client = this->create_client<Myimage>("yolo");
//         RCLCPP_INFO(this->get_logger(), "客户端创建，等待连接服务端！");
//     }

//     bool connect_server() {
//         while (!client->wait_for_service(1s)) {
//             if (!rclcpp::ok()) {
//                 RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "强制退出！");
//                 return false;
//             }
//             RCLCPP_INFO(this->get_logger(), "服务连接中，请稍候...");
//         }
//         return true;
//     }

//     rclcpp::Client<Myimage>::FutureAndRequestId send_request(sensor_msgs::msg::Image image) {
//         auto request = std::make_shared<Myimage::Request>();
//         request->client_image = image;
//         return client->async_send_request(request);
//     }
    
// private:
//     rclcpp::Client<Myimage>::SharedPtr client;
// };

// int main(int argc, char **argv) {
//     rclcpp::init(argc, argv);

//     auto client = std::make_shared<ClientNode>();
//     bool flag = client->connect_server();
//     if (!flag) {
//         RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务连接失败！");
//         return 0;
//     }

//     // 打开USB摄像头
//     cv::VideoCapture cap(0);
//     if (!cap.isOpened()) {
//         RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "无法打开摄像头！");
//         return 1;
//     }

//     cv::Mat frame;
//     while (rclcpp::ok()) {
//         cap >> frame;
//         if (frame.empty()) {
//             RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "无法读取摄像头帧！");
//             break;
//         }

//         sensor_msgs::msg::Image::SharedPtr img_msg = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame).toImageMsg();
//         auto response = client->send_request(*img_msg);

//         if (rclcpp::spin_until_future_complete(client, response) == rclcpp::FutureReturnCode::SUCCESS) {
//             RCLCPP_INFO(client->get_logger(), "请求正常处理");

//             cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(response.get()->server_image, sensor_msgs::image_encodings::BGR8);
//             cv::Mat s_img = cv_ptr->image;
//             // cv::resize(s_img, s_img, cv::Size(s_img.size().width, s_img.size().height));
//             // cv::resize(s_img, s_img, cv::Size(1920, 1080));
//             // cv::imshow("client", s_img);
//             // int key = cv::waitKey(1);
//             // if (key == 'q' || key == 'Q' || key == 27) { // Press 'q' or 'ESC' to exit
//             //     break;
//             // }

            
//             // rclcpp::Publisher<Image>::SharedPtr result_img_pub;
//             // result_img_pub = client->create_publisher<Image>("result_img" ,10);
//             // result_img_pub->publish(response.get()->server_image);
//             // RCLCPP_INFO(client->get_logger(), "发布结果图像");
        
//         } else {
//             RCLCPP_INFO(client->get_logger(), "请求异常");
//         }
//     }

//     cap.release();
//     cv::destroyAllWindows();
//     rclcpp::shutdown();
//     return 0;
// }

// // #include "rclcpp_components/register_node_macro.hpp"
// // RCLCPP_COMPONENTS_REGISTER_NODE(ClientNode)

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"
#include "opencv2/opencv.hpp"
#include "interfaces/srv/myimage.hpp"

using sensor_msgs::msg::Image;

using interfaces::srv::Myimage;
using namespace std::chrono_literals;

class ClientNode : public rclcpp::Node {
public:
    ClientNode(const rclcpp::NodeOptions & options): Node("client_node", options){
        rmw_qos_profile_t qos_profile = rmw_qos_profile_default;
        qos_profile.depth = 1;
        this->declare_parameter<std::string>("client_name", "yolo");
        client = this->create_client<Myimage>(this->get_parameter("client_name").as_string(), qos_profile);
        RCLCPP_INFO(this->get_logger(), "客户端创建，等待连接服务端！");
        result_callback();
    }
    bool connect_server() {
        while (!client->wait_for_service(1s)) {
            if (!rclcpp::ok()) {
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "强制退出！");
                return false;
            }
            RCLCPP_INFO(this->get_logger(), "服务连接中，请稍候...");
        }
        return true;
    }

    rclcpp::Client<Myimage>::FutureAndRequestId send_request(sensor_msgs::msg::Image image) {
        auto request = std::make_shared<Myimage::Request>();
        request->client_image = image;
        return client->async_send_request(request);
    }
    
private:
    rclcpp::Client<Myimage>::SharedPtr client;

    int result_callback() {
        bool flag = connect_server();
        if (!flag) {
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务连接失败！");
            return 0;
        }
    
        // 打开USB摄像头
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "正在打开摄像头");
        cv::VideoCapture cap(0);
        
        if (!cap.isOpened()) {
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "无法打开摄像头！");
            return 1;
        }
        cv::Mat frame;
        while (rclcpp::ok()) {
            cap >> frame;
            if (frame.empty()) {
                RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "无法读取摄像头帧！");
                break;
            }
            sensor_msgs::msg::Image::SharedPtr img_msg = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame).toImageMsg();
            auto response = this->send_request(*img_msg);

            if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), response.future) == rclcpp::FutureReturnCode::SUCCESS) {
                RCLCPP_INFO(this->get_logger(), "请求正常处理");
    
                cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(response.get()->server_image, sensor_msgs::image_encodings::BGR8);
                cv::Mat s_img = cv_ptr->image;
            
            } else {
                RCLCPP_INFO(this->get_logger(), "请求异常");
            }
        }
        RCLCPP_INFO(this->get_logger(), "退出循环");
        cap.release();
        cv::destroyAllWindows();
        return 0;
    }
};

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(ClientNode)