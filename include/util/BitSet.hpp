#if !defined(UTIL_BITSET_HPP)
#define UTIL_BITSET_HPP

#include "util/exception/Exception.hpp"
#include "util/Log.hpp"
#include <bitset>

/*
 * A dynamic bitset. Wrapper for the STL bitset, allowing the bitset to grow.
 *
 * @author Tuukka Haapasalo
 * @created 2010-12-02
 */

#define UTILS_BITSET_PIECE_SIZE sizeof(long)

namespace util {
class BitSet {
private:
    size_t nElements;
    size_t allocElements;
    std::bitset<UTILS_BITSET_PIECE_SIZE> *pieces;
public:
    BitSet() :
        nElements(0), allocElements(0), pieces(0) {
        ensureCapacity(UTILS_BITSET_PIECE_SIZE);
    }
    BitSet(size_t initialElements) :
        nElements(0), allocElements(0), pieces(0) {
        ensureCapacity(
                initialElements > UTILS_BITSET_PIECE_SIZE ? initialElements
                        : UTILS_BITSET_PIECE_SIZE);
    }
    BitSet(BitSet const & set) :
        nElements(0), allocElements(0), pieces(0) {
        (*this) = set;
    }
    ~BitSet() {
        delete[] pieces;
    }

    BitSet & operator=(BitSet const & set) {
        if (this != &set) {
            delete[] pieces;
            allocElements = set.allocElements;
            nElements = set.nElements;
            size_t blocks = allocElements / UTILS_BITSET_PIECE_SIZE;
            ASSERTEQUALS(allocElements % UTILS_BITSET_PIECE_SIZE, (size_t) 0);
            pieces = new std::bitset<UTILS_BITSET_PIECE_SIZE>[blocks];
            for (size_t i = 0; i < blocks; i++) {
                pieces[i] = set.pieces[i];
            }
        }
        return *this;
    }

    size_t capacity() const {
        return allocElements;
    }

    size_t size() const {
        return nElements;
    }

    bool operator[](size_t pos) const {
        if (pos > nElements)
            throw util::IndexOutOfBounds(pos, nElements);
        return pieces[pos / UTILS_BITSET_PIECE_SIZE][pos
                % UTILS_BITSET_PIECE_SIZE];
    }

    std::bitset<UTILS_BITSET_PIECE_SIZE>::reference operator[](size_t pos) {
        if (pos >= allocElements) {
            ensureCapacity(pos + 1);
        }
        if (pos >= nElements) {
            nElements = pos + 1;
        }
        return pieces[pos / UTILS_BITSET_PIECE_SIZE][pos
                % UTILS_BITSET_PIECE_SIZE];
    }

    void ensureCapacity(size_t size);

private:
    static size_t numBlocks(size_t numBits) {
        return numBits / UTILS_BITSET_PIECE_SIZE + ((numBits
                % UTILS_BITSET_PIECE_SIZE) != 0 ? 1 : 0);
    }
    size_t numBlocks() const {
        return numBlocks(allocElements);
    }

    friend std::ostream
    & operator<<(std::ostream & os, util::BitSet const & bs);
};

std::ostream & operator<<(std::ostream & os, util::BitSet const & bs);

}

#endif // !defined(UTIL_BITSET_HPP)
