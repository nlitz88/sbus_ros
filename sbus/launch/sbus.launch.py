# Launch file to bring up the micro ros XRCE DDS agent.

from pathlib import Path
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration

def generate_launch_description():

    sbus_pkg_dir = Path(get_package_share_directory("sbus"))

    # Define config file path launch argument
    config_arg_dec = DeclareLaunchArgument(
        "config",
        default_value=str(sbus_pkg_dir / "config" / "sbus_params.yaml"),
        description="Path to config file with parameters for the vesc nodes brought up in this launch file"
    )
    config = LaunchConfiguration("config")

    # sbus joy node description
    sbus_joy_node_desc = Node(
        package='sbus',
        executable='sbus_joy',
        name='sbus_joy',
        parameters=[config]
    )

    return LaunchDescription([
        config_arg_dec,
        sbus_joy_node_desc
    ])