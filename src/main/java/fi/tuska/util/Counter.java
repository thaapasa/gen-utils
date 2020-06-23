package fi.tuska.util;

/**
 * A simple counter class. Typical usage scenario: used from callbacks that
 * cannot interact with primitive types.
 * 
 * @author thaapasa
 */
public class Counter {

    private long count;

    public Counter() {
        count = 0;
    }

    public long getCount() {
        return count;
    }

    public void advance() {
        count++;
    }

    public void advance(long amount) {
        count += amount;
    }

    public void reset() {
        count = 0;
    }

    public void set(long value) {
        count = value;
    }

    public String toString() {
        return "Counter: " + count;
    }

}
