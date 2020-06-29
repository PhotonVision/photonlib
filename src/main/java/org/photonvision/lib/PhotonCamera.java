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

  public PhotonCamera(NetworkTable rootTable) {
    rawBytesEntry = rootTable.getEntry("rawBytes");
  }

  public PhotonCamera(String tableName) {
    this(NetworkTableInstance.getDefault().getTable(tableName));
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
