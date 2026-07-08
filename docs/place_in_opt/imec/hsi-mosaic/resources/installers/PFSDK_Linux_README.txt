Getting started with PFSDK under Linux x86

In the resources directory you can find install_resources.sh to aid the installing process.
During the installation process, dkms is used.  It could be dkms fails to install.
If so, use the following
    sudo apt update
    sudo apt install dkms

Then again run the script that installs the filter driver
    cd /opt/photonfocus/PFSDK/driver/dkms
    sudo chmod u+x install_with_dkms.sh
    ./install_with_dkms.sh

Since most cameras consume a lot of network bandwidth, some network adapter settings need to
be changed.  Follow these steps:

* Power the camera and connect the camera to an on-board NIC.  
  If the camera is connected directly without a switch, do the following.  
  In the network adaptor settings for your camera, in the ipv4 and ipv6 tabs, set your connection to link-local.  
  This will make sure an IP address in the 169.x.x.x range is assigned, instead of waiting for an IP address 
  to be assigned externally.

* Enable Jumbo packets by adapting the MTU
  - In principle this can be set under the identity tab of the network adaptor settings, but this is not 
    guaranteed to work.
  - Instead, inside a terminal, find out the name of the network interface using
    ip address show
  - Then set the MTU as follows to 9000
    sudo ip link set enp0s31f6 mtu 9000
    (replace enp0s31f6 with the name of your network interface)
  - Best to unpower/power your camera to ensure its state (including any DeviceStreamChannelPacketSize) is reset and wait for 10 seconds.
  - Double check the new MTU size by using another call to
    ip address show

* There is extra information from PhotonFocus found within PFSDK_Linux_quickstart_guide.PDF

