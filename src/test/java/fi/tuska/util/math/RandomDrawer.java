package fi.tuska.util.math;

import java.awt.Color;
import java.awt.Graphics;
import java.util.Random;

import fi.tuska.util.media.gui.BufferedCanvas;

public class RandomDrawer extends BufferedCanvas {

    private static final long serialVersionUID = 1315746099780277158L;

    private final Random random;

    public RandomDrawer(int w, int h, Random random) {
        super(w, h, true);
        this.random = random;
    }

    @Override
    protected void drawBuffer(Graphics g, int width, int height) {
        int amount = width * 100;
        int[] ams = new int[width];
        int max = 0;
        for (int i = 0; i < amount; i++) {
            int x = random.nextInt(width);
            // int x = (int) ((double) random.nextInt() * width /
            // Integer.MAX_VALUE);
            // int x = (int) (random.nextFloat() * width);
            // int x = (int) (random.nextDouble() * width);
            // int x = (int) ((double) random.nextLong() * width /
            // Long.MAX_VALUE);
            assert x < width;
            assert x >= 0;
            if (++ams[x] > max) {
                max = ams[x];
            }
        }

        g.setColor(Color.BLACK);
        int topPad = 10;
        int botPad = 10;
        int hmax = height - topPad - botPad;

        for (int x = 1; x < width; x++) {
            int y1 = height - botPad - (ams[x - 1] * hmax / max);
            int y2 = height - botPad - (ams[x] * hmax / max);
            assert y1 <= hmax;
            assert y2 <= hmax;

            g.drawLine(x - 1, y1, x, y2);
        }
    }

}
