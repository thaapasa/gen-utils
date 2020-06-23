package fi.tuska.util.geom;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.JFrame;

import junit.framework.TestCase;
import fi.tuska.util.media.gui.BufferedCanvas;
import fi.tuska.util.media.gui.FrameUtils;

public class HilbertCurveTest extends TestCase {

    private void checkHilbertValue(long x, long y, long index, int level) {
        // Encode
        assertEquals(x + "," + y + " != " + index + "@" + level, index, HilbertCurve.encode(x, y,
            level));

        // Decode
        long[] res = HilbertCurve.decode(index, level, 2);
        assertEquals(2, res.length);
        checkEquals(x, res[0], x, y, index, level);
        checkEquals(y, res[1], x, y, index, level);
    }

    private void checkEquals(long a, long b, long x, long y, long index, long level) {
        assertEquals(x + "," + y + " != " + index + "@" + level, a, b);
    }

    public void testValues() {
        // Curve of order 1
        checkHilbertValue(0, 0, 0, 1);
        checkHilbertValue(0, 1, 1, 1);
        checkHilbertValue(1, 1, 2, 1);
        checkHilbertValue(1, 0, 3, 1);

        // Curve of order 2
        checkHilbertValue(0, 0, 0, 2);
        checkHilbertValue(1, 0, 1, 2);
        checkHilbertValue(1, 1, 2, 2);
        checkHilbertValue(0, 1, 3, 2);
        checkHilbertValue(0, 2, 4, 2);
        checkHilbertValue(0, 3, 5, 2);
        checkHilbertValue(1, 3, 6, 2);

        checkHilbertValue(3, 2, 11, 2);
        checkHilbertValue(2, 0, 14, 2);
        checkHilbertValue(3, 0, 15, 2);

        // Curve of order 3
        checkHilbertValue(0, 0, 0, 3);
        checkHilbertValue(0, 1, 1, 3);
        checkHilbertValue(1, 1, 2, 3);
        checkHilbertValue(1, 0, 3, 3);
        checkHilbertValue(2, 0, 4, 3);
        checkHilbertValue(3, 0, 5, 3);
        // Last point, 2^(order*2) - 1; x-value = 2^order - 1
        checkHilbertValue(7, 0, 63, 3);

        checkHilbertValue(4, 5, 33, 4);
        printValue(107, 4);
        printValue(206, 4);
        // assertEquals(107, HilbertCurve.encode(6, 8, 4));
        checkHilbertValue(7, 14, 107, 4);

        // Other curves
        for (int order = 1; order < 10; order++) {
            checkHilbertValue(0, 0, 0, order);
            long xVal = (long) Math.pow(2, order) - 1;
            long hOrder = (long) Math.pow(2, (order * 2)) - 1;
            checkHilbertValue(xVal, 0, hOrder, order);
        }
    }

    private void printValue(long index, int order) {
        long[] r = HilbertCurve.decode(index, order, 2);
        System.out.println(r[0] + "," + r[1]);
    }

    public void testEncodeDecode() {
        for (int order = 1; order < 6; order++) {
            long maxV = (1 << order) - 1;

            for (long x = 0; x <= maxV; x++) {
                for (long y = 0; y <= maxV; y++) {
                    long index = HilbertCurve.encode(x, y, order);
                    long[] c = HilbertCurve.decode(index, order, 2);
                    checkEquals(x, c[0], x, y, index, order);
                    checkEquals(y, c[1], x, y, index, order);
                }
            }
        }

    }

    public static void main(String[] args) {
        final HilbertDrawer d = new HilbertDrawer(500, 500);
        d.setOrder(5);

        JFrame frame = FrameUtils.createWindow(d, "Hilbert curve drawer");

        frame.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_UP || e.getKeyCode() == KeyEvent.VK_RIGHT) {
                    d.setOrder(d.getOrder() + 1);
                } else if (e.getKeyCode() == KeyEvent.VK_DOWN
                    || e.getKeyCode() == KeyEvent.VK_LEFT) {
                    d.setOrder(d.getOrder() - 1);
                }
            }

        });
    }

    private static class HilbertDrawer extends BufferedCanvas {

        private static final long serialVersionUID = 5714169148666801822L;
        private static final Color COLOR = Color.BLACK;
        private static final int MARGIN = 10;
        private int order = 1;

        public HilbertDrawer(int width, int height) {
            super(width, height, true);
        }

        public void setOrder(int order) {
            this.order = Math.max(order, 1);
            update();
        }

        public int getOrder() {
            return order;
        }

        @Override
        protected void drawBuffer(Graphics g, int width, int height) {
            g.setColor(COLOR);
            long maxHilbertValue = 1L << (order * 2);
            long maxAxisValue = (1L << order) - 1;
            long[] prev = null;
            for (long i = 0; i < maxHilbertValue; i++) {
                long[] cur = HilbertCurve.decode(i, order, 2);
                // Self-test
                assert HilbertCurve.encode(cur[0], cur[1], order) == i : HilbertCurve.encode(
                    cur[0], cur[1], order)
                    + " != " + i;
                if (prev != null) {
                    drawLine(g, prev, cur, maxAxisValue, width, height);
                }
                prev = cur;
            }
        }

        private void drawLine(Graphics g, long[] p1, long[] p2, long maxValue, int width,
            int height) {
            int uw = width - 2 * MARGIN;
            int uh = height - 2 * MARGIN;
            int x1 = (int) (p1[0] * uw / maxValue) + MARGIN;
            int y1 = height - ((int) (p1[1] * uh / maxValue) + MARGIN);
            int x2 = (int) (p2[0] * uw / maxValue) + MARGIN;
            int y2 = height - ((int) (p2[1] * uh / maxValue) + MARGIN);
            g.drawLine(x1, y1, x2, y2);
        }
    }

}
