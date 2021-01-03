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

#include <frc/geometry/Pose2d.h>


namespace photonlib {

/**
 * Represents a camera that is connected to PhotonVision.ß
 */
class SimVisionTarget {
 public:

  explicit SimVisionTarget(frc::Pose2d& targetPos, double targetHeightAboveGroundMeters, double targetWidthMeters, double targetHeightMeters);


  private:
    frc::Pose2d targetPos;
    double targetHeightAboveGroundMeters;
    double targetWidthMeters;
    double targetHeightMeters;
    double targetInfill_pct;
    double tgtAreaMeters2;

};

}  // namespace photonlib
