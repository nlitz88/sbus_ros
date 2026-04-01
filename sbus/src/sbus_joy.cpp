
#include <sbus/sbus_joy.hpp>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>

#include <sbus_interfaces/msg/sbus_packet.hpp>
#include <sensor_msgs/msg/joy.hpp>

namespace sbus
{
SbusJoy::SbusJoy(const rclcpp::NodeOptions& options) : Node("sbus_joy", options)
{
  // Declare parameters

  // Enable overriding for specific policies (history, depth, reliability, etc.)
  rclcpp::PublisherOptions pub_options;
  pub_options.qos_overriding_options = rclcpp::QosOverridingOptions::with_default_policies();

  rclcpp::SubscriptionOptions sub_options;
  sub_options.qos_overriding_options = rclcpp::QosOverridingOptions::with_default_policies();

  // Allocate sbus subscriber
  sbus_sub_ = this->create_subscription<sbus_interfaces::msg::SbusPacket>(
      "sbus", 1, std::bind(&SbusJoy::sbusPacketCallback, this, std::placeholders::_1), sub_options);
  // Allocate joy publisher
  joy_pub_ = this->create_publisher<sensor_msgs::msg::Joy>("joy", 1, pub_options);

  RCLCPP_INFO_STREAM(this->get_logger(), "sbus joy node initialized!");
}

void SbusJoy::sbusPacketCallback(const sbus_interfaces::msg::SbusPacket::SharedPtr sbus_msg)
{
  RCLCPP_INFO_STREAM(this->get_logger(), "Sbus ch1 value: " << sbus_msg->ch1);
  sensor_msgs::msg::Joy joy_msg;
  joy_msg.header.stamp = this->get_clock()->now();

  joy_msg.axes[0] = sbus_msg->ch1;
  joy_msg.axes[1] = sbus_msg->ch2;
  // TODO: Fill out the rest of the joy message.

  joy_pub_->publish(joy_msg);
}

}  // namespace sbus

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
// Taken from
// https://github.com/ros2/demos/blob/7dffb2cc9b6e6711c8877572cb8bebbb7dae74b1/composition/src/talker_component.cpp#L59-L62
RCLCPP_COMPONENTS_REGISTER_NODE(sbus::SbusJoy)