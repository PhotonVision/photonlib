package org.photonvision.lib;

import edu.wpi.first.wpilibj.geometry.Rotation2d;
import edu.wpi.first.wpilibj.geometry.Translation2d;

public final class PhotonUtils {

  private PhotonUtils() {
    // Utility class
  }

  /**
   * Algorithm from https://docs.limelightvision.io/en/latest/cs_estimating_distance.html
   * Estimates range to a target using the target's elevation. This method can produce more stable
   * results than SolvePNP when well tuned, if the full 6d robot pose is not required.
   *
   * <p> Units can be converted using the {@link edu.wpi.first.wpilibj.util.Units} class.
   *
   * @param cameraHeightMeters The height of the camera off the floor in meters.
   * @param targetHeightMeters The physical height of the camera off the floor in meters.
   * @param cameraPitchRadians The pitch of the camera from the horizontal plane in radians. Positive values up.
   * @param targetPitchRadians The pitch of the target in the camera's lens in radians. Positive values up.
   * @return The estimated distance to the target in meters.
   */
  public static double calculateDistanceToTargetMeters(double cameraHeightMeters,
                                                       double targetHeightMeters,
                                                       double cameraPitchRadians,
                                                       double targetPitchRadians) {
    return (targetHeightMeters - cameraHeightMeters) / Math.tan(cameraPitchRadians + targetPitchRadians);
  }

  /**
   * Estimate the {@link Translation2d} of the target relative to the robot.
   *
   * @param targetDistanceMeters The distance to the target in meters.
   * @param yaw                  The observed yaw of the target.
   * @return The target's robot-relative translation.
   */
  public static Translation2d estimateTargetTranslation2d(double targetDistanceMeters, Rotation2d yaw) {
    return new Translation2d(yaw.getCos() * targetDistanceMeters, yaw.getSin() * targetDistanceMeters);
  }
}
