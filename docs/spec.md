# IBEX

ROS2 workspace for the IBEX ground vehicle's autonomy stack: GTSAM-based state estimation, sensor drivers, and bringup/launch orchestration. Mixed Python (`rclpy`) and C++ (`rclcpp`) ROS2 packages; several sensor drivers (`ouster-ros`, `kiss-icp`, `insta360_ros_driver`) are vendored source in this workspace, not just binary dependencies, so driver-level bugs are fixable in-repo.

## Architecture

- **`ibex_state`** -- GTSAM-based state estimation. `graph_frontender.py` is a thin ROS2 node wrapper: a ~6Hz timer drives `add_primary` (bicycle dynamics), and sensor subscriptions drive `add_lidar`/`add_imu` (GPS not yet wired). It never touches gtsam types directly -- it extracts plain tuples/arrays from ROS messages and hands them to the estimator, which owns all gtsam-specific conversions.
- **`estimator/estimators.py`**'s `GraphReckoner` is the actual factor graph: a fixed-lag smoother (`gtsam_unstable.IncrementalFixedLagSmoother`) with a primary pose+velocity backbone, lidar odometry via a latent drift-chain + residual nodes, dual IMU preintegration (Ouster + Insta360, extrinsics via `setBodyPSensor`), a non-holonomic constraint, and a roll/pitch-rate chain.
- **`estimator/factors.py`** holds custom factors (NHC, rate-tie, rate constant-velocity, lidar-drift), all `gtsam.CustomFactor`s with numerical (not analytic) Jacobians. **`estimator/symbols.py`** holds the fixed single-character gtsam symbol prefixes, asserted unique at import time.
- **`ibex_bringup`** launch files layer as `static_tf.launch.py` (sensor extrinsics) -> `sensors.launch.py` (`ouster_ros`, `insta360_ros_driver`) -> `processing.launch.py` (`kiss_icp` lidar odometry, `graph_frontender`) -> `control.launch.py` (`shared_link_bridge` vehicle control bridge), all included by `system_bringup.launch.py`.
- **`docs/ben_notes.md`** tracks dated, most-recent-first status and debugging entries. This file (`spec.md`) is for durable project structure and conventions -- put status updates and debugging narratives in `ben_notes.md` instead.

## Process

For any non-trivial feature or change (not one-line fixes):

1. **Interview first.** Ask clarifying questions throughout -- before planning, and again whenever a decision comes up mid-implementation that the user hasn't already settled (e.g. naming, scope, units/frame/library choices). Don't guess on ambiguous or high-blast-radius decisions.
2. **Plan, then get approval before writing code.** Produce a checklist of the concrete steps (tracked live, e.g. via TodoWrite), then summarize the proposed changes and wait for explicit approval/discussion before implementing anything.
3. **Implement incrementally**, checking items off the plan as they land.
4. **Verify before handing back.** For ROS2 changes this usually means an actual `ros2 launch` smoke test, not just a syntax check -- a clean import or passing type check is not the same as confirmed working behavior.
5. **Stop and wait for feedback** at the end of a batch of changes rather than continuing on to unrequested follow-on work.

## Coding style

- Short, physics/math-convention variable names are the norm in estimator code (`dt`, `dx`, `roll`, `pitch`, `yaw`, `v`, `delta`) -- don't rename these to spelled-out equivalents.
- Inline comments justify non-obvious decisions (e.g. `add_lidar`'s note on why the drift node's creation is deferred to the first real reading, or the `TODO: update with heading/velocity from GPS sensor` markers), never restate the code.
- Debug prints are tagged by method name, e.g. `print("[add_lidar] ...")`, `print("[_push] ...")`, gated by a per-node/estimator `debugging`/`enable_debugging` constructor flag, off by default.
- Named sections within a file or class are separated by a centered label between rule characters -- e.g. `#==========# Parameters #==========#` in `graph_frontender.py`, or `#------------------# Helpers #------------------#` in `estimators.py`. Match whichever weight the surrounding file already uses rather than inventing a third style.
- Multi-line docstrings are used freely when there's real rationale to convey (a design tradeoff, a bug workaround, a non-obvious invariant) -- brevity is not prioritized over explaining the *why*.

## Additional Rules

- Do not run git or GitHub commands (`add`, `commit`, `push`, branch operations, PR operations, etc.) on this repo -- the user manages version control themselves.
