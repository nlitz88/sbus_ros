# sbus
ROS package with utilities to interface with SBUS RC receivers.

## `sbus_joy` node
Node that translates [SbusPacket](../sbus_interfaces/msg/SbusPacket.msg) messages to [sensor_msgs/Joy](https://docs.ros.org/en/noetic/api/sensor_msgs/html/msg/Joy.html) messages.

## `sbus_driver` node
TODO: Create a node that opens a UART serial device (/dev/ttyyACM0, for example) (configurable via a
parameter) and reads + parses the SBUS packets it reads from it.

