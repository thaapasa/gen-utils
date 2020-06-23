package fi.tuska.util.media.gui;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.image.BufferedImage;

import javax.swing.JComponent;

/**
 * An abstract base class that implements an image-based buffer. Also supports
 * resizing the image.
 * 
 * @author thaapasa
 */
public abstract class BufferedCanvas extends JComponent {

    private static final long serialVersionUID = -3644432111199580002L;
    private static final Dimension DEF_MAX_SIZE = new Dimension(9000, 9000);
    private static final Color DEFAULT_BG_COLOR = Color.WHITE;

    private BufferedImage buffer;
    private Color bgColor = DEFAULT_BG_COLOR;

    public BufferedCanvas(int width, int height, boolean resizable) {
        Dimension size = new Dimension(width, height);
        setSize(size);
        setPreferredSize(size);

        if (resizable) {
            setMaximumSize(DEF_MAX_SIZE);
        } else {
            setMaximumSize(size);
            setMinimumSize(size);
        }
    }

    /**
     * Implement this to draw the actual contents. This will be called from
     * the update() function, which is called whenever needed (manually, or
     * when resizing the image).
     * 
     * @param g the buffer graphics object
     * @param width the current width of the canvas and buffer
     * @param height the current height of the canvas and buffer
     */
    protected abstract void drawBuffer(Graphics g, int width, int height);

    public void update() {
        updateBuffer();
        repaint();
    }

    public synchronized void updateBuffer() {
        int width = getWidth();
        int height = getHeight();

        buffer = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
        assert buffer != null;

        Graphics g = buffer.getGraphics();
        g.setColor(bgColor);
        g.fillRect(0, 0, width, height);

        drawBuffer(g, width, height);
    }

    private void checkBuffer() {
        if (buffer == null || buffer.getHeight() != getHeight()
            || buffer.getWidth() != getWidth()) {
            updateBuffer();
        }
    }

    @Override
    public void update(Graphics g) {
        checkBuffer();
        assert buffer != null;
        g.drawImage(buffer, 0, 0, this);
    }

    @Override
    public void paint(Graphics g) {
        update(g);
    }

}
