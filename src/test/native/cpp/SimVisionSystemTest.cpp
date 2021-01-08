/**
 * Copyright (C) 2020-2021 Photon Vision.
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

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>

#include <units/angle.h>
#include <units/length.h>

#include "gtest/gtest.h"
#include "photonlib/PhotonCamera.h"
#include "photonlib/SimVisionSystem.h"

TEST(SimVisionSystemTest, testEmpty) {
  photonlib::SimVisionSystem sysUnderTest(
      "Test", units::angle::degree_t(80.0), units::angle::degree_t(0.0),
      frc::Transform2d(), units::length::meter_t(1.0),
      units::length::meter_t(99999.0), 320, 240, 0.0);

  for (int loopIdx = 0; loopIdx < 100; loopIdx++) {
    sysUnderTest.ProcessFrame(frc::Pose2d());
  }
}

class SimVisionSystemTestDistParam : public testing::TestWithParam<double> {};
INSTANTIATE_TEST_SUITE_P(SimVisionSystemTestDistParamInst,
                         SimVisionSystemTestDistParam,
                         testing::Values(5, 10, 15, 20, 25, 30));

TEST_P(SimVisionSystemTestDistParam, testDistanceAligned) {
  double dist = GetParam();

  auto targetPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(35), units::meter_t(0)),
                  frc::Rotation2d());

  photonlib::SimVisionSystem sysUnderTest(
      "Test", units::angle::degree_t(80.0), units::angle::degree_t(0.0),
      frc::Transform2d(), units::length::meter_t(1.0),
      units::length::meter_t(99999.0), 320, 240, 0.0);

  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPose, units::meter_t(0.0),
                                 units::meter_t(1.0), units::meter_t(1.0)));

  auto robotPose = frc::Pose2d(
      frc::Translation2d(units::meter_t(35.0 - dist), units::meter_t(0)),
      frc::Rotation2d());

  sysUnderTest.ProcessFrame(robotPose);
  auto result = sysUnderTest.cam.GetLatestResult();
  ASSERT_TRUE(result.HasTargets());
  ASSERT_EQ(result.GetBestTarget()
                .GetCameraRelativePose()
                .Translation()
                .Norm()
                .to<double>(),
            dist);
}

TEST(SimVisionSystemTest, testVisibilityCupidShuffle) {
  auto targetPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(35), units::meter_t(0)),
                  frc::Rotation2d());

  photonlib::SimVisionSystem sysUnderTest(
      "Test", units::angle::degree_t(80.0), units::angle::degree_t(0.0),
      frc::Transform2d(), units::length::meter_t(1.0),
      units::length::meter_t(99999.0), 320, 240, 0.0);

  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPose, units::meter_t(0.0),
                                 units::meter_t(3.0), units::meter_t(3.0)));

  // To the right, to the right
  auto robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(5.0), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(-70.0)));
  sysUnderTest.ProcessFrame(robotPose);
  auto result = sysUnderTest.cam.GetLatestResult();
  ASSERT_FALSE(result.HasTargets());

  // To the right, to the right
  robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(5.0), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(-95.0)));
  sysUnderTest.ProcessFrame(robotPose);
  result = sysUnderTest.cam.GetLatestResult();
  ASSERT_FALSE(result.HasTargets());

  // To the left, to the left
  robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(5.0), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(90.0)));
  sysUnderTest.ProcessFrame(robotPose);
  result = sysUnderTest.cam.GetLatestResult();
  ASSERT_FALSE(result.HasTargets());

  // To the left, to the left
  robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(5.0), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(65.0)));
  sysUnderTest.ProcessFrame(robotPose);
  result = sysUnderTest.cam.GetLatestResult();
  ASSERT_FALSE(result.HasTargets());

  // now kick, now kick
  robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(2.0), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(5.0)));
  sysUnderTest.ProcessFrame(robotPose);
  result = sysUnderTest.cam.GetLatestResult();
  ASSERT_TRUE(result.HasTargets());

  // now kick, now kick
  robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(2.0), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(-5.0)));
  sysUnderTest.ProcessFrame(robotPose);
  result = sysUnderTest.cam.GetLatestResult();
  ASSERT_TRUE(result.HasTargets());

  // now walk it by yourself
  robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(2.0), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(-179.0)));
  sysUnderTest.ProcessFrame(robotPose);
  result = sysUnderTest.cam.GetLatestResult();
  ASSERT_FALSE(result.HasTargets());

  // now walk it by yourself
  sysUnderTest.MoveCamera(
      frc::Transform2d(frc::Translation2d(),
                       frc::Rotation2d(units::degree_t(180))),
      units::meter_t(0.0), units::degree_t(1.0));
  sysUnderTest.ProcessFrame(robotPose);
  result = sysUnderTest.cam.GetLatestResult();
  ASSERT_TRUE(result.HasTargets());
}
