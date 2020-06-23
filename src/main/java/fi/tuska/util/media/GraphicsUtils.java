package fi.tuska.util.media;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.Stroke;
import java.awt.geom.Rectangle2D;

public class GraphicsUtils {

    private GraphicsUtils() {
        // Private constructor to prevent instantiation
    }

    private static final Stroke DASHED_STROKE = new BasicStroke(0, BasicStroke.CAP_BUTT,
        BasicStroke.JOIN_MITER, 1, new float[] { 3 }, 0);

    public static void drawRect(Rectangle2D coords, double targetMaxY, Color color, Graphics g) {
        if (color != null) {
            g.setColor(color);
        }
        g.drawRect((int) coords.getMinX(), (int) (targetMaxY - coords.getMaxY()), (int) coords
            .getWidth(), (int) coords.getHeight());
    }

    public static void fillRect(Rectangle2D coords, double targetMaxY, Color color, Graphics g) {
        if (color != null) {
            g.setColor(color);
        }
        g.fillRect((int) coords.getMinX(), (int) (targetMaxY - coords.getMaxY()), (int) coords
            .getWidth(), (int) coords.getHeight());
    }

    public static void dashedRect(Rectangle2D coords, double targetMaxY, Color color, Graphics g) {
        if (color != null) {
            g.setColor(color);
        }

        Graphics2D g2 = (Graphics2D) g;
        Stroke curStroke = g2.getStroke();
        g2.setStroke(DASHED_STROKE);
        g2.draw(flipY(coords, targetMaxY));
        g2.setStroke(curStroke);
    }

    public static Rectangle2D flipY(Rectangle2D original, double maxY) {
        return new Rectangle((int) original.getMinX(), (int) (maxY - original.getMaxY()),
            (int) original.getWidth(), (int) original.getHeight());
    }
}
