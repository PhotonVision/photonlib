/**
 * Copyright (C) 2020-2021 Photon Vision.
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

#include <frc/geometry/Pose2d.h>
#include <frc/geometry/Rotation2d.h>
#include <frc/geometry/Transform2d.h>
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

  /**
   * Estimate the position of the robot in the field.
   *
   * @param cameraHeightMeters The physical height of the camera off the floor
   * in meters.
   * @param targetHeightMeters The physical height of the target off the floor
   * in meters. This should be the height of whatever is being targeted (i.e. if
   * the targeting region is set to top, this should be the height of the top of
   * the target).
   * @param cameraPitchRadians The pitch of the camera from the horizontal plane
   *                           in radians. Positive values up.
   * @param targetPitchRadians The pitch of the target in the camera's lens in
   *                           radians. Positive values up.
   * @param targetYaw          The observed yaw of the target. Note that this
   *                           *must* be CCW-positive, and Photon returns
   *                           CW-positive.
   * @param gyroAngle          The current robot gyro angle, likely from
   * odometry.
   * @param fieldToTarget      A frc::Pose2d representing the target position in
   * the field coordinate system.
   * @param cameraToRobot      The position of the robot relative to the camera.
   *                           If the camera was mounted 3 inches behind the
   *                           "origin" (usually physical center) of the robot,
   *                           this would be frc::Transform2d(3 inches, 0
   * inches, 0 degrees).
   * @return The position of the robot in the field.
   */
  static frc::Pose2d EstimateFieldToRobot(
      units::meter_t cameraHeight, units::meter_t targetHeight,
      units::radian_t cameraPitch, units::radian_t targetPitch,
      frc::Rotation2d targetYaw, frc::Rotation2d gyroAngle,
      frc::Pose2d fieldToTarget, frc::Transform2d cameraToRobot) {
    return EstimateFieldToRobot(
        EstimateCameraToTarget(
            EstimateTargetTranslation(
                CalculateDistanceToTarget(cameraHeight, targetHeight,
                                          cameraPitch, targetPitch),
                targetYaw),
            fieldToTarget, gyroAngle),
        fieldToTarget, cameraToRobot);
  }

  /**
   * Estimates a {@link frc::Transform2d} that maps the camera position to the
   * target position, using the robot's gyro. Note that the gyro angle provided
   * *must* line up with the field coordinate system -- that is, it should read
   * zero degrees when pointed towards the opposing alliance station, and
   * increase as the robot rotates CCW.
   *
   * @param cameraToTargetTranslation A Translation2d that encodes the x/y
   *                                  position of the target relative to the
   *                                  camera.
   * @param fieldToTarget             A frc::Pose2d representing the target
   * position in the field coordinate system.
   * @param gyroAngle                 The current robot gyro angle, likely from
   *                                  odometry.
   * @return A frc::Transform2d that takes us from the camera to the target.
   */
  static frc::Transform2d EstimateCameraToTarget(
      const frc::Translation2d& cameraToTargetTranslation,
      const frc::Pose2d& fieldToTarget, const frc::Rotation2d& gyroAngle) {
    // This pose maps our camera at the origin out to our target, in the robot
    // reference frame
    // The translation part of this frc::Transform2d is from the above step, and
    // the rotation uses our robot's gyro.
    return frc::Transform2d(cameraToTargetTranslation,
                            -gyroAngle - fieldToTarget.Rotation());
  }

  /**
   * Estimates the pose of the robot in the field coordinate system, given the
   * position of the target relative to the camera, the target relative to the
   * field, and the robot relative to the camera.
   *
   * @param cameraToTarget The position of the target relative to the camera.
   * @param fieldToTarget  The position of the target in the field.
   * @param cameraToRobot  The position of the robot relative to the camera. If
   *                       the camera was mounted 3 inches behind the "origin"
   *                       (usually physical center) of the robot, this would be
   *                       frc::Transform2d(3 inches, 0 inches, 0 degrees).
   * @return The position of the robot in the field.
   */
  static frc::Pose2d EstimateFieldToRobot(frc::Transform2d cameraToTarget,
                                          frc::Pose2d fieldToTarget,
                                          frc::Transform2d cameraToRobot) {
    return EstimateFieldToCamera(cameraToTarget, fieldToTarget)
        .TransformBy(cameraToRobot);
  }

  /**
   * Estimates the pose of the camera in the field coordinate system, given the
   * position of the target relative to the camera, and the target relative to
   * the field. This *only* tracks the position of the camera, not the position
   * of the robot itself.
   *
   * @param cameraToTarget The position of the target relative to the camera.
   * @param fieldToTarget  The position of the target in the field.
   * @return The position of the camera in the field.
   */
  static frc::Pose2d EstimateFieldToCamera(
      const frc::Transform2d& cameraToTarget,
      const frc::Pose2d& fieldToTarget) {
    auto targetToCamera = cameraToTarget.Inverse();
    return fieldToTarget.TransformBy(targetToCamera);
  }
};
}  // namespace photonlib
