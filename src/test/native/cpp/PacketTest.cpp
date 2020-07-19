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

#include <iostream>

#include <units/angle.h>

#include "gtest/gtest.h"
#include "photonlib/common/SimplePipelineResult.h"
#include "photonlib/common/SimpleTrackedTarget.h"

TEST(PacketTest, SimpleTrackedTarget) {
  photonlib::SimpleTrackedTarget target{3.0, 4.0, 9.0, -5.0,
                                        frc::Pose2d(1_m, 2_m, 1.5_rad)};
  photonlib::Packet p;
  p << target;

  photonlib::SimpleTrackedTarget b;
  p >> b;

  EXPECT_EQ(target, b);
}

TEST(PacketTest, SimplePipelineResult) {
  photonlib::SimplePipelineResult result{1_s, {}};
  photonlib::Packet p;
  p << result;

  photonlib::SimplePipelineResult b;
  p >> b;

  EXPECT_EQ(result, b);

  wpi::SmallVector<photonlib::SimpleTrackedTarget, 2> targets{
      photonlib::SimpleTrackedTarget{3.0, -4.0, 9.0, 4.0,
                                     frc::Pose2d(1_m, 2_m, 1.5_rad)},
      photonlib::SimpleTrackedTarget{3.0, -4.0, 9.1, 6.7,
                                     frc::Pose2d(1_m, 5_m, 1.5_rad)}};

  photonlib::SimplePipelineResult result2{2_s, targets};
  photonlib::Packet p2;
  p2 << result2;

  photonlib::SimplePipelineResult b2;
  p2 >> b2;

  EXPECT_EQ(result2, b2);
}

TEST(PacketTest, BytePackFromJava) {
  std::vector<signed char> bytePack{
      64, 8, 0,  0,  0,  0,   0,  0,  64,  16,  0,   0,   0,   0,
      0,  0, 64, 34, 0,  0,   0,  0,  0,   0,   -64, 20,  0,   0,
      0,  0, 0,  0,  63, -16, 0,  0,  0,   0,   0,   0,   64,  0,
      0,  0, 0,  0,  0,  0,   64, 85, 124, 101, 19,  -54, -47, 122};

  std::vector<char> bytes;
  for (auto a : bytePack) bytes.emplace_back(static_cast<char>(a));

  photonlib::Packet packet{bytes};

  photonlib::SimpleTrackedTarget res;
  packet >> res;

  photonlib::SimpleTrackedTarget target{3.0, 4.0, 9.0, -5.0,
                                        frc::Pose2d(1_m, 2_m, 1.5_rad)};

  EXPECT_EQ(res, target);
}
