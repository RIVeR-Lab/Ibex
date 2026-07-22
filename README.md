# IBEX

> [TODO: one-line description — e.g. "Autonomous off-road sensing and navigation platform built on a modified Yamaha Wolverine X4 850 R-Spec UTV."]

<!-- Optional: badges, a photo of the platform, DEVCOM funding acknowledgment -->

## Overview

[TODO: 2–4 sentences. What IBEX is, what this repo contains, who it's for.
Mention the four subsystems if useful: platform, convoy HMI, terrain traversability,
avatar evaluation.]

This is the **top-level IBEX repository**. It does not contain source code directly —
instead, the ROS 2 packages that make up the system live under `Packages/`. Most are
included as **git submodules** (some are forks maintained by the lab, some track
upstream); a few packages are committed directly into the repo. You must clone
recursively (see Installation) or the submodule directories under `Packages/` will be
empty.

## Repository Structure

```
ibex/
├── Data/            # rosbag files
├── Docs/            # documentation, hardware notes, datasheets, IBEX user manual
├── Packages/        # ROS 2 packages: git submodules (forks + upstream) and some committed directly
├── .gitignore
├── .gitmodules      # submodule definitions (URL + path for each package)
├── README.md
└── command_sheet.md # quick-reference commands
```

### Packages

| Package | Type | Purpose | Upstream |
|---------|------|---------|----------|
| `Packages/hyper_drive` | in-repo | [TODO] IMEC SWIR hyperspectral camera driver | [TODO: reference-package URL] |
| `Packages/hyper_drive_interface` | in-repo | [TODO] message/service definitions for hyper_drive | [TODO: reference-package URL] |
| `Packages/ibex_bringup` | in-repo | [TODO] top-level launch / system bringup | — |
| `Packages/ibex_state` | in-repo | [TODO] platform state estimation / publishing | — |
| `Packages/insta360_ros_drivers` | submodule (fork) | [TODO] Insta360 X4 360° camera driver | [TODO: fork + upstream URL] |
| `Packages/kiss-icp` | submodule (fork) | [TODO] LiDAR odometry front-end | [TODO: fork + upstream URL] |
| `Packages/ouster-ros` | submodule (fork) | [TODO] Ouster OS1-64 LiDAR driver | [TODO: fork + upstream URL] |
| `Packages/shared_link_bridge` | submodule (fork) | [TODO] | [TODO: fork + upstream URL] |
| `Packages/spectrometer_drivers` | in-repo | Ibsen VNIR/NIR point spectrometer driver nodes | — |
| `Packages/spectrometer_interfaces` | in-repo | Message/service definitions for spectrometer_drivers | — |

Pinned submodule commits (from `git submodule status` / `.gitmodules`): `insta360_ros_drivers @ 15d0c2b`,
`kiss-icp @ 1ffa7d7`, `ouster-ros @ a46cda1`. `shared_link_bridge` [TODO: add pinned SHA].

<!-- "Type" tells a reader whether a folder is a submodule (and if a fork) or a package
committed directly into this repo. hyper_drive and hyper_drive_interface are committed
directly here but are derived from / kept in sync with an external reference package —
note that reference source and how updates flow. For the forked submodules, note which
branch is authoritative and why the fork exists (e.g. patches not yet upstreamed). The
canonical list of submodules is .gitmodules; anything under Packages/ not listed there
is committed directly. -->

## Prerequisites

- **OS:** Ubuntu 22.04
- **ROS 2:** Humble
- **Build tool:** colcon
- Git (with submodule support — any modern Git)
- [TODO: system packages / drivers not handled by rosdep — e.g. FTDI libft4222 for the
  spectrometers, Ouster driver deps, CUDA/NVDEC for the Insta360. List per package or
  link to the relevant Docs/ note.]

## Installation

### 1. Create a colcon workspace and clone into it

This repo is a container, not a colcon workspace itself, so it lives inside a workspace's
`src/` directory. Create the workspace first, then clone into it. Because most packages
are git submodules, clone recursively:

```bash
mkdir -p ~/ibex_ws/src
cd ~/ibex_ws/src
git clone --recurse-submodules https://github.com/RIVeR-Lab/ibex.git
```

`--recurse-submodules` fetches the submodule packages under `Packages/`; the packages
committed directly into the repo come down as part of the normal clone. Both end up
populated.

**Already cloned without `--recurse-submodules`?** The submodules will be empty — pull
them in from inside the repo:

```bash
cd ~/ibex_ws/src/ibex
git submodule update --init --recursive
```

### 2. Install dependencies

[TODO: system-level deps that rosdep won't cover — e.g. the spectrometer driver needs
FTDI's libft4222 plus a udev rule. See Docs/<spectrometer note>.]

Resolve ROS 2 package dependencies with rosdep:

```bash
cd ~/ibex_ws
sudo rosdep init   # first time only
rosdep update
rosdep install --from-paths src --ignore-src -r -y
```

### 3. Build

```bash
cd ~/ibex_ws
colcon build
source install/setup.bash
```

colcon discovers every package under `Packages/` and resolves build order from the
dependency graph. To build a single package instead:

```bash
colcon build --packages-select <package-name>
```

> Add the source line to your `~/.bashrc` so the workspace is available in every shell:
> ```bash
> echo "source ~/ibex_ws/install/setup.bash" >> ~/.bashrc
> ```

### 4. Verify the build

```bash
ros2 pkg list | grep <expected-package>
```

[TODO: a concrete smoke-test — e.g. launching a driver or listing expected executables.]

## Usage

See `command_sheet.md` for a quick reference of common commands.

[TODO: a few representative launch commands, e.g.:]

```bash
# Example: spectrometer reference channel
ros2 launch spectrometer_drivers ibsen_launch.py
```

Recorded runs live in `Data/`; per-subsystem setup and field-test procedures are in `Docs/`.

## Working with Submodules

Update all submodules to the commits currently pinned by this repo:

```bash
git submodule update --init --recursive
```

Pull the latest changes from each submodule's tracked branch (only if you intend to
advance the pins):

```bash
git submodule update --remote
```

After updating submodule pointers, commit the new pins in this repo:

```bash
git add Packages/<name>
git commit -m "Bump <name> to <short-sha>"
```

**Editing code inside a submodule:** commit and push inside the submodule (`Packages/<name>`)
first, then commit the updated pointer here in `ibex`. A change isn't captured by the
top-level repo until you commit the moved pointer.

## Troubleshooting

| Symptom | Cause / Fix |
|---------|-------------|
| `Packages/` subdirectories are empty after clone | Cloned without `--recurse-submodules`. Run `git submodule update --init --recursive`. |
| colcon finds no packages | The repo must be cloned inside the workspace's `src/` (i.e. `~/ibex_ws/src/ibex`). Build from `~/ibex_ws`, not from inside the repo. |
| `colcon build` fails on a missing dependency | Run the rosdep step; some drivers need system libs not covered by rosdep — see `Docs/`. |
| A submodule is on the wrong commit after pulling | Run `git submodule update --init --recursive` to reset submodules to the repo's pinned commits. |
| [TODO: known IBEX-specific gotcha] | [TODO] |

## Documentation

Hardware notes, datasheets, team-handoff documentation, and the **IBEX user manual**
live in `Docs/`. [TODO: link the user manual directly once its path is set, e.g.
`Docs/user-manual/README.md`.]

`command_sheet.md` (repo root) is a quick-reference cheat sheet for common commands.

## Acknowledgments

[TODO: DEVCOM funding, RIVeR Lab / Northeastern, contributors.]

## License

[TODO: license or "Internal — RIVeR Lab" if not open-sourced.]