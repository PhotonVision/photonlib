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

#include <string>

#include <units/time.h>
#include <wpi/ArrayRef.h>
#include <wpi/SmallVector.h>

#include "photonlib/common/Packet.h"
#include "photonlib/common/SimpleTrackedTarget.h"

namespace photonlib {
/**
 * Represents a pipeline result from a PhotonCamera.
 */
class SimplePipelineResult {
 public:
  /**
   * Constructs an empty pipeline result.
   */
  SimplePipelineResult() = default;

  /**
   * Constructs a pipeline result.
   * @param latency The latency in the pipeline.
   * @param targets The list of targets identified by the pipeline.
   */
  SimplePipelineResult(units::second_t latency,
                       wpi::ArrayRef<SimpleTrackedTarget> targets);

  /**
   * Returns the latency in the pipeline.
   * @return The latency in the pipeline.
   */
  units::second_t GetLatency() const { return latency; }

  /**
   * Returns whether the pipeline has targets.
   * @return Whether the pipeline has targets.
   */
  bool HasTargets() const { return hasTargets; }

  /**
   * Returns a reference to the vector of targets.
   * @return A reference to the vector of targets.
   */
  wpi::ArrayRef<SimpleTrackedTarget> GetTargets() { return targets; }

  bool operator==(const SimplePipelineResult& other) const;
  bool operator!=(const SimplePipelineResult& other) const;

  friend Packet& operator<<(Packet& packet, const SimplePipelineResult& result);
  friend Packet& operator>>(Packet& packet, SimplePipelineResult& result);

 private:
  units::second_t latency;
  bool hasTargets;
  wpi::SmallVector<SimpleTrackedTarget, 10> targets;
};
}  // namespace photonlib
