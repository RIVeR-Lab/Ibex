# Ben Notes

## Weird debugging of Kiss-ICP

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