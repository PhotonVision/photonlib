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

package org.photonvision.common;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Objects;

/**
 * Represents a pipeline result from a PhotonCamera.
 */
public class SimplePipelineResult {
  // Targets to store.
  public final List<SimpleTrackedTarget> targets = new ArrayList<>();

  // Latency in milliseconds.
  private double latencyMillis;

  // Whether targets exist.
  private boolean hasTargets;

  /**
   * Constructs an empty pipeline result.
   */
  public SimplePipelineResult() {
  }

  /**
   * Constructs a pipeline result.
   *
   * @param latencyMillis The latency in the pipeline.
   * @param targets       The list of targets identified by the pipeline.
   */
  public SimplePipelineResult(double latencyMillis,
                              List<SimpleTrackedTarget> targets) {
    this.latencyMillis = latencyMillis;
    this.hasTargets = targets.size() != 0;
    this.targets.addAll(targets);
  }

  /**
   * Returns the size of the packet needed to store this pipeline result.
   * @return The size of the packet needed to store this pipeline result.
   */
  int getPacketSize() {
    return targets.size() * SimpleTrackedTarget.PACK_SIZE_BYTES + 8 + 2;
  }

  /**
   * Returns the latency in the pipeline.
   *
   * @return The latency in the pipeline.
   */
  public double getLatencyMillis() {
    return latencyMillis;
  }

  /**
   * Returns whether the pipeline has targets.
   *
   * @return Whether the pipeline has targets.
   */
  public boolean hasTargets() {
    return hasTargets;
  }

  /**
   * Returns a copy of the vector of targets.
   *
   * @return A copy of the vector of targets.
   */
  public List<SimpleTrackedTarget> getTargets() {
    List<SimpleTrackedTarget> r = new ArrayList<>(targets.size());
    Collections.copy(r, targets);
    return r;
  }

  @Override
  public boolean equals(Object o) {
    if (this == o) return true;
    if (o == null || getClass() != o.getClass()) return false;
    SimplePipelineResult that = (SimplePipelineResult) o;
    boolean latencyMatch = Double.compare(that.latencyMillis, latencyMillis) == 0;
    boolean hasTargetsMatch = that.hasTargets == hasTargets;
    boolean targetsMatch = that.targets.equals(targets);
    return latencyMatch && hasTargetsMatch && targetsMatch;
  }

  @Override
  public int hashCode() {
    return Objects.hash(latencyMillis, hasTargets, targets);
  }

  /**
   * Populates the fields of the pipeline result from the packet.
   *
   * @param packet The incoming packet.
   * @return The incoming packet.
   */
  public Packet createFromPacket(Packet packet) {
    // Decode latency, existence of targets, and number of targets.
    latencyMillis = packet.decodeDouble();
    hasTargets = packet.decodeBoolean();
    byte targetCount = packet.decodeByte();

    targets.clear();

    // Decode the information of each target.
    for (int i = 0; i < (int) targetCount; ++i) {
      var target = new SimpleTrackedTarget();
      target.createFromPacket(packet);
      targets.add(target);
    }

    return packet;
  }

  /**
   * Populates the outgoing packet with information from this pipeline result.
   *
   * @param packet The outgoing packet.
   * @return The outgoing packet.
   */
  public Packet populatePacket(Packet packet) {
    // Encode latency, existence of targets, and number of targets.
    packet.encode(latencyMillis);
    packet.encode(hasTargets);
    packet.encode((byte) targets.size());

    // Encode the information of each target.
    for (var target : targets) target.populatePacket(packet);

    // Return the packet.
    return packet;
  }
}
