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

package org.photonvision;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Rotation2d;
import edu.wpi.first.wpilibj.geometry.Transform2d;
import edu.wpi.first.wpilibj.geometry.Translation2d;


class SimVisionSystemTest {
    @Test
    public void testEmpty() {
        Assertions.assertDoesNotThrow(() -> {
            var sysUnderTest = new SimVisionSystem("Test", 80.0, 0.0, new Transform2d(), 1, 99999, 320, 240, 0);
            sysUnderTest.addSimVisionTarget(new SimVisionTarget(new Pose2d(), 0.0, 1.0, 1.0));
            for(int loopIdx = 0; loopIdx < 100; loopIdx++){
                sysUnderTest.processFrame(new Pose2d());
            }
        });
    }


    @Test
    public void testDistanceAligned() {

        final var targetPose = new Pose2d(new Translation2d(35,0), new Rotation2d());
        var sysUnderTest = new SimVisionSystem("Test", 80.0, 0.0, new Transform2d(), 1, 99999, 320, 240, 0);
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPose, 0.0, 1.0, 1.0));

        for(double dist = 5; dist < 30; dist++){

            final var robotPose  = new Pose2d(new Translation2d(35-dist,0), new Rotation2d());
            sysUnderTest.processFrame(robotPose);

            var result = sysUnderTest.cam.getLatestResult();

            assertTrue(result.hasTargets());
            assertEquals(result.getBestTarget().getCameraToTarget().getTranslation().getNorm(), dist);
        }
    }

    @Test
    public void testVisiblityCupidShuffle() {
        final var targetPose = new Pose2d(new Translation2d(35,0), new Rotation2d());
        var sysUnderTest = new SimVisionSystem("Test", 80.0, 0.0, new Transform2d(), 1, 99999, 640, 480, 0);
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPose, 1.0, 3.0, 3.0));

        //To the right, to the right
        var robotPose  = new Pose2d(new Translation2d(5,0), Rotation2d.fromDegrees(-70));
        sysUnderTest.processFrame(robotPose);
        assertFalse(sysUnderTest.cam.getLatestResult().hasTargets());

        //To the right, to the right
        robotPose  = new Pose2d(new Translation2d(5,0), Rotation2d.fromDegrees(-95));
        sysUnderTest.processFrame(robotPose);
        assertFalse(sysUnderTest.cam.getLatestResult().hasTargets());

        // To the left, to the left
        robotPose  = new Pose2d(new Translation2d(5,0), Rotation2d.fromDegrees(90));
        sysUnderTest.processFrame(robotPose);
        assertFalse(sysUnderTest.cam.getLatestResult().hasTargets());

        // To the left, to the left
        robotPose  = new Pose2d(new Translation2d(5,0), Rotation2d.fromDegrees(65));
        sysUnderTest.processFrame(robotPose);
        assertFalse(sysUnderTest.cam.getLatestResult().hasTargets());

        //now kick, now kick
        robotPose  = new Pose2d(new Translation2d(2,0), Rotation2d.fromDegrees(5));
        sysUnderTest.processFrame(robotPose);
        assertTrue(sysUnderTest.cam.getLatestResult().hasTargets());

        //now kick, now kick
        robotPose  = new Pose2d(new Translation2d(2,0), Rotation2d.fromDegrees(-5));
        sysUnderTest.processFrame(robotPose);
        assertTrue(sysUnderTest.cam.getLatestResult().hasTargets());

        //now walk it by yourself
        robotPose  = new Pose2d(new Translation2d(2,0), Rotation2d.fromDegrees(-179));
        sysUnderTest.processFrame(robotPose);
        assertFalse(sysUnderTest.cam.getLatestResult().hasTargets());

        // now walk it by yourself
        sysUnderTest.moveCamera(new Transform2d( new Translation2d(), Rotation2d.fromDegrees(180)), 0);
        sysUnderTest.processFrame(robotPose);
        assertTrue(sysUnderTest.cam.getLatestResult().hasTargets());
    }

    @Test
    public void testNotVisibleVert1() {
        final var targetPose = new Pose2d(new Translation2d(35,0), new Rotation2d());
        var sysUnderTest = new SimVisionSystem("Test", 80.0, 0.0, new Transform2d(), 1, 99999, 640, 480, 0);
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPose, 1.0, 3.0, 3.0));

        var robotPose  = new Pose2d(new Translation2d(5,0), Rotation2d.fromDegrees(5));
        sysUnderTest.processFrame(robotPose);
        assertTrue(sysUnderTest.cam.getLatestResult().hasTargets());

        sysUnderTest.moveCamera(new Transform2d(), 5000); //vooop selfie stick
        sysUnderTest.processFrame(robotPose);
        assertFalse(sysUnderTest.cam.getLatestResult().hasTargets());
    }

    @Test
    public void testNotVisibleVert2() {
        final var targetPose = new Pose2d(new Translation2d(35,0), new Rotation2d());
        var sysUnderTest = new SimVisionSystem("Test", 80.0, 45.0, new Transform2d(), 1, 99999, 1234, 1234, 0);
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPose, 3.0, 0.5, 0.5));

        var robotPose  = new Pose2d(new Translation2d(32,0), Rotation2d.fromDegrees(5));
        sysUnderTest.processFrame(robotPose);
        assertTrue(sysUnderTest.cam.getLatestResult().hasTargets());

        //Pitched back camera should mean target goes out of view below the robot as distance increases
        robotPose  = new Pose2d(new Translation2d(0,0), Rotation2d.fromDegrees(5));
        sysUnderTest.processFrame(robotPose);
        assertFalse(sysUnderTest.cam.getLatestResult().hasTargets());
    }

    @Test
    public void testNotVisibleTgtSize() {
        final var targetPose = new Pose2d(new Translation2d(35,0), new Rotation2d());
        var sysUnderTest = new SimVisionSystem("Test", 80.0, 0.0, new Transform2d(), 1, 99999, 640, 480, 20.0);
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPose, 1.0, 0.25, 0.1));

        var robotPose  = new Pose2d(new Translation2d(32,0), Rotation2d.fromDegrees(5));
        sysUnderTest.processFrame(robotPose);
        assertTrue(sysUnderTest.cam.getLatestResult().hasTargets());

        robotPose  = new Pose2d(new Translation2d(0,0), Rotation2d.fromDegrees(5));
        sysUnderTest.processFrame(robotPose);
        assertFalse(sysUnderTest.cam.getLatestResult().hasTargets());
    }

    @Test
    public void testNotVisibleTooFarForLEDs() {
        final var targetPose = new Pose2d(new Translation2d(35,0), new Rotation2d());
        var sysUnderTest = new SimVisionSystem("Test", 80.0, 0.0, new Transform2d(), 1, 10, 640, 480, 1.0);
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPose, 1.0, 0.25, 0.1));

        var robotPose  = new Pose2d(new Translation2d(28,0), Rotation2d.fromDegrees(5));
        sysUnderTest.processFrame(robotPose);
        assertTrue(sysUnderTest.cam.getLatestResult().hasTargets());

        robotPose  = new Pose2d(new Translation2d(0,0), Rotation2d.fromDegrees(5));
        sysUnderTest.processFrame(robotPose);
        assertFalse(sysUnderTest.cam.getLatestResult().hasTargets());
    }

    @Test
    public void testYawAngles() {
        final var targetPose = new Pose2d(new Translation2d(35,0), new Rotation2d(Math.PI/4));
        var sysUnderTest = new SimVisionSystem("Test", 80.0, 0.0, new Transform2d(), 1, 99999, 640, 480, 0);
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPose, 0.0, 0.5,0.5));

        for(double testYaw = -5; testYaw <=5; testYaw+= 0.5){
            var robotPose  = new Pose2d(new Translation2d(32,0), Rotation2d.fromDegrees(testYaw));
            sysUnderTest.processFrame(robotPose);
            var res = sysUnderTest.cam.getLatestResult();
            assertTrue(res.hasTargets());
            var tgt = res.getBestTarget();
            assertEquals(tgt.getYaw(), testYaw, 0.0001);
        }
    }

    @Test
    public void testPitchAngles() {
        final var targetPose = new Pose2d(new Translation2d(35,0), new Rotation2d(Math.PI/4));
        var sysUnderTest = new SimVisionSystem("Test", 80.0, 0.0, new Transform2d(), 1, 99999, 640, 480, 0);
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPose, 0.0, 0.5,0.5));

        for(double testPitch = -10; testPitch <=10; testPitch+= 0.5){
            //TODO
        }
    }


}
