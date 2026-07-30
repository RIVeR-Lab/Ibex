"""GraphReckoner: fixed-interval primary pose chain, with lidar/gps
attached via transient residual nodes .

Interface is deliberately callback-shaped (add_primary / add_lidar /
add_gps / add_imu / get_estimate) so a ROS2 node can be a thin wrapper:
timer -> add_primary, each subscription -> its add_* method.
"""
import numpy as np
import gtsam
import gtsam_unstable

from .factors import make_nhc_factor, make_rate_tie_factor, make_rate_cv_factor, make_lidar_drift_factor
from . import symbols as S

GRAVITY = 9.81

# =========================================== #

def _state_to_pose3(state):
    """x, y, z, roll, pitch, yaw -> gtsam.Pose3"""
    x, y, z, roll, pitch, yaw = state
    return gtsam.Pose3(gtsam.Rot3.Ypr(yaw, pitch, roll), gtsam.Point3(x, y, z))


def _pose3_to_state(pose):
    """gtsam.Pose3 -> x, y, z, roll, pitch, yaw"""
    rot = pose.rotation()
    return np.array([pose.x(), pose.y(), pose.z(), rot.roll(), rot.pitch(), rot.yaw()])


def _sigmas_xyzrpy_to_gtsam(std_xyzrpy):
    """Reorder (x, y, z, roll, pitch, yaw) into
    gtsam's Pose3 tangent order, which is rotation-first:
    (roll, pitch, yaw, x, y, z)."""
    x, y, z, roll, pitch, yaw = std_xyzrpy
    return np.array([roll, pitch, yaw, x, y, z])


def _bias_noise_sigmas(gyro_bias_walk_std, accel_bias_walk_std, dt):
    """Build the 6 sigmas for a BetweenFactorConstantBias's random-walk
    noise, from separate gyro (rad/s) and accel (m/s^2) bias-walk
    stds
 
    Each std is scaled by sqrt(dt), consistent with modeling bias as a
    random walk (variance grows linearly with time, so std grows as
    sqrt(time))
    """
    accel_sigma = accel_bias_walk_std * np.sqrt(dt)
    gyro_sigma = gyro_bias_walk_std * np.sqrt(dt)
    return np.array([accel_sigma] * 3 + [gyro_sigma] * 3)



# =========================================== #




class GraphReckoner:
    def __init__(self, wheelbase = 1.2,
                 init_time=0.0,
                 init_state=(0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
                 prior_noise_std=0.001,
                 dyn_noise_std=(0.005, 0.005, 1.0, 1.0, 1.0, 0.001),
                 lidar_noise_std=(0.05, 0.05, 0.05, 0.02, 0.02, 0.02),
                 lidar_drift_prior_std=(0.01, 0.01, 0.01, 0.005, 0.005, 0.005),
                 lidar_drift_process_noise_std=(0.01, 0.01, 0.01, 0.005, 0.005, 0.005),
                 imu_configs=None, init_velocity=(0.0, 0.0, 0.0), gravity=GRAVITY,
                 init_velocity_noise_std=1.0,
                 nhc_noise_std=(0.001, 0.001),
                 enable_lidar=True, enable_IMUs=True, enable_NHC=True,
                 enable_rate=True, enable_gps=True,
                 rate_prior_std=(1.0, 1.0),
                 rate_process_noise_std=(0.5, 0.5),
                 rate_tie_noise_std=(0.01, 0.01),
                 gps_noise_std=(1.5, 1.5, 3.0),
                 lag_seconds=5.0,
                 residual_prop_noise_std=(0.01, 0.01, 1.0, 1.0, 1.0, 0.01),
                 gps_prop_noise_std=(0.5, 0.5, 0.5, 0.5, 0.5, 0.5),
                 fallback_vel_noise_std=5.0,
                 debugging=False,
                ):
        """Implements a GTSAM-based factor graph for IBEX state estimation"""

        self.wheelbase = wheelbase
        self.gravity = gravity
        self.last_primary_time = init_time

        self.enable_lidar = enable_lidar
        self.enable_IMUs = enable_IMUs
        self.enable_NHC = enable_NHC
        self.enable_rate = enable_rate
        self.enable_gps = enable_gps
        self.uses_velocity = enable_IMUs or enable_NHC or enable_rate

        self.dyn_noise_std = dyn_noise_std
        self.dyn_noise = gtsam.noiseModel.Diagonal.Sigmas(_sigmas_xyzrpy_to_gtsam(dyn_noise_std))
        self.residual_prop_noise = gtsam.noiseModel.Diagonal.Sigmas(_sigmas_xyzrpy_to_gtsam(residual_prop_noise_std))
        self.gps_prop_noise = gtsam.noiseModel.Diagonal.Sigmas(_sigmas_xyzrpy_to_gtsam(gps_prop_noise_std))
        self.nhc_noise = gtsam.noiseModel.Diagonal.Sigmas(np.array(nhc_noise_std))
        self.gps_noise = gtsam.noiseModel.Diagonal.Sigmas(np.array(gps_noise_std))

        self.lidar_measurement_noise = gtsam.noiseModel.Diagonal.Sigmas(_sigmas_xyzrpy_to_gtsam(lidar_noise_std))
        self.lidar_drift_prior_noise = gtsam.noiseModel.Diagonal.Sigmas(_sigmas_xyzrpy_to_gtsam(lidar_drift_prior_std))
        self.lidar_drift_process_noise = gtsam.noiseModel.Diagonal.Sigmas(_sigmas_xyzrpy_to_gtsam(lidar_drift_process_noise_std))
        self._lidar_drift_process_noise_std = np.array(lidar_drift_process_noise_std)
        self.fallback_vel_noise = gtsam.noiseModel.Isotropic.Sigma(3, fallback_vel_noise_std)

        self.debugging = debugging

        # Set up based on enabled sensors
        if enable_IMUs:
            imu_configs = imu_configs or {}
            self.imu_names = list(imu_configs.keys())
            self.imu_pim = {}
            self.imu_bias_prefix = {}
            self.imu_bias_walk_std = {}
            prefixes = {}
            for name in self.imu_names:
                if 'ouster' in name:
                    prefixes[name] = S.IMU_BIAS_OUSTER
                elif 'insta' in name:
                    prefixes[name] = S.IMU_BIAS_INSTA
                else:
                    raise ValueError(
                        f"No fixed symbol prefix configured for IMU '{name}' -- "
                        f"add one to symbols.py rather than assigning ad hoc."
                    )
            for name in self.imu_names:
                cfg = imu_configs[name]
                params = gtsam.PreintegrationParams.MakeSharedU(gravity)
                params.setGyroscopeCovariance(np.eye(3) * cfg.get('gyro_noise_std', 0.01) ** 2)
                params.setAccelerometerCovariance(np.eye(3) * cfg.get('accel_noise_std', 0.05) ** 2)
                params.setIntegrationCovariance(np.eye(3) * cfg.get('integration_noise_std', 1e-4) ** 2)
                self.imu_pim[name] = gtsam.PreintegratedImuMeasurements(params, gtsam.imuBias.ConstantBias())
                self.imu_bias_prefix[name] = prefixes[name]
                self.imu_bias_walk_std[name] = (
                    cfg.get('gyro_bias_walk_std', 0.0005),
                    cfg.get('accel_bias_walk_std', 0.001),
                )

        if enable_rate:
            self.rate_prior_noise = gtsam.noiseModel.Diagonal.Sigmas(np.array(rate_prior_std))
            self.rate_process_noise_std = np.array(rate_process_noise_std)  # scaled by sqrt(dt) at point of use
            self.rate_tie_noise = gtsam.noiseModel.Diagonal.Sigmas(np.array(rate_tie_noise_std))

        # Graph / smoother setup
        self.graph = gtsam.NonlinearFactorGraph()
        self.initial = gtsam.Values()
        self.timestamps = gtsam_unstable.FixedLagSmootherKeyTimestampMap()
        isam_params = gtsam.ISAM2Params()
        isam_params.relinearizeSkip = 1
        self.smoother = gtsam_unstable.IncrementalFixedLagSmoother(lag_seconds, isam_params)

        self.pose_index = 0
        self.drift_index = 0
        self.lidar_res_index = 0
        self.gps_res_index = 0
        self._lidar_drift_initialized = False

        self.pending_residuals = []  # list of (key, t, kind) awaiting a propagation link to the next primary

        # Init graph
        key0 = gtsam.symbol(S.POSE, 0)
        pose0 = _state_to_pose3(init_state)
        prior_noise = gtsam.noiseModel.Diagonal.Sigmas(np.array([prior_noise_std] * 6))
        self.graph.add(gtsam.PriorFactorPose3(key0, pose0, prior_noise))
        self.initial.insert(key0, pose0)
        self.timestamps.insert((key0, 0.0))

        if self.uses_velocity:
            vel0_key = gtsam.symbol(S.VELOCITY, 0)
            vel0 = np.array(init_velocity, dtype=float)
            vel_prior_noise = gtsam.noiseModel.Isotropic.Sigma(3, init_velocity_noise_std)
            self.graph.add(gtsam.PriorFactorVector(vel0_key, vel0, vel_prior_noise))
            self.initial.insert(vel0_key, vel0)
            self.timestamps.insert((vel0_key, 0.0))

        if enable_IMUs:
            bias_prior_noise = gtsam.noiseModel.Isotropic.Sigma(6, 0.1)
            for name in self.imu_names:
                bkey = gtsam.symbol(self.imu_bias_prefix[name], 0)
                self.graph.add(gtsam.PriorFactorConstantBias(bkey, gtsam.imuBias.ConstantBias(), bias_prior_noise))
                self.initial.insert(bkey, gtsam.imuBias.ConstantBias())
                self.timestamps.insert((bkey, 0.0))

        if enable_rate:
            r0_key = gtsam.symbol(S.RATE, 0)
            r0 = np.zeros(2)
            self.graph.add(gtsam.PriorFactorVector(r0_key, r0, self.rate_prior_noise))
            self.initial.insert(r0_key, r0)
            self.timestamps.insert((r0_key, 0.0))

        self._push()
        est = self.smoother.calculateEstimate()
        self.state = _pose3_to_state(est.atPose3(key0))
        if self.uses_velocity:
            self.velocity = est.atVector(vel0_key)



        
        if self.debugging:
            print("[__init__] Finished init")




    #------------------# Helpers #------------------#

    def _push(self):
        """Push accumulated graph/initial/timestamps into the smoother
        and reset the accumulators. Called after every add_* method
        
        IncrementalFixedLagSmoother is designed for
        frequent small incremental updates.
        """
        self.smoother.update(self.graph, self.initial, self.timestamps)
        if self.debugging:
            print("[_push] Smoother updated")

        self.graph = gtsam.NonlinearFactorGraph()
        if self.debugging:
            print("[_push] New graph made")

        self.initial = gtsam.Values()
        if self.debugging:
            print("[_push] New values")

        self.timestamps = gtsam_unstable.FixedLagSmootherKeyTimestampMap()
        if self.debugging:
            print("[_push] New timestamps")

    def _predict_dynamics(self, v, delta, dt, vz=0.0, roll_rate=0.0, pitch_rate=0.0):
        """Bicycle relative-motion model (x, y, yaw), and z/roll/pitch use 
        constant-velocity/rate extrapolation. vz/roll_rate/pitch_rate default
        to 0.0.

        dy is hardcoded 0 (no lateral-slip term) -
        """
        dx = v * dt
        dy = 0.0
        dz = vz * dt
        droll = roll_rate * dt
        dpitch = pitch_rate * dt
        dyaw = (v / self.wheelbase) * np.tan(delta) * dt
        return dx, dy, dz, droll, dpitch, dyaw



    #------------------# Sensor Methods #------------------#

    def add_imu(self, imu_name, omega, accel, dt):
        """Accumulate one IMU sample into the running preintegration for `imu_name`"""
        self.imu_pim[imu_name].integrateMeasurement(accel, omega, dt)


    def add_lidar(self, t_lidar, lidar_state_xyzrpy):
        """lidar_state_xyzrpy: raw lidar-odometry global pose reading,
        (x, y, z, roll, pitch, yaw).  Creates a residual node + drift-chain step immediately;
        the propagation link to the next primary is deferred (see
        pending_residuals, resolved in add_primary).
        """
        if not self.enable_lidar:
            return

        if t_lidar < self.last_primary_time:
            return

        est = self.smoother.calculateEstimate()

        self.lidar_res_index += 1
        res_key = gtsam.symbol(S.LIDAR_RESIDUAL, self.lidar_res_index)

        lidar_pose = _state_to_pose3(lidar_state_xyzrpy)

        if not self._lidar_drift_initialized:
            # First real lidar reading -- create the drift chain's root
            # node now (with its prior), rather than at __init__ time.
            # Deliberately deferred: a drift node created at construction
            # but never touched (e.g. lidar hasn't started publishing
            # yet) sits as an isolated prior-only variable in the Bayes
            # tree, which crashed gtsam's IncrementalFixedLagSmoother
            # (segfault inside ISAM2::marginalizeLeaves) the first time
            # it tried to marginalize with no other factor ever having
            # touched it. Deferring creation to first real use avoids
            # that isolated-node state ever existing.
            drift_curr_key = gtsam.symbol(S.LIDAR_DRIFT, 0)
            d0 = gtsam.Pose3()
            self.graph.add(gtsam.PriorFactorPose3(drift_curr_key, d0, self.lidar_drift_prior_noise))
            self.initial.insert(drift_curr_key, d0)
            self.timestamps.insert((drift_curr_key, t_lidar))
            self._lidar_drift_initialized = True
        else:
            self.drift_index += 1
            drift_prev_key = gtsam.symbol(S.LIDAR_DRIFT, self.drift_index - 1)
            drift_curr_key = gtsam.symbol(S.LIDAR_DRIFT, self.drift_index)

            dt_drift = max(t_lidar - self.last_primary_time, 1e-3)
            drift_process_noise = gtsam.noiseModel.Diagonal.Sigmas(
                _sigmas_xyzrpy_to_gtsam(self._lidar_drift_process_noise_std) * np.sqrt(dt_drift)
            )
            self.graph.add(gtsam.BetweenFactorPose3(drift_prev_key, drift_curr_key, gtsam.Pose3(), drift_process_noise))
            prev_drift_est = est.atPose3(drift_prev_key)
            self.initial.insert(drift_curr_key, prev_drift_est)
            self.timestamps.insert((drift_curr_key, t_lidar))

        self.graph.add(make_lidar_drift_factor(res_key, drift_curr_key, lidar_pose, self.lidar_measurement_noise))
        self.initial.insert(res_key, lidar_pose)
        self.timestamps.insert((res_key, t_lidar))

        self.pending_residuals.append((res_key, t_lidar, 'lidar'))
        self._push()

    def add_gps(self, t_gps, gps_xyz):
        """gps_xyz: raw (x, y, z) GPS position reading. No drift chain --
        GPS is treated as unbiased/stationary-noise.
        """
        if not self.enable_gps:
            return

        est = self.smoother.calculateEstimate()

        if t_gps < self.last_primary_time:
            # Stale reading
            return

        self.gps_res_index += 1
        res_key = gtsam.symbol(S.GPS_RESIDUAL, self.gps_res_index)

        # GPS only observes position; orientation is unconstrained by
        # the GPSFactor itself, but the node still needs a full Pose3
        # (the propagation-to-primary factor is a full 6DoF BetweenFactor).
        # Seed orientation from the current backbone estimate -- the
        # dynamics propagation factor added later will pull it toward
        # something consistent once resolved.
        # TODO: update with heading/velocity from GPS sensor
        curr_pose_key = gtsam.symbol(S.POSE, self.pose_index)
        seed_rotation = est.atPose3(curr_pose_key).rotation()
        seed_pose = gtsam.Pose3(seed_rotation, gtsam.Point3(*gps_xyz))

        self.graph.add(gtsam.GPSFactor(res_key, gtsam.Point3(*gps_xyz), self.gps_noise))

        # GPSFactor only constrains translation (3 DOF) -- rotation is
        # otherwise completely unconstrained until add_primary later
        # adds the propagation BetweenFactor, which ISAM2 correctly
        # rejects as an indeterminate/underconstrained system the moment
        # _push() tries to solve. This weak prior's translation sigmas
        # are enormous (GPSFactor already owns that info) and rotation
        # sigmas are loose but finite, just enough to keep the linear
        # system full rank in the meantime.
        # TODO: update with heading/velocity from GPS sensor
        weak_prior_noise = gtsam.noiseModel.Diagonal.Sigmas(
            _sigmas_xyzrpy_to_gtsam((1e3, 1e3, 1e3, 1.0, 1.0, 1.0))
        )
        self.graph.add(gtsam.PriorFactorPose3(res_key, seed_pose, weak_prior_noise))
        self.initial.insert(res_key, seed_pose)
        self.timestamps.insert((res_key, t_gps))

        self.pending_residuals.append((res_key, t_gps, 'gps'))
        self._push()




    #------------------# Main Methods #------------------#

    def add_primary(self, t, v, delta):
        """Timer callback: create the next primary pose node, connect it
        to the previous one via the dynamics factor (+ IMU factor if
        enabled) over the true elapsed dt, then resolve every pending
        lidar/gps residual by connecting it to this new node via its own
        dynamics-propagation factor. 
        
        v, delta: the control input to use
        for the primary dynamics factor and (per-residual) the
        propagation factors -- current commanded input is used as a
        constant approximation across the whole window, since we don't
        track a full control-input history here.
        """

        if self.debugging:
            print("[add_primary] Starting add_primary")

        prev_key = gtsam.symbol(S.POSE, self.pose_index)
        prev_vel_key = gtsam.symbol(S.VELOCITY, self.pose_index) if self.uses_velocity else None
        self.pose_index += 1
        curr_key = gtsam.symbol(S.POSE, self.pose_index)
        curr_vel_key = gtsam.symbol(S.VELOCITY, self.pose_index) if self.uses_velocity else None

        if self.debugging:
            print("[add_primary] symbols made")

        dt = t - self.last_primary_time
        if dt <= 0:
            raise ValueError(f"add_primary called with non-increasing time (dt={dt})")

        # Fetch the estimate before building the dynamics prediction --
        # needed now to read the previous node's velocity/rate for the
        # constant-velocity/rate extrapolation below. Nothing is solved
        # between here and the second use of `est` later in this method,
        # so one fetch covers both.
        est = self.smoother.calculateEstimate()
        prev_pose = est.atPose3(prev_key)

        if self.debugging:
            print("[add_primary] Estimate made")

        if self.uses_velocity:
            v_world_prev = est.atVector(prev_vel_key)
            R_prev = prev_pose.rotation().matrix()
            v_body_prev = R_prev.T @ v_world_prev  # world -> prev's own body frame
            vz_prev = v_body_prev[2]
        else:
            vz_prev = 0.0
        if self.debugging:
            print("[add_primary] Velocity")

        if self.enable_rate:
            r_prev_key_for_dyn = gtsam.symbol(S.RATE, self.pose_index - 1)  # matches prev_key's index
            roll_rate_prev, pitch_rate_prev = est.atVector(r_prev_key_for_dyn)
        else:
            roll_rate_prev, pitch_rate_prev = 0.0, 0.0
        if self.debugging:
            print("[add_primary] Rate")

        dx, dy, dz, droll, dpitch, dyaw = self._predict_dynamics(
            v, delta, dt, vz=vz_prev, roll_rate=roll_rate_prev, pitch_rate=pitch_rate_prev)
        dyn_pose = gtsam.Pose3(gtsam.Rot3.Ypr(dyaw, dpitch, droll), gtsam.Point3(dx, dy, dz))
        self.graph.add(gtsam.BetweenFactorPose3(prev_key, curr_key, dyn_pose, self.dyn_noise))
        if self.debugging:
            print("[add_primary] Dynamics")

        imu_factor_added = False
        if self.enable_IMUs:
            for name in self.imu_names:
                pim = self.imu_pim[name]

                if pim.deltaTij() <= 0.0:
                    prev_bias_key = gtsam.symbol(self.imu_bias_prefix[name], self.pose_index - 1)
                    curr_bias_key = gtsam.symbol(self.imu_bias_prefix[name], self.pose_index)
                    gyro_walk_std, accel_walk_std = self.imu_bias_walk_std[name]
                    bias_noise = gtsam.noiseModel.Diagonal.Sigmas(_bias_noise_sigmas(gyro_walk_std, accel_walk_std, dt))
                    self.graph.add(gtsam.BetweenFactorConstantBias(
                        prev_bias_key, curr_bias_key, gtsam.imuBias.ConstantBias(), bias_noise))
                    prev_bias_est = est.atConstantBias(prev_bias_key)
                    self.initial.insert(curr_bias_key, prev_bias_est)
                    self.timestamps.insert((curr_bias_key, t))
                    if self.debugging:
                        print("[add_primary] IMU excluded")
                    continue

                imu_factor_added = True

                prev_bias_key = gtsam.symbol(self.imu_bias_prefix[name], self.pose_index - 1)
                curr_bias_key = gtsam.symbol(self.imu_bias_prefix[name], self.pose_index)
                self.graph.add(gtsam.ImuFactor(prev_key, prev_vel_key, curr_key, curr_vel_key, prev_bias_key, pim))

                gyro_walk_std, accel_walk_std = self.imu_bias_walk_std[name]
                bias_noise = gtsam.noiseModel.Diagonal.Sigmas(_bias_noise_sigmas(gyro_walk_std, accel_walk_std, dt))
                self.graph.add(gtsam.BetweenFactorConstantBias(
                    prev_bias_key, curr_bias_key, gtsam.imuBias.ConstantBias(), bias_noise))

                prev_bias_est = est.atConstantBias(prev_bias_key)
                self.initial.insert(curr_bias_key, prev_bias_est)
                self.timestamps.insert((curr_bias_key, t))
                pim.resetIntegrationAndSetBias(prev_bias_est)

                if self.debugging:
                    print("[add_primary] IMU added")
        if self.debugging:
            print("[add_primary] IMU")

        if self.uses_velocity and not imu_factor_added:
            # No real IMU factor this tick (either enable_IMUs is False, or
            # every enabled IMU had zero data) -- add a loose fallback tie
            # so prev_vel_key -> curr_vel_key stays well-posed.
            self.graph.add(gtsam.BetweenFactorVector(prev_vel_key, curr_vel_key, np.zeros(3), self.fallback_vel_noise))
            if self.debugging:
                print("[add_primary] Fallback IMU")

        if self.enable_NHC:
            self.graph.add(make_nhc_factor(curr_key, curr_vel_key, self.nhc_noise))
        if self.debugging:
            print("[add_primary] NHC")

        if self.enable_rate:
            r_prev_key = gtsam.symbol(S.RATE, self.pose_index - 1)
            r_curr_key = gtsam.symbol(S.RATE, self.pose_index)
            rate_cv_noise = gtsam.noiseModel.Diagonal.Sigmas(self.rate_process_noise_std * np.sqrt(dt))
            self.graph.add(make_rate_tie_factor(prev_key, curr_key, r_prev_key, dt, self.rate_tie_noise))
            self.graph.add(make_rate_cv_factor(r_prev_key, r_curr_key, rate_cv_noise))
            r_prev_est = est.atVector(r_prev_key)
            self.initial.insert(r_curr_key, r_prev_est)
            self.timestamps.insert((r_curr_key, t))
        if self.debugging:
            print("[add_primary] Rate 2")

        # Resolve pending lidar/gps residuals: connect each to curr_key
        # via its own dynamics-propagation factor over (t - t_residual).
        for res_key, t_res, kind in self.pending_residuals:
            dt_res = t - t_res
            if dt_res < 0:
                raise ValueError(f"pending {kind} residual timestamp is after this primary's time")
            ddx, ddy, ddz, ddroll, ddpitch, ddyaw = self._predict_dynamics(
                v, delta, dt_res, vz=vz_prev, roll_rate=roll_rate_prev, pitch_rate=pitch_rate_prev)
            prop_pose = gtsam.Pose3(gtsam.Rot3.Ypr(ddyaw, ddpitch, ddroll), gtsam.Point3(ddx, ddy, ddz))
            prop_noise = self.gps_prop_noise if kind == 'gps' else self.residual_prop_noise
            self.graph.add(gtsam.BetweenFactorPose3(res_key, curr_key, prop_pose, prop_noise))
        self.pending_residuals = []
        if self.debugging:
            print("[add_primary] Resolve")

        # Initial guesses for the new primary pose/velocity
        curr_pose_guess = prev_pose.compose(dyn_pose)
        self.initial.insert(curr_key, curr_pose_guess)
        self.timestamps.insert((curr_key, t))
        if self.debugging:
            print("[add_primary] Initial values")

        if self.uses_velocity:
            prev_vel = est.atVector(prev_vel_key)
            R_prev = prev_pose.rotation().matrix()
            R_curr = curr_pose_guess.rotation().matrix()
            v_body = R_prev.T @ prev_vel
            vel_guess = R_curr @ v_body
            self.initial.insert(curr_vel_key, vel_guess)
            self.timestamps.insert((curr_vel_key, t))
        if self.debugging:
            print("[add_primary] Velocity 2")

        self._push()
        if self.debugging:
            print("[add_primary] Push")

        est = self.smoother.calculateEstimate()
        self.state = _pose3_to_state(est.atPose3(curr_key))
        if self.uses_velocity:
            self.velocity = est.atVector(curr_vel_key)
        if self.debugging:
            print("[add_primary] Estimate completed")

        self.last_primary_time = t
        return self.state.copy()





    #------------------# Publisher Methods #------------------#

    def get_estimate(self):
        return self.state.copy()