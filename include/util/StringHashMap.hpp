#if !defined(UTIL_STRING_HASHMAP_HPP)
#define UTIL_STRING_HASHMAP_HPP

/*
 * Helper functions and definitions for a string-keyed hash map
 * 
 * @author Tuukka Haapasalo
 * @created 2004-08-02
 */

#include "util/HashMap.hpp"
#include "util/String.hpp"
#include "util/Iterator.hpp"

namespace util {

/*
 * Equality comparison to use with hash maps
 */
class StringEquals {
public:
    bool operator()(String const & s1, String const & s2) const {
        return s1 == s2;
    }
};

/*
 * Hashing function to use with hash maps
 */
class StringHash {
public:
    size_t operator()(String const & s) const {
        sgi::hash<char const *> H;
        return H(s.c_str());
    }
};
}

namespace util {

/**
 * A specialization for a String-keyed hash_map
 */
template<typename x>
class StringHashMap: public sgi::hash_map<String const, x, StringHash,
        StringEquals> {
};

/**
 * Value iterator for StringHashMap
 */
template<typename x>
class StringHashMapValueIterator: public HashMapValueIterator<String const, x,
        StringHash, StringEquals> {
public:
    StringHashMapValueIterator(StringHashMap<x> const & src) :
        HashMapValueIterator<String const, x, StringHash, StringEquals> (src) {
    }
};

/**
 * Key iterator for StringHashMap
 */
template<typename x>
class StringHashMapKeyIterator: public HashMapKeyIterator<String const, x,
        StringHash, StringEquals> {
public:
    StringHashMapKeyIterator(StringHashMap<x> const & src) :
        HashMapKeyIterator<String const, x, StringHash, StringEquals> (src) {
    }
};

}

#endif // !defined(UTIL_STRING_HASHMAP_HPP)
