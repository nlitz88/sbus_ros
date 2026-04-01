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
};

}  // namespace sbus

#endif