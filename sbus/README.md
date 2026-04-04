# sbus
ROS package with utilities to interface with SBUS RC receivers.

---

## `sbus_joy` node

Translates `sbus_interfaces/msg/SbusPacket` messages to standard `sensor_msgs/msg/Joy` messages for compatibility with ROS joystick interfaces.

### Subscribed Topics
| Topic | Type | Description |
|-------|------|-------------|
| `/sbus` | `sbus_interfaces/msg/SbusPacket` | Raw SBUS packet messages received from receiver |

### Published Topics
| Topic | Type | Description |
|-------|------|-------------|
| `/joy` | `sensor_msgs/msg/Joy` | Standard joystick message with mapped SBUS channels |

### Parameters
| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `ch1_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 1 |
| `ch2_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 2 |
| `ch3_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 3 |
| `ch4_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 4 |
| `ch5_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 5 |
| `ch6_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 6 |
| `ch7_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 7 |
| `ch8_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 8 |
| `ch9_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 9 |
| `ch10_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 10 |
| `ch11_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 11 |
| `ch12_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 12 |
| `ch13_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 13 |
| `ch14_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 14 |
| `ch15_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 15 |
| `ch16_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for channel 16 |

> **Note:** Channels 17 and 18 are digital on/off signals and do not have deadzone offset parameters. All 18 channels are mapped directly to the `axes` array of the output Joy message.

---

## `sbus_driver` node

**Not implemented yet**

Planned functionality: Serial port driver node that opens a UART serial device, reads raw SBUS protocol data, parses it, and publishes `SbusPacket` messages.

Will include:
- Configurable serial port path
- Baud rate configuration
- Hardware flow control settings
- Lost frame / failsafe detection