package fi.tuska.util;

import java.lang.reflect.Field;
import java.nio.ByteBuffer;
import java.util.BitSet;

public class StorableBitSet extends BitSet {

    private static final long serialVersionUID = 6823745127609193601L;
    private int nBits;
    private boolean useShortcut = true;

    public StorableBitSet(int nBits) {
        super(nBits);
        this.nBits = nBits;
    }

    public StorableBitSet(int[] words, int nBits) {
        super(nBits);
        this.nBits = nBits;
        setByWordArray(words, nBits);
    }

    public void setUseShortcut(boolean state) {
        this.useShortcut = state;
    }

    public int getSizeInBits() {
        return nBits;
    }

    public int getSizeInWords() {
        return getSizeInWords(nBits);
    }

    public int getSizeInBytes() {
        return getSizeInBytes(nBits);
    }

    /**
     * @param nBits number of bits
     * @return the number of words required to store the given amount of bits
     */
    public static int getSizeInWords(int nBits) {
        if (nBits <= 0)
            return 0;
        int wordCount = (int) Math.ceil((double) nBits / 32.0d);
        assert (wordCount * 32) >= nBits;
        assert ((wordCount - 1) * 32) < nBits;
        return wordCount;
    }

    /**
     * @param nBits number of bits
     * @return the number of bytes required to store the given amount of bits
     */
    public static int getSizeInBytes(int nBits) {
        if (nBits <= 0)
            return 0;
        int byteCount = (int) Math.ceil((double) nBits / 8.0d);
        assert (byteCount * 8) >= nBits;
        assert ((byteCount - 1) * 8) < nBits;
        return byteCount;
    }

    public int[] toWordArray() {
        int nWords = getSizeInWords(nBits);
        if (nWords == 0)
            return new int[0];
        long[] bitArray = getBitArray();
        int[] res = new int[nWords];
        for (int i = 0; i < nWords; i++) {
            res[i] = getWord(i, bitArray);
        }
        return res;
    }

    public byte[] toByteArray() {
        int nBytes = getSizeInBytes(nBits);
        if (nBytes == 0)
            return new byte[0];

        byte[] res = new byte[nBytes];
        long[] bitArray = getBitArray();
        for (int i = 0; i < nBytes; i++) {
            res[i] = getByte(i, bitArray);
        }
        return res;
    }

    /**
     * Writes the bit set to a ByteBuffer, one word (integer, 4 bytes) at a
     * time. The bytes of the integer are stored in the order defined by the
     * ByteBuffer.
     */
    public void writeToBytes(ByteBuffer buffer) {
        int nWords = getSizeInWords();
        long[] bitArray = getBitArray();
        for (int i = 0; i < nWords; i++) {
            buffer.putInt(getWord(i, bitArray));
        }
    }

    /**
     * Reads a stored bit set from the byte array. Assumes that the bits are
     * stored word-by-word (in 4 byte blocks). The bytes of individual words
     * are stored in the order defined by the byte array.
     */
    public static StorableBitSet readFromBytes(ByteBuffer buffer, int nBits) {
        int nWords = getSizeInWords(nBits);
        int[] words = new int[nWords];
        for (int i = 0; i < nWords; i++) {
            words[i] = buffer.getInt();
        }
        StorableBitSet bset = new StorableBitSet(words, nBits);
        return bset;
    }

    private static final long CLEAR_WORD_MASKS[] = { 0xffffffff00000000l, 0x00000000ffffffffl };

    public void setByWordArray(int[] words, int nBits) {
        long[] bitArray = getBitArray();
        if (bitArray != null) {
            // Set the max bit so that all bits are in use and we are using
            // shortcut
            set(nBits - 1);
        }
        for (int i = 0; i < words.length; i++) {
            setWord(i, bitArray, words[i]);
        }
    }

    /**
     * A method for directly accessing the underlying bit storage (words-array
     * in the BitSet class). Uses Java Reflection to bypass the private
     * protection of the array.
     */
    private long[] getBitArray() {
        if (!useShortcut)
            return null;
        try {
            Field wordField = BitSet.class.getDeclaredField("words");
            wordField.setAccessible(true);
            long[] words = (long[]) wordField.get(this);
            return words;
        } catch (NoSuchFieldException e) {
            return null;
        } catch (IllegalAccessException e) {
            return null;
        }
    }

    private void setWord(int wordIndex, long[] bitArray, int word) {
        if (bitArray != null) {
            int w = wordIndex >> 1;
            int m = wordIndex & 1;
            // Clear previous stuff
            bitArray[w] &= CLEAR_WORD_MASKS[m];
            // Add the bits from the given word
            bitArray[w] |= (m == 0) ? (long) word & CLEAR_WORD_MASKS[1]
                : (long) ((long) word << 32);
        } else {
            int startIndex = wordIndex * 32;
            long m = 1;
            for (int b = 0; b < 32; b++) {
                if ((word & m) > 0)
                    set(startIndex + b);
                m <<= 1;
            }
        }
    }

    private int getWord(int wordIndex, long[] bitArray) {
        if (bitArray != null) {
            long w = bitArray[wordIndex >> 1];
            return ((wordIndex & 0x1) == 0) ? (int) w : (int) (w >> 32);
        } else {
            int w = 0;
            int startBit = wordIndex * 32;
            int mask = 1;
            for (int i = 0; i < 32; i++) {
                if (get(startBit + i)) {
                    w |= mask;
                }
                mask <<= 1;
            }
            return w;
        }
    }

    private byte getByte(int byteIndex, long[] bitArray) {
        if (bitArray != null) {
            long w = bitArray[(byteIndex >> 3)];
            // Shift by (3 last bits from i) * 8
            int shifter = ((byteIndex & 0x07) << 3);
            return (byte) ((w >> shifter) & 0xff);
        } else {
            int w = 0;
            int startBit = byteIndex * 8;
            int mask = 1;
            for (int i = 0; i < 8; i++) {
                if (get(startBit + i)) {
                    w |= mask;
                }
                mask <<= 1;
            }
            return (byte) w;
        }
    }

}
