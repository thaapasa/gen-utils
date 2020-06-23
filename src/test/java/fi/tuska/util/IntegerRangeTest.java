package fi.tuska.util;

import org.apache.log4j.Logger;

import junit.framework.TestCase;

public class IntegerRangeTest extends TestCase {

    private static final Logger log = Logger.getLogger(IntegerRangeTest.class);

    public void testIntRangeTest() {
        IntegerRange range = new IntegerRange(100, 1000);
        assertEquals(901, range.getCount());

        range = new IntegerRange(Integer.MIN_VALUE, Integer.MAX_VALUE);
        assertEquals((long) Integer.MAX_VALUE - (long) Integer.MIN_VALUE + 1L, range.getCount());

        range = new IntegerRange(Integer.MAX_VALUE, Integer.MIN_VALUE);
        assertEquals((long) Integer.MAX_VALUE - (long) Integer.MIN_VALUE + 1L, range.getCount());
    }

    private void testAscRange(int start, int end) {
        log.debug(String.format("Testing ascending range from %d to %d", start, end));
        assertTrue(start <= end);
        IntegerRange range = new IntegerRange(start, end);

        for (long i = start; i <= end; i++) {
            // log.debug("Range should contain " + i);
            assertEquals(end - start + 1, range.getCount());
            assertTrue(range.hasNext());
            assertEquals(i, range.next().intValue());
        }
        assertFalse(range.hasNext());
        try {
            range.next();
            fail("No exception thrown");
        } catch (Exception e) {
            // OK, range.next() fails after range has ended
        }
    }

    private void testDescRange(int start, int end) {
        log.debug(String.format("Testing descending range from %d to %d", start, end));
        assertTrue(start >= end);
        IntegerRange range = new IntegerRange(start, end);

        for (long i = start; i >= end; i--) {
            // log.debug("Range should contain " + i);
            assertEquals(start - end + 1, range.getCount());
            assertTrue(range.hasNext());
            assertEquals(i, range.next().intValue());
        }
        assertFalse(range.hasNext());
        try {
            range.next();
            fail("No exception thrown");
        } catch (Exception e) {
            // OK, range.next() fails after range has ended
        }
    }

    public void testRange() {
        testAscRange(-4, 6);
        testAscRange(10, 100);
        testAscRange(Integer.MAX_VALUE - 20, Integer.MAX_VALUE);
        testAscRange(Integer.MIN_VALUE, Integer.MIN_VALUE + 20);

        testDescRange(6, -4);
        testDescRange(100, 10);
        testDescRange(Integer.MIN_VALUE + 20, Integer.MIN_VALUE);
        testDescRange(Integer.MAX_VALUE, Integer.MAX_VALUE - 20);
    }

}
