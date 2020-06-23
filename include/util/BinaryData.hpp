#if !defined(UTIL_BINARYDATA_HPP)
#define UTIL_BINARYDATA_HPP

/*
 * A storage class for storing/transporting binary data. Automatically adjusts
 * the size of the backing storage array. The unused part of the array is always
 * initialized to contain zeros.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-06-22
 */

#include "util/String.hpp"
#include <cstddef>

namespace util {

class BinaryData {
public:
    static size_t const MinimumCapacity = 16;

    /** Creates an empty data array of capacity initialCapacity */
    BinaryData(size_t initialCapacity);
    /** Creates a data array. Copies length characters from the given source */
    BinaryData(char const * source, size_t length);
    /** Creates a data array that contains the characters of a string */
    BinaryData(String const & string);
    /** Copies the binary data */
    BinaryData(BinaryData const & other);
    /** Assignment operator */
    BinaryData const & operator=(BinaryData const & other);
    /** Destroys the binary data array, freeing used resources */
    ~BinaryData();

    /** 
     * Writes length characters from source to the beginning of the data 
     * array. Equivalent to call write(0, source, length)
     */
    bool write(char const * source, size_t length);
    /** 
     * Writes length characters from source to a point statring offset 
     * characters from the beginning of the data array
     */
    bool write(size_t offset, char const * source, size_t length);

    /** 
     * Writes length characters from source to the end of the current data
     * array. Equivalent to call write(size, source, length)
     */
    bool append(char const * source, size_t length);

    /** Clears the data array */
    void clear();

    /** 
     * Ensures that the capacity of the data array is at least the given size
     * Returns true on success, or false if an error occured (out of memory) 
     */
    bool ensureCapacity(size_t size);

    /** Returns the size of the data */
    size_t getSize() const {
        return size;
    }

    size_t getCapacity() const {
        return capacity;
    }

    /** Returns a pointer to the data array at given offset for reading */
    char const * getData(size_t offset = 0) const;

    /** Returns a pointer to the data array for reading at given offset */
    char * getDataForWriting(size_t offset = 0);

    /** 
     * Returns the data as a string (note: this will not work correctly if the
     * data contains any nul-characters ('\0')) 
     */
    String getString() const;

private:
    size_t size;
    size_t capacity;
    size_t initialCapacity;
    char * data;
};

}

#endif // !defined(UTIL_BINARYDATA_HPP)
