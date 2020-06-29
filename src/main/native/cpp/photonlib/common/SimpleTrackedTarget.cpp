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
                                         const frc::Pose2d& pose) {
  this->yaw = yaw;
  this->pitch = pitch;
  this->area = area;
  robotRelativePose = pose;
}

bool SimpleTrackedTarget::operator==(const SimpleTrackedTarget& other) const {
  return other.yaw == yaw && other.pitch == pitch && other.area == area &&
         other.robotRelativePose == robotRelativePose;
}

bool SimpleTrackedTarget::operator!=(const SimpleTrackedTarget& other) const {
  return !operator==(other);
}

std::string SimpleTrackedTarget::ToByteArray() {
  ResetBufferPosition();
  std::string data(kPackSizeBytes, ' ');

  BufferData(yaw, &data);
  BufferData(pitch, &data);
  BufferData(area, &data);
  BufferData(robotRelativePose.Translation().X().to<double>(), &data);
  BufferData(robotRelativePose.Translation().Y().to<double>(), &data);
  BufferData(robotRelativePose.Rotation().Degrees().to<double>(), &data);

  return data;
}

void SimpleTrackedTarget::FromByteArray(std::string src) {
  ResetBufferPosition();
  if (src.length() < kPackSizeBytes) {
    std::cout << "An error occurred when deserializing data." << std::endl;
    return;
  }

  yaw = UnbufferDouble(src);
  pitch = UnbufferDouble(src);
  area = UnbufferDouble(src);

  auto poseX = units::meter_t(UnbufferDouble(src));
  auto poseY = units::meter_t(UnbufferDouble(src));
  auto poseR = units::degree_t(UnbufferDouble(src));

  robotRelativePose = frc::Pose2d(poseX, poseY, frc::Rotation2d(poseR));
}
