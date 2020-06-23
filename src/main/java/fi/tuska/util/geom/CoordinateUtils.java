package fi.tuska.util.geom;

import java.awt.Rectangle;
import java.awt.geom.Rectangle2D;

import fi.tuska.util.Range;
import fi.tuska.util.Range.Axis;

public class CoordinateUtils {

    private CoordinateUtils() {
        // Private constructor to prevent instantiation
    }

    /**
     * @param object the object to show, in same coordinate system as extents
     * @param extents the extents to show in target
     * @param target the target coordinate system, in which to show the object
     */
    public static Rectangle2D scaleRect(Rectangle2D object, Rectangle2D extents,
        Rectangle2D target) {
        Range xRange = new Range(object, Axis.X_AXIS).scaleTo(new Range(extents, Axis.X_AXIS),
            new Range(target, Axis.X_AXIS));
        Range yRange = new Range(object, Axis.Y_AXIS).scaleTo(new Range(extents, Axis.Y_AXIS),
            new Range(target, Axis.Y_AXIS));
        return new Rectangle((int) xRange.getMin(), (int) yRange.getMin(), (int) xRange
            .getRange(), (int) yRange.getRange());
    }

    public static Rectangle2D clipRect(Rectangle2D object, Rectangle2D bounds) {
        if (bounds.contains(object)) {
            return object;
        }
        double x1 = clipToRange(object.getMinX(), bounds.getMinX(), bounds.getMaxX());
        double x2 = clipToRange(object.getMaxX(), bounds.getMinX(), bounds.getMaxX());
        double y1 = clipToRange(object.getMinY(), bounds.getMinY(), bounds.getMaxY());
        double y2 = clipToRange(object.getMaxY(), bounds.getMinY(), bounds.getMaxY());
        return new Rectangle((int) x1, (int) y1, (int) (x2 - x1), (int) (y2 - y1));
    }

    public static Rectangle2D extendRect(Rectangle2D object, Rectangle2D min) {
        if (object.contains(min)) {
            return object;
        }
        double x1 = Math.min(object.getMinX(), min.getMinX());
        double x2 = Math.max(object.getMaxX(), min.getMaxX());
        double y1 = Math.min(object.getMinY(), min.getMinY());
        double y2 = Math.max(object.getMaxY(), min.getMaxY());
        return new Rectangle((int) x1, (int) y1, (int) (x2 - x1), (int) (y2 - y1));
    }

    public static double clipToRange(double value, double min, double max) {
        return Math.min(Math.max(value, min), max);
    }

}
