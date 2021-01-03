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

#include "photonlib/SimPhotonCamera.h"
#include "photonlib/SimVisionTarget.h"
#include <frc/geometry/Translation2d.h>
#include <wpi/ArrayRef.h>
#include <wpi/SmallVector.h>
#include <list>

namespace photonlib {

/**
 * Represents a camera that is connected to PhotonVision.ß
 */
class SimVisionSystem {
 public:

  explicit SimVisionSystem(const std::string&  name, 
           double camDiagFOVDegrees, 
           double camPitchDegrees, 
           const frc::Transform2d& robotToCamera,
           double cameraHeightOffGroundMeters, 
           double maxLEDRangeMeters, 
           int cameraResWidth, 
           int cameraResHeight, 
           double minTargetArea);


  private:
  SimPhotonCamera * cam;
  double camDiagFOVDegrees;
  double camPitchDegrees;
  frc::Transform2d robotToCamera;
  double cameraHeightOffGroundMeters;
  double maxLEDRangeMeters;
  int cameraResWidth;
  int cameraResHeight;
  double minTargetArea;
  double camHorizFOVDegrees;
  double camVertFOVDegrees;
   std::list<SimVisionTarget> tgtList = {};
};

}  // namespace photonlib
