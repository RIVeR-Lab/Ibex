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