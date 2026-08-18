# IBEX

ROS2 workspace for the IBEX ground vehicle's autonomy stack: GTSAM-based state estimation, sensor drivers, and bringup/launch orchestration. Mixed Python (`rclpy`) and C++ (`rclcpp`) ROS2 packages, including several sensor drivers (`ouster-ros`, `kiss-icp`, `insta360_ros_driver`).




## TODO
1. Validate GPS + `gravity_align` + `odom_offset` with the vehicle actually moving -- validated stationary (indoors, no GPS fix) only so far. See `docs/ben_notes.md`.
2. Verify the Insta360 IMU extrinsic's yaw component (`insta_sensor -> insta_imu` in `static_tf.launch.py`) against the physical IMU die's real orientation -- current value is a shortest-arc estimate; a single accelerometer reading can't observe twist about the gravity axis. See `docs/ben_notes.md`.





## Architecture

- **`ibex_state`** -- GTSAM-based state estimation. `graph_frontender.py` is a thin ROS2 node wrapper: a ~6Hz timer drives `add_primary` (bicycle dynamics) and publishes `get_aligned_state()`/`get_aligned_covariance()` (frame `aligned_odom`, not raw `odom`) plus the `odom_offset` (UTM easting/northing/yaw) topic; sensor subscriptions drive `add_lidar`/`add_imu`/`add_gps` (`shared_link_bridge/GpsValues` on `/gps_values`). It never touches gtsam types directly -- it extracts plain tuples/arrays from ROS messages and hands them to the estimator, which owns all gtsam-specific conversions.
- **`estimator/estimators.py`**'s `GraphReckoner` is the actual factor graph: a fixed-lag smoother (`gtsam_unstable.IncrementalFixedLagSmoother`) with a primary pose+velocity backbone, lidar odometry via a latent drift-chain + residual nodes, dual IMU preintegration (Ouster + Insta360, extrinsics via `setBodyPSensor`), a non-holonomic constraint, a roll/pitch-rate chain, a `gravity_align` chain (odom's tilt relative to true gravity-level, estimated from stationary accelerometer readings -- see `docs/ben_notes.md`), and an `odom_offset` chain (odom origin's true UTM easting/northing/yaw, estimated from GPS). `gravity_align`/`odom_offset` are deliberately decoupled from the backbone pose and lidar-drift chain (composing gravity correction into the backbone directly was tried and reverted -- see `ben_notes.md`); `get_aligned_state()`/`get_aligned_covariance()` compose them onto the backbone only at read time, into the `aligned_odom` frame.
- **`estimator/factors.py`** holds custom factors (NHC, rate-tie, rate constant-velocity, lidar-drift, gravity-align, GPS/odom-offset), all `gtsam.CustomFactor`s with numerical (not analytic) Jacobians. **`estimator/symbols.py`** holds the fixed single-character gtsam symbol prefixes, asserted unique at import time.
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
