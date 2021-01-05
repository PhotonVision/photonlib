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

#include "gtest/gtest.h"
#include "photonlib/SimVisionSystem.h"
#include "photonlib/PhotonCamera.h"



#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>

TEST(SimVisionSystemTest, testEmpty) {


    photonlib::SimVisionSystem sysUnderTest ("Test",
           units::angle::degree_t(80.0),
           units::angle::degree_t(0.0),
           frc::Transform2d(frc::Translation2d(units::length::meter_t(35.0),units::length::meter_t(0.0)), frc::Rotation2d()),
           units::length::meter_t(1.0),
           units::length::meter_t(99999.0),
           320,
           240,
           0.0);

    for(int loopIdx = 0; loopIdx < 100; loopIdx++){
        sysUnderTest.ProcessFrame(frc::Pose2d());
    }

}

TEST(PhotonCamBaseTest, testInstantiate) {


    photonlib::PhotonCamera testCam ("Test");

}
