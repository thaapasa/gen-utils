package fi.tuska.util;

import java.util.Iterator;

/**
 * An iterable collection of integer ranges. Can be used, for example, to
 * create loops:
 * 
 * for (int i : new IntegerRangeProvider(new IngegerRange(1, 10), new
 * IntegerRange(10, 5), new IntegerRange(50, 100)) { doSomething(); }
 * 
 * 
 * @author thaapasa
 */
public class IntegerRangeProvider implements Iterable<Integer>, Iterator<Integer> {

    private IntegerRange[] ranges;
    private int curRange;

    public IntegerRangeProvider(IntegerRange... ranges) {
        this.ranges = ranges;
        this.curRange = 0;
    }

    @Override
    public Iterator<Integer> iterator() {
        return this;
    }

    @Override
    public boolean hasNext() {
        if (curRange >= ranges.length) {
            return false;
        }
        if (ranges[curRange].hasNext()) {
            return true;
        }
        curRange++;
        return curRange < ranges.length;
    }

    @Override
    public Integer next() {
        if (curRange >= ranges.length) {
            throw new IllegalStateException("Out of ranges");
        }
        if (!ranges[curRange].hasNext()) {
            curRange++;
        }
        if (curRange >= ranges.length) {
            throw new IllegalStateException("Out of ranges");
        }
        return ranges[curRange].next();
    }

    @Override
    public void remove() {
        throw new UnsupportedOperationException("remove() not supported");
    }

}
