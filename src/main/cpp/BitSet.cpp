#include "util/BitSet.hpp"

using namespace std;
using namespace util;

ostream & util::operator<<(ostream & os, BitSet const & bs) {
    size_t blocks = bs.numBlocks();
    for (size_t i = 0; i < blocks; i++) {
        os << bs.pieces[i];
    }
    return os;
}

void BitSet::ensureCapacity(size_t ensureSize) {
    if (allocElements >= ensureSize)
        return;
    size_t blocks = ensureSize / UTILS_BITSET_PIECE_SIZE;
    if (ensureSize % UTILS_BITSET_PIECE_SIZE != 0)
        blocks++;

    size_t newSize = blocks * UTILS_BITSET_PIECE_SIZE;
    ASSERT(newSize >= ensureSize);

    if (allocElements < newSize) {
        allocElements = newSize;
    }
    std::bitset < UTILS_BITSET_PIECE_SIZE > *oldPieces = pieces;
    pieces = new std::bitset<UTILS_BITSET_PIECE_SIZE>[blocks];
    size_t copyBlocks = numBlocks(nElements);
    for (size_t i = 0; i < copyBlocks; i++) {
        // Can't do memcpy, because the elements may be objects that have constructors & destructors
        pieces[i] = oldPieces[i];
    }
    delete[] oldPieces;
}
