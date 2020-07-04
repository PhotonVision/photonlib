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

#include <units/angle.h>

#include "gtest/gtest.h"
#include "photonlib/common/BytePackable.h"
#include "photonlib/common/SimplePipelineResult.h"
#include "photonlib/common/SimpleTrackedTarget.h"

TEST(BytePackableTest, SimpleTrackedTarget) {
  photonlib::SimpleTrackedTarget target{3.0, 4.0, 9.0,
                                        frc::Pose2d(1_m, 2_m, 1.5_rad)};
  std::vector<char> packed = target.ToByteArray();

  for (auto& a : packed) std::cout << static_cast<int>(a) << ", ";

  photonlib::SimpleTrackedTarget b;
  b.FromByteArray(packed);

  EXPECT_EQ(target, b);
}

TEST(BytePackableTest, SimplePipelineResult) {
  photonlib::SimplePipelineResult result{1_s, false, {}};
  std::vector<char> packed = result.ToByteArray();

  photonlib::SimplePipelineResult b;
  b.FromByteArray(packed);

  EXPECT_EQ(result, b);

  photonlib::SimplePipelineResult result2{
      2_s,
      true,
      {photonlib::SimpleTrackedTarget{3.0, -4.0, 9.0,
                                      frc::Pose2d(1_m, 2_m, 1.5_rad)},
       photonlib::SimpleTrackedTarget{3.0, -4.0, 9.1,
                                      frc::Pose2d(1_m, 5_m, 1.5_rad)}}};
  std::vector<char> packed2 = result2.ToByteArray();

  photonlib::SimplePipelineResult b2;
  b2.FromByteArray(packed2);

  EXPECT_EQ(result2, b2);
}

TEST(BytePackableTest, BytePackFromJava) {
  std::vector<char> bytePack{
      64, 8,  0, 0, 0, 0, 0, 0, 64, 16,  0,   0,   0,  0,   0,   0,
      64, 34, 0, 0, 0, 0, 0, 0, 63, -16, 0,   0,   0,  0,   0,   0,
      64, 0,  0, 0, 0, 0, 0, 0, 64, 85,  124, 101, 19, -54, -47, 122};
  photonlib::SimpleTrackedTarget res;
  res.FromByteArray(bytePack);

  photonlib::SimpleTrackedTarget target{3.0, 4.0, 9.0,
                                        frc::Pose2d(1_m, 2_m, 1.5_rad)};

  EXPECT_EQ(res, target);
}
