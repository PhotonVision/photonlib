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

#include <units/time.h>
#include <wpi/ArrayRef.h>

#include <string>
#include <vector>

#include "photonlib/common/Packet.h"
#include "photonlib/common/SimpleTrackedTarget.h"

namespace photonlib {

class SimplePipelineResult {
 public:
  SimplePipelineResult();
  SimplePipelineResult(units::second_t latency, bool hasTargets,
                       std::vector<SimpleTrackedTarget> targets);

  virtual ~SimplePipelineResult() = default;

  units::second_t GetLatency() const { return latency; }
  bool HasTargets() const { return hasTargets; }
  const std::vector<SimpleTrackedTarget>& GetTargets() { return targets; }

  bool operator==(const SimplePipelineResult& other) const;
  bool operator!=(const SimplePipelineResult& other) const;

  void FromPacket(Packet packet);

  units::second_t latency;
  bool hasTargets;
  std::vector<SimpleTrackedTarget> targets;
};
}  // namespace photonlib
