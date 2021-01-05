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

#include <vector>

#include <frc/geometry/Translation2d.h>
#include <units/angle.h>
#include <units/area.h>
#include <units/length.h>
#include <units/time.h>
#include <wpi/ArrayRef.h>
#include <wpi/SmallVector.h>

#include "photonlib/SimPhotonCamera.h"
#include "photonlib/SimVisionTarget.h"

namespace photonlib {

/**
 * Represents a camera that is connected to PhotonVision.
 */
class SimVisionSystem {
 public:

  explicit SimVisionSystem(const std::string&  name,
           units::angle::degree_t camDiagFOVDegrees,
           units::angle::degree_t camPitchDegrees,
           frc::Transform2d robotToCamera,
           units::length::meter_t cameraHeightOffGroundMeters,
           units::length::meter_t maxLEDRangeMeters,
           int cameraResWidth,
           int cameraResHeight,
           double minTargetArea);

  void AddSimVisionTarget(SimVisionTarget tgt);
  void MoveCamera(frc::Transform2d newRobotToCamera, units::length::meter_t newCamHeightMeters, units::angle::degree_t newCamPitchDegrees);
  void ProcessFrame(frc::Pose2d robotPoseMeters);

  private:
  units::angle::degree_t camDiagFOVDegrees;
  units::angle::degree_t camPitchDegrees;
  frc::Transform2d robotToCamera;
  units::length::meter_t cameraHeightOffGroundMeters;
  units::length::meter_t maxLEDRangeMeters;
  int cameraResWidth;
  int cameraResHeight;
  double minTargetArea;
  units::angle::degree_t camHorizFOVDegrees;
  units::angle::degree_t camVertFOVDegrees;
  std::vector<SimVisionTarget> tgtList = {};

  double getM2PerPx(units::length::meter_t dist);
  bool camCanSeeTarget(units::length::meter_t distMeters, units::angle::degree_t yaw, units::angle::degree_t pitch, double area);

  public:
  SimPhotonCamera cam = photonlib::SimPhotonCamera("Default");

};

}  // namespace photonlib
