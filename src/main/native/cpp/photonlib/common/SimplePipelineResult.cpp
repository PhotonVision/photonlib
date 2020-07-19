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

namespace photonlib {
SimplePipelineResult::SimplePipelineResult(
    units::second_t latency, wpi::ArrayRef<SimpleTrackedTarget> targets)
    : latency(latency),
      targets(targets.data(), targets.data() + targets.size()) {
  hasTargets = targets.size() != 0;
}

bool SimplePipelineResult::operator==(const SimplePipelineResult& other) const {
  return latency == other.latency && hasTargets == other.hasTargets &&
         targets == other.targets;
}

bool SimplePipelineResult::operator!=(const SimplePipelineResult& other) const {
  return !operator==(other);
}

Packet& operator<<(Packet& packet, const SimplePipelineResult& result) {
  // Encode latency, existence of targets, and number of targets.
  packet << result.latency.to<double>() * 1000 << result.hasTargets
         << static_cast<char>(result.targets.size());

  // Encode the information of each target.
  for (auto& target : result.targets) packet << target;

  // Return the packet
  return packet;
}

Packet& operator>>(Packet& packet, SimplePipelineResult& result) {
  // Decode latency, existence of targets, and number of targets.
  char targetCount = 0;
  double latencyMillis = 0;
  packet >> latencyMillis >> result.hasTargets >> targetCount;
  result.latency = units::second_t(latencyMillis / 1000.0);

  result.targets.clear();

  // Decode the information of each target.
  for (int i = 0; i < static_cast<int>(targetCount); ++i) {
    SimpleTrackedTarget target;
    packet >> target;
    result.targets.push_back(target);
  }
  return packet;
}

}  // namespace photonlib
