/**
 * Copyright (C) 2020 Photon Vision.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package org.photonvision;

import edu.wpi.first.wpilibj.geometry.Rotation2d;
import edu.wpi.first.wpilibj.geometry.Translation2d;

public final class PhotonUtils {
  private PhotonUtils() {
    // Utility class
  }

  /**
   * Algorithm from https://docs.limelightvision.io/en/latest/cs_estimating_distance.html
   * Estimates range to a target using the target's elevation. This method can produce more stable
   * results than SolvePNP when well tuned, if the full 6d robot pose is not required. Note that
   * this method requires the camera to have 0 roll (not be skewed clockwise or CCW relative to
   * the floor), and for there to exist a height differential between goal and camera. The larger
   * this differential, the more accurate the distance estimate will be.
   *
   * <p> Units can be converted using the {@link edu.wpi.first.wpilibj.util.Units} class.
   *
   * @param cameraHeightMeters The physical height of the camera off the floor in meters.
   * @param targetHeightMeters The physical height of the target off the floor in meters. This should be
   *                           the height of whatever is being targeted (i.e. if the targeting region is set
   *                           to top, this should be the height of the top of the target).
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
   * Estimate the {@link Translation2d} of the target relative to the camera.
   *
   * @param targetDistanceMeters The distance to the target in meters.
   * @param yaw                  The observed yaw of the target.
   * @return The target's camera-relative translation.
   */
  public static Translation2d estimateTargetTranslation2d(double targetDistanceMeters, Rotation2d yaw) {
    return new Translation2d(yaw.getCos() * targetDistanceMeters, yaw.getSin() * targetDistanceMeters);
  }


  /**
   * Wraps an arbitrary angle (in degrees) to the (-180, 180] range.
   * @param angle
   * @return Wrapped-to-range angle
   */
  static double wrapAngleDeg(double angle){
    angle %=360;
    angle = angle > 180.0 ? angle-360 : angle;
    angle = angle <= -180.0 ? angle+360 : angle;
    return angle;
  }
}
