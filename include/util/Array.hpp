#if !defined(UTIL_ARRAY_HPP)
#define UTIL_ARRAY_HPP

#include "util/exception/Exception.hpp"

/*
 * A dynamic array class. Wrapper for a normal C array.
 *
 * @author Tuukka Haapasalo
 * @created 2010-11-30
 */

#define UTILS_ARRAY_DEFAULT_CAPACITY 8

namespace util {
template<typename T>
class Array {
private:
    size_t nElements;
    size_t allocElements;
    T * vals;
public:
    Array() :
        nElements(0), allocElements(0), vals(0) {
        ensureCapacity(UTILS_ARRAY_DEFAULT_CAPACITY);
    }
    Array(size_t initialElements) :
        nElements(0), allocElements(0), vals(0) {
        ensureCapacity(
                initialElements > UTILS_ARRAY_DEFAULT_CAPACITY ? initialElements
                        : UTILS_ARRAY_DEFAULT_CAPACITY);
        nElements = initialElements;
    }
    Array(Array const & ar) :
        nElements(0), allocElements(0), vals(0) {
        (*this) = ar;
    }
    ~Array() {
        delete[] vals;
    }

    Array & operator=(Array const & ar) {
        if (this != &ar) {
            delete[] vals;
            allocElements = ar.allocElements;
            nElements = ar.nElements;
            vals = new T[allocElements];
            for (size_t i = 0; i < nElements; i++) {
                vals[i] = ar.vals[i];
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

    T & operator[](size_t pos) {
        return element(pos);
    }

    T const & operator[](size_t pos) const {
        return element(pos);
    }

    T & element(size_t pos) {
        if (pos >= allocElements) {
            ensureCapacity(pos + 1);
        }
        if (pos >= nElements) {
            nElements = pos + 1;
        }
        return vals[pos];
    }

    T const & element(size_t pos) const {
        if (pos >= nElements)
            throw util::IndexOutOfBounds(pos, nElements);
        return vals[pos];
    }

    void add(T const & item) {
        element(nElements) = item;
    }

    void ensureCapacity(size_t size);

    template<typename E>
    friend std::ostream & operator<<(std::ostream & os,
            util::Array<E> const & ar);
};

template<typename T>
std::ostream & operator<<(std::ostream & os, util::Array<T> const & ar) {
    os << "[";
    for (size_t i = 0; i < ar.size(); i++) {
        if (i != 0)
            os << ", ";
        os << ar[i];
    }
    os << "]";
    return os;
}

template<typename T>
void Array<T>::ensureCapacity(size_t ensureSize) {
    if (allocElements >= ensureSize)
        return;
    allocElements *= 2;
    if (allocElements < ensureSize) {
        allocElements = ensureSize;
    }
    T * oldVals = vals;
    vals = new T[allocElements]();
    for (size_t i = 0; i < nElements; i++) {
        // Can't do memcpy, because the elements may be objects that have constructors & destructors
        vals[i] = oldVals[i];
    }
    delete[] oldVals;
}
}

#endif // !defined(UTIL_ARRAY_HPP)
