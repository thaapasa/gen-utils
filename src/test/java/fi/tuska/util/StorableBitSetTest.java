package fi.tuska.util;

import java.nio.ByteBuffer;
import java.util.Arrays;

public class StorableBitSetTest extends ATestCase {

    public void testWordSize() {
        assertEquals(0, StorableBitSet.getSizeInWords(0));
        assertEquals(0, StorableBitSet.getSizeInWords(-1));
        assertEquals(0, StorableBitSet.getSizeInWords(-200));

        assertEquals(1, StorableBitSet.getSizeInWords(1));
        assertEquals(1, StorableBitSet.getSizeInWords(7));
        assertEquals(1, StorableBitSet.getSizeInWords(8));
        assertEquals(1, StorableBitSet.getSizeInWords(9));
        assertEquals(1, StorableBitSet.getSizeInWords(31));
        assertEquals(1, StorableBitSet.getSizeInWords(32));

        assertEquals(2, StorableBitSet.getSizeInWords(33));
        assertEquals(2, StorableBitSet.getSizeInWords(63));
        assertEquals(2, StorableBitSet.getSizeInWords(64));

        assertEquals(3, StorableBitSet.getSizeInWords(65));

        assertEquals(7229, StorableBitSet.getSizeInWords(231321));
    }

    public void testByteSize() {
        assertEquals(0, StorableBitSet.getSizeInBytes(0));
        assertEquals(0, StorableBitSet.getSizeInBytes(-1));
        assertEquals(0, StorableBitSet.getSizeInBytes(-200));

        assertEquals(1, StorableBitSet.getSizeInBytes(1));
        assertEquals(1, StorableBitSet.getSizeInBytes(7));
        assertEquals(1, StorableBitSet.getSizeInBytes(8));

        assertEquals(2, StorableBitSet.getSizeInBytes(9));
        assertEquals(4, StorableBitSet.getSizeInBytes(31));
        assertEquals(4, StorableBitSet.getSizeInBytes(32));

        assertEquals(5, StorableBitSet.getSizeInBytes(33));

        assertEquals(42906, StorableBitSet.getSizeInBytes(343243));
    }

    public void testToWordArray() {
        StorableBitSet bset = new StorableBitSet(35);
        assertEquals(35, bset.getSizeInBits());

        assertEquals(new int[] { 0, 0 }, bset.toWordArray());

        bset.set(0);
        assertEquals(new int[] { 1, 0 }, bset.toWordArray());
        bset.set(1);
        assertEquals(new int[] { 3, 0 }, bset.toWordArray());
        bset.clear(0);
        assertEquals(new int[] { 2, 0 }, bset.toWordArray());

        bset.set(31);
        assertEquals(new int[] { 0x80000002, 0 }, bset.toWordArray());

        bset.set(32);
        assertEquals(new int[] { 0x80000002, 1 }, bset.toWordArray());
    }

    public void testToByteArray() {
        StorableBitSet bset = new StorableBitSet(35);
        assertEquals(35, bset.getSizeInBits());

        assertEquals(new byte[] { 0, 0, 0, 0, 0 }, bset.toByteArray());

        bset.set(0);
        assertEquals(new byte[] { 1, 0, 0, 0, 0 }, bset.toByteArray());
        bset.set(1);
        assertEquals(new byte[] { 3, 0, 0, 0, 0 }, bset.toByteArray());
        bset.clear(0);
        assertEquals(new byte[] { 2, 0, 0, 0, 0 }, bset.toByteArray());

        bset.set(7);
        assertEquals(new byte[] { (byte) 0x82, 0, 0, 0, 0 }, bset.toByteArray());

        bset.set(31);
        assertEquals(new byte[] { (byte) 0x82, 0, 0, (byte) 0x80, 0 }, bset.toByteArray());
        bset.set(32);
        assertEquals(new byte[] { (byte) 0x82, 0, 0, (byte) 0x80, 1 }, bset.toByteArray());
    }

    public void testToByteArray2() {
        StorableBitSet bset = new StorableBitSet(35);
        assertEquals("00000000 00000000 00000000 00000000 00000000", toBitString(bset
            .toByteArray()));

        bset.set(3);
        bset.set(8);
        bset.set(15);
        bset.set(32, 35); // Sets 32, 33, 34
        bset.setUseShortcut(true);
        assertEquals("00010000 10000001 00000000 00000000 11100000", toBitString(bset
            .toByteArray()));
        bset.setUseShortcut(false);
        assertEquals("00010000 10000001 00000000 00000000 11100000", toBitString(bset
            .toByteArray()));
    }

    public void testToWordArray2() {
        StorableBitSet bset = new StorableBitSet(35);
        assertEquals("00000000000000000000000000000000 00000000000000000000000000000000",
            toBitString(bset.toWordArray()));

        bset.set(3);
        bset.set(8);
        bset.set(15);
        bset.set(32, 35); // Sets 32, 33, 34
        bset.setUseShortcut(true);
        assertEquals("00010000100000010000000000000000 11100000000000000000000000000000",
            toBitString(bset.toWordArray()));
        bset.setUseShortcut(false);
        assertEquals("00010000100000010000000000000000 11100000000000000000000000000000",
            toBitString(bset.toWordArray()));
    }

    public void testDifferentWordAccessTimes() {
        StorableBitSet bset = new StorableBitSet(10000);
        for (int i = 0; i < 99; i++) {
            bset.set(i * 100 + 5);
        }
        int iters = 1000;
        int[] ar1 = null;
        int[] ar2 = null;
        {
            bset.setUseShortcut(false);
            long start = System.currentTimeMillis();
            for (int i = 0; i < iters; i++) {
                ar1 = bset.toWordArray();
            }
            long end = System.currentTimeMillis();
            System.out.println("Normal access time (ms): " + (end - start));
        }

        {
            bset.setUseShortcut(true);
            long start = System.currentTimeMillis();
            for (int i = 0; i < iters; i++) {
                ar2 = bset.toWordArray();
            }
            long end = System.currentTimeMillis();
            System.out.println("Shortcutted access time (ms): " + (end - start));
        }
        assertEquals(ar1, ar2);
    }

    public void testDifferentByteAccessTimes() {
        StorableBitSet bset = new StorableBitSet(10000);
        for (int i = 0; i < 99; i++) {
            bset.set(i * 100 + 5);
        }
        int iters = 1000;
        byte[] ar1 = null;
        byte[] ar2 = null;
        {
            bset.setUseShortcut(false);
            long start = System.currentTimeMillis();
            for (int i = 0; i < iters; i++) {
                ar1 = bset.toByteArray();
            }
            long end = System.currentTimeMillis();
            System.out.println("Normal access time (ms): " + (end - start));
        }

        {
            bset.setUseShortcut(true);
            long start = System.currentTimeMillis();
            for (int i = 0; i < iters; i++) {
                ar2 = bset.toByteArray();
            }
            long end = System.currentTimeMillis();
            System.out.println("Shortcutted access time (ms): " + (end - start));
        }
        assertEquals(ar1, ar2);
    }

    public void testStoreToByteBuffer() {
        StorableBitSet bset = new StorableBitSet(500);
        for (int i = 0; i < 500; i++) {
            if (i % 3 == 0)
                bset.set(i);
        }
        assertEquals("10010010 01001001 00100100", toBitString(Arrays.copyOf(bset.toByteArray(),
            3)));
        assertEquals("10010010010010010010010010010010", toBitString(Arrays.copyOf(bset
            .toWordArray(), 1)));
        ByteBuffer buf = ByteBuffer.allocate(bset.getSizeInBytes() + 20);
        bset.writeToBytes(buf);
        buf.rewind();

        StorableBitSet bset2 = StorableBitSet.readFromBytes(buf, bset.getSizeInBits());
        assertEquals(bset, bset2);
    }

}
