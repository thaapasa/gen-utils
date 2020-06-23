package fi.tuska.util;

import junit.framework.TestCase;

public class StringUtilsTest extends TestCase {

    public void testPad() {
        assertEquals("00000123", StringUtils.pad("123", 8, "0", true));
        assertEquals("12300000", StringUtils.pad("123", 8, "0", false));

        assertEquals("123", StringUtils.pad("123", 2, "0", true));
        assertEquals("123", StringUtils.pad("123", 2, "0", false));

        assertEquals("xoxox123", StringUtils.pad("123", 8, "xo", true));
        assertEquals("123xoxox", StringUtils.pad("123", 8, "xo", false));
    }

    public void testReverse() {
        assertNull(StringUtils.reverse(null));
        assertEquals("", StringUtils.reverse(""));
        assertEquals("k", StringUtils.reverse("k"));
        assertEquals("kk", StringUtils.reverse("kk"));

        assertEquals("ab", StringUtils.reverse("ba"));

        assertEquals("tsuFfe", StringUtils.reverse("efFust"));
    }

    public void testToBinaryString() {
        assertEquals("00000000", StringUtils.toBinaryString((byte) 0x00, true));
        assertEquals("00000000", StringUtils.toBinaryString((byte) 0x00, false));
        assertEquals("10000000", StringUtils.toBinaryString((byte) 0x01, true));
        assertEquals("11111111", StringUtils.toBinaryString((byte) 0xff, true));
        assertEquals("11110001", StringUtils.toBinaryString((byte) 0x8f, true));
        assertEquals("01000000", StringUtils.toBinaryString((byte) 0x02, true));
        assertEquals("00000010", StringUtils.toBinaryString((byte) 0x02, false));

        assertEquals("00000000000000000000000000000000", StringUtils.toBinaryString(0x00000000,
            true));
        assertEquals("00000000000000000000000000000000", StringUtils.toBinaryString(0x00000000,
            false));
        assertEquals("10000000000000000000000000000000", StringUtils.toBinaryString(0x00000001,
            true));
        assertEquals("00000000000000000000000000000001", StringUtils.toBinaryString(0x00000001,
            false));

        assertEquals("00000000100011110000000000000000", StringUtils.toBinaryString(0x0000f100,
            true));
        assertEquals("00000000000000001111000100000000", StringUtils.toBinaryString(0x0000f100,
            false));
        assertEquals("11111111111111111111111111111111", StringUtils.toBinaryString(0xffffffff,
            false));
        assertEquals("11111111111111111111111111111111", StringUtils.toBinaryString(0xffffffff,
            true));

        assertEquals("0000000000000000000000000000000000000000000000000000000000000000",
            StringUtils.toBinaryString(0x0000000000000000l, true));
        assertEquals("0000000000000000000000000000000000000000000000000000000000000000",
            StringUtils.toBinaryString(0x0000000000000000l, false));
        assertEquals("1000000000000000000000000000000000000000000000000000000000000000",
            StringUtils.toBinaryString(0x0000000000000001l, true));
        assertEquals("0000000000000000000000000000000000000000000000000000000000000001",
            StringUtils.toBinaryString(0x0000000000000001l, false));

        assertEquals("0001100011110000000000000000000000000000000000000000000000001111",
            StringUtils.toBinaryString(0xf000000000000f18l, true));
        assertEquals("1111000000000000000000000000000000000000000000000000111100011000",
            StringUtils.toBinaryString(0xf000000000000f18l, false));

        assertEquals("1111111111111111111111111111111111111111111111111111111111111111",
            StringUtils.toBinaryString(0xffffffffffffffffl, true));
        assertEquals("1111111111111111111111111111111111111111111111111111111111111111",
            StringUtils.toBinaryString(0xffffffffffffffffl, false));
    }

}
