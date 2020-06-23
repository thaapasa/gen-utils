package fi.tuska.util;

import java.awt.geom.Rectangle2D;

public class Range {

    private double min;
    private double max;

    public enum Axis {
        X_AXIS, Y_AXIS
    }

    public Range(Rectangle2D rect, Axis axis) {
        if (axis == Axis.X_AXIS) {
            set(rect.getMinX(), rect.getMaxX());
        } else {
            assert axis == Axis.Y_AXIS;
            set(rect.getMinY(), rect.getMaxY());
        }
    }

    public Range(double min, double max) {
        set(min, max);
    }

    public double getMin() {
        return min;
    }

    public double getMax() {
        return max;
    }

    public double getRange() {
        return max - min;
    }

    public double random() {
        double val = Math.random() * (max - min) + min;
        assert val < max;
        assert val >= min;
        return val;
    }

    public int randomInt() {
        int val = (int) Math.floor(random());
        assert (double) val < max;
        assert (double) val >= min;
        return val;
    }

    public void set(double min, double max) {
        if (min <= max) {
            this.min = min;
            this.max = max;
        } else {
            this.min = max;
            this.max = min;
        }
    }

    public Range scaleTo(Range extents, Range target) {
        double totalRange = extents.getRange();
        double targetRange = target.getRange();

        double start = ((min - extents.getMin()) * targetRange) / totalRange;
        double len = getRange() * targetRange / totalRange;

        double startNew = start + target.getMin();
        return new Range(startNew, startNew + len);
    }

    @Override
    public int hashCode() {
        return new Double(min).hashCode() + new Double(max).hashCode();
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof Range)) {
            return false;
        }
        Range r = (Range) o;
        return min == r.min && max == r.max;
    }

    public String toString() {
        return String.format("[%.2f; %.2f]", min, max);
    }

}
