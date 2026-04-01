
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

  // Allocate joy publisher
  joy_pub_ = this->create_publisher<sensor_msgs::msg::Joy>("joy", 10);
}

void SbusJoy::sbusPacketCallback(const sbus_interfaces::msg::SbusPacket::SharedPtr sbus_msg)
{
  RCLCPP_INFO_STREAM(this->get_logger(), "Sbus ch1 value: " << sbus_msg->ch1);
}

}  // namespace sbus

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
// Taken from
// https://github.com/ros2/demos/blob/7dffb2cc9b6e6711c8877572cb8bebbb7dae74b1/composition/src/talker_component.cpp#L59-L62
RCLCPP_COMPONENTS_REGISTER_NODE(sbus::SbusJoy)