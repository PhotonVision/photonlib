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

/**
 * Copyright (C) 2020 Photon Vision.
 * <p>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * <p>
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * <p>
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package org.photonvision.common;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Rotation2d;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

class BytePackableTest {
  @Test
  void testSimpleTrackedTarget() {
    var target = new SimpleTrackedTarget(3.0, 4.0, 9.0, -5.0,
        new Pose2d(1, 2, new Rotation2d(1.5)));
    byte[] packed = target.toByteArray();

    System.out.println(Arrays.toString(packed));

    var b = new SimpleTrackedTarget();
    b.fromByteArray(packed);

    byte[] bytes = new byte[Double.BYTES];
    target.resetBufferPosition();
    target.bufferData(4.0, bytes);

    Assertions.assertEquals(target, b);
  }

  @Test
  void testSimplePipelineResult() {
    var result = new SimplePipelineResult(1, false,
        new ArrayList<>());
    byte[] packed = result.toByteArray();

    var b = new SimplePipelineResult();
    b.fromByteArray(packed);

    Assertions.assertEquals(result, b);

    var result2 = new SimplePipelineResult(2, true,
        List.of(
            new SimpleTrackedTarget(3.0, -4.0, 9.0, 4.0,
                new Pose2d(1, 2, new Rotation2d(1.5))),
            new SimpleTrackedTarget(3.0, -4.0, 9.1, 6.7,
                new Pose2d(1, 5, new Rotation2d(1.5)))));
    byte[] packed2 = result2.toByteArray();

    var b2 = new SimplePipelineResult();
    b2.fromByteArray(packed2);

    Assertions.assertEquals(result2, b2);
  }

  @Test
  void testBytePackFromCpp() {
    byte[] bytePack = {64, 8, 0, 0, 0, 0, 0, 0, 64, 16, 0, 0, 0, 0, 0, 0, 64, 34, 0, 0, 0, 0, 0, 0,
        -64, 20, 0, 0, 0, 0, 0, 0, 63, -16, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 64, 85, 124,
        101, 19, -54, -47, 122};
    var t = new SimpleTrackedTarget();
    t.fromByteArray(bytePack);

    var target = new SimpleTrackedTarget(3.0, 4.0, 9.0, -5.0,
        new Pose2d(1, 2, new Rotation2d(1.5)));

    Assertions.assertEquals(t, target);
  }
}
