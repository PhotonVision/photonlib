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

#include "photonlib/common/SimplePipelineResult.h"

using namespace photonlib;

SimplePipelineResult::SimplePipelineResult() {}

SimplePipelineResult::SimplePipelineResult(
    units::second_t latency, bool hasTargets,
    std::vector<SimpleTrackedTarget> targets)
    : latency(latency), hasTargets(hasTargets), targets(targets) {}

bool SimplePipelineResult::operator==(const SimplePipelineResult& other) const {
  return latency == other.latency && hasTargets == other.hasTargets &&
         targets == other.targets;
}

void SimplePipelineResult::FromPacket(Packet packet) {
  // Decode latency, existence of targets, and number of targets.
  double latencyMillis = 0;
  char size = 0;
  packet >> latencyMillis >> hasTargets >> size;

  latency = units::second_t(latencyMillis / 1000);

  // Encode the information of each target.
  for (int i = 0; i < static_cast<int>(size); ++i) {
    double yaw = 0;
    double area = 0;
    double pitch = 0;
    double skew = 0;
    double x = 0;
    double y = 0;
    double rot = 0;

    packet >> yaw >> pitch >> area >> skew >> x >> y >> rot;
    targets.emplace_back(yaw, pitch, area, skew,
                         frc::Pose2d(units::meter_t(x), units::meter_t(y),
                                     units::degree_t(rot)));
  }
}
