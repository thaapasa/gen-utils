package fi.tuska.util.media.gui;

import java.awt.Dimension;

import javax.swing.JComponent;

public class FixedSizeComponent extends JComponent {

    private static final long serialVersionUID = -4477250572718125272L;

    public FixedSizeComponent(int width, int height) {
        Dimension size = new Dimension(width, height);
        setSize(size);
        setMinimumSize(size);
        setMaximumSize(size);
        setPreferredSize(size);
    }

}
