#include "util/BinaryData.hpp"

/*
 * A storage class for storing/transporting binary data. Automatically adjusts
 * the size of the backing storage array. The unused part of the array is always
 * initialized to contain zeros.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-06-22
 * 
 * $Id: BinaryData.cpp,v 1.3 2005-02-25 14:55:26 tuska Exp $
 */

#include "util/Log.hpp"
/*
 * NOTE: the allocation of the binary data array is done with the c-style
 * memory allocation to allow reallocating the array when expanding it. This 
 * is, of course, totally invisible to the user.
 */
#include <cstdlib>
#include <cstring>

using namespace util;

/** 
 * Creates an empty data array of capacity initialCapacity 
 */
BinaryData::BinaryData(size_t initialCapacity) :
    size(0), capacity(0), initialCapacity(0), data(0) {
    ASSERT(initialCapacity >= 0);
    ensureCapacity(initialCapacity);
    initialCapacity = capacity;
}

/** 
 * Creates a data array. Copies length characters from the given source 
 */
BinaryData::BinaryData(char const * source, size_t length) :
    size(0), capacity(0), initialCapacity(0), data(0) {
    ASSERT(length >= 0);
    ASSERT(source != NULL);
    write(source, length);
    initialCapacity = capacity;
}

/** 
 * Creates a data array that contains the characters of a string
 */
BinaryData::BinaryData(String const & string) :
    size(0), capacity(0), initialCapacity(0), data(0) {
    write(string.c_str(), string.size());
    initialCapacity = capacity;
}

/** 
 * Copies the binary data 
 */
BinaryData::BinaryData(BinaryData const & other) :
    size(0), capacity(0), initialCapacity(0), data(0) {
    write(other.data, other.size);
    initialCapacity = capacity;
}

/** 
 * Assignment operator 
 */
BinaryData const & BinaryData::operator=(BinaryData const & other) {
    clear();
    write(other.data, other.size);
    initialCapacity = capacity;
    return *this;
}

/** 
 * Destroys the binary data array, freeing used resources 
 */
BinaryData::~BinaryData() {
    clear();
}

/** 
 * Writes length characters from source to the beginning of the data 
 * array. Equivalent to call write(0, source, length)
 */
bool BinaryData::write(char const * source, size_t length) {
    return write(0, source, length);
}

/** 
 * Writes length characters from source to a point statring offset 
 * characters from the beginning of the data array
 */
bool BinaryData::write(size_t offset, char const * source, size_t length) {
    size_t neededSize = offset + length;
    if (!ensureCapacity(neededSize))
        return false;

    memcpy(data + offset, source, length);
    if (size < neededSize)
        size = neededSize;
    return true;
}

/** 
 * Writes length characters from source to the end of the current data
 * array. Equivalent to call write(size, source, length)
 */
bool BinaryData::append(char const * source, size_t length) {
    return write(size, source, length);
}

/** 
 * Ensures that the capacity of the data array is at least the given size
 * Returns true on success, or false if an error occured (out of memory) 
 */
bool BinaryData::ensureCapacity(size_t size) {
    if (capacity >= size)
        return true;

    size_t newCapacity = capacity > MinimumCapacity ? capacity
            : MinimumCapacity;
    while (newCapacity < size)
        newCapacity *= 2;
    // newCapacity now holds the new capacity
    // Chech that newCapacity is not less than the initial capacity
    if (newCapacity < initialCapacity)
        newCapacity = initialCapacity;

    char * newData = (char *) realloc(data, newCapacity * sizeof(char));
    if (newData == NULL) {
        return false;
    }
    // Clear the allocated memory (not the original data!)
    memset(newData + capacity, 0, newCapacity - capacity);

    // Set the member variables
    capacity = newCapacity;
    data = newData;
    return true;
}

/** 
 * Clears the data array 
 */
void BinaryData::clear() {
    free( data);
    data = 0;
    size = 0;
    capacity = 0;
}

/** 
 * Returns the data as a string (note: this will not work correctly if the
 * data contains any nul-characters ('\0')) 
 */
String BinaryData::getString() const {
    return std::string(data, size);
}

/** 
 * Returns a pointer to the data array at given offset for reading 
 */
char const * BinaryData::getData(size_t offset) const {
    ASSERT(offset >= 0);
    return data + offset;
}

/**
 * Returns a pointer to the data array for reading at given offset 
 */
char * BinaryData::getDataForWriting(size_t offset) {
    ASSERT(offset >= 0);
    return data + offset;
}

