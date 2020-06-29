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
    std::vector<SimpleTrackedTarget> targets) {
  this->latency = latency;
  this->hasTargets = hasTargets;
  this->targets = targets;
}

bool SimplePipelineResult::operator==(const SimplePipelineResult& other) const {
  return latency == other.latency && hasTargets == other.hasTargets &&
         targets == other.targets;
}

bool SimplePipelineResult::operator!=(const SimplePipelineResult& other) const {
  return !operator==(other);
}

std::string SimplePipelineResult::ToByteArray() {
  ResetBufferPosition();
  int bufferSize = 10 + targets.size() * 48;
  std::string buff(bufferSize, ' ');

  BufferData(latency.to<double>() * 1000, &buff);
  BufferData(hasTargets, &buff);
  BufferData(static_cast<char>(targets.size()), &buff);

  for (auto& target : targets) {
    BufferData(target.ToByteArray(), &buff);
  }

  return buff;
}

void SimplePipelineResult::FromByteArray(std::string src) {
  ResetBufferPosition();

  latency = units::second_t(UnbufferDouble(src) / 1000.0);
  hasTargets = UnbufferBoolean(src);
  char targetCount = UnbufferByte(src);

  targets.clear();
  for (int i = 0; i < static_cast<int>(targetCount); i++) {
    SimpleTrackedTarget target;
    target.FromByteArray(UnbufferBytes(src, 48));
    targets.emplace_back(target);
  }
}
