package fi.tuska.util;

import java.util.Iterator;

import junit.framework.TestCase;

public class SortedListTest extends TestCase {

    public void testSortedList() {
        SortedList<Integer, String> list = new SortedList<Integer, String>();
        assertEquals(0, list.size());
        assertFalse(list.contains(4));
        assertTrue(list.isEmpty());

        assertNull(list.getFirstKey());
        assertNull(list.getLastKey());

        list.add(4, "kala");
        assertFalse(list.isEmpty());
        assertEquals(1, list.size());
        assertTrue(list.contains(4));
        assertFalse(list.contains(5));
        assertEquals("kala", list.getFirst(4));

        assertEquals(4, list.getFirstKey().intValue());
        assertEquals(4, list.getLastKey().intValue());

        list.add(5, "lintu");
        assertEquals(2, list.size());
        assertTrue(list.contains(4));
        assertTrue(list.contains(5));
        assertEquals("kala", list.getFirst(4));
        assertEquals("lintu", list.getFirst(5));
        assertEquals(CollectionUtils.getList("kala"), list.getAll(4));
        assertEquals(CollectionUtils.getList("lintu"), list.getAll(5));

        list.add(4, "lohi");
        assertEquals(3, list.size());
        assertTrue(list.contains(4));
        assertTrue(list.contains(5));
        assertEquals("kala", list.getFirst(4));
        assertEquals("lintu", list.getFirst(5));
        assertEquals(CollectionUtils.getList("kala", "lohi"), list.getAll(4));
        assertEquals(CollectionUtils.getList("lintu"), list.getAll(5));

        list.add(1, "matelija");
        list.add(1, "lisko");
        list.add(1, "k‰‰rme");

        list.add(5, "lokki");
        list.add(5, "pingviini");

        assertEquals(8, list.size());
        assertTrue(list.contains(1));
        assertFalse(list.contains(2));
        assertFalse(list.contains(3));
        assertTrue(list.contains(4));
        assertTrue(list.contains(5));
        assertEquals(CollectionUtils.getList("kala", "lohi"), list.getAll(4));
        assertEquals(CollectionUtils.getList("lintu", "lokki", "pingviini"), list.getAll(5));

        assertEquals(CollectionUtils.getList("lintu", "lokki", "pingviini"), list.getAllFloor(7)
            .getSecond());
        assertEquals(CollectionUtils.getList("lintu", "lokki", "pingviini"), list.getAllFloor(5)
            .getSecond());
        assertEquals(CollectionUtils.getList("kala", "lohi"), list.getAllFloor(4).getSecond());
        assertEquals(CollectionUtils.getList("k‰‰rme", "lisko", "matelija"), list.getAllFloor(3)
            .getSecond());

        assertEquals(new Pair<Integer, String>(4, "lohi"), list.nextEntry(4, "kala"));
        assertEquals(new Pair<Integer, String>(5, "lintu"), list.nextEntry(4, "lohi"));
        assertEquals(new Pair<Integer, String>(5, "lokki"), list.nextEntry(5, "lintu"));
        assertEquals(new Pair<Integer, String>(5, "pingviini"), list.nextEntry(5, "lokki"));
        assertNull(list.nextEntry(5, "pingviini"));

        assertEquals(new Pair<Integer, String>(5, "lintu"), list.previousEntry(5, "lokki"));
        assertEquals(new Pair<Integer, String>(4, "lohi"), list.previousEntry(5, "lintu"));
        assertEquals(new Pair<Integer, String>(4, "kala"), list.previousEntry(4, "lohi"));
        assertEquals(new Pair<Integer, String>(1, "k‰‰rme"), list.previousEntry(1, "lisko"));
        assertNull(list.previousEntry(1, "k‰‰rme"));

        assertEquals(1, list.getFirstKey().intValue());
        assertEquals(5, list.getLastKey().intValue());

        list.removeFirst(5);
        assertEquals(7, list.size());
        assertTrue(list.contains(5));
        assertEquals(CollectionUtils.getList("lokki", "pingviini"), list.getAll(5));

        list.removeFirst(5);
        assertEquals(6, list.size());
        assertTrue(list.contains(5));
        assertEquals(CollectionUtils.getList("pingviini"), list.getAll(5));

        list.removeFirst(5);
        assertEquals(5, list.size());
        assertFalse(list.contains(5));
        assertEquals(CollectionUtils.getList(), list.getAll(5));

        assertFalse(list.remove(4, "kuha"));
        assertEquals(5, list.size());
        assertTrue(list.remove(4, "lohi"));
        assertEquals(4, list.size());
        assertEquals(CollectionUtils.getList("kala"), list.getAll(4));

        assertTrue(list.remove(4, "kala"));
        assertEquals(3, list.size());
        assertFalse(list.contains(4));
        assertEquals(CollectionUtils.getList(), list.getAll(4));
    }

    private SortedList<Integer, String> getList() {
        SortedList<Integer, String> list = new SortedList<Integer, String>();
        list.add(1, "el‰in");
        list.add(2, "kala");
        list.add(3, "lintu");
        list.add(6, "lohik‰‰rme");
        list.add(1, "kettu");
        list.add(3, "kana");
        list.add(3, "peipponen");
        list.add(1, "susi");
        list.add(2, "lohi");
        return list;
    }

    public void testGetAt() {
        SortedList<Integer, String> list = getList();
        try {
            list.getAt(-1);
            fail();
        } catch (IndexOutOfBoundsException e) {
            // OK
        }
        assertEquals(new Pair<Integer, String>(1, "el‰in"), list.getAt(0));
        assertEquals(new Pair<Integer, String>(1, "kettu"), list.getAt(1));
        assertEquals(new Pair<Integer, String>(1, "susi"), list.getAt(2));
        assertEquals(new Pair<Integer, String>(2, "kala"), list.getAt(3));
        assertEquals(new Pair<Integer, String>(2, "lohi"), list.getAt(4));
        assertEquals(new Pair<Integer, String>(3, "kana"), list.getAt(5));
        assertEquals(new Pair<Integer, String>(3, "lintu"), list.getAt(6));
        assertEquals(new Pair<Integer, String>(3, "peipponen"), list.getAt(7));
        assertEquals(new Pair<Integer, String>(6, "lohik‰‰rme"), list.getAt(8));
        try {
            list.getAt(9);
            fail();
        } catch (IndexOutOfBoundsException e) {
            // OK
        }

        int c = 0;
        for (Pair<Integer, String> p : list) {
            assertEquals(p, list.getAt(c++));
        }
    }

    public void testRemoveIterator() {
        SortedList<Integer, String> list = getList();
        Iterator<String> it = list.iterator(1);
        assertEquals("el‰in", it.next());
        it.remove();
        assertEquals("kettu", it.next());
        it.remove();
        assertEquals("susi", it.next());
        it.remove();
        assertFalse(list.contains(1));
    }

    public void testLastFloor() {
        SortedList<Integer, String> list = getList();
        assertEquals(null, list.getLastFloor(Integer.MIN_VALUE));
        assertEquals(null, list.getLastFloor(-10));
        assertEquals(null, list.getLastFloor(0));
        assertEquals(new Pair<Integer, String>(1, "susi"), list.getLastFloor(1));
        assertEquals(new Pair<Integer, String>(2, "lohi"), list.getLastFloor(2));
        assertEquals(new Pair<Integer, String>(3, "peipponen"), list.getLastFloor(3));
        assertEquals(new Pair<Integer, String>(3, "peipponen"), list.getLastFloor(4));
        assertEquals(new Pair<Integer, String>(3, "peipponen"), list.getLastFloor(5));
        assertEquals(new Pair<Integer, String>(6, "lohik‰‰rme"), list.getLastFloor(6));
        assertEquals(new Pair<Integer, String>(6, "lohik‰‰rme"), list.getLastFloor(7));
        assertEquals(new Pair<Integer, String>(6, "lohik‰‰rme"), list.getLastFloor(1000));
        assertEquals(new Pair<Integer, String>(6, "lohik‰‰rme"), list
            .getLastFloor(Integer.MAX_VALUE));
    }

    // (1, "el‰in")
    // (1, "kettu")
    // (1, "susi")
    // (2, "kala")
    // (2, "lohi")
    // (3, "kana")
    // (3, "lintu")
    // (3, "peipponen")
    // (6, "lohik‰‰rme")

    public void testFirstCeiling() {
        SortedList<Integer, String> list = getList();
        assertEquals("el‰in", list.getFirstCeiling(Integer.MIN_VALUE).getSecond());
        assertEquals("el‰in", list.getFirstCeiling(-10).getSecond());
        assertEquals("el‰in", list.getFirstCeiling(0).getSecond());
        assertEquals("el‰in", list.getFirstCeiling(1).getSecond());
        assertEquals("kala", list.getFirstCeiling(2).getSecond());
        assertEquals("kana", list.getFirstCeiling(3).getSecond());
        assertEquals(new Pair<Integer, String>(6, "lohik‰‰rme"), list.getFirstCeiling(4));
        assertEquals(new Pair<Integer, String>(6, "lohik‰‰rme"), list.getFirstCeiling(5));
        assertEquals(new Pair<Integer, String>(6, "lohik‰‰rme"), list.getFirstCeiling(6));
        assertEquals(null, list.getFirstCeiling(7));
        assertEquals(null, list.getFirstCeiling(1000));
        assertEquals(null, list.getFirstCeiling(Integer.MAX_VALUE));
    }

    public void testKeyIterator() {
        SortedList<Integer, String> list = getList();
        Iterator<String> vals = list.iterator(2);
        assertEquals(9, list.size());
        assertTrue(vals.hasNext());
        assertEquals("kala", vals.next());
        assertTrue(vals.hasNext());
        assertEquals("lohi", vals.next());
        assertFalse(vals.hasNext());
        assertEquals(9, list.size());

        vals = list.iterator(3);
        assertTrue(vals.hasNext());
        assertEquals("kana", vals.next());
        vals.remove();
        assertEquals(8, list.size());
        assertTrue(vals.hasNext());
        assertEquals("lintu", vals.next());
        assertTrue(vals.hasNext());
        assertEquals("peipponen", vals.next());
        vals.remove();
        assertEquals(7, list.size());
        assertFalse(vals.hasNext());
        assertEquals(CollectionUtils.getList("lintu"), list.getAll(3));
    }

    public void testIterator() {
        SortedList<Integer, String> list = getList();
        Iterator<Pair<Integer, String>> it = list.iterator();
        assertTrue(it.hasNext());
        assertEquals(new Pair<Integer, String>(1, "el‰in"), it.next());
        assertEquals(new Pair<Integer, String>(1, "kettu"), it.next());
        assertEquals(new Pair<Integer, String>(1, "susi"), it.next());
        assertEquals(new Pair<Integer, String>(2, "kala"), it.next());
        assertEquals(new Pair<Integer, String>(2, "lohi"), it.next());
        assertEquals(new Pair<Integer, String>(3, "kana"), it.next());
        assertEquals(new Pair<Integer, String>(3, "lintu"), it.next());
        assertEquals(new Pair<Integer, String>(3, "peipponen"), it.next());
        assertEquals(new Pair<Integer, String>(6, "lohik‰‰rme"), it.next());
        assertFalse(it.hasNext());
        assertEquals(9, list.size());

        it = list.iterator();
        assertTrue(it.hasNext());
        assertEquals(new Pair<Integer, String>(1, "el‰in"), it.next());
        assertEquals(new Pair<Integer, String>(1, "kettu"), it.next());
        assertEquals(9, list.size());
        it.remove();
        assertEquals(8, list.size());
        assertEquals(new Pair<Integer, String>(1, "susi"), it.next());
        assertEquals(new Pair<Integer, String>(2, "kala"), it.next());
        assertEquals(new Pair<Integer, String>(2, "lohi"), it.next());
        assertEquals(new Pair<Integer, String>(3, "kana"), it.next());
        assertEquals(new Pair<Integer, String>(3, "lintu"), it.next());
        it.remove();
        assertEquals(7, list.size());
        assertEquals(new Pair<Integer, String>(3, "peipponen"), it.next());
        assertEquals(new Pair<Integer, String>(6, "lohik‰‰rme"), it.next());
        assertEquals(7, list.size());
        it.remove();
        assertEquals(6, list.size());
        assertFalse(it.hasNext());

        assertEquals(6, list.size());
        assertTrue(list.contains(1));
        assertFalse(list.contains(6));
        assertEquals(CollectionUtils.getList("el‰in", "susi"), list.getAll(1));
        assertEquals(CollectionUtils.getList("kala", "lohi"), list.getAll(2));
        assertEquals(CollectionUtils.getList("kana", "peipponen"), list.getAll(3));
        assertEquals(CollectionUtils.getList(), list.getAll(6));

        it = list.iterator();
        assertTrue(it.hasNext());
        assertEquals(new Pair<Integer, String>(1, "el‰in"), it.next());
        assertEquals(new Pair<Integer, String>(1, "susi"), it.next());
        assertEquals(6, list.size());
        it.remove();
        assertEquals(5, list.size());
        assertEquals(new Pair<Integer, String>(2, "kala"), it.next());
        it.remove();
        assertEquals(4, list.size());
        assertEquals(new Pair<Integer, String>(2, "lohi"), it.next());
        it.remove();
        assertEquals(3, list.size());
        assertEquals(new Pair<Integer, String>(3, "kana"), it.next());
        assertEquals(new Pair<Integer, String>(3, "peipponen"), it.next());
        assertFalse(it.hasNext());
        assertEquals(3, list.size());

        assertFalse(list.isEmpty());
        assertTrue(list.contains(1));
        assertFalse(list.contains(2));
        assertTrue(list.contains(3));
        assertFalse(list.contains(6));
        assertEquals(CollectionUtils.getList("el‰in"), list.getAll(1));
        assertEquals(CollectionUtils.getList(), list.getAll(2));
        assertEquals(CollectionUtils.getList("kana", "peipponen"), list.getAll(3));

        it = list.iterator();
        assertTrue(it.hasNext());
        assertEquals(new Pair<Integer, String>(1, "el‰in"), it.next());
        it.remove();
        assertEquals(new Pair<Integer, String>(3, "kana"), it.next());
        it.remove();
        assertEquals(new Pair<Integer, String>(3, "peipponen"), it.next());
        it.remove();
        assertFalse(it.hasNext());

        assertEquals(0, list.size());
        assertTrue(list.isEmpty());
        for (int i = 0; i < 10; i++) {
            assertFalse(list.contains(i));
            assertEquals(CollectionUtils.getList(), list.getAll(i));
        }

    }

    public void testValueOrdering() {
        SortedList<Integer, Integer> list = new SortedList<Integer, Integer>();
        list.add(1, 10);
        list.add(2, 10);
        list.add(3, 10);
        list.add(1, 15);
        list.add(1, 5);
        list.add(0, 5);
        list.add(1, 6);
        list.add(1, 5);
        list.add(1, 20);
        list.add(1, -5);

        Iterator<Integer> it = list.iterator(1);
        assertEquals(-5, it.next().intValue());
        assertEquals(5, it.next().intValue());
        assertEquals(5, it.next().intValue());
        assertEquals(6, it.next().intValue());
        assertEquals(10, it.next().intValue());
        assertEquals(15, it.next().intValue());
        assertEquals(20, it.next().intValue());
    }

}
