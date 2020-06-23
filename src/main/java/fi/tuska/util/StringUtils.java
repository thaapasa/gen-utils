package fi.tuska.util;

/**
 * Various utilities for working with strings.
 * 
 * @author thaapasa
 */
public class StringUtils {

    /**
     * Pads the given string with the given padding. Does not crop to the
     * given length, if the original is longer than the expected length.
     * 
     * @param str the original string
     * @param expLen the expected lenght of the string (with padding added)
     * @param padding the padding to use
     * @param padToFront true to add padding to the front, false to add
     * padding to the end of the string
     * @return the padded string; or the original, if the original is already
     * long enough
     */
    public static String pad(String str, int expLen, String padding, boolean padToFront) {
        if (str.length() >= expLen)
            return str;

        StringBuilder b = new StringBuilder(expLen + padding.length());
        if (padToFront) {
            // First padding
            int expPad = expLen - str.length();
            while (b.length() < expPad) {
                b.append(padding);
            }
            // If there is too much padding, crop away last of it
            if (b.length() > expPad) {
                b.setLength(expPad);
            }
            // Finally the string
            b.append(str);
        } else {
            // First the string
            b.append(str);
            // Then padding
            while (b.length() < expLen) {
                b.append(padding);
            }
            // If there is too much padding, crop away last of it
            if (b.length() > expLen) {
                b.setLength(expLen);
            }
        }
        assert b.length() == expLen;
        return b.toString();
    }

    /**
     * Reverses the given string.
     */
    public static String reverse(String str) {
        if (str == null)
            return null;
        if (str.length() <= 1)
            return str;

        StringBuilder b = new StringBuilder(str.length());
        for (int i = str.length() - 1; i >= 0; i--) {
            b.append(str.charAt(i));
        }
        return b.toString();
    }

    /**
     * @param ascending when true, returns LSB as first char; when false, LSB
     * is the last char
     */
    public static String toBinaryString(byte b, boolean ascending) {
        String str = StringUtils.pad(Integer.toBinaryString((int) (b & 0xff)), 8, "0", true);
        return ascending ? StringUtils.reverse(str) : str;
    }

    /**
     * @param ascending when true, returns LSB as first char; when false, LSB
     * is the last char
     */
    public static String toBinaryString(int i, boolean ascending) {
        String str = StringUtils.pad(Integer.toBinaryString(i), 32, "0", true);
        return ascending ? StringUtils.reverse(str) : str;
    }

    /**
     * @param ascending when true, returns LSB as first char; when false, LSB
     * is the last char
     */
    public static String toBinaryString(long l, boolean ascending) {
        int lo = (int) (l & 0x00000000ffffffffl);
        int hi = (int) (l >> 32);
        if (ascending) {
            return toBinaryString(lo, ascending) + toBinaryString(hi, ascending);
        } else {
            return toBinaryString(hi, ascending) + toBinaryString(lo, ascending);
        }
    }
}
