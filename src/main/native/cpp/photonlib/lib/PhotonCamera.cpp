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

#include "photonlib/lib/PhotonCamera.h"

#include <networktables/NetworkTableInstance.h>

#include "photonlib/common/Packet.h"

using namespace photonlib;

PhotonCamera::PhotonCamera(std::shared_ptr<nt::NetworkTable> rootTable)
    : rawBytesEntry(rootTable->GetEntry("rawBytes")),
      driverModeEntry(rootTable->GetEntry("driverMode")),
      pipelineIndexEntry(rootTable->GetEntry("pipelineIndex")) {
  driverMode = driverModeEntry.GetBoolean(false);
  pipelineIndex = static_cast<int>(pipelineIndexEntry.GetDouble(0.0));
}

PhotonCamera::PhotonCamera(const std::string& cameraName)
    : PhotonCamera(nt::NetworkTableInstance::GetDefault()
                       .GetTable("photonvision")
                       ->GetSubTable(cameraName)) {}

SimplePipelineResult PhotonCamera::GetLastResult() {
  SimplePipelineResult result;
  Packet packet;
  packet << rawBytesEntry.GetRaw(std::string());
  packet >> result;
  return result;
}

void PhotonCamera::SetDriverMode(bool driverMode) {
  this->driverMode = driverMode;
  driverModeEntry.SetBoolean(this->driverMode);
}

bool PhotonCamera::GetDriverMode() const { return driverMode; }

void PhotonCamera::SetPipelineIndex(int index) {
  pipelineIndex = index;
  pipelineIndexEntry.SetDouble(static_cast<double>(pipelineIndex));
}

int PhotonCamera::GetPipelineIndex() const { return pipelineIndex; }
