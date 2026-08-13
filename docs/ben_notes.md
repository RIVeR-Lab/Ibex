# Ben Notes

## 13 Aug 26 1651 -- GPS wiring blocked on hardware/protocol research

Next up is wiring GPS into `graph_frontender` via `shared_link_bridge`'s custom `GpsValues` message (topic `/gps_values`: `auto_gps_head`, `auto_gps_lat`, `auto_gps_lon`, `auto_gps_vel`, `auto_gps_references`, `auto_gps_quality`, `auto_gps_stamp`). The estimator's `add_gps` already has two `TODO: update with heading/velocity from GPS sensor` comments, and it turns out the real message carries exactly that -- heading and velocity beyond just position.

Blocked on Ben researching (likely from the Kairos hardware manual, or empirical testing against a known reference) before implementation can proceed:

- Units of `auto_gps_lat`/`auto_gps_lon` (degrees vs radians), `auto_gps_head` (degrees? true vs magnetic north? course-over-ground, which is unreliable near-zero speed?), and `auto_gps_vel` (m/s vs knots vs km/h) -- nothing in `shared_link_bridge_node.py` documents any of this, it's a straight `float()`/`int()` parse of the raw Kairos UDP protocol.
- What `auto_gps_quality` and `auto_gps_references` actually encode -- no covariance is transmitted in the message at all, so these are the only fix-confidence proxies, but they're unmapped raw ints today.
- Physical mounting offset of the GPS antenna relative to `base_link` -- there's no static-tf entry for it yet, unlike the Ouster/Insta360 work.
- How to align the true-North-referenced lat/lon conversion with the estimator's world frame, which is anchored to whatever heading the vehicle happened to be pointed at on startup, not true North.

## 13 Aug 26 1651 -- IMU integration status (Ouster + Insta360)

Both IMUs are now wired into `graph_frontender` and confirmed working end-to-end on a live launch: every primary tick logs `IMU added` twice (once per IMU), and the estimated pose stays sane (sub-mm/sub-mrad noise, no NaNs) with the vehicle stationary.

Along the way:

- Renamed the Insta360 driver's hardcoded IMU `frame_id` from `imu_frame` to `insta_imu`, and added a static transform for it (`insta_sensor -> insta_imu`, identity offset -- the IMU die's real offset from `insta_sensor` hasn't been measured, so this is a placeholder).
- Namespaced the whole Insta360 pipeline under `/insta360` (new `config/namespace.yaml` in `insta360_ros_driver`), since several of its topics were hardcoded absolute and needed fixing to actually respect a namespace.
- Extrinsics for both IMUs (`base_link -> os_imu`, `base_link -> insta_imu`) are looked up via tf2 at `graph_frontender` startup and fed into GTSAM's `setBodyPSensor`, since neither IMU is collocated with `base_link` (the Ouster IMU alone is ~1.4m forward / 1.7m up -- a real lever arm during turns).
- `graph_frontender` was never actually included in any bringup launch file until now -- it's wired into `ibex_bringup/launch/processing.launch.py` alongside `kiss_icp`.
- Fixed a segfault on the very first `add_lidar` call: `GraphReckoner.__init__` timestamped its `t=0` nodes (`pose0`, `vel0`, both IMU `bias0`s, `rate0`) with the literal float `0.0` in the smoother's timestamp map, while everything added later uses real epoch wall-clock time. That ~1.79-billion-second gap made the smoother try to marginalize all the `t=0` nodes on the next update, including the IMU bias priors, which are only prior-connected (isolated) until the first `add_primary` call -- the same "isolated variable" crash shape as the lidar-drift-node bug below, just via different variables. Fixed by timestamping those nodes with `init_time` instead of `0.0`. In hindsight, the original lidar-drift fix (deferring node creation to the first real lidar reading) plausibly worked for the same underlying reason, since it also happened to give that node a real-epoch timestamp instead of `0.0` -- unconfirmed, but the isolated-node framing alone may have been an incomplete diagnosis.

Open item: `imu_filter_madgwick` (which we don't depend on, but runs alongside the Insta360 driver) warned "still waiting for data" for its entire ~70s runtime in earlier test runs. Inconclusive whether this is a real problem or just needs a longer test window -- worth a 30s+ soak test to confirm it goes quiet.

## 30 Jul 26 -- Weird debugging of Kiss-ICP

Seems like empty lidar messages were causing a time delta of 0 and division then led to NaN then crash.  At least, that is hypothesis.

**Note**: deskew is now set to false for kiss-icp, which may cause accuracy issues down the road

Claude debugging summary:

```bash
Debugging Summary

The estimator failed on the first live primary tick. The node used wall-clock time, but the estimator started its internal clock at zero. This made the first time step very large. Wrong noise values and a bad pose came from this large time step. We fixed this by passing the real start time into the estimator at startup.

The estimator then crashed with a segfault inside GTSAM's smoother. We found the cause with gdb: a lidar-drift node was created at startup but never used, because lidar was not connected yet. This isolated node broke the smoother's internal marginalization step. We fixed this by creating the drift node only when the first real lidar reading arrives.

We found and fixed two unrelated node bugs. The ROS node crashed on shutdown due to a missing import. A second node crashed at startup due to a missing class import. Both fixes were small import corrections.

We found that the Ouster lidar driver dropped packets. We ruled out CPU load, USB bandwidth, and network link quality as causes. The true cause was excess processing work in one single-threaded pipeline stage. We fixed this by disabling unused processing outputs in the driver config.

KISS-ICP then crashed on real lidar data. We used gdb to find the exact error: a NaN value inside a motion calculation during deskewing. We traced this to empty lidar scans, where no points had valid data. We fixed this in two ways: we told the driver to skip scans with too few valid points, and we disabled deskewing as a backup safeguard.
```
