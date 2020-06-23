package fi.tuska.util;

import java.util.SortedSet;
import java.util.TreeSet;

import junit.framework.TestCase;

public class CollectionUtilsTest extends TestCase {

    public void testFloorEntry() {
        SortedSet<Integer> set = new TreeSet<Integer>();
        set.add(20);
        set.add(5);
        set.add(10);
        set.add(40);

        assertEquals(null, CollectionUtils.floorEntry(set, 4));
        assertEquals(5, CollectionUtils.floorEntry(set, 5).intValue());
        assertEquals(5, CollectionUtils.floorEntry(set, 6).intValue());
        assertEquals(5, CollectionUtils.floorEntry(set, 9).intValue());
        assertEquals(10, CollectionUtils.floorEntry(set, 10).intValue());
        assertEquals(10, CollectionUtils.floorEntry(set, 11).intValue());
        assertEquals(10, CollectionUtils.floorEntry(set, 19).intValue());
        assertEquals(20, CollectionUtils.floorEntry(set, 20).intValue());
        assertEquals(20, CollectionUtils.floorEntry(set, 21).intValue());
        assertEquals(20, CollectionUtils.floorEntry(set, 30).intValue());
        assertEquals(40, CollectionUtils.floorEntry(set, 40).intValue());
        assertEquals(40, CollectionUtils.floorEntry(set, 1040).intValue());
    }

}
