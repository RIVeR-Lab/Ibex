# Command Cheat Sheet


## System Bringup
Brings up the sensors and static transforms
```bash
ros2 launch ibex_bringup system_bringup.launch.py
```











## Individual Launches

### Launch ouster driver 

First, make lidar IP
```bash
avahi-browse -lrt _roger._tcp
```

Next, run (takes a moment to come up)
```bash
ros2 launch ouster_ros driver.launch.py params_file:=/home/river/ibex_ws/src/ibex/packages/ouster-ros/ouster-ros/config/ibex_ouster_sensor_config.yaml viz:=false
```

*no need to use this (gives metadata file at working directory)*
```bash
ros2 launch ouster_ros sensor.launch.xml sensor_hostname:=169.254.105.158 viz:=false
```





### Launch insta360
```bash
ros2 launch insta360_ros_driver insta_bringup.launch.py equirectangular:=true
```


### Launch Hyperspectral and RGB
```bash
echo 0 | sudo tee /sys/module/usbcore/parameters/usbfs_memory_mb
```
```bash
lsusb
```
```bash
sudo chmod 777 /dev/bus/usb/XXX/XXX
```
```bash
ros2 launch hyper_drive synchronous_cameras_launch.py
```


