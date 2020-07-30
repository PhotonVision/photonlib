package org.photonvision;

import edu.wpi.first.networktables.NetworkTablesJNI;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.function.Executable;
;

import java.io.IOException;

class PhotonCameraTest {
    @Test
    public void testEmpty() {
        Assertions.assertDoesNotThrow(() -> {
            var packet = new Packet(1);
            var ret = new PhotonPipelineResult();
            packet.setData(new byte[0]);
            if(packet.size < 1) {
                return;
            }
            ret.createFromPacket(packet);
        });
    }
}