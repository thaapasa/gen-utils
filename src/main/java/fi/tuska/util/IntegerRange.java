package fi.tuska.util;

import java.util.Iterator;

/**
 * An iterable range of integers. Can be used, for example, to create loops:
 * 
 * for (int i : new IntegerRange(5, 10)) doSomething();
 * 
 * Can also count backwards.
 * 
 * @author thaapasa
 */
public class IntegerRange implements Iterable<Integer>, Iterator<Integer> {

    private int pos;
    private boolean ascending;
    private int start;
    private int end;
    private boolean stopped;

    public IntegerRange(int start, int end) {
        this.ascending = start <= end;
        this.start = start;
        this.end = end;
        // pos is always the "next value" to return
        this.pos = start;
    }

    /**
     * Note that some ranges cannot fit in integers (for example, range -100
     * to Integer.MAX_VALUE contains Integer.MAX_VALUE + 101 entries; this
     * value cannot be expressed as an integer.
     * 
     * @return the amount of integers contained by this range
     */
    public long getCount() {
        return ascending ? (long) end - (long) start + 1L : (long) start - (long) end + 1;
    }

    @Override
    public boolean hasNext() {
        return !stopped;
    }

    @Override
    public Integer next() {
        if (stopped) {
            throw new IllegalStateException("Out of range");
        }
        Integer toReturn = new Integer(pos);
        if (pos == end) {
            stopped = true;
        } else {
            if (ascending) {
                pos++;
            } else {
                pos--;
            }
        }
        return toReturn;
    }

    @Override
    public void remove() {
        throw new UnsupportedOperationException("remove() not supported");
    }

    @Override
    public Iterator<Integer> iterator() {
        return this;
    }

}
