#include <rclcpp/rclcpp.hpp>
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>
#include "interfaces/srv/myimage.hpp"
#include "onnxruntime_cxx_api.h"

using sensor_msgs::msg::Image;

using interfaces::srv::Myimage;
using std::placeholders::_1;
using std::placeholders::_2;

const std::vector<std::string> class_names = {
    "person", "bicycle", "car", "motorbike", "aeroplane", "bus", "train", "truck", "boat", "traffic light",
    "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
    "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
    "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
    "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
    "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "sofa",
    "pottedplant", "bed", "diningtable", "toilet", "tvmonitor", "laptop", "mouse", "remote", "keyboard",
    "cell phone", "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors",
    "teddy bear", "hair drier", "toothbrush"
};

class ServiceNode : public rclcpp::Node {
public:
    ServiceNode(const rclcpp::NodeOptions & options): Node("service_node", options){
        this->declare_parameter<std::string>("service_name", "yolo");
        this->declare_parameter<std::string>("yolo_topic_name", "result_img");
        yolo_service = this->create_service<Myimage>(this->get_parameter("service_name").as_string(), std::bind(&ServiceNode::yolo_service_callback, this, _1, _2));
        RCLCPP_INFO(this->get_logger(), "YOLO 服务端已经启动，等待客户端请求...");
        result_img_pub = this->create_publisher<Image>(this->get_parameter("yolo_topic_name").as_string() ,10); 
    }

private:
    rclcpp::Service<Myimage>::SharedPtr yolo_service;
    rclcpp::Publisher<Image>::SharedPtr result_img_pub;
    
    void yolo_service_callback(const Myimage::Request::SharedPtr req, const Myimage::Response::SharedPtr res){
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(req->client_image, sensor_msgs::image_encodings::BGR8);
        cv::Mat image = cv_ptr->image;

        cv::Mat yolo_detection = perform_yolo_detection(image); 

        sensor_msgs::msg::Image::SharedPtr msg = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", yolo_detection).toImageMsg();
        res->server_image = *msg;

        result_img_pub->publish(*msg);
        RCLCPP_INFO(this->get_logger(), "发布结果图像");

        // cv::resize(yolo_detection, yolo_detection, cv::Size(1920, 1080));
        // cv::imshow("service", yolo_detection);
        // int key = cv::waitKey(1);
        // if (key == 'q' || key == 'Q' || key == 27) { // Press 'q' or 'ESC' to exit
        //     cv::destroyAllWindows();
        // }
    }
    
    cv::Mat letterbox(const cv::Mat &img, cv::Size new_shape, cv::Scalar color, bool _auto, bool scaleFill, bool scaleup, int stride)
    {
        float width = img.cols;
        float height = img.rows;
        float r = std::min(new_shape.width / width, new_shape.height / height);
        if (!scaleup)
            r = std::min(r, 1.0f);
        int new_unpadW = int(std::round(width * r));
        int new_unpadH = int(std::round(height * r));
        int dw = new_shape.width - new_unpadW;
        int dh = new_shape.height - new_unpadH;
        if (_auto)
        {
            dw %= stride;
            dh %= stride;
        }
        dw /= 2, dh /= 2;
        cv::Mat dst;
        cv::resize(img, dst, cv::Size(new_unpadW, new_unpadH), 0, 0, cv::INTER_LINEAR);
        int top = int(std::round(dh - 0.1));
        int bottom = int(std::round(dh + 0.1));
        int left = int(std::round(dw - 0.1));
        int right = int(std::round(dw + 0.1));
        cv::copyMakeBorder(dst, dst, top, bottom, left, right, cv::BORDER_CONSTANT, color);
        return dst;
    }

    cv::Mat perform_yolo_detection_onnx(const cv::Mat &image) {
        
        cv::dnn::Net model = cv::dnn::readNetFromONNX("/home/jwang/jwang/data/projects/yolov5/yolov5s.onnx");
        std::cout << "image size: " << image.size() << std::endl;
        // cv::Mat image = cv::imread("/home/jwang/jwang/data/test.png", 1);
        // cv::Mat img = image.clone();
        cv::Mat img = letterbox(image, cv::Size(640, 640), cv::Scalar(114, 114, 114), true, false, true, 32);
        std::cout << "img size: " << img.size() << std::endl;
        cv::Mat blob;    
        // cv::dnn::blobFromImage(img, blob, 1 / 255.0f, cv::Size(img.cols, img.rows), cv::Scalar(0, 0, 0), true, false);
        cv::dnn::blobFromImage(img, blob, 1 / 255.0f, cv::Size(640, 640), cv::Scalar(0, 0, 0), true, false);
        model.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        model.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        model.setInput(blob);
        std::vector<std::string> outLayerNames = model.getUnconnectedOutLayersNames();
        std::vector<cv::Mat> result;
        model.forward(result, outLayerNames);

        std::vector<int> class_ids;
        std::vector<float> confidences;

        cv::Mat out = cv::Mat(result[0].size[1], result[0].size[2], CV_32F, result[0].ptr<float>());
        std::vector<cv::Rect> boxes;
        std::vector<int> indices;
        std::vector<float> scores;
        for (int r = 0; r < out.size[0]; r++)
        {
            float cx = out.at<float>(r, 0);
            float cy = out.at<float>(r, 1);
            float w = out.at<float>(r, 2);
            float h = out.at<float>(r, 3);
            float sc = out.at<float>(r, 4);
            cv::Mat confs = out.row(r).colRange(5,85);
            confs *= sc;
            double minV=0,maxV=0;
            double *minI=&minV;
            double *maxI=&maxV;
            minMaxIdx(confs,minI,maxI);
            scores.push_back(maxV);
            boxes.push_back(cv::Rect(cx - w / 2, cy - h / 2, w, h));
            indices.push_back(r);
        }
        cv::dnn::NMSBoxes(boxes, scores, 0.25f, 0.45f, indices);
        for (int idx : indices) {
            cv::Rect box = boxes[idx];
            int class_id = class_ids[idx];
            float confidence = confidences[idx];
    
            cv::rectangle(image, box, cv::Scalar(0, 255, 0), 2);
    
            std::string label = cv::format("%s: %.2f", class_names[class_id].c_str(), confidence);
            int base_line;
            cv::Size label_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &base_line);
            int top = std::max(box.y, label_size.height);
            cv::rectangle(image, cv::Point(box.x, top - label_size.height),
                          cv::Point(box.x + label_size.width, top + base_line), cv::Scalar(255, 255, 255), cv::FILLED);
            cv::putText(image, label, cv::Point(box.x, top), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
            std::cout << "class: " << cv::Point(box.x, top) << std::endl;
        }
        return image;
    }

    cv::Mat perform_yolo_detection(const cv::Mat &image) {
        static cv::dnn::Net net = cv::dnn::readNetFromDarknet("/home/jwang/jwang/data/yolo_ros2/src/ros2_yolo_project/yolov3.cfg", "/home/jwang/jwang/data/yolo_ros2/src/ros2_yolo_project/yolov3.weights");
    
        cv::Mat blob;
        cv::dnn::blobFromImage(image, blob, 1/255.0, cv::Size(416, 416), cv::Scalar(0, 0, 0), true, false);
        net.setInput(blob);
    
        std::vector<cv::Mat> outputs;
        net.forward(outputs, net.getUnconnectedOutLayersNames());

        std::vector<int> class_ids;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;
    
        for (const auto &output : outputs) {
            for (int i = 0; i < output.rows; ++i) {
                const auto &data = output.ptr<float>(i);
                float confidence = data[4];
                if (confidence > 0.3) {
                    int centerX = static_cast<int>(data[0] * image.cols);
                    int centerY = static_cast<int>(data[1] * image.rows);
                    int width = static_cast<int>(data[2] * image.cols);
                    int height = static_cast<int>(data[3] * image.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;
    
                    cv::Mat scores = output.row(i).colRange(5, output.cols);
                    cv::Point class_id_point;
                    double max_class_score;
                    minMaxLoc(scores, 0, &max_class_score, 0, &class_id_point);
                    int class_id = class_id_point.x;
    
                    class_ids.push_back(class_id);
                    confidences.push_back(confidence);
                    boxes.push_back(cv::Rect(left, top, width, height));
                }
            }
        }
    
        std::vector<int> indices;
        cv::dnn::NMSBoxes(boxes, confidences, 0.3, 0.4, indices); 
    
        for (int idx : indices) {
            cv::Rect box = boxes[idx];
            int class_id = class_ids[idx];
            float confidence = confidences[idx];
    
            cv::rectangle(image, box, cv::Scalar(0, 255, 0), 2);
    
            std::string label = cv::format("%s: %.2f", class_names[class_id].c_str(), confidence);
            int base_line;
            cv::Size label_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &base_line);
            int top = std::max(box.y, label_size.height);
            cv::rectangle(image, cv::Point(box.x, top - label_size.height),
                          cv::Point(box.x + label_size.width, top + base_line), cv::Scalar(255, 255, 255), cv::FILLED);
            cv::putText(image, label, cv::Point(box.x, top), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
        }
    
        return image;
    }
};

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(ServiceNode)