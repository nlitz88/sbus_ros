/**
 * @file sbus_joy.hpp
 * @author Nathan Litzinger (nlitz88@gmail.com)
 * @brief SBUS Joy Node declaration
 * @version 0.1
 * @date 2026-03-31
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef SBUS_JOY_NODE_HPP
#define SBUS_JOY_NODE_HPP

#include <rclcpp/rclcpp.hpp>

#include <sensor_msgs/msg/joy.hpp>
#include <sbus_interfaces/msg/sbus_packet.hpp>

namespace sbus
{

class SbusJoy : public rclcpp::Node
{
public:
  /**
   * @brief Constructor for the SbusJoy node
   * @param options Additional options to control the creation of the node
   */
  explicit SbusJoy(const rclcpp::NodeOptions& options);

protected:
  /**
   * @brief Sbus packet callback
   * @param sbus_msg Sbus packet packed into an SbusPacket message
   */
  void sbusPacketCallback(const sbus_interfaces::msg::SbusPacket::SharedPtr sbus_msg);

  // Sbus subscriber
  rclcpp::Subscription<sbus_interfaces::msg::SbusPacket>::SharedPtr sbus_sub_;
  // Joy publisher
  rclcpp::Publisher<sensor_msgs::msg::Joy>::SharedPtr joy_pub_;

  /**
   * @brief Configurable offset for each channel. Useful for making the resting position of a
   * controller potentiometer register as 0.
   */
  int ch1_deadzone_offset_;
  int ch2_deadzone_offset_;
  int ch3_deadzone_offset_;
  int ch4_deadzone_offset_;
  int ch5_deadzone_offset_;
  int ch6_deadzone_offset_;
  int ch7_deadzone_offset_;
  int ch8_deadzone_offset_;
  int ch9_deadzone_offset_;
  int ch10_deadzone_offset_;
  int ch11_deadzone_offset_;
  int ch12_deadzone_offset_;
  int ch13_deadzone_offset_;
  int ch14_deadzone_offset_;
  int ch15_deadzone_offset_;
  int ch16_deadzone_offset_;
};

}  // namespace sbus

#endif