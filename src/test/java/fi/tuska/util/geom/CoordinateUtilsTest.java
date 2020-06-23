package fi.tuska.util.geom;

import java.awt.Rectangle;
import java.awt.geom.Rectangle2D;

import junit.framework.TestCase;

public class CoordinateUtilsTest extends TestCase {

    public void testScaleRect() {
        Rectangle2D extents = new Rectangle(0, 0, 20, 10);
        Rectangle2D target = new Rectangle(0, 0, 100, 200);

        assertEquals(new Rectangle(10, 40, 10, 80), CoordinateUtils.scaleRect(new Rectangle(2, 2,
            2, 4), extents, target));
    }

    public void testClipRect() {
        Rectangle2D bounds = new Rectangle(0, 0, 10, 10);
        assertEquals(new Rectangle(5, 5, 3, 3), CoordinateUtils.clipRect(
            new Rectangle(5, 5, 3, 3), bounds));

        assertEquals(new Rectangle(5, 5, 5, 5), CoordinateUtils.clipRect(
            new Rectangle(5, 5, 8, 8), bounds));

        assertEquals(new Rectangle(9, 8, 1, 2), CoordinateUtils.clipRect(
            new Rectangle(9, 8, 8, 8), bounds));

        assertEquals(new Rectangle(0, 0, 3, 6), CoordinateUtils.clipRect(new Rectangle(-2, -1, 5,
            7), bounds));
        assertEquals(new Rectangle(10, 10, 0, 0), CoordinateUtils.clipRect(new Rectangle(12, 13,
            5, 8), bounds));
    }

}
