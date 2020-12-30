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

#pragma once

#include <frc/geometry/Rotation2d.h>
#include <frc/geometry/Translation2d.h>
#include <units/angle.h>
#include <units/length.h>
#include <units/math.h>

namespace photonlib {
class PhotonUtils {
 public:
  /**
   * Algorithm from
   * https://docs.limelightvision.io/en/latest/cs_estimating_distance.html
   * Estimates range to a target using the target's elevation. This method can
   * produce more stable results than SolvePNP when well tuned, if the full 6d
   * robot pose is not required.
   *
   * @param cameraHeight The height of the camera off the floor.
   * @param targetHeight The height of the target off the floor.
   * @param cameraPitch The pitch of the camera from the horizontal plane.
   * Positive valueBytes up.
   * @param targetPitch The pitch of the target in the camera's lens. Positive
   * values up.
   * @return The estimated distance to the target.
   */
  static units::meter_t CalculateDistanceToTarget(units::meter_t cameraHeight,
                                                  units::meter_t targetHeight,
                                                  units::radian_t cameraPitch,
                                                  units::radian_t targetPitch) {
    return (targetHeight - cameraHeight) /
           units::math::tan(cameraPitch + targetPitch);
  }

  /**
   * Estimate the Translation2d of the target relative to the camera.
   *
   * @param targetDistance The distance to the target.
   * @param yaw            The observed yaw of the target.
   *
   * @return The target's camera-relative translation.
   */
  static frc::Translation2d EstimateTargetTranslation(
      units::meter_t targetDistance, frc::Rotation2d yaw) {
    return {targetDistance * yaw.Cos(), targetDistance * yaw.Sin()};
  }
};
}  // namespace photonlib
