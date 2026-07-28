"""A place to create custom factors for use in IBEX state estimation"""
import gtsam
import numpy as np

def make_nhc_factor(pose_key, vel_key, noise_model, eps=1e-6):
    """Non-holonomic constraint: body-frame lateral + vertical velocity are ~0 (no sideslip, no independent bounce)."""

    def error_func(this, values, jacobians):
        pose = values.atPose3(pose_key)
        vel = values.atVector(vel_key)

        def residual(pose_, vel_):
            R = pose_.rotation().matrix()
            v_body = R.T @ vel_
            return v_body[1:3]  # (lateral, vertical) body-frame velocity

        err = residual(pose, vel)

        if jacobians is not None:
            J_pose = np.zeros((2, 6))
            for i in range(6):
                d = np.zeros(6); d[i] = eps
                J_pose[:, i] = (residual(pose.retract(d), vel)
                                - residual(pose.retract(-d), vel)) / (2 * eps)
            jacobians[0] = J_pose

            J_vel = np.zeros((2, 3))
            for i in range(3):
                d = np.zeros(3); d[i] = eps
                J_vel[:, i] = (residual(pose, vel + d) - residual(pose, vel - d)) / (2 * eps)
            jacobians[1] = J_vel

        return err

    return gtsam.CustomFactor(noise_model, [pose_key, vel_key], error_func)


def make_rate_tie_factor(pose_i_key, pose_j_key, r_key, dt, noise_model, eps=1e-6):
    """Ties consecutive poses' roll/pitch change to the estimated rate
    to let the graph estimate a real angular rate from evidence across the smoothing
    window.

    Uses raw subtraction -- fine for the small attitude ranges, but would need 
    proper angle-wrapping near +/-90 degrees pitch (Euler gimbal lock), which this doesn't handle.
    """
    def error_func(this, values, jacobians):
        pose_i = values.atPose3(pose_i_key)
        pose_j = values.atPose3(pose_j_key)
        r = values.atVector(r_key)

        def residual(pi, pj, r_):
            roll_i, pitch_i = pi.rotation().roll(), pi.rotation().pitch()
            roll_j, pitch_j = pj.rotation().roll(), pj.rotation().pitch()
            return np.array([
                (roll_j - roll_i) - r_[0] * dt,
                (pitch_j - pitch_i) - r_[1] * dt,
            ])

        err = residual(pose_i, pose_j, r)

        if jacobians is not None:
            J_i = np.zeros((2, 6))
            for k in range(6):
                d = np.zeros(6); d[k] = eps
                J_i[:, k] = (residual(pose_i.retract(d), pose_j, r)
                             - residual(pose_i.retract(-d), pose_j, r)) / (2 * eps)
            jacobians[0] = J_i

            J_j = np.zeros((2, 6))
            for k in range(6):
                d = np.zeros(6); d[k] = eps
                J_j[:, k] = (residual(pose_i, pose_j.retract(d), r)
                             - residual(pose_i, pose_j.retract(-d), r)) / (2 * eps)
            jacobians[1] = J_j

            J_r = np.zeros((2, 2))
            for k in range(2):
                d = np.zeros(2); d[k] = eps
                J_r[:, k] = (residual(pose_i, pose_j, r + d) - residual(pose_i, pose_j, r - d)) / (2 * eps)
            jacobians[2] = J_r

        return err

    return gtsam.CustomFactor(noise_model, [pose_i_key, pose_j_key, r_key], error_func)


def make_rate_cv_factor(r_i_key, r_j_key, noise_model):
    """Constant-velocity chain for r: r_{i+1} ~= r_i
    """
    def error_func(this, values, jacobians):
        r_i = values.atVector(r_i_key)
        r_j = values.atVector(r_j_key)
        err = r_j - r_i

        if jacobians is not None:
            jacobians[0] = -np.eye(2)
            jacobians[1] = np.eye(2)

        return err

    return gtsam.CustomFactor(noise_model, [r_i_key, r_j_key], error_func)




def make_lidar_drift_factor(pose_key, drift_key, lidar_measured_pose, noise_model, eps=1e-6):
    """Ties a pose variable and a latent 'lidar drift' variable to what
    lidar actually published:
    residual = pose.compose(drift).localCoordinates(lidar_measured_pose)
    i.e. "pose composed with the current drift should explain the raw
    lidar reading." drift is a slowly-varying latent Pose3 -- same idea
    as IMU bias, just modeling lidar's accumulated offset from truth
    instead of a physical sensor bias.
    """
    def error_func(this, values, jacobians):
        pose = values.atPose3(pose_key)
        drift = values.atPose3(drift_key)

        def residual(pose_, drift_):
            predicted = pose_.compose(drift_)
            return predicted.localCoordinates(lidar_measured_pose)

        err = residual(pose, drift)

        if jacobians is not None:
            J_pose = np.zeros((6, 6))
            for i in range(6):
                d = np.zeros(6); d[i] = eps
                J_pose[:, i] = (residual(pose.retract(d), drift)
                                - residual(pose.retract(-d), drift)) / (2 * eps)
            jacobians[0] = J_pose

            J_drift = np.zeros((6, 6))
            for i in range(6):
                d = np.zeros(6); d[i] = eps
                J_drift[:, i] = (residual(pose, drift.retract(d))
                                 - residual(pose, drift.retract(-d))) / (2 * eps)
            jacobians[1] = J_drift

        return err

    return gtsam.CustomFactor(noise_model, [pose_key, drift_key], error_func)