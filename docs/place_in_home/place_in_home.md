# Changes to the Home Directory

This file describes changes to the home directory.


## .bashrc

We added the below section to the bottom of the `.bashrc` file (full file CAO 22 Jul 26 found in the `../docs/place_in_home` directory) to facilitate easy usage of the IBEX system.  This first sources ROS with the set workspace.  Second, it forces `fastdds` (the default ROS2 DDS) and points it to the configuration file (also found in `../docs/place_in_home` CAO 22 Jul 26) to deconflict the Ouster (ethernet) network from the main IBEX (USB ethernet) network.  Finally, it fixes the QT platform path (fixing nonfunctional RVIZ2 and RQT, CAO 22 JUL 26 unknown cause but prior to Ben working with the ROS2 system).

```bash
# Sourcing ROS
export CURRENT_WS=ibex_ws
export ROS_DOMAIN_ID=0

source /opt/ros/humble/setup.bash
source ~/$CURRENT_WS/install/setup.bash

source /usr/share/colcon_cd/function/colcon_cd.sh
export _colcon_cd_root=/opt/ros/humble/
source /usr/share/colcon_argcomplete/hook/colcon-argcomplete.bash

echo "Sourced ROS 2 for $CURRENT_WS on ROS Domain ID $ROS_DOMAIN_ID"

export RMW_IMPLEMENTATION=rmw_fastrtps_cpp
export FASTRTPS_DEFAULT_PROFILES_FILE=$HOME/fastdds_ibex_config.xml
export RMW_FASTRTPS_USE_QOS_FROM_XML=0
echo "Set RMW Implementation to $RMW_IMPLEMENTATION with profile $FASTRTPS_DEFAULT_PROFILES_FILE"

# Override QT Platforms Path to fix RQT and RVIZ2
export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt5/plugins/platforms
echo "Overwrote QT_QPA_PLATFORM_PLUGIN_PATH for RQT and RVIZ2"
```





## fastdds_ibex_config.xml

This file was created to configure `fastdds` to deconflict the Ouster (ethernet) network from the main IBEX (USB ethernet) network for ROS2 node discovery and broadcasting via multicast.  The contents are found below and the file can be found in the `../docs/place_in_home` directory.  This is CAO 22 Jul 26.

```xml
<?xml version="1.0" encoding="UTF-8"?>
<dds>
  <profiles xmlns="http://www.eprosima.com/XMLSchemas/fastRTPS_Profiles">
    <transport_descriptors>
      <transport_descriptor>
        <transport_id>usb_only</transport_id>
        <type>UDPv4</type>
        <interfaceWhiteList>
          <address>192.168.200.199</address>
          <address>127.0.0.1</address>
        </interfaceWhiteList>
        <maxMessageSize>1470</maxMessageSize>
      </transport_descriptor>
    </transport_descriptors>
    <participant profile_name="usb_participant" is_default_profile="true">
      <rtps>
        <userTransports>
          <transport_id>usb_only</transport_id>
        </userTransports>
        <useBuiltinTransports>false</useBuiltinTransports>
      </rtps>
    </participant>
    <data_writer profile_name="default_writer" is_default_profile="true">
      <qos>
        <publishMode>
          <kind>ASYNCHRONOUS</kind>
        </publishMode>
      </qos>
    </data_writer>
  </profiles>
</dds>
```
The IP under address is the machine's own IP.  Note that localhost is needed as well (for it to broadcast on its internal network).  We also decrease the max message size so that large ROS message fragmentation happens at the DDS not Wifi level.  Setting the writer to asynchronous mode makes packets nonblocking.

**Note**: It was also necessary to allow incoming information through the `ufw` firewall via the USB ethernet interface by adding a rule with the following command.

```bash
sudo ufw allow in on enxc8a362bd9823
```

Note that `enxc8a362bd9823` is the USB ethernet device.


### Problem with /ouster/points

The 3D Ouster points message is large.  Originally, the split was occuring on the wifi-side, so we reduced the max message so the fragmentation occured in DDS.  However, this resulted in too many packets (~2200 1400b messages) being sent to the wifi radio too quickly, so packets would be dropped.  This was tested with python scripts, a sender on the `volta` side and a receiver on the `bow` side.

Volta:

```bash
river@volta:~$ python3 -c "
import socket
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
for i in range(2200):
    msg = str(i).encode() + b'x' * (1400 - len(str(i)))
    s.sendto(msg, ('192.168.200.200', 9999))
print('Sent 2200 packets')"
Sent 2200 packets
river@volta:~$ python3 -c "
import socket, time
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
for i in range(2200):
    msg = str(i).encode() + b'x' * (1400 - len(str(i)))
    s.sendto(msg, ('192.168.200.200', 9999))
    time.sleep(0.0005)
print('Sent 2200 packets with delay')"
Sent 2200 packets with delay
river@volta:~$  
```


Bow:
```bash
convoy@RIVeR-Bow:~$ python3 -c "
import socket
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('0.0.0.0', 9999))
received = set()
s.settimeout(5)
try:
    while True:
        data, addr = s.recvfrom(2000)
        num = int(data.decode().rstrip('x'))
        received.add(num)
except socket.timeout:
    pass
print(f'Received {len(received)} out of 2')"nt(f'First few missing: {missing[:20]}
Received 1437 out of 2200 expected
Missing count: 763
First few missing: [256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275]
convoy@RIVeR-Bow:~$ python3 -c "
import socket
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('0.0.0.0', 9999))
received = set()
s.settimeout(5)
try:
    while True:
        data, addr = s.recvfrom(2000)
        num = int(data.decode().rstrip('x'))
        received.add(num)
except socket.timeout:
    pass
print(f'Received {len(received)} out of 2')"nt(f'First few missing: {missing[:20]}
Received 2200 out of 2200 expected
Missing count: 0
First few missing: []
```

At a 5us delay, a few packets are lost, but at 10us, all arrive safely.  If we could add a rate limiter to the fastdds xml, this would be solved and the Ouster lidar messages would likely flow over WiFi.  For now, though, those messages will only successfully transmit via ethernet.  If Rviz2/etc is needed on another computer, connect to the Ibex network (192.168.200.X) via ethernet.


#### Troubleshooting steps ruled out

Several other causes were investigated and ruled out before identifying WiFi radio TX pacing as the root cause:

- **Socket receive buffers** (`net.core.rmem_max` / `rmem_default`): temporarily raised from default (212992) to 8388608 on Bow and Volta during testing. Did not resolve the issue. Reverted back to default (212992) and confirmed ethernet transmission still works cleanly — this setting is not required.

- **IP-level reassembly** (`/proc/net/snmp` `ReasmFails`): with FastDDS's default `maxMessageSize`, the ~3MB point cloud triggered IP-level fragmentation (~44 fragments per message), and `ReasmFails` confirmed the vast majority failed to reassemble over WiFi. Fixing `maxMessageSize` to 1470 (below MTU) moved fragmentation into FastDDS/RTPS instead of IP, which resolved the reassembly failures — but only shifted the bottleneck to WiFi TX pacing (see below).

- **`net.core.netdev_max_backlog`**: raised from 1000 to 5000 on Bow. Did not change loss rate or pattern in raw UDP burst tests.

- **FastDDS flow controllers / rate limiting**: attempted to add a `flow_controller_descriptor` to pace outgoing RTPS fragments, which would have solved this at the DDS level over WiFi. However, this requires setting `RMW_FASTRTPS_USE_QOS_FROM_XML=1` for `rmw_fastrtps` to honor the XML's publish mode/flow controller settings — and doing so caused a repeatable segfault in this ROS2 Humble + FastDDS 2.6.11 setup, even with correctly-formed XML. Do not set `RMW_FASTRTPS_USE_QOS_FROM_XML=1` in this environment unless this is revisited and solved differently. A future attempt might try implementing rate limiting outside of DDS (e.g., in the sensor driver itself) instead.

- **WiFi signal/power management**: signal strength was strong (-50 dBm, 60/70 link quality) and disabling WiFi power management (`iwconfig wlp0s20f3 power off`) made no difference — ruling out weak signal or radio sleep cycling as the cause.

#### Root cause (confirmed)

Raw UDP burst testing (Python scripts, no ROS/DDS involved) proved that sending ~2200 packets of ~1400 bytes back-to-back over WiFi drops 25-45% of them, while adding even a small delay (10 microseconds per packet) between sends resulted in zero loss. This confirms the WiFi radio's transmit queue cannot keep up with FastDDS's un-paced fragment bursts — packets are dropped at the radio TX level before ever reaching the air, not due to bandwidth, signal quality, or any OS-level buffer/queue setting.

#### Ethernet confirmed working

Connecting Bow directly to the IBEX network (192.168.200.X) via USB ethernet was tested and **confirmed** to resolve the issue completely — `/ouster/points` streams cleanly to RViz2 at full rate with no drops, using default kernel settings (no `rmem` tuning needed). This is the current recommended approach for viewing/using any high-bandwidth topic (point clouds, etc.) from a remote machine, until a native WiFi-side pacing solution is found.
