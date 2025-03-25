// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:srv/Myimage.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__SRV__DETAIL__MYIMAGE__BUILDER_HPP_
#define INTERFACES__SRV__DETAIL__MYIMAGE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "interfaces/srv/detail/myimage__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace interfaces
{

namespace srv
{

namespace builder
{

class Init_Myimage_Request_client_image
{
public:
  Init_Myimage_Request_client_image()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::interfaces::srv::Myimage_Request client_image(::interfaces::srv::Myimage_Request::_client_image_type arg)
  {
    msg_.client_image = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::srv::Myimage_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::srv::Myimage_Request>()
{
  return interfaces::srv::builder::Init_Myimage_Request_client_image();
}

}  // namespace interfaces


namespace interfaces
{

namespace srv
{

namespace builder
{

class Init_Myimage_Response_server_image
{
public:
  Init_Myimage_Response_server_image()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::interfaces::srv::Myimage_Response server_image(::interfaces::srv::Myimage_Response::_server_image_type arg)
  {
    msg_.server_image = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::srv::Myimage_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::srv::Myimage_Response>()
{
  return interfaces::srv::builder::Init_Myimage_Response_server_image();
}

}  // namespace interfaces

#endif  // INTERFACES__SRV__DETAIL__MYIMAGE__BUILDER_HPP_
