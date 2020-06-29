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

#include <frc/geometry/Pose2d.h>

#include <cstddef>
#include <string>

#include "photonlib/common/BytePackable.h"

namespace photonlib {
class SimpleTrackedTarget : public BytePackable {
 public:
  constexpr static int kPackSizeBytes = sizeof(double) / sizeof(std::byte) * 6;

  SimpleTrackedTarget();
  SimpleTrackedTarget(double yaw, double pitch, double area,
                      const frc::Pose2d& pose);

  virtual ~SimpleTrackedTarget() = default;

  double GetYaw() const { return yaw; }
  double GetPitch() const { return pitch; }
  double GetArea() const { return area; }

  frc::Pose2d GetRobotRelativePose() const { return robotRelativePose; }

  bool operator==(const SimpleTrackedTarget& other) const;
  bool operator!=(const SimpleTrackedTarget& other) const;

  std::string ToByteArray() override;
  void FromByteArray(std::string src) override;

 private:
  double yaw = 0;
  double pitch = 0;
  double area = 0;
  frc::Pose2d robotRelativePose;
};
}  // namespace photonlib
