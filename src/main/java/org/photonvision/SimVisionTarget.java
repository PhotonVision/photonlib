package org.photonvision;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpiutil.math.Pair;

public class SimVisionTarget {

    Pose2d targetPos;
    double targetHeight_m;
    Pair<Double, Double> targetDims_m;
    double targetInfill_pct;
    double tgtArea_m2;

    /**
     * Describes a vision target located somewhere on the field that your SimVisionSystem should detect.
     * 
     * @param targetPos Pose2d of the target on the field. Define it such that, if you are standing on the middle of the field facing the target, the Y axis points to your left, and the X axis points away from you.
     * @param targetHeight_m Height of the target above the field plane, in meters.
     * @param targetDims_m Pair of floating point values describing the width and height of the outter bounding box of the target. Both are in meters.
     */
    public SimVisionTarget(Pose2d targetPos, double targetHeight_m, Pair<Double, Double> targetDims_m){
        this.targetPos = targetPos;
        this.targetHeight_m = targetHeight_m;
        this.targetDims_m = targetDims_m;
        this.tgtArea_m2 = this.targetDims_m.getFirst() * this.targetDims_m.getSecond();
    }

}
