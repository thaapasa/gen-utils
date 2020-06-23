package fi.tuska.util;

import junit.framework.TestCase;

public abstract class ATestCase extends TestCase {

    protected void assertEquals(int[] a, int[] b) {
        if (a == null) {
            assertNull(b);
            return;
        } else
            assertNotNull(b);

        assertEquals(a.length, b.length);

        for (int i = 0; i < a.length; i++) {
            assertEquals("For index " + i, a[i], b[i]);
        }
    }

    protected void assertEquals(byte[] a, byte[] b) {
        if (a == null) {
            assertNull(b);
            return;
        } else
            assertNotNull(b);

        assertEquals(a.length, b.length);

        for (int i = 0; i < a.length; i++) {
            assertEquals("For index " + i, a[i], b[i]);
        }
    }

    protected String toBitString(byte[] ar) {
        StringBuilder b = new StringBuilder();

        for (int i = 0; i < ar.length; i++) {
            if (i != 0)
                b.append(" ");
            b.append(StringUtils.toBinaryString(ar[i], true));
        }
        return b.toString();
    }

    protected String toBitString(int[] ar) {
        StringBuilder b = new StringBuilder();

        for (int i = 0; i < ar.length; i++) {
            if (i != 0)
                b.append(" ");
            b.append(StringUtils.toBinaryString(ar[i], true));
        }
        return b.toString();
    }
}
