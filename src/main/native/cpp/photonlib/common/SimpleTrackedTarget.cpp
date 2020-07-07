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

#include "photonlib/common/SimpleTrackedTarget.h"

#include <iostream>

using namespace photonlib;

SimpleTrackedTarget::SimpleTrackedTarget() {}
SimpleTrackedTarget::SimpleTrackedTarget(double yaw, double pitch, double area,
                                         const frc::Pose2d& pose)
    : yaw(yaw), pitch(pitch), area(area), robotRelativePose(pose) {}

bool SimpleTrackedTarget::operator==(const SimpleTrackedTarget& other) const {
  return other.yaw == yaw && other.pitch == pitch && other.area == area &&
         other.robotRelativePose == robotRelativePose;
}

bool SimpleTrackedTarget::operator!=(const SimpleTrackedTarget& other) const {
  return !operator==(other);
}

std::vector<char> SimpleTrackedTarget::ToByteArray() {
  // Reset the buffer position to zero.
  ResetBufferPosition();

  // Create the byte array.
  std::vector<char> bytes(kPackSizeBytes);

  // Encode information.
  BufferData<double>(yaw, &bytes);
  BufferData<double>(pitch, &bytes);
  BufferData<double>(area, &bytes);
  BufferData<double>(robotRelativePose.Translation().X().to<double>(), &bytes);
  BufferData<double>(robotRelativePose.Translation().Y().to<double>(), &bytes);
  BufferData<double>(robotRelativePose.Rotation().Degrees().to<double>(),
                     &bytes);

  return bytes;
}

void SimpleTrackedTarget::FromByteArray(const std::vector<char>& src) {
  // Reset the buffer position to zero.
  ResetBufferPosition();

  yaw = UnbufferData<double>(src);
  pitch = UnbufferData<double>(src);
  area = UnbufferData<double>(src);

  auto poseX = units::meter_t(UnbufferData<double>(src));
  auto poseY = units::meter_t(UnbufferData<double>(src));
  auto poseR = units::degree_t(UnbufferData<double>(src));

  robotRelativePose = frc::Pose2d(poseX, poseY, frc::Rotation2d(poseR));
}
