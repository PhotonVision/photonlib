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
#include "photonlib/PhotonUtils.h"
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

TEST(SimVisionSystemTest, testNotVisibleVert1) {
  auto targetPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(35), units::meter_t(0)),
                  frc::Rotation2d());

  photonlib::SimVisionSystem sysUnderTest(
      "Test", units::angle::degree_t(80.0), units::angle::degree_t(0.0),
      frc::Transform2d(), units::length::meter_t(1.0),
      units::length::meter_t(99999.0), 640, 480, 0.0);

  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPose, units::meter_t(1.0),
                                 units::meter_t(3.0), units::meter_t(2.0)));

  auto robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(5.0), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(5.0)));

  sysUnderTest.ProcessFrame(robotPose);
  auto result = sysUnderTest.cam.GetLatestResult();
  ASSERT_TRUE(result.HasTargets());

  sysUnderTest.MoveCamera(frc::Transform2d(frc::Translation2d(),
                                           frc::Rotation2d(units::degree_t(0))),
                          units::meter_t(5000.0), units::degree_t(1.0));
  sysUnderTest.ProcessFrame(robotPose);
  result = sysUnderTest.cam.GetLatestResult();
  ASSERT_FALSE(result.HasTargets());
}

TEST(SimVisionSystemTest, testNotVisibleVert2) {
  auto targetPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(35), units::meter_t(0)),
                  frc::Rotation2d());

  photonlib::SimVisionSystem sysUnderTest(
      "Test", units::angle::degree_t(80.0), units::angle::degree_t(45.0),
      frc::Transform2d(), units::length::meter_t(1.0),
      units::length::meter_t(99999.0), 1234, 1234, 0.5);

  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPose, units::meter_t(3.0),
                                 units::meter_t(0.5), units::meter_t(0.5)));

  auto robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(32.0), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(5.0)));

  sysUnderTest.ProcessFrame(robotPose);
  auto result = sysUnderTest.cam.GetLatestResult();
  ASSERT_TRUE(result.HasTargets());

  robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(0.0), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(5.0)));
  sysUnderTest.ProcessFrame(robotPose);
  result = sysUnderTest.cam.GetLatestResult();
  ASSERT_FALSE(result.HasTargets());
}

TEST(SimVisionSystemTest, testNotVisibleTgtSize) {
  auto targetPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(35), units::meter_t(0)),
                  frc::Rotation2d());

  photonlib::SimVisionSystem sysUnderTest(
      "Test", units::angle::degree_t(80.0), units::angle::degree_t(0.0),
      frc::Transform2d(), units::length::meter_t(1.0),
      units::length::meter_t(99999.0), 640, 480, 20.0);

  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPose, units::meter_t(1.10),
                                 units::meter_t(0.25), units::meter_t(0.1)));

  auto robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(32.0), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(5.0)));
  sysUnderTest.ProcessFrame(robotPose);
  auto result = sysUnderTest.cam.GetLatestResult();
  ASSERT_TRUE(result.HasTargets());

  robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(0.0), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(5.0)));
  sysUnderTest.ProcessFrame(robotPose);
  result = sysUnderTest.cam.GetLatestResult();
  ASSERT_FALSE(result.HasTargets());
}

TEST(SimVisionSystemTest, testNotVisibleTooFarForLEDs) {
  auto targetPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(35), units::meter_t(0)),
                  frc::Rotation2d());

  photonlib::SimVisionSystem sysUnderTest(
      "Test", units::angle::degree_t(80.0), units::angle::degree_t(0.0),
      frc::Transform2d(), units::length::meter_t(1.0),
      units::length::meter_t(10.0), 640, 480, 1.0);

  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPose, units::meter_t(1.10),
                                 units::meter_t(0.25), units::meter_t(0.1)));

  auto robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(28.0), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(5.0)));
  sysUnderTest.ProcessFrame(robotPose);
  auto result = sysUnderTest.cam.GetLatestResult();
  ASSERT_TRUE(result.HasTargets());

  robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(0.0), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(5.0)));
  sysUnderTest.ProcessFrame(robotPose);
  result = sysUnderTest.cam.GetLatestResult();
  ASSERT_FALSE(result.HasTargets());
}

class SimVisionSystemTestYawParam : public testing::TestWithParam<double> {};
INSTANTIATE_TEST_SUITE_P(SimVisionSystemTestYawParamInst,
                         SimVisionSystemTestYawParam,
                         testing::Values(-10, -5, -0, -1, -2, 5, 7, 10.23));
TEST_P(SimVisionSystemTestYawParam, testYawAngles) {
  double testYaw = GetParam();  // Nope, Chuck testYaw
  auto targetPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(35), units::meter_t(0)),
                  frc::Rotation2d(units::degree_t(45)));

  photonlib::SimVisionSystem sysUnderTest(
      "Test", units::angle::degree_t(80.0), units::angle::degree_t(0.0),
      frc::Transform2d(), units::length::meter_t(1.0),
      units::length::meter_t(99999.0), 640, 480, 0.0);

  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPose, units::meter_t(0.0),
                                 units::meter_t(0.5), units::meter_t(0.5)));

  auto robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(32), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(testYaw)));

  sysUnderTest.ProcessFrame(robotPose);
  auto result = sysUnderTest.cam.GetLatestResult();
  ASSERT_TRUE(result.HasTargets());
  auto tgt = result.GetBestTarget();
  ASSERT_DOUBLE_EQ(tgt.GetYaw(), testYaw);
}

class SimVisionSystemTestCameraPitchParam
    : public testing::TestWithParam<double> {};
INSTANTIATE_TEST_SUITE_P(SimVisionSystemTestCameraPitchParamInst,
                         SimVisionSystemTestCameraPitchParam,
                         testing::Values(-10, -5, -0, -1, -2, 5, 7, 10.23,
                                         20.21, -19.999));
TEST_P(SimVisionSystemTestCameraPitchParam, testCameraPitch) {
  double testPitch = GetParam();
  auto targetPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(35), units::meter_t(0)),
                  frc::Rotation2d(units::degree_t(45)));

  auto robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(30), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(0.0)));

  photonlib::SimVisionSystem sysUnderTest(
      "Test", units::angle::degree_t(80.0), units::angle::degree_t(0.0),
      frc::Transform2d(), units::length::meter_t(1.0),
      units::length::meter_t(99999.0), 640, 480, 0.0);

  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPose, units::meter_t(0.0),
                                 units::meter_t(0.5), units::meter_t(0.5)));

  sysUnderTest.MoveCamera(
      frc::Transform2d(frc::Translation2d(), frc::Rotation2d()),
      units::meter_t(0.0), units::degree_t(testPitch));

  sysUnderTest.ProcessFrame(robotPose);
  auto result = sysUnderTest.cam.GetLatestResult();
  ASSERT_TRUE(result.HasTargets());
  auto tgt = result.GetBestTarget();
  // If the camera is pitched down by 10 degrees, the target should appear
  // in the upper part of the image (ie, pitch positive). Therefor,
  // pass/fail involves -1.0.
  ASSERT_DOUBLE_EQ(tgt.GetPitch(), -1.0 * testPitch);
}

class SimVisionSystemTestDistCalcParam
    : public testing::TestWithParam<std::tuple<double, double, double>> {};
INSTANTIATE_TEST_SUITE_P(
    SimVisionSystemTestDistCalcParamInst, SimVisionSystemTestDistCalcParam,
    testing::Values(std::tuple<double, double, double>(5, 35, 0),
                    std::tuple<double, double, double>(6, 35, 1),
                    std::tuple<double, double, double>(10, 35, 0),
                    std::tuple<double, double, double>(15, 35, 2),
                    std::tuple<double, double, double>(19.95, 35, 0),
                    std::tuple<double, double, double>(20, 35, 0),
                    std::tuple<double, double, double>(5, 42, 1),
                    std::tuple<double, double, double>(6, 42, 0),
                    std::tuple<double, double, double>(10, 42, 2),
                    std::tuple<double, double, double>(15, 42, 0.5),
                    std::tuple<double, double, double>(19.42, 35, 0),
                    std::tuple<double, double, double>(20, 42, 0),
                    std::tuple<double, double, double>(5, 55, 2),
                    std::tuple<double, double, double>(6, 55, 0),
                    std::tuple<double, double, double>(10, 54, 2.2),
                    std::tuple<double, double, double>(15, 53, 0),
                    std::tuple<double, double, double>(19.52, 35, 1.1),
                    std::tuple<double, double, double>(20, 51, 2.87),
                    std::tuple<double, double, double>(20, 55, 3)));
TEST_P(SimVisionSystemTestDistCalcParam, testDistanceCalc) {
  std::tuple<double, double, double> testArgs = GetParam();
  double testDist = std::get<0>(testArgs);
  double testPitch = std::get<1>(testArgs);
  double testHeight = std::get<2>(testArgs);

  auto targetPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(35), units::meter_t(0)),
                  frc::Rotation2d(units::radian_t(3.14159 / 42)));

  auto robotPose = frc::Pose2d(
      frc::Translation2d(units::meter_t(35 - testDist), units::meter_t(0.0)),
      frc::Rotation2d(units::degree_t(0.0)));

  photonlib::SimVisionSystem sysUnderTest(
      "absurdlylongnamewhichshouldneveractuallyhappenbuteehwelltestitanywaysoho"
      "wsyourdaygoingihopegoodhaveagreatrestofyourlife",
      units::angle::degree_t(160.0), units::angle::degree_t(testPitch),
      frc::Transform2d(), units::length::meter_t(testHeight),
      units::length::meter_t(99999.0), 640, 480, 0.0);

  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPose, units::meter_t(testDist),
                                 units::meter_t(0.5), units::meter_t(0.5)));

  sysUnderTest.ProcessFrame(robotPose);
  auto result = sysUnderTest.cam.GetLatestResult();
  ASSERT_TRUE(result.HasTargets());
  auto tgt = result.GetBestTarget();
  ASSERT_DOUBLE_EQ(tgt.GetYaw(), 0.0);
  units::meter_t distMeas = photonlib::PhotonUtils::CalculateDistanceToTarget(
      units::meter_t(testHeight), units::meter_t(testDist),
      units::degree_t(testPitch), units::degree_t(tgt.GetPitch()));
  ASSERT_DOUBLE_EQ(distMeas.to<double>(), testDist);
}

TEST(SimVisionSystemTest, testMultipleTargets) {
  auto targetPoseL =
      frc::Pose2d(frc::Translation2d(units::meter_t(35), units::meter_t(2)),
                  frc::Rotation2d(units::radian_t()));
  auto targetPoseC =
      frc::Pose2d(frc::Translation2d(units::meter_t(35), units::meter_t(0)),
                  frc::Rotation2d(units::radian_t()));
  auto targetPoseR =
      frc::Pose2d(frc::Translation2d(units::meter_t(35), units::meter_t(-2)),
                  frc::Rotation2d(units::radian_t()));

  photonlib::SimVisionSystem sysUnderTest(
      "test", units::angle::degree_t(160.0), units::angle::degree_t(0.0),
      frc::Transform2d(), units::length::meter_t(5.0),
      units::length::meter_t(99999.0), 640, 480, 20.0);

  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPoseL, units::meter_t(0.0),
                                 units::meter_t(0.25), units::meter_t(0.1)));
  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPoseC, units::meter_t(1.0),
                                 units::meter_t(0.25), units::meter_t(0.1)));
  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPoseR, units::meter_t(2.0),
                                 units::meter_t(0.25), units::meter_t(0.1)));
  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPoseL, units::meter_t(3.0),
                                 units::meter_t(0.25), units::meter_t(0.1)));
  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPoseC, units::meter_t(4.0),
                                 units::meter_t(0.25), units::meter_t(0.1)));
  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPoseR, units::meter_t(5.0),
                                 units::meter_t(0.25), units::meter_t(0.1)));
  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPoseL, units::meter_t(6.0),
                                 units::meter_t(0.25), units::meter_t(0.1)));
  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPoseC, units::meter_t(7.0),
                                 units::meter_t(0.25), units::meter_t(0.1)));
  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPoseL, units::meter_t(8.0),
                                 units::meter_t(0.25), units::meter_t(0.1)));
  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPoseR, units::meter_t(9.0),
                                 units::meter_t(0.25), units::meter_t(0.1)));
  sysUnderTest.AddSimVisionTarget(
      photonlib::SimVisionTarget(targetPoseL, units::meter_t(10.0),
                                 units::meter_t(0.25), units::meter_t(0.1)));

  auto robotPose =
      frc::Pose2d(frc::Translation2d(units::meter_t(30), units::meter_t(0.0)),
                  frc::Rotation2d(units::degree_t(5.0)));

  sysUnderTest.ProcessFrame(robotPose);
  auto result = sysUnderTest.cam.GetLatestResult();
  ASSERT_TRUE(result.HasTargets());

  auto tgtList = result.GetTargets();
  ASSERT_EQ(11ul, tgtList.size());
}
