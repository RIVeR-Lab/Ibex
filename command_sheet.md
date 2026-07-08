# Command Cheat Sheet

Launch ouster driver (gives metadata file at working directory)
```bash
ros2 launch ouster_ros sensor.launch.xml sensor_hostname:=169.254.105.158 viz:=false
```

Other method (takes a moment to come up)
```bash
ros2 launch ouster_ros driver.launch.py params_file:=/home/river/ibex_ws/src/ibex/packages/ouster-ros/ouster-ros/config/ibex_ouster_sensor_config.yaml viz:=false
```



Launch insta360
```bash
ros2 launch insta360_ros_driver bringup.launch.xml equirectangular:=true
```


