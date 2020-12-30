package org.photonvision;

import java.util.LinkedList;

import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.wpilibj.geometry.Transform2d;

public class SimPhotonCamera extends PhotonCamera {
    
    private Packet packet = new Packet(1);

    LinkedList<PhotonTrackedTarget> trackedTargetList = new LinkedList<PhotonTrackedTarget>();
  
    /**
     * Constructs a Simulated PhotonCamera from a root table.
     *
     * @param rootTable The root table that the camera is broadcasting information
     *                  over.
     */
    public SimPhotonCamera(NetworkTable rootTable) {
      super(rootTable);
      constructorCommon();
    }
  
    /**
     * Constructs a Simulated PhotonCamera from the name of the camera.
     *
     * @param cameraName The nickname of the camera (found in the PhotonVision
     *                   UI).
     */
    public SimPhotonCamera(String cameraName) {
      super(cameraName);
      constructorCommon();
    }

    private void constructorCommon(){
      trackedTargetList.clear();
    }

    /**
     * Report simple information about a target detected using a 3d pipeline
     * @param cameraToTarget Transform from the camera's pose to the target's pose, in meters.
     */
    public void reportDetectedTarget(Transform2d cameraToTarget){
      reportDetectedTarget(cameraToTarget, 0.0, 1.0, 0.0);
    }

    /**
     * Report detailed information about a target detected using a 3d pipeline
     * @param cameraToTarget Transform from the camera's pose to the target's pose, in meters.
     * @param pitch Target centroid's position up or down in the image
     * @param area Target area in square pixels 
     * @param skew Target in-plane skew rotation in the image in degrees.
     */
    public void reportDetectedTarget(Transform2d cameraToTarget, double pitch, double area, double skew){
      double yaw = wrapAngleDeg(cameraToTarget.getRotation().getDegrees());
      var reportedTarget = new PhotonTrackedTarget(yaw, pitch, area, skew, cameraToTarget);
      trackedTargetList.add(reportedTarget);
    }

    /**
     * Report a target detected using a 2d pipeline
     * @param yaw Target centroid's position to the left or right of the image
     * @param pitch Target centroid's position up or down in the image
     * @param area Target area in square pixels 
     * @param skew Target in-plane skew rotation in the image in degrees.
     */
    public void reportDetectedTarget(double yaw, double pitch, double area, double skew){
      var reportedTarget = new PhotonTrackedTarget(yaw, pitch, area, skew, new Transform2d());
      trackedTargetList.add(reportedTarget);
    }

    /**
     * Simulate one processed frame of vision data, putting one result to NT.
     */
    public void update(){
      PhotonPipelineResult newResult = new PhotonPipelineResult(0,trackedTargetList);
      var newPacket = new org.photonvision.Packet(newResult.getPacketSize());
      newResult.populatePacket(newPacket);
      rawBytesEntry.setRaw(newPacket.getData());
      trackedTargetList.clear();
    }

    public static double wrapAngleDeg(double angle){
      angle %=360;
      angle= angle>180 ? angle-360 : angle;
      angle= angle<-180 ? angle+360 : angle;
      return angle;
  }

}