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

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>

#include <string>

#include "photonlib/common/SimplePipelineResult.h"

namespace photonlib {
class PhotonCamera {
 public:
  explicit PhotonCamera(std::shared_ptr<nt::NetworkTable> rootTable);
  explicit PhotonCamera(const std::string& tableName);

  SimplePipelineResult GetLastResult();

  void SetDriverMode(bool driverMode);
  bool GetDriverMode() const;

  void SetPipelineIndex(int index);
  int GetPipelineIndex() const;

  bool HasTargets() { return GetLastResult().HasTargets(); }

  double GetFirstTargetPitch() {
    return GetLastResult().GetTargets()[0].GetPitch();
  }

  double GetFirstTargetYaw() {
    return GetLastResult().GetTargets()[0].GetYaw();
  }

  double GetBestTargetArea() {
    return GetLastResult().GetTargets()[0].GetArea();
  }

 private:
  nt::NetworkTableEntry rawBytesEntry;
  nt::NetworkTableEntry driverModeEntry;
  nt::NetworkTableEntry pipelineIndexEntry;

  bool driverMode;
  double pipelineIndex;
};

}  // namespace photonlib
