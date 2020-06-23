package fi.tuska.util;

import junit.framework.TestCase;

public class RangeTest extends TestCase {

    public void testScaleTo() {
        assertEquals(new Range(50, 80), new Range(5, 8).scaleTo(new Range(0, 10), new Range(0,
            100)));
        assertEquals(new Range(60, 90), new Range(5, 8).scaleTo(new Range(0, 10), new Range(10,
            110)));

        assertEquals(new Range(20, 50), new Range(5, 8).scaleTo(new Range(3, 13), new Range(0,
            100)));
        assertEquals(new Range(30, 60), new Range(5, 8).scaleTo(new Range(3, 13), new Range(10,
            110)));
    }

}
