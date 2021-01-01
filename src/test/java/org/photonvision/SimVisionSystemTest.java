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

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Transform2d;


class SimVisionSystemTest {
    @Test
    public void testEmpty() {
        Assertions.assertDoesNotThrow(() -> {
            var sysUnderTest = new SimVisionSystem("Test", 80.0, 0.0, new Transform2d(), 1, 99999, 320, 240, 0);
            sysUnderTest.addSimVisionTarget(new SimVisionTarget(new Pose2d(), 0.0, 1.0, 1.0));
            for(int loopIdx = 0; loopIdx < 100; loopIdx++){
                sysUnderTest.processFrame(new Pose2d());
            }
        });

    }
}
