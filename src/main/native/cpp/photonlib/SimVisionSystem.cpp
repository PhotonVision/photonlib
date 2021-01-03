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

#include "photonlib/SimVisionSystem.h"
#include <cmath> 

namespace photonlib {

    SimVisionSystem::SimVisionSystem(const std::string&  name, 
                                    double camDiagFOVDegrees, 
                                    double camPitchDegrees, 
                                    const frc::Transform2d& robotToCamera,
                                    double cameraHeightOffGroundMeters, 
                                    double maxLEDRangeMeters, 
                                    int cameraResWidth, 
                                    int cameraResHeight, 
                                    double minTargetArea): 
                                    camDiagFOVDegrees(camDiagFOVDegrees), 
                                    camPitchDegrees(camPitchDegrees), 
                                    robotToCamera(robotToCamera), 
                                    cameraHeightOffGroundMeters(cameraHeightOffGroundMeters), 
                                    maxLEDRangeMeters(maxLEDRangeMeters), 
                                    cameraResWidth(cameraResWidth), 
                                    cameraResHeight(cameraResHeight), 
                                    minTargetArea(minTargetArea) {
      double hypotPixels = hypot(cameraResWidth, cameraResHeight);
      camHorizFOVDegrees = camDiagFOVDegrees * cameraResWidth/hypotPixels;
      camVertFOVDegrees  = camDiagFOVDegrees * cameraResHeight/hypotPixels;

      cam = new SimPhotonCamera(name);
      tgtList.clear();
    }




}  // namespace photonlib
