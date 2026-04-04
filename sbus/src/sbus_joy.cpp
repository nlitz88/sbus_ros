
#include <sbus/sbus_joy.hpp>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>

#include <sbus_interfaces/msg/sbus_packet.hpp>
#include <sensor_msgs/msg/joy.hpp>

namespace sbus
{
SbusJoy::SbusJoy(const rclcpp::NodeOptions& options)
  : Node("sbus_joy", options)
  , ch1_deadzone_offset_{ 0 }
  , ch2_deadzone_offset_{ 0 }
  , ch3_deadzone_offset_{ 0 }
  , ch4_deadzone_offset_{ 0 }
  , ch5_deadzone_offset_{ 0 }
  , ch6_deadzone_offset_{ 0 }
  , ch7_deadzone_offset_{ 0 }
  , ch8_deadzone_offset_{ 0 }
  , ch9_deadzone_offset_{ 0 }
  , ch10_deadzone_offset_{ 0 }
  , ch11_deadzone_offset_{ 0 }
  , ch12_deadzone_offset_{ 0 }
  , ch13_deadzone_offset_{ 0 }
  , ch14_deadzone_offset_{ 0 }
  , ch15_deadzone_offset_{ 0 }
  , ch16_deadzone_offset_{ 0 }
{
  // Enable overriding for specific policies (history, depth, reliability, etc.)
  rclcpp::PublisherOptions pub_options;
  pub_options.qos_overriding_options = rclcpp::QosOverridingOptions::with_default_policies();

  rclcpp::SubscriptionOptions sub_options;
  sub_options.qos_overriding_options = rclcpp::QosOverridingOptions::with_default_policies();

  // Declare parameters
  ch1_deadzone_offset_ = this->declare_parameter("ch1_deadzone_offset", 0);
  ch2_deadzone_offset_ = this->declare_parameter("ch2_deadzone_offset", 0);
  ch3_deadzone_offset_ = this->declare_parameter("ch3_deadzone_offset", 0);
  ch4_deadzone_offset_ = this->declare_parameter("ch4_deadzone_offset", 0);
  ch5_deadzone_offset_ = this->declare_parameter("ch5_deadzone_offset", 0);
  ch6_deadzone_offset_ = this->declare_parameter("ch6_deadzone_offset", 0);
  ch7_deadzone_offset_ = this->declare_parameter("ch7_deadzone_offset", 0);
  ch8_deadzone_offset_ = this->declare_parameter("ch8_deadzone_offset", 0);
  ch9_deadzone_offset_ = this->declare_parameter("ch9_deadzone_offset", 0);
  ch10_deadzone_offset_ = this->declare_parameter("ch10_deadzone_offset", 0);
  ch11_deadzone_offset_ = this->declare_parameter("ch11_deadzone_offset", 0);
  ch12_deadzone_offset_ = this->declare_parameter("ch12_deadzone_offset", 0);
  ch13_deadzone_offset_ = this->declare_parameter("ch13_deadzone_offset", 0);
  ch14_deadzone_offset_ = this->declare_parameter("ch14_deadzone_offset", 0);
  ch15_deadzone_offset_ = this->declare_parameter("ch15_deadzone_offset", 0);
  ch16_deadzone_offset_ = this->declare_parameter("ch16_deadzone_offset", 0);

  // Allocate sbus subscriber
  sbus_sub_ = this->create_subscription<sbus_interfaces::msg::SbusPacket>(
      "sbus", 1, std::bind(&SbusJoy::sbusPacketCallback, this, std::placeholders::_1), sub_options);
  // Allocate joy publisher
  joy_pub_ = this->create_publisher<sensor_msgs::msg::Joy>("joy", 1, pub_options);

  RCLCPP_INFO_STREAM(this->get_logger(), "sbus joy node initialized!");
}

void SbusJoy::sbusPacketCallback(const sbus_interfaces::msg::SbusPacket::SharedPtr sbus_msg)
{
  std::unique_ptr<sensor_msgs::msg::Joy> joy_msg_ptr = std::make_unique<sensor_msgs::msg::Joy>();
  joy_msg_ptr->header.stamp = this->get_clock()->now();

  // TODO: Adjust for controller deadzones!
  // Could add a parameter for the deadzone for each channel.

  joy_msg_ptr->axes.resize(18);
  joy_msg_ptr->axes.at(0) = sbus_msg->ch1 - ch1_deadzone_offset_;
  joy_msg_ptr->axes.at(1) = sbus_msg->ch2 - ch2_deadzone_offset_;
  joy_msg_ptr->axes.at(2) = sbus_msg->ch3 - ch3_deadzone_offset_;
  joy_msg_ptr->axes.at(3) = sbus_msg->ch4 - ch4_deadzone_offset_;
  joy_msg_ptr->axes.at(4) = sbus_msg->ch5 - ch5_deadzone_offset_;
  joy_msg_ptr->axes.at(5) = sbus_msg->ch6 - ch6_deadzone_offset_;
  joy_msg_ptr->axes.at(6) = sbus_msg->ch7 - ch7_deadzone_offset_;
  joy_msg_ptr->axes.at(7) = sbus_msg->ch8 - ch8_deadzone_offset_;
  joy_msg_ptr->axes.at(8) = sbus_msg->ch9 - ch9_deadzone_offset_;
  joy_msg_ptr->axes.at(9) = sbus_msg->ch10 - ch10_deadzone_offset_;
  joy_msg_ptr->axes.at(10) = sbus_msg->ch11 - ch11_deadzone_offset_;
  joy_msg_ptr->axes.at(11) = sbus_msg->ch12 - ch12_deadzone_offset_;
  joy_msg_ptr->axes.at(12) = sbus_msg->ch13 - ch13_deadzone_offset_;
  joy_msg_ptr->axes.at(13) = sbus_msg->ch14 - ch14_deadzone_offset_;
  joy_msg_ptr->axes.at(14) = sbus_msg->ch15 - ch15_deadzone_offset_;
  joy_msg_ptr->axes.at(15) = sbus_msg->ch16 - ch16_deadzone_offset_;
  // No deadzone offsets are applied to channel 17 and 18, as they are binary on/off signals.
  joy_msg_ptr->axes.at(16) = sbus_msg->ch17;
  joy_msg_ptr->axes.at(17) = sbus_msg->ch18;

  joy_pub_->publish(std::move(joy_msg_ptr));
}

}  // namespace sbus

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
// Taken from
// https://github.com/ros2/demos/blob/7dffb2cc9b6e6711c8877572cb8bebbb7dae74b1/composition/src/talker_component.cpp#L59-L62
RCLCPP_COMPONENTS_REGISTER_NODE(sbus::SbusJoy)