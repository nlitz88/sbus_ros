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
| `ch1_deadzone_offset` ... `ch16_deadzone_offset` | `int` | `0` | Deadzone adjustment offset for the respective channel |
| `button_mappings` | `string_array` | `[]` | List of unique names for virtual buttons to be created |
| `<button_name>.channel` | `int` | `0` | The **0-indexed** SBUS channel to monitor (e.g., CH1 is 0) |
| `<button_name>.threshold` | `int` | `1500` | The SBUS value at which the button state toggles |
| `<button_name>.direction` | `string` | `"above"` | `"above"` or `"below"`. Determines which side of the threshold is "Pressed" |

### Virtual Button Configuration
The `sbus_joy` node allows you to map specific SBUS channel values to the `buttons` array in the `sensor_msgs/msg/Joy` message. This is particularly useful for 2-position or 3-position switches.

**How it works:**
1. Define a list of button names in `button_mappings`.
2. For each name, provide the `channel`, `threshold`, and `direction`.
3. The `joy.buttons` array will be ordered based on the sequence in your `button_mappings` list.

**Example YAML Configuration:**
```yaml
sbus_joy:
  ros__parameters:
    # Define three virtual buttons
    button_mappings: ["arm", "mode_high", "mode_low"]
    
    # Toggle switch on CH6 (index 5)
    arm:
      channel: 5
      threshold: 1500
      direction: "above"
      
    # 3-position rocker on CH7 (index 6) mapped to two buttons
    mode_high:
      channel: 6
      threshold: 1700
      direction: "above"
    mode_low:
      channel: 6
      threshold: 300
      direction: "below"
```

> **Note:** Channels 17 and 18 are digital on/off signals and do not have deadzone offset parameters. All 18 channels are mapped directly to the `axes` array of the output Joy message. The `buttons` array size is determined dynamically by the number of entries in `button_mappings`.

---

## `sbus_driver` node

**Not implemented yet**

Planned functionality: Serial port driver node that opens a UART serial device, reads raw SBUS protocol data, parses it, and publishes `SbusPacket` messages.

Will include:
- Configurable serial port path
- Baud rate configuration
- Hardware flow control settings
- Lost frame / failsafe detection