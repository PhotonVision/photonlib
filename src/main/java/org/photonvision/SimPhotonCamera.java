package org.photonvision;

import java.util.Arrays;
import java.util.List;

import edu.wpi.first.networktables.NetworkTable;

public class SimPhotonCamera extends PhotonCamera {
      
    /**
     * Constructs a Simulated PhotonCamera from a root table.
     *
     * @param rootTable The root table that the camera is broadcasting information
     *                  over.
     */
    public SimPhotonCamera(NetworkTable rootTable) {
      super(rootTable);
    }
  
    /**
     * Constructs a Simulated PhotonCamera from the name of the camera.
     *
     * @param cameraName The nickname of the camera (found in the PhotonVision
     *                   UI).
     */
    public SimPhotonCamera(String cameraName) {
      super(cameraName);
    }


    /**
     * Simulate one processed frame of vision data, putting one result to NT.
     * @param latencyMillis
     * @param targets Each target detected
     */
    public void submitProcessedFrame(double latencyMillis, PhotonTrackedTarget... targets){
      submitProcessedFrame(latencyMillis, Arrays.asList(targets));
    }

    /**
     * Simulate one processed frame of vision data, putting one result to NT.
     * @param latencyMillis
     * @param tgtList List of targets detected
     */
    public void submitProcessedFrame(double latencyMillis, List<PhotonTrackedTarget> tgtList){
      if(!getDriverMode()){
        PhotonPipelineResult newResult = new PhotonPipelineResult(latencyMillis, tgtList);
        var newPacket = new org.photonvision.Packet(newResult.getPacketSize());
        newResult.populatePacket(newPacket);
        rawBytesEntry.setRaw(newPacket.getData());
      }
    }

}