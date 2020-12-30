package org.photonvision;

import java.util.LinkedList;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Transform2d;
import edu.wpi.first.wpiutil.math.Pair;

public class SimVisionSystem {

    SimPhotonCamera cam;

    double camDiagFOV_deg;
    double camHorizFOV_deg;
    double camVertFOV_deg;
    double cameraHeightOffGround_m;
    double maxLEDRange_m;
    double camPitch_deg;
    Pair<Integer, Integer> cameraRes_px;
    double minTargetArea;
    Transform2d robotToCamera;

    LinkedList<SimVisionTarget> tgtList;

    /**
     * Create a simulated vision system involving a camera and coprocessor mounted on a mobile robot running Photonvision, detecting one or more targets scattered around the field.
     * This assumes a fairly simple and distortionless pinhole camera model.
     * @param camName Name of the photonvision camera to create. Align it with the settings you use in the PhotonVision GUI.
     * @param camDiagFOV_deg Diagonal Field of View of the camera used. Align it with the manufacturer specifications, and/or whatever is configured in the PhotonVision Setting page.
     * @param camPitch_deg pitch of the camera's view axis back from horizontal. Make this the same as whatever is configured in the PhotonVision Setting page.
     * @param robotToCamera Pose Transform to move from the robot's position to the camera's mount position
     * @param cameraHeightOffGround_m Height of the camera off the ground in meters
     * @param maxLEDRange_m Maximum distance at which your camera can illuminate the target and make it visible. Set to 9000 or more if your vision system does not rely on LED's.
     * @param cameraRes_px Pair of integers representing the Width and Height (respectively) of your camera's image sensor in pixels
     * @param minTargetArea Minimum area that that the target should be before it's recognized as a target by the camera. Match this with your contour filtering settings in the PhotonVision GUI.
     */
    public SimVisionSystem(String camName, double camDiagFOV_deg, double camPitch_deg, Transform2d robotToCamera, double cameraHeightOffGround_m, double maxLEDRange_m, Pair<Integer, Integer> cameraRes_px, double minTargetArea){
        this.camDiagFOV_deg = camDiagFOV_deg;
        this.camPitch_deg = camPitch_deg;
        this.robotToCamera  = robotToCamera;
        this.cameraHeightOffGround_m  = cameraHeightOffGround_m;
        this.maxLEDRange_m = maxLEDRange_m;
        this.cameraRes_px = cameraRes_px;
        this.minTargetArea = minTargetArea;

        //Calculate horizontal/vertical FOV by similar triangles
        double hypotPixels = Math.hypot(cameraRes_px.getFirst(), cameraRes_px.getSecond());
        this.camHorizFOV_deg = camDiagFOV_deg * cameraRes_px.getFirst()/hypotPixels;
        this.camVertFOV_deg  = camDiagFOV_deg * cameraRes_px.getSecond()/hypotPixels;
        
        cam = new SimPhotonCamera(camName);
        tgtList = new LinkedList<SimVisionTarget>();
    }

    /**
     * Add a target on the field which your vision system is designed to detect.
     * The photoncamera from this system will report the location of the robot rellative
     * to the subste of these targets which are visible from the given robot position.
     * @param tgt 
     */
    public void addSimVisionTarget(SimVisionTarget tgt){
        tgtList.add(tgt);
    }

    /**
     * Adjust the camera position relative to the robot.
     * Use this if your camera is on a gimbal or turret or some other mobile platform.
     * @param newRobotToCamera New Tranform from the robot to the camera
     * @param newCamHeight New height of the camera off the floor
     */
    public void moveCamera(Transform2d newRobotToCamera, double newCamHeight){
        this.robotToCamera  = newRobotToCamera;
        this.cameraHeightOffGround_m  = newCamHeight;
    }

    /**
     * Periodic update. Call this once per frame of image data you wish to process and send to NetworkTables
     * @param robotPose current pose of the robot on the field. Will be used to calcualte which targets are actually in view, where they are at relative to the robot, and relevant PhotonVision parameters.
     */
    public void processFrame(Pose2d robotPose){

        Pose2d cameraPos = robotPose.transformBy(robotToCamera);

        LinkedList<PhotonTrackedTarget> visibleTgtList = new LinkedList<PhotonTrackedTarget>();

        tgtList.forEach((tgt) -> {
            var camToTargetTrans = new Transform2d(cameraPos, tgt.targetPos);

            double dist_along_ground_m = camToTargetTrans.getTranslation().getNorm();
            double dist_vertical_m = tgt.targetHeight_m - this.cameraHeightOffGround_m;
            double dist_m = Math.hypot(dist_along_ground_m, dist_vertical_m);

            double area_px = tgt.tgtArea_m2 / getM2PerPx(dist_along_ground_m);
            
            double yaw_deg = PhotonUtils.wrapAngleDeg(camToTargetTrans.getRotation().getDegrees());
            double pitch_deg = Math.atan2(dist_vertical_m, dist_along_ground_m) - this.camPitch_deg;

            if(camCanSeeTarget(dist_m, yaw_deg, pitch_deg, area_px)){
                visibleTgtList.add(new PhotonTrackedTarget(yaw_deg, pitch_deg, area_px, 0.0, camToTargetTrans));
            }
        });

        cam.submitProcessedFrame(0.0, visibleTgtList);

    }

    double getM2PerPx(double dist){
        double widthMPerPx  = 2 * dist * Math.tan(this.camHorizFOV_deg/2) / this.cameraRes_px.getFirst();
        double heightMPerPx = 2 * dist * Math.tan(this.camVertFOV_deg/2)  / this.cameraRes_px.getSecond();
        return widthMPerPx * heightMPerPx;
    }


    boolean camCanSeeTarget(double dist_m, double yaw, double pitch, double area){ 
        boolean inRange = (dist_m < this.maxLEDRange_m);
        boolean inHorizAngle = Math.abs(yaw) < (this.camHorizFOV_deg/2);
        boolean inVertAngle = Math.abs(pitch) <  (this.camVertFOV_deg/2);
        boolean targetBigEnough = area > this.minTargetArea;
        return (inRange & inHorizAngle & inVertAngle & targetBigEnough);
    }

    
}
