package fi.tuska.util;

import junit.framework.TestCase;

public class FixableLeakingMapTest extends TestCase {

    public void testPageFix() {
        FixableLeakingMap<Integer, Integer> map = new FixableLeakingMap<Integer, Integer>(5);

        map.put(1, 101);
        map.put(2, 102);
        map.put(3, 103);
        map.put(4, 104);
        map.put(5, 105);

        try {
            map.put(6, 106);
            fail("No exception thrown even though buffer is full of fixed pages");
        } catch (IndexOutOfBoundsException e) {
            // OK
        }

        map.unfix(1);
        map.put(6, 106);

        assertEquals(102, map.get(2).intValue());
        assertEquals(103, map.get(3).intValue());
        assertEquals(104, map.get(4).intValue());
        assertEquals(105, map.get(5).intValue());
        assertEquals(106, map.get(6).intValue());
        assertEquals(null, map.get(1));
    }

}
