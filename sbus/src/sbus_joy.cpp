#include <sbus/sbus_joy.hpp>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>

#include <sbus_interfaces/msg/sbus_packet.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <vector>
#include <string>

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

  // Declare deadzone parameters
  ch1_deadzone_offset_ = this->declare_parameter<int>("ch1_deadzone_offset", 0);
  ch2_deadzone_offset_ = this->declare_parameter<int>("ch2_deadzone_offset", 0);
  ch3_deadzone_offset_ = this->declare_parameter<int>("ch3_deadzone_offset", 0);
  ch4_deadzone_offset_ = this->declare_parameter<int>("ch4_deadzone_offset", 0);
  ch5_deadzone_offset_ = this->declare_parameter<int>("ch5_deadzone_offset", 0);
  ch6_deadzone_offset_ = this->declare_parameter<int>("ch6_deadzone_offset", 0);
  ch7_deadzone_offset_ = this->declare_parameter<int>("ch7_deadzone_offset", 0);
  ch8_deadzone_offset_ = this->declare_parameter<int>("ch8_deadzone_offset", 0);
  ch9_deadzone_offset_ = this->declare_parameter<int>("ch9_deadzone_offset", 0);
  ch10_deadzone_offset_ = this->declare_parameter<int>("ch10_deadzone_offset", 0);
  ch11_deadzone_offset_ = this->declare_parameter<int>("ch11_deadzone_offset", 0);
  ch12_deadzone_offset_ = this->declare_parameter<int>("ch12_deadzone_offset", 0);
  ch13_deadzone_offset_ = this->declare_parameter<int>("ch13_deadzone_offset", 0);
  ch14_deadzone_offset_ = this->declare_parameter<int>("ch14_deadzone_offset", 0);
  ch15_deadzone_offset_ = this->declare_parameter<int>("ch15_deadzone_offset", 0);
  ch16_deadzone_offset_ = this->declare_parameter<int>("ch16_deadzone_offset", 0);

  // Declare parameters for defining virtual buttons
  this->declare_parameter("button_mappings", std::vector<std::string>{});
  auto button_names = this->get_parameter("button_mappings").as_string_array();
  for (const auto& name : button_names)
  {
    auto button_channel_index_desc = rcl_interfaces::msg::ParameterDescriptor();
    std::stringstream button_channel_index_desc_str;
    button_channel_index_desc_str << "SBUS channel index the " << name << " button's value is based on";
    button_channel_index_desc.description = button_channel_index_desc_str.str();
    rcl_interfaces::msg::IntegerRange button_channel_index_range;
    button_channel_index_range.from_value = 0;
    button_channel_index_range.to_value = 17;
    button_channel_index_range.step = 1;
    button_channel_index_desc.integer_range.push_back(button_channel_index_range);
    int button_channel_index = this->declare_parameter<int>(name + ".channel", 0, button_channel_index_desc);

    auto button_threshold_desc = rcl_interfaces::msg::ParameterDescriptor();
    button_threshold_desc.description =
        "Value over or under which the channe's value must be to be considered a button press";
    rcl_interfaces::msg::IntegerRange button_threshold_range;
    button_threshold_range.from_value = 0;
    button_threshold_range.to_value = 2000;
    button_threshold_range.step = 1;
    int button_press_threshold = this->declare_parameter<int>(name + ".threshold", 1500, button_threshold_desc);

    auto threshold_dir_desc = rcl_interfaces::msg::ParameterDescriptor();
    threshold_dir_desc.description = "Whether the channel's value must be 'above' or 'below' the specified threshold.";
    std::string threshold_direction =
        this->declare_parameter<std::string>(name + ".direction", "above", threshold_dir_desc);

    // TODO: Maybe add a parameter validation callback in the future, rather than just doing it in
    // the constructor here.
    if (threshold_direction != "above" && threshold_direction != "below")
    {
      RCLCPP_ERROR_STREAM(this->get_logger(), "Virtual button " << name << " threshold direction "
                                                                << threshold_direction
                                                                << " is not 'above' or 'below'. Skipping.");
      rclcpp::shutdown();
    }

    // If this virtual button's config was valid, add it to the list.
    VirtualButton btn;
    btn.name = name;
    btn.channel = button_channel_index;
    btn.threshold = button_press_threshold;
    if (threshold_direction == "above")
    {
      btn.trigger_above = true;
    }
    else
    {
      btn.trigger_above = false;
    }
    button_configs_.push_back(btn);

    RCLCPP_INFO(this->get_logger(), "Mapped virtual button '%s' on channel index %d", name.c_str(), btn.channel);
  }

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

  // Populate joy axes with sbus channels, corrected with deadzone offsets.
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

  // Populate virtual buttons derived from sbus channels.
  // Pack raw channels into an array so we can index them dynamically (0-based)
  uint16_t raw_channels[18] = { sbus_msg->ch1,  sbus_msg->ch2,  sbus_msg->ch3,  sbus_msg->ch4,  sbus_msg->ch5,
                                sbus_msg->ch6,  sbus_msg->ch7,  sbus_msg->ch8,  sbus_msg->ch9,  sbus_msg->ch10,
                                sbus_msg->ch11, sbus_msg->ch12, sbus_msg->ch13, sbus_msg->ch14, sbus_msg->ch15,
                                sbus_msg->ch16, sbus_msg->ch17, sbus_msg->ch18 };

  joy_msg_ptr->buttons.resize(button_configs_.size(), 0);

  for (size_t i = 0; i < button_configs_.size(); ++i)
  {
    const auto& button_config = button_configs_[i];
    int btn_channel_val = raw_channels[button_config.channel];

    if (button_config.trigger_above)
    {
      joy_msg_ptr->buttons[i] = (btn_channel_val > button_config.threshold) ? 1 : 0;
    }
    else
    {
      joy_msg_ptr->buttons[i] = (btn_channel_val < button_config.threshold) ? 1 : 0;
    }
  }

  joy_pub_->publish(std::move(joy_msg_ptr));
}

}  // namespace sbus

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(sbus::SbusJoy)