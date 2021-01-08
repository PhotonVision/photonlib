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

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;
import org.junit.jupiter.params.provider.ValueSource;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Rotation2d;
import edu.wpi.first.wpilibj.geometry.Transform2d;
import edu.wpi.first.wpilibj.geometry.Translation2d;
import edu.wpi.first.wpilibj.util.Units;

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


    @ParameterizedTest
    @ValueSource(doubles = {5,10,15,20,25,30})
    public void testDistanceAligned( double dist) {

        final var targetPose = new Pose2d(new Translation2d(35,0), new Rotation2d());
        var sysUnderTest = new SimVisionSystem("Test", 80.0, 0.0, new Transform2d(), 1, 99999, 320, 240, 0);
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPose, 0.0, 1.0, 1.0));

        final var robotPose  = new Pose2d(new Translation2d(35-dist,0), new Rotation2d());
        sysUnderTest.processFrame(robotPose);

        var result = sysUnderTest.cam.getLatestResult();

        assertTrue(result.hasTargets());
        assertEquals(result.getBestTarget().getCameraToTarget().getTranslation().getNorm(), dist);

    }

    @Test
    public void testVisibilityCupidShuffle() {
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
        sysUnderTest.moveCamera(new Transform2d( new Translation2d(), Rotation2d.fromDegrees(180)), 0, 1.0);
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

        sysUnderTest.moveCamera(new Transform2d(), 5000, 1.0); //vooop selfie stick
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

    @ParameterizedTest
    @ValueSource(doubles = {-10,-5,-0,-1,-2,5,7,10.23})
    public void testYawAngles(double testYaw) {
        final var targetPose = new Pose2d(new Translation2d(35,0), new Rotation2d(Math.PI/4));
        var sysUnderTest = new SimVisionSystem("Test", 80.0, 0.0, new Transform2d(), 1, 99999, 640, 480, 0);
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPose, 0.0, 0.5,0.5));

        var robotPose  = new Pose2d(new Translation2d(32,0), Rotation2d.fromDegrees(testYaw));
        sysUnderTest.processFrame(robotPose);
        var res = sysUnderTest.cam.getLatestResult();
        assertTrue(res.hasTargets());
        var tgt = res.getBestTarget();
        assertEquals(tgt.getYaw(), testYaw, 0.0001);
    }

    @ParameterizedTest
    @ValueSource(doubles = {-10,-5,-0,-1,-2,5,7,10.23,20.21,-19.999})
    public void testCameraPitch(double testPitch) {
        final var targetPose = new Pose2d(new Translation2d(35,0), new Rotation2d(Math.PI/4));
        final var robotPose  = new Pose2d(new Translation2d(30,0), new Rotation2d(0));
        var sysUnderTest = new SimVisionSystem("Test", 80.0, 0.0, new Transform2d(), 0.0, 99999, 640, 480, 0);
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPose, 0.0, 0.5,0.5));

        sysUnderTest.moveCamera(new Transform2d(), 0.0, testPitch);
        sysUnderTest.processFrame(robotPose);
        var res = sysUnderTest.cam.getLatestResult();
        assertTrue(res.hasTargets());
        var tgt = res.getBestTarget();
        //If the camera is pitched down by 10 degrees, the target should appear
        // in the upper part of the image (ie, pitch positive). Therefor,
        // pass/fail involves -1.0.
        assertEquals(tgt.getPitch(), -1.0 * testPitch, 0.0001);
    }


    private static Stream<Arguments> distCalCParamProvider() {
        //Arbitrary and fairly random assortment of distances, camera pitches, and heights
        return Stream.of(
                Arguments.of(5,35,0),
                Arguments.of(6,35,1),
                Arguments.of(10,35,0),
                Arguments.of(15,35,2),
                Arguments.of(19.95,35,0),
                Arguments.of(20,35,0),
                Arguments.of(5,42,1),
                Arguments.of(6,42,0),
                Arguments.of(10,42,2),
                Arguments.of(15,42,0.5),
                Arguments.of(19.42,35,0),
                Arguments.of(20,42,0),
                Arguments.of(5,55,2),
                Arguments.of(6,55,0),
                Arguments.of(10,54,2.2),
                Arguments.of(15,53,0),
                Arguments.of(19.52,35,1.1),
                Arguments.of(20,51,2.87),
                Arguments.of(20,55,3)
        );
    }

    @ParameterizedTest
    @MethodSource("distCalCParamProvider")
    public void testDistanceCalc(double testDist, double testPitch, double testHeight) {
        //Assume dist along ground and tgt height the same. Iterate over other parameters.

        final var targetPose = new Pose2d(new Translation2d(35,0), new Rotation2d(Math.PI/42));
        final var robotPose  = new Pose2d(new Translation2d(35 - testDist,0), new Rotation2d(0));
        var sysUnderTest = new SimVisionSystem("absurdlylongnamewhichshouldneveractuallyhappenbuteehwelltestitanywaysohowsyourdaygoingihopegoodhaveagreatrestofyourlife!",
                                            160.0, testPitch, new Transform2d(), testHeight, 99999, 640, 480, 0);
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPose, testDist, 0.5,0.5));

        sysUnderTest.processFrame(robotPose);
        var res = sysUnderTest.cam.getLatestResult();
        assertTrue(res.hasTargets());
        var tgt = res.getBestTarget();
        assertEquals(tgt.getYaw(), 0.0, 0.0001);
        double distMeas = PhotonUtils.calculateDistanceToTargetMeters(testHeight, testDist, Units.degreesToRadians(testPitch), Units.degreesToRadians(tgt.getPitch()));
        assertEquals(distMeas, testDist, 0.001);

    }

    @Test
    public void testMultipleTargets() {
        final var targetPoseL = new Pose2d(new Translation2d(35,2), new Rotation2d());
        final var targetPoseC = new Pose2d(new Translation2d(35,0), new Rotation2d());
        final var targetPoseR = new Pose2d(new Translation2d(35,-2), new Rotation2d());
        var sysUnderTest = new SimVisionSystem("Test", 160.0, 0.0, new Transform2d(), 5.0, 99999, 640, 480, 20.0);

        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPoseL, 0.0,  0.25, 0.1));
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPoseC, 1.0,  0.25, 0.1));
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPoseR, 2.0,  0.25, 0.1));
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPoseL, 3.0,  0.25, 0.1));
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPoseC, 4.0,  0.25, 0.1));
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPoseR, 5.0,  0.25, 0.1));
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPoseL, 6.0,  0.25, 0.1));
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPoseC, 7.0,  0.25, 0.1));
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPoseL, 8.0,  0.25, 0.1));
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPoseR, 9.0,  0.25, 0.1));
        sysUnderTest.addSimVisionTarget(new SimVisionTarget(targetPoseL, 10.0, 0.25, 0.1));

        var robotPose  = new Pose2d(new Translation2d(30,0), Rotation2d.fromDegrees(5));
        sysUnderTest.processFrame(robotPose);
        var res = sysUnderTest.cam.getLatestResult();
        assertTrue(res.hasTargets());
        List<PhotonTrackedTarget> tgtList = new ArrayList<PhotonTrackedTarget>();
        tgtList = res.getTargets();
        assertEquals(tgtList.size(), 11);

    }


}
