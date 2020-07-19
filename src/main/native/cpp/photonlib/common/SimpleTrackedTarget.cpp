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
                                         double skew, const frc::Pose2d& pose)
    : yaw(yaw), pitch(pitch), area(area), skew(skew), robotRelativePose(pose) {}

bool SimpleTrackedTarget::operator==(const SimpleTrackedTarget& other) const {
  return other.yaw == yaw && other.pitch == pitch && other.area == area &&
         other.skew == skew && other.robotRelativePose == robotRelativePose;
}

bool SimpleTrackedTarget::operator!=(const SimpleTrackedTarget& other) const {
  return !operator==(other);
}

void SimpleTrackedTarget::FromPacket(Packet packet) {
  double x = 0;
  double y = 0;
  double rot = 0;
  packet >> yaw >> pitch >> area >> skew >> x >> y >> rot;
  robotRelativePose =
      frc::Pose2d(units::meter_t(x), units::meter_t(y), units::degree_t(rot));
}
