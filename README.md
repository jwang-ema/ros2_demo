### ROS2

#### 1.使用Composible Node的方式开发具有sub，pub，service的demo

- 包括tf2的使用

- 包括通过parameter文件进行参数配置

- 通过launch文件进行启动

- 全部代码使用c++完成

  

目录结构

```
├── workspace
│   ├── src
│   |	└── interfaces # 自定义接口
│   |	|	├── msg
│   │   │   |	└── Num.msg # 定义int 64类型变量num
│   │   |	└── srv
│   |	|	|	└── Myimage.srv # 定义request和response
│   |	|	└── ...
│   |   └── composable_node_demo # 可组合节点
│   |	|	├── src
│   │   │   |	└── publisher_node.cpp # 发布节点，发布从0开始累加的数字
│   |	|	|	└── subscriber_node.cpp # 两个订阅节点
│   |	|	|	└── service_node.cpp # 服务端节点和yolo检测结果发布节点
│   |	|	|	└── client_node.cpp # 客户端，打开usb摄像头发送图像帧
│   |	|	|	└── tf2_publisher.cpp # 坐标系转换关系广播
│   |	|	|	└── tf2_listener.cpp # 坐标系转换关系监听和转换
│   │   |	└── lauch
│   |	|	|	└── launch.py # 启动文件
│   │   |	└── param
│   |	|	|	└── demo_params.yaml # 每个节点的参数配置文件
│   │   |	└── ...
```

1、编译并运行

​	`cd $workspace && colcon build && . install/setup.sh ` 

​	` ros2 launch composable_node_demo demo_launch.py `

​	`workspace`为实际的工作目录

终端显示部分结果如下：

```
[component_container-1] [INFO] [1742887066.583530608] [tf2_publisher]: Loaded frame_id: base_link1
[component_container-1] [INFO] [1742887066.683486544] [tf2_publisher]: Loaded frame_id: base_link1
[component_container-1] [INFO] [1742887066.783528358] [tf2_publisher]: Loaded frame_id: base_link1
[component_container-1] [INFO] [1742887066.858004063] [publisher_node]: 发布消息: 0
[component_container-1] [INFO] [1742887066.858651064] [subscriber_node]: 接收消息: 0
[component_container-1] [INFO] [1742887066.883520464] [tf2_publisher]: Loaded frame_id: base_link1
[component_container-1] [INFO] [1742887066.983575870] [tf2_publisher]: Loaded frame_id: base_link1
[component_container-1] [INFO] [1742887067.083534218] [tf2_publisher]: Loaded frame_id: base_link1
[component_container-1] [INFO] [1742887067.183673768] [tf2_publisher]: Loaded frame_id: base_link1
[component_container-1] [INFO] [1742887067.283506085] [tf2_publisher]: Loaded frame_id: base_link1
[component_container-1] [INFO] [1742887067.357773904] [publisher_node]: 发布消息: 1
[component_container-1] [INFO] [1742887067.357903184] [subscriber_node]: 接收消息: 1
[component_container-1] [INFO] [1742887067.383557686] [tf2_publisher]: Loaded frame_id: base_link1
[component_container-1] [INFO] [1742887067.483486995] [tf2_publisher]: Loaded frame_id: base_link1
[component_container-1] [INFO] [1742887067.499648286] [tf2_listener]: 找到base_link坐标系到camera_link坐标系的转换关系:
[component_container-1] [INFO] [1742887067.499673302] [tf2_listener]:   位移: [1.00, 0.00, 0.50]
[component_container-1] [INFO] [1742887067.499688228] [tf2_listener]:   旋转: [0.00, 0.00, 0.00, 1.00]
[component_container-1] [INFO] [1742887067.499757114] [tf2_listener]: 在base_link坐标系的原点转换到camera_link坐标系的坐标: [-1.00, 0.00, -0.50]
[component_container-1] [INFO] [1742887067.583501721] [tf2_publisher]: Loaded frame_id: base_link1
```

(Option) 可以另起一个终端测试客户端和服务端交互

​	`cd $workspace && . install/setup.sh `

​	` ros2 run composable_node_demo client_node --ros-args --params-file $params_file_path`

​	`params_file_path`为实际参数配置文件路径`src/composable_node_demo/param/demo_params.yaml`

#### 2. 完成官网以下3个demo的演示和讲解：

- Using quality-of-service settings for lossy networks

- Setting up efficient intra-process communication

- Understanding real-time programming
