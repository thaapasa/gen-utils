package fi.tuska.util.geom;

/**
 * Encoding taken from
 * http://stackoverflow.com/questions/106237/calculate-the-hilbert-value
 * -of-a-point-for-use-in-a-hilbert-r-tree
 * 
 * <p>
 * Decoding taken from
 * http://web.archive.org/web/20041102084739/www.caam.rice.
 * edu/~dougm/twiddle/Hilbert/hilbert.c
 * 
 * @author thaapasa
 */
public class HilbertCurve {

    /**
     * Find the Hilbert order (=vertex index) for the given grid cell
     * coordinates.
     * 
     * @param x cell column (from 0)
     * @param y cell row (from 0)
     * @param r resolution of Hilbert curve (grid will have Math.pow(2,r) rows
     * and cols)
     * @return Hilbert order
     */
    public static long encode(long x, long y, int r) {
        long mask = (1L << r) - 1;
        long hodd = 0;
        long heven = x ^ y;
        long notx = ~x & mask;
        long noty = ~y & mask;
        long temp = notx ^ y;

        long v0 = 0, v1 = 0;
        for (int k = 1; k < r; k++) {
            v1 = ((v1 & heven) | ((v0 ^ noty) & temp)) >> 1;
            v0 = ((v0 & (v1 ^ notx)) | (~v0 & (v1 ^ noty))) >> 1;
        }
        hodd = (~v0 & (v1 ^ x)) | (v0 & (v1 ^ noty));

        return interleaveBits(hodd, heven);
    }

    /**
     * Interleave the bits from two input integer values
     * 
     * @param odd integer holding bit values for odd bit positions
     * @param even integer holding bit values for even bit positions
     * @return the integer that results from interleaving the input bits
     * 
     * @todo: I'm sure there's a more elegant way of doing this !
     */
    private static long interleaveBits(long odd, long even) {
        long val = 0;
        long max = Math.max(odd, even);
        int n = 0;
        while (max > 0) {
            n++;
            max >>= 1;
        }

        for (long i = 0; i < n; i++) {
            long bitMask = 1L << i;
            long a = (even & bitMask) > 0 ? (1L << (i << 1)) : 0;
            long b = (odd & bitMask) > 0 ? (1L << ((i << 1) + 1)) : 0;
            // val += a + b;
            // Optimization, because a, b and val are known to have only
            // non-intersecting bits
            val |= a | b;
        }

        return val;
    }

    /**
     * Convert an index into a Hilbert curve to a set of coordinates.
     * 
     * @param index The index, contains nDims*nBits bits (so nDims*nBits must
     * be <= 8*sizeof(bitmask_t)).
     * @param nBits Number of bits per axis (the curve level)
     * @param nDims Number of coordinate axes.
     * @return coord: The list of nDims coordinates, each with nBits bits.
     * Assumptions: nDims*nBits <= (sizeof index) * (bits_per_byte)
     */
    public static long[] decode(long index, int nBits, int nDims) {
        long coord[] = new long[nDims];

        if (nDims > 1) {
            long coords;
            long nbOnes = ((1L << nBits) - 1);

            if (nBits > 1) {
                long nDimsBits = (long) nDims * nBits;
                long ndOnes = ((1L << nDims) - 1);
                /* for adjust_rotation */
                long nd1Ones = ndOnes >> 1;

                long b = nDimsBits;
                long rotation = 0;
                long flipBit = 0;
                long nthbits = ((2L << (nDimsBits - 1)) - 1) / ndOnes;
                index ^= (index ^ nthbits) >> 1;
                coords = 0;
                do {
                    long bits = (index >> (b -= nDims)) & ndOnes;
                    coords <<= nDims;
                    coords |= (((bits << rotation) | (bits >> (nDims - rotation))) & ((1L << nDims) - 1))
                        ^ flipBit;

                    flipBit = 1L << rotation;
                    // adjust_rotation(rotation,nDims,bits);
                    bits &= (-bits) & nd1Ones;
                    while (bits != 0) {
                        bits >>= 1;
                        ++rotation;
                    }
                    if (++rotation >= nDims) {
                        rotation -= nDims;
                    }

                } while (b != 0);
                for (b = nDims; b < nDimsBits; b *= 2) {
                    coords ^= coords >> b;
                }
                coords = bitTranspose(nBits, nDims, coords);
            } else {
                coords = index ^ (index >> 1);
            }

            for (int d = nDims - 1; d >= 0; d--) {
                coord[d] = coords & nbOnes;
                coords >>= nBits;
            }
        } else {
            coord[0] = index;
        }

        return coord;
    }

    private static long bitTranspose(long nDims, long nBits, long inCoords) {
        long nDims1 = nDims - 1;
        long inB = nBits;
        long utB;
        long inFieldEnds = 1;
        long inMask = (((2L) << (inB - 1)) - 1);
        long coords = 0;

        while ((utB = inB / 2) != 0) {
            long shiftAmt = nDims1 * utB;
            long utFieldEnds = inFieldEnds | (inFieldEnds << (shiftAmt + utB));
            long utMask = (utFieldEnds << utB) - utFieldEnds;
            long utCoords = 0;
            if ((inB & 1) != 0) {
                final long inFieldStarts = inFieldEnds << (inB - 1);
                long oddShift = 2 * shiftAmt;
                for (int d = 0; d < nDims; ++d) {
                    long in = inCoords & inMask;
                    inCoords >>= inB;
                    coords |= (in & inFieldStarts) << oddShift++;
                    in &= ~inFieldStarts;
                    in = (in | (in << shiftAmt)) & utMask;
                    utCoords |= in << (d * utB);
                }
            } else {
                for (int d = 0; d < nDims; ++d) {
                    long in = inCoords & inMask;
                    inCoords >>= inB;
                    in = (in | (in << shiftAmt)) & utMask;
                    utCoords |= in << ((long) d * utB);
                }
            }
            inCoords = utCoords;
            inB = utB;
            inFieldEnds = utFieldEnds;
            inMask = utMask;
        }
        coords |= inCoords;
        return coords;
    }

}
