package fi.tuska.util.iterator;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import junit.framework.TestCase;

public class ReverseIteratorTest extends TestCase {

    private List<Integer> getList() {
        List<Integer> list = new ArrayList<Integer>();
        list.add(3);
        list.add(7);
        list.add(10);
        list.add(1);
        return list;
    }

    public void testIterate() {
        List<Integer> list = getList();
        Iterator<Integer> i = new ReverseIterator<Integer>(list);
        assertTrue(i.hasNext());
        assertEquals(1, i.next().intValue());
        assertTrue(i.hasNext());
        assertEquals(10, i.next().intValue());
        assertTrue(i.hasNext());
        assertEquals(7, i.next().intValue());
        assertTrue(i.hasNext());
        assertEquals(3, i.next().intValue());
        assertFalse(i.hasNext());
    }

    public void testRemove() {
        List<Integer> list = getList();
        Iterator<Integer> i = new ReverseIterator<Integer>(list);
        // Remove 1 and 7
        assertTrue(i.hasNext());
        assertEquals(1, i.next().intValue());
        i.remove();
        assertTrue(i.hasNext());
        assertEquals(10, i.next().intValue());
        assertTrue(i.hasNext());
        assertEquals(7, i.next().intValue());
        i.remove();
        assertTrue(i.hasNext());
        assertEquals(3, i.next().intValue());
        assertFalse(i.hasNext());

        assertEquals(2, list.size());
        assertEquals(3, list.get(0).intValue());
        assertEquals(10, list.get(1).intValue());

        // Remove 3
        list = getList();
        i = new ReverseIterator<Integer>(list);
        assertTrue(i.hasNext());
        assertEquals(1, i.next().intValue());
        assertTrue(i.hasNext());
        assertEquals(10, i.next().intValue());
        assertTrue(i.hasNext());
        assertEquals(7, i.next().intValue());
        assertTrue(i.hasNext());
        assertEquals(3, i.next().intValue());
        assertFalse(i.hasNext());
        i.remove();

        assertEquals(3, list.size());
        assertEquals(7, list.get(0).intValue());
        assertEquals(10, list.get(1).intValue());
        assertEquals(1, list.get(2).intValue());

        // Remove all
        list = getList();
        i = new ReverseIterator<Integer>(list);
        assertTrue(i.hasNext());
        assertEquals(1, i.next().intValue());
        i.remove();
        assertTrue(i.hasNext());
        assertEquals(10, i.next().intValue());
        i.remove();
        assertTrue(i.hasNext());
        assertEquals(7, i.next().intValue());
        i.remove();
        assertTrue(i.hasNext());
        assertEquals(3, i.next().intValue());
        i.remove();
        assertFalse(i.hasNext());

        assertEquals(0, list.size());
    }

}
