# IBEX Workspace

ROS 2 workspace for the IBEX autonomous ground vehicle platform.

## Prerequisites

- Ubuntu 22.04
- ROS 2 Humble
- `python3-vcstool`, `python3-colcon-common-extensions`, `python3-rosdep`

```bash
sudo apt install python3-vcstool python3-colcon-common-extensions python3-rosdep
sudo rosdep init   # only first time on a machine
rosdep update
```

## Setup

```bash
git clone <this-repo-url> ibex_ws
cd ibex_ws
mkdir -p src
vcs import src < ibex.repos
rosdep install --from-paths src --ignore-src -r -y
colcon build --symlink-install
source install/setup.bash
```

## System dependencies

### Ouster LiDAR
```bash
sudo apt install libzip-dev
sudo ufw allow 7502/udp
sudo ufw allow 7503/udp
```
Configure sensor to a link-local subnet (169.254.x.x range) matching host.

### Insta360 camera
[any specific notes about USB rules, drivers, etc.]

## Package overview

- `<your_package_1>` — brief description
- `<your_package_2>` — brief description
- [etc.]

## Updating third-party packages

To pull the latest pinned versions:
```bash
vcs import src < ibex.repos --force
```

To update a pin, edit `ibex.repos` and commit.

# Hyperspectral Camera Installation Guide

> **Platform:** ROS2 Humble | Ubuntu 22.04

---

## Table of Contents
- [Step 1: Install Required Libraries](#step-1-install-required-libraries)
- [Step 2: Connect the Cameras](#step-2-connect-the-cameras)
- [Step 3: Set Up ROS2](#step-3-set-up-ros2)
- [Step 4: Start and View Camera Output](#step-4-start-and-view-camera-output)

---

## Step 1: Install Required Libraries

The following four libraries are required:

| Library | Version |
|---|---|
| IMEC HSI Mosaic | 1.12.0.0 |
| Ximea SDK | LTS v4.32.0.0 |
| Pleora SDK | 6.5.3 |
| Photon Focus SDK | 2025.1.0_Linux64 |

### Easy Installation

Copy the contents of the `place_in_opt` folder into `/opt`.

> **Note:** Copy the *contents* of the folder, not the folder itself. You will need superuser privileges to edit `/opt`. Run the following command to open the file explorer with superuser privileges:
> ```bash
> sudo nautilus
> ```

### Alternate Installation

Each library can be installed separately from the corresponding vendor websites:

<details>
<summary><b>IMEC HSI Mosaic</b></summary>

- **URL:** https://imecinternational.sharepoint.com/sites/hsisupport
- **Contact:** hsisupport@imec.be (contact for portal access)
- Navigate to: `Camera > Software > HSI Mosaic` and download the Linux installer

> The default installer installs version 2.11.10.0. Contact support to request version 1.12.0.0.

</details>

<details>
<summary><b>Ximea SDK</b></summary>

- **URL:** https://www.ximea.com/software-downloads
- Place the downloaded `.tgz` installer inside `/opt/imec/hsi-mosaic/resources/installers`
- Follow the installation instructions on the website

</details>

<details>
<summary><b>Pleora SDK</b></summary>

- **URL:** https://supportcenter.pleora.com/s/article/eBUS-SDK-6-x-x-Software-and-Release-Notes-Dwnload
- **Contact:** support@pleora.com (contact for access)
- **Account:** Email reichenberg.a@northeastern.edu for login credentials
- Place the downloaded `.tgz` installer inside `/opt/imec/hsi-mosaic/resources/installers`
- Install to the `/opt` folder

</details>

<details>
<summary><b>Photon Focus SDK</b></summary>

- **URL:** https://www.photonfocus.com/support/software/
- Place the downloaded `.tgz` installer inside `/opt/imec/hsi-mosaic/resources/installers`
- Install to the `/opt` folder

</details>

After installation, verify that the following directories exist in `/opt`:
```
/opt/imec
/opt/XIMEA
/opt/pleora
/opt/PFSDK_2025.1.0_Linux64
```

---

### Required Symbolic Links for HSI Mosaic

> **Background:** HSI Mosaic was developed for Ubuntu 18 and is not natively compatible with Ubuntu 22. The SDK versions listed above are Ubuntu 22 compatible, but HSI Mosaic still references the old library filenames. Symbolic links redirect HSI Mosaic to the updated libraries. This works because Pleora eBUS SDK 6.5.3 (Ubuntu 22) uses the same headers as 6.1.1 (Ubuntu 18).
>
> Pleora eBUS SDK 7.0.0 is **not** compatible with this approach — use version 6.5.3.

#### In `/opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib`

Run the following commands:

```bash
cd /opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib

ln -s libPvBase.so.6.5.3.7155 libPvBase.so.6.1.1.5002
ln -s libPvBuffer.so.6.5.3.7155 libPvBuffer.so.6.1.1.5002
ln -s libPvDevice.so.6.5.3.7155 libPvDevice.so.6.1.1.5002
ln -s libPvGenICam.so.6.5.3.7155 libPvGenICam.so.6.1.1.5002
ln -s libPvSerial.so.6.5.3.7115 libPvSerial.so.6.1.1.5002
ln -s libPvStream.so.6.5.3.7115 libPvStream.so.6.1.1.5002
ln -s libPvSystem.so.6.5.3.7115 libPvStream.so.6.1.1.5002
```

#### In `/opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib/genicam/bin/Linux64_x64`

Run the following commands:

```bash
cd /opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib/genicam/bin/Linux64_x64

ln -s libGCBase_gcc48_v3_1.so libGCBase_gcc42_v3_1.so
ln -s liblog4cpp_gcc48_v3_4.so liblog4cpp_gcc42_v3_1.so
ln -s libNodeMapData_gcc48_v3_4.so libNodeMapData_gcc42_v3_1.so
```

---

## Step 2: Connect the Cameras

> **The IMEC camera must be connected and disconnected in a specific order:**
> - **Connecting:** 1. Connect USB to computer > 2. Connect power
> - **Disconnecting:** 1. Disconnect power > 2. Disconnect USB from computer

The Ximea and Alvium cameras can be connected via USB in any order.

After connecting all cameras, set the correct USB permissions:

```bash
# List connected USB devices and find the bus and device numbers
lsusb

# Set permissions for each camera (repeat for all three)
sudo chmod 777 /dev/bus/usb/[bus_number]/[device_number]
```

Run the `chmod` command three times — once for each camera (Pleora/IMEC, Ximea, Allied Vision).

---

## Step 3: Set Up ROS2

Install ROS2 Humble if not already installed:
- https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debs.html

Copy `aidan_ws` to your desired directory, then open a terminal and navigate to it:

```bash
cd aidan_ws
```

Source the ROS2 Humble underlay:

```bash
source /opt/ros/humble/setup.bash
```

> **Note:** On the lab computer this may happen automatically. To make it permanent, run:
> ```bash
> echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
> ```
> Reference: https://docs.ros.org/en/humble/Tutorials/Beginner-CLI-Tools/Configuring-ROS2-Environment.html

Build the workspace and source the overlay:

```bash
colcon build
source install/setup.bash
```

---

## Step 4: Start and View Camera Output

> **Note:** The current configuration is set up for testing and only outputs images from the IMEC camera.

In a terminal while in `aidan_ws`, launch the cameras:

```bash
ros2 launch hyper_drive hyper_drive_launch.py
```

In a new terminal, open the image viewer:

```bash
ros2 run rqt_image_view rqt_image_view
```

Click the **refresh button** at the top of the image viewer, then select a topic from the dropdown to view the output of the different spectral bands from the hyperspectral camera.
