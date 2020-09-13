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

package org.photonvision;

import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableEntry;
import edu.wpi.first.networktables.NetworkTableInstance;

/**
 * Represents a camera that is connected to PhotonVision.
 */
public class PhotonCamera {
  private final NetworkTableEntry rawBytesEntry;
  private final NetworkTableEntry driverModeEntry;
  private final NetworkTableEntry pipelineIndexEntry;
  private final NetworkTableEntry ledModeEntry;

  private final NetworkTable mainTable = NetworkTableInstance.getDefault().getTable("photonvision");

  private boolean driverMode;
  private int pipelineIndex;
  private LEDMode mode;

  private Packet packet = new Packet(1);

  /**
   * Constructs a PhotonCamera from a root table.
   *
   * @param rootTable The root table that the camera is broadcasting information
   *                  over.
   */
  public PhotonCamera(NetworkTable rootTable) {
    rawBytesEntry = rootTable.getEntry("rawBytes");
    driverModeEntry = rootTable.getEntry("driverMode");
    pipelineIndexEntry = rootTable.getEntry("pipelineIndex");
    ledModeEntry = mainTable.getEntry("ledMode");

    driverMode = driverModeEntry.getBoolean(false);
    pipelineIndex = pipelineIndexEntry.getNumber(0).intValue();
    getLEDMode();
  }

  /**
   * Constructs a PhotonCamera from the name of the camera.
   *
   * @param cameraName The nickname of the camera (found in the PhotonVision
   *                   UI).
   */
  public PhotonCamera(String cameraName) {
    this(NetworkTableInstance.getDefault().getTable("photonvision").getSubTable(cameraName));
  }

  /**
   * Returns the latest pipeline result.
   *
   * @return The latest pipeline result.
   */
  public PhotonPipelineResult getLatestResult() {
    // Clear the packet.
    packet.clear();

    // Create latest result.
    var ret = new PhotonPipelineResult();

    // Populate packet and create result.
    packet.setData(rawBytesEntry.getRaw(new byte[]{}));
    if(packet.size < 1) return ret;
    ret.createFromPacket(packet);

    // Return result.
    return ret;
  }

  /**
   * Returns whether the camera is in driver mode.
   * @return Whether the camera is in driver mode.
   */
  public boolean getDriverMode() {
    return driverMode;
  }

  /**
   * Toggles driver mode.
   * @param driverMode Whether to set driver mode.
   */
  public void setDriverMode(boolean driverMode) {
    if (this.driverMode != driverMode) {
      this.driverMode = driverMode;
      driverModeEntry.setBoolean(this.driverMode);
    }
  }

  /**
   * Returns the active pipeline index.
   *
   * @return The active pipeline index.
   */
  public int getPipelineIndex() {
    return pipelineIndex;
  }

  /**
   * Allows the user to select the active pipeline index.
   *
   * @param index The active pipeline index.
   */
  public void setPipelineIndex(int index) {
    if (pipelineIndex != index) {
      pipelineIndex = index;
      pipelineIndexEntry.setNumber(pipelineIndex);
    }
  }

  /**
   * Returns the current LED mode.
   *
   * @return The current LED mode.
   */
  public LEDMode getLEDMode() {
    int value = ledModeEntry.getNumber(-1).intValue();
    switch (value) {
      case 0:
        mode = LEDMode.kOff;
        break;
      case 1:
        mode = LEDMode.kOn;
        break;
      case 2:
        mode = LEDMode.kBlink;
        break;
      case -1:
      default:
        mode = LEDMode.kDefault;
        break;
    }
    return mode;
  }

  /**
   * Sets the LED mode.
   *
   * @param led The mode to set to.
   */
  public void setLED(LEDMode led) {
    if (led != mode) {
      ledModeEntry.setNumber(led.value);
    }
  }

  /**
   * Returns whether the latest target result has targets.
   *
   * @return Whether the latest target result has targets.
   */
  public boolean hasTargets() {
    return getLatestResult().hasTargets();
  }

  /**
   * Returns the pitch of the best target. The best target is defined in the
   * PhotonVision UI.
   *
   * @return The pitch of the best target.
   */
  public double getBestTargetPitch() {
    return getLatestResult().getBestTarget().getPitch();
  }

  /**
   * Returns the yaw of the best target. The best target is defined in the
   * PhotonVision UI.
   *
   * @return The yaw of the best target.
   */
  public double getBestTargetYaw() {
    return getLatestResult().getBestTarget().getYaw();
  }

  /**
   * Returns the area of the best target (0-100). The best target is defined in
   * the PhotonVision UI.
   *
   * @return The area of the best target.
   */
  public double getBestTargetArea() {
    return getLatestResult().getBestTarget().getArea();
  }

  /**
   * Returns the skew of the best target (counter-clockwise positive). The best
   * target is defined in the PhotonVision UI.
   *
   * @return The skew of the best target.
   */
  public double getBestTargetSkew() {
    return getLatestResult().getBestTarget().getSkew();
  }
}
