package fi.tuska.util;

import java.util.Iterator;

import junit.framework.TestCase;

public class LinkedListTest extends TestCase {

    public void testList() {
        LinkedList<String> list = new LinkedList<String>();
        list.addFirst("Piffi");
        list.addFirst("Perunat");
        list.addFirst("Muusi");
        assertEquals(3, list.getSize());

        assertTrue(list.getLastNode().isInList());
        assertEquals("Piffi", list.removeLastNode().getValue());
        assertEquals(2, list.getSize());
        assertTrue(list.getLastNode().isInList());
        assertEquals("Perunat", list.removeLastNode().getValue());
        assertEquals(1, list.getSize());
        assertTrue(list.getLastNode().isInList());

        assertNotNull(list.getLast());
        assertEquals(list.getLast(), list.getFirst());
        assertEquals("Muusi", list.removeLastNode().getValue());
        assertEquals(0, list.getSize());
        assertNull(list.getLastNode());
        assertNull(list.getFirstNode());
    }

    public void testListIterator() {
        LinkedList<String> list = getList("first", "2", "3rd");
        assertListEquals(list, "first", "2", "3rd");
    }

    public void testListClear() {
        LinkedList<String> list = getList("first", "2", "3rd");
        assertListEquals(list, "first", "2", "3rd");
        assertFalse(list.isEmpty());
        assertEquals(3, list.size());
        list.clear();
        assertTrue(list.isEmpty());
        assertEquals(0, list.size());
    }

    public void testListRemove() {
        LinkedList<String> list = getList("first", "2", "3rd");
        assertFalse(list.isEmpty());
        assertListEquals(list, "first", "2", "3rd");
        list.remove(0);
        assertListEquals(list, "2", "3rd");
        list.remove(1);
        assertListEquals(list, "2");
        assertFalse(list.isEmpty());
        try {
            list.remove(1);
            fail("No IOOBE thrown");
        } catch (IndexOutOfBoundsException e) {
            // Expected
        }
        assertListEquals(list, "2");
        list.remove(0);
        assertTrue(list.isEmpty());

        list = getList("1", "2", "3", "4", "5");
        list.remove(4);
        assertListEquals(list, "1", "2", "3", "4");
        list.remove(2);
        assertListEquals(list, "1", "2", "4");
    }

    public void testIteratorRemove() {
        LinkedList<String> list = getList("1", "2", "3", "4", "5");
        Iterator<String> it = list.iterator();
        assertEquals("1", it.next());
        assertEquals("2", it.next());
        it.remove();
        assertEquals("3", it.next());
        assertEquals("4", it.next());
        assertTrue(it.hasNext());
        assertEquals("5", it.next());
        assertFalse(it.hasNext());
        assertListEquals(list, "1", "3", "4", "5");

        it = list.iterator();
        assertEquals("1", it.next());
        it.remove();
        assertEquals("3", it.next());
        assertTrue(it.hasNext());
        assertListEquals(list, "3", "4", "5");

        it = list.iterator();
        assertEquals("3", it.next());
        assertEquals("4", it.next());
        assertEquals("5", it.next());
        it.remove();
        assertFalse(it.hasNext());
        assertListEquals(list, "3", "4");

        list = getList("1", "2", "3", "4", "5");
        it = list.iterator();
        assertEquals("1", it.next());
        it.remove();
        assertEquals("2", it.next());
        it.remove();
        assertEquals("3", it.next());
        it.remove();
        assertEquals("4", it.next());
        it.remove();
        assertTrue(it.hasNext());
        assertEquals("5", it.next());
        assertFalse(it.hasNext());
        it.remove();
        assertFalse(it.hasNext());
        assertTrue(list.isEmpty());
    }

    public void testListContains() {
        LinkedList<String> list = getList("first", "2", "3rd");
        assertListEquals(list, "first", "2", "3rd");
        assertTrue(list.contains("first"));
        assertFalse(list.contains("1st"));
        assertTrue(list.contains("2"));
        assertFalse(list.contains("3"));
        list.remove(0);
        assertListEquals(list, "2", "3rd");
        assertFalse(list.contains("first"));
        assertFalse(list.contains("1st"));
        assertTrue(list.contains("2"));
        assertFalse(list.contains("3"));
    }

    private static <T> LinkedList<T> getList(T... params) {
        LinkedList<T> list = new LinkedList<T>();
        for (T p : params) {
            list.add(p);
        }
        return list;
    }

    private <T> void assertListEquals(LinkedList<T> list, T... expectedValues) {
        list.checkConsistency();
        Iterator<T> it = list.iterator();
        for (T exp : expectedValues) {
            assertTrue(it.hasNext());
            assertEquals(exp, it.next());
        }
        assertFalse(it.hasNext());
    }

}
