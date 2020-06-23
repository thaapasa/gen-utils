package fi.tuska.util.media.gui;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.JFrame;

public class FrameUtils {

    public static JFrame createWindow(Component component, String title) {
        JFrame frame = new JFrame(title);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().setLayout(new BorderLayout());
        frame.getContentPane().add(component);

        frame.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyChar() == KeyEvent.VK_ESCAPE || e.getKeyChar() == 'q'
                    || e.getKeyChar() == 'Q') {
                    System.exit(0);
                }
            }

        });
        frame.pack();
        frame.setVisible(true);

        return frame;
    }

}
