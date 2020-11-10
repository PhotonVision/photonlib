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

#pragma once

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>

#include <memory>
#include <string>

#include "photonlib/PhotonPipelineResult.h"

namespace photonlib {

enum LEDMode : int { kDefault = -1, kOff = 0, kOn = 1, kBlink = 2 };

/**
 * Represents a camera that is connected to PhotonVision.ß
 */
class PhotonCamera {
 public:
  /**
   * Constructs a PhotonCamera from a root table.
   * @param rootTable The root table that the camera is broadcasting information
   * over.
   */
  explicit PhotonCamera(std::shared_ptr<nt::NetworkTable> rootTable);

  /**
   * Constructs a PhotonCamera from the name of the camera.
   * @param cameraName The nickname of the camera (found in the PhotonVision
   * UI).
   */
  explicit PhotonCamera(const std::string& cameraName);

  /**
   * Returns the latest pipeline result.
   * @return The latest pipeline result.
   */
  PhotonPipelineResult GetLatestResult() const;

  /**
   * Toggles driver mode.
   * @param driverMode Whether to set driver mode.
   */
  void SetDriverMode(bool driverMode);

  /**
   * Returns whether the camera is in driver mode.
   * @return Whether the camera is in driver mode.
   */
  bool GetDriverMode() const;

  /**
   * Request the camera to save a new image file from the input
   * camera stream with overlays.
   * Images take up space in the filesystem of the PhotonCamera.
   * Calling it frequently will fill up disk space and eventually
   * cause the system to stop working.
   * Clear out images in /opt/photonvision/photonvision_config/imgSaves
   * frequently to prevent issues.
   */
  void TakeInputSnapshot(void);

  /**
   * Request the camera to save a new image file from the output
   * stream with overlays.
   * Images take up space in the filesystem of the PhotonCamera.
   * Calling it frequently will fill up disk space and eventually
   * cause the system to stop working.
   * Clear out images in /opt/photonvision/photonvision_config/imgSaves
   * frequently to prevent issues.
   */
  void TakeOutputSnapshot(void);

  /**
   * Allows the user to select the active pipeline index.
   * @param index The active pipeline index.
   */
  void SetPipelineIndex(int index);

  /**
   * Returns the active pipeline index.
   * @return The active pipeline index.
   */
  int GetPipelineIndex() const;

  /**
   * Returns the current LED mode.
   * @return The current LED mode.
   */
  LEDMode GetLEDMode() const;

  /**
   * Sets the LED mode.
   * @param led The mode to set to.
   */
  void SetLEDMode(LEDMode led);

  /**
   * Returns whether the latest target result has targets.
   * @return Whether the latest target result has targets.
   */
  bool HasTargets() const { return GetLatestResult().HasTargets(); }

  /**
   * Returns the pitch of the best target. The best target is defined in the
   * PhotonVision UI.
   * @return The pitch of the best target.
   */
  double GetBestTargetPitch() const {
    return GetLatestResult().GetBestTarget().GetPitch();
  }

  /**
   * Returns the yaw of the best target. The best target is defined in the
   * PhotonVision UI.
   * @return The yaw of the best target.
   */
  double GetBestTargetYaw() const {
    return GetLatestResult().GetBestTarget().GetYaw();
  }

  /**
   * Returns the area of the best target (0-100). The best target is defined in
   * the PhotonVision UI.
   * @return The area of the best target.
   */
  double GetBestTargetArea() const {
    return GetLatestResult().GetBestTarget().GetArea();
  }

  /**
   * Returns the skew of the best target (counter-clockwise positive). The best
   * target is defined in the PhotonVision UI.
   * @return The skew of the best target.
   */
  double GetBestTargetSkew() const {
    return GetLatestResult().GetBestTarget().GetSkew();
  }

 private:
  nt::NetworkTableEntry rawBytesEntry;
  nt::NetworkTableEntry driverModeEntry;
  nt::NetworkTableEntry inputSaveImgEntry;
  nt::NetworkTableEntry outputSaveImgEntry;
  nt::NetworkTableEntry pipelineIndexEntry;
  nt::NetworkTableEntry ledModeEntry;

  std::shared_ptr<nt::NetworkTable> mainTable =
      nt::NetworkTableInstance::GetDefault().GetTable("photonvision");

  mutable Packet packet;

  bool driverMode;
  double pipelineIndex;
  mutable LEDMode mode;
};

}  // namespace photonlib
