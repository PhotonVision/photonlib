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

bool SimplePipelineResult::operator!=(const SimplePipelineResult& other) const {
  return !operator==(other);
}

std::vector<char> SimplePipelineResult::ToByteArray() {
  // Reset the buffer position to zero.
  ResetBufferPosition();

  // Calculate the buffer size based on the number of targets.
  int bufferSize = 10 + targets.size() * 48;

  // Create the byte array.
  std::vector<char> bytes(bufferSize);

  // Encoded latency, existence of targets, and the number of targets.
  BufferData<double>(latency.to<double>() * 1000, &bytes);
  BufferData<bool>(hasTargets, &bytes);
  BufferData<char>(static_cast<char>(targets.size()), &bytes);

  // Encode the information of each target.
  for (auto& target : targets) {
    // Get the bytes representing the target.
    auto targetBytes = target.ToByteArray();

    // Copy the target bytes into the byte array of this object.
    bytes.insert(bytes.begin() + bufferPosition, targetBytes.begin(),
                 targetBytes.end());
    bufferPosition += targetBytes.size();
  }
  return bytes;
}

void SimplePipelineResult::FromByteArray(std::vector<char> src) {
  // Reset the buffer position to zero.
  ResetBufferPosition();

  // Get latency, existence of targets, and the number of targets.
  latency = units::second_t(UnbufferData<double>(src) / 1000.0);
  hasTargets = UnbufferData<bool>(src);
  char targetCount = UnbufferData<char>(src);

  // Clear the targets vector.
  targets.clear();

  // Populate the targets vector.
  for (int i = 0; i < static_cast<int>(targetCount); i++) {
    // Create a simple tracked target.
    SimpleTrackedTarget target;

    // Populate the target with data.
    target.FromByteArray(std::vector<char>(src.begin() + bufferPosition,
                                           src.begin() + bufferPosition + 48));
    bufferPosition += 48;

    // Add the target to the list.
    targets.emplace_back(target);
  }
}
