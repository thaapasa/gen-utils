package fi.tuska.util.math;

import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.JFrame;

import fi.tuska.util.media.gui.FrameUtils;

public class UniformRandomTest {

    public static void main(String[] args) {
        final RandomDrawer d = new RandomDrawer(500, 500, new UniformRandom());
        JFrame frame = FrameUtils.createWindow(d, "Uniform random test");
        frame.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyChar() == KeyEvent.VK_ESCAPE || e.getKeyChar() == ' ') {
                    d.update();
                }
            }
        });
    }

}
