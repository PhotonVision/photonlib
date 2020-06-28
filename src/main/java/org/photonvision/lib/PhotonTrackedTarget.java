package org.photonvision.lib;

import edu.wpi.first.wpilibj.geometry.Pose2d;

import java.util.List;

public class PhotonTrackedTarget {
  final double pitch, yaw, skew, area;
  final Pose2d robotRelativePose;
  final List<Double> rawTvec, rawRvec;

  public PhotonTrackedTarget(double pitch, double yaw, double skew, double area, Pose2d robotRelativePose, List<Double> rawTvec, List<Double> rawRvec) {
    this.pitch = pitch;
    this.yaw = yaw;
    this.skew = skew;
    this.area = area;
    this.robotRelativePose = robotRelativePose;
    this.rawTvec = rawTvec;
    this.rawRvec = rawRvec;
  }
}
