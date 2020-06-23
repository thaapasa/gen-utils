package fi.tuska.util.iterator;

import java.util.Collections;

import fi.tuska.util.CollectionUtils;
import fi.tuska.util.iterator.Iterables;

import junit.framework.TestCase;

/**
 * Tests for Iterables
 * 
 * @author Tuukka Haapasalo
 */
public class IterablesTest extends TestCase {

    public void testGetString() {
        int i = 0;
        for (char c : Iterables.get("Nakki")) {
            switch (i) {
            case 0:
                assertEquals('N', c);
                break;
            case 1:
                assertEquals('a', c);
                break;
            case 2:
            case 3:
                assertEquals('k', c);
                break;
            case 4:
                assertEquals('i', c);
                break;
            default:
                fail("Index out of bounds: " + i);
            }
            i++;
        }
    }

    public void testGetEnumeration() {
        int i = 0;
        for (int c : Iterables.get(Collections
            .enumeration(CollectionUtils.getList(4, 7, 2, 6, 5)))) {
            switch (i) {
            case 0:
                assertEquals(4, c);
                break;
            case 1:
                assertEquals(7, c);
                break;
            case 2:
                assertEquals(2, c);
                break;
            case 3:
                assertEquals(6, c);
                break;
            case 4:
                assertEquals(5, c);
                break;
            default:
                fail("Index out of bounds: " + i);
            }
            i++;
        }
    }
}
