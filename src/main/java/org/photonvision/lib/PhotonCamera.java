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

package org.photonvision.lib;

import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableEntry;
import edu.wpi.first.networktables.NetworkTableInstance;
import org.photonvision.common.SimplePipelineResult;

public class PhotonCamera {
  private final NetworkTableEntry rawBytesEntry;
  private final NetworkTableEntry driverModeEntry;
  private final NetworkTableEntry pipelineIndexEntry;

  private boolean driverMode;
  private int pipelineIndex;

  public PhotonCamera(NetworkTable rootTable) {
    rawBytesEntry = rootTable.getEntry("rawBytes");
    driverModeEntry = rootTable.getEntry("driverMode");
    pipelineIndexEntry = rootTable.getEntry("pipelineIndex");

    driverMode = driverModeEntry.getBoolean(false);
    pipelineIndex = (int) pipelineIndexEntry.getNumber(0);
  }

  public PhotonCamera(String cameraName) {
    this(NetworkTableInstance.getDefault().getTable("photonvision").getSubTable(cameraName));
  }

  /**
   * Get all the data sent by PhotonVision atomically. All the data is guaranteed to be from the same
   * loop of the vision software.
   */
  public SimplePipelineResult getLastResult() {
    var ret = new SimplePipelineResult();
    ret.fromByteArray(rawBytesEntry.getRaw(new byte[]{}));
    return ret;
  }

  /**
   * Sets whether the camera is in driver mode.
   * @param driverMode Whether the camera should be in driver mode.
   */
  public void setDriverMode(boolean driverMode) {
    this.driverMode = driverMode;
    driverModeEntry.setBoolean(this.driverMode);
  }

  /**
   * Returns whether driver mode is enabled.
   * @return Whether driver mode is enabled.
   */
  public boolean getDriverMode() {
    return driverMode;
  }

  /**
   * Sets the pipeline index number.
   * @param index The pipeline index number.
   */
  public void setPipelineIndex(int index) {
    pipelineIndex = index;
    pipelineIndexEntry.setNumber(pipelineIndex);
  }

  /**
   * Returns the pipeline index number.
   * @return The pipeline index number.
   */
  public int getPipelineIndex() {
    return pipelineIndex;
  }

  public boolean hasTargets() {
    return getLastResult().hasTargets();
  }

  public double getFirstTargetPitch() {
    return getLastResult().targets.get(0).getPitch();
  }

  public double getFirstTargetYaw() {
    return getLastResult().targets.get(0).getYaw();
  }

  public double getBestTargetArea() {
    return getLastResult().targets.get(0).getArea();
  }
}
