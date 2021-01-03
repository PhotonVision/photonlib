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

#include "photonlib/SimVisionSystem.h"

#include <cmath>

namespace photonlib {

    SimVisionSystem::SimVisionSystem(const std::string&  name,
                                    units::angle::degree_t camDiagFOVDegrees,
                                    units::angle::degree_t camPitchDegrees,
                                    frc::Transform2d& robotToCamera,
                                    units::length::meter_t cameraHeightOffGroundMeters,
                                    units::length::meter_t maxLEDRangeMeters,
                                    int cameraResWidth,
                                    int cameraResHeight,
                                    double minTargetArea):
                                    camDiagFOVDegrees(camDiagFOVDegrees),
                                    camPitchDegrees(camPitchDegrees),
                                    robotToCamera(robotToCamera),
                                    cameraHeightOffGroundMeters(cameraHeightOffGroundMeters),
                                    maxLEDRangeMeters(maxLEDRangeMeters),
                                    cameraResWidth(cameraResWidth),
                                    cameraResHeight(cameraResHeight),
                                    minTargetArea(minTargetArea) {
      double hypotPixels = std::hypot(cameraResWidth, cameraResHeight);
      camHorizFOVDegrees = camDiagFOVDegrees * cameraResWidth/hypotPixels;
      camVertFOVDegrees  = camDiagFOVDegrees * cameraResHeight/hypotPixels;

      cam = new SimPhotonCamera(name);
      tgtList.clear();
    }

    void SimVisionSystem::AddSimVisionTarget(SimVisionTarget tgt){
        tgtList.push_back(tgt);
    }

    void SimVisionSystem::MoveCamera(frc::Transform2d& newRobotToCamera, units::length::meter_t newCamHeightMeters, units::angle::degree_t newCamPitchDegrees){
        robotToCamera  = newRobotToCamera;
        cameraHeightOffGroundMeters  = newCamHeightMeters;
        camPitchDegrees = newCamPitchDegrees;
    }

    void SimVisionSystem::ProcessFrame(frc::Pose2d& robotPoseMeters){

        frc::Pose2d cameraPos = robotPoseMeters.TransformBy(robotToCamera);
        std::vector<PhotonTrackedTarget> visibleTgtList = {};


        std::vector<SimVisionTarget>::iterator tgt;
        for (tgt = tgtList.begin(); tgt != tgtList.end(); ++tgt){
            frc::Transform2d camToTargetTrans = frc::Transform2d(cameraPos, tgt->targetPos);

            units::length::meter_t distAlongGroundMeters = camToTargetTrans.Translation().Norm();
            units::length::meter_t distVerticalMeters = tgt->targetHeightAboveGroundMeters - cameraHeightOffGroundMeters;
            units::length::meter_t distMeters = units::math::hypot(distAlongGroundMeters, distVerticalMeters);

            double area = tgt->tgtAreaMeters2.to<double>() / getM2PerPx(distAlongGroundMeters);

            //2D yaw mode considers the target as a point, and should ignore target rotation.
            // Photon reports it in the correct robot reference frame.
            // IE: targets to the left of the image should report negative yaw.
            units::angle::degree_t yawDegrees = -1.0 * units::math::atan2(camToTargetTrans.Translation().Y(),camToTargetTrans.Translation().X());
            units::angle::degree_t pitchDegrees = units::math::atan2(distVerticalMeters, distAlongGroundMeters) - camPitchDegrees;

            if(camCanSeeTarget(distMeters, yawDegrees, pitchDegrees, area)){
                PhotonTrackedTarget * newTgt = new PhotonTrackedTarget(yawDegrees.to<double>(), pitchDegrees.to<double>(), area, 0.0, camToTargetTrans);
                visibleTgtList.push_back(*newTgt);
            }
        }

        units::second_t procDelay (0.0); //Future - tie this to something meaningful
        cam->submitProcessedFrame(procDelay, wpi::MutableArrayRef<PhotonTrackedTarget>(visibleTgtList));

    }

    double SimVisionSystem::getM2PerPx(units::length::meter_t dist){
        double heightMPerPx = 2 * dist.to<double>() * units::math::tan(camVertFOVDegrees/2)  / cameraResHeight;
        double widthMPerPx  = 2 * dist.to<double>() * units::math::tan(camHorizFOVDegrees/2) / cameraResWidth;
        return widthMPerPx * heightMPerPx;
    }


    bool SimVisionSystem::camCanSeeTarget(units::length::meter_t distMeters, units::angle::degree_t yaw, units::angle::degree_t pitch, double area){
        bool inRange = (distMeters < maxLEDRangeMeters);
        bool inHorizAngle = units::math::abs(yaw) < (camHorizFOVDegrees/2);
        bool inVertAngle = units::math::abs(pitch) <  (camVertFOVDegrees/2);
        bool targetBigEnough = area > minTargetArea;
        return (inRange && inHorizAngle && inVertAngle && targetBigEnough);
    }


}  // namespace photonlib
