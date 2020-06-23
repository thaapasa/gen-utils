#if !defined(UTIL_ITERATOR_HPP)
#define UTIL_ITERATOR_HPP

/*
 * Defines a Java-style iterator that can be used to traverse through
 * an unspecified list and knows the end itself. Can be used in interfaces,
 * must be implemented elsewhere.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-07-28
 */

#include "util/HashMap.hpp"

#include <vector>
#include <deque>

namespace util {

template<class T>
class Iterator {
public:
    virtual ~Iterator() {
    }

    /* Are there any more items to be iterated */
    virtual bool hasNext() const = 0;

    /* Returns the next object in the iterator */
    virtual T next() = 0;

    /* Peeks at the next object without moving the iterator ahead */
    virtual T peek() const = 0;

    /* Resets the iterator to beginning */
    virtual void reset() = 0;

};

/*
 * Iterator for std::vector
 */
template<class T>
class VectorIterator: public Iterator<T> {
public:
    VectorIterator(std::vector<T> const & src_) :
        src(src_), current(src_.begin()) {
    }

    /* Are there any more items to be iterated */
    virtual bool hasNext() const {
        return current != src.end();
    }

    /* Returns the next object in the iterator */
    virtual T next() {
        return *current++;
    }

    /* Peeks at the next object without moving the iterator ahead */
    virtual T peek() const {
        return *current;
    }

    /* Resets the iterator to beginning */
    virtual void reset() {
        current = src.begin();
    }

private:
    std::vector<T> const & src;
    typename std::vector<T>::const_iterator current;
};

/*
 * Iterator for std::deque
 */
template<class T>
class DequeIterator: public Iterator<T> {
public:
    DequeIterator(std::deque<T> const & src_) :
        src(src_), current(src_.begin()) {
    }

    /* Are there any more items to be iterated */
    virtual bool hasNext() const {
        return current != src.end();
    }

    /* Returns the next object in the iterator */
    virtual T next() {
        return *current++;
    }

    /* Peeks at the next object without moving the iterator ahead */
    virtual T peek() const {
        return *current;
    }

    /* Resets the iterator to beginning */
    virtual void reset() {
        current = src.begin();
    }

private:
    std::deque<T> const & src;
    typename std::deque<T>::const_iterator current;
};

/*
 * Iterator for std::strings
 */
class StringIterator: public Iterator<char> {
public:
    StringIterator(std::string const & src_) :
        src(src_), current(src_.begin()) {
    }

    /* Are there any more items to be iterated */
    virtual bool hasNext() const {
        return current != src.end();
    }

    /* Peeks at the next object without moving the iterator ahead */
    virtual char peek() const {
        return *current;
    }

    /* Returns the next object in the iterator */
    virtual char next() {
        char value = *current;
        ++current;
        return value;
    }

    /* Resets the iterator to beginning */
    virtual void reset() {
        current = src.begin();
    }

private:
    std::string const & src;
    std::string::const_iterator current;
};

/*
 * Iterator for std::hash_map values
 */
template<class K, class V, class _HashFcn = sgi::hash<K>,
        class _EqualKey = std::equal_to<K> >
class HashMapValueIterator: public Iterator<V> {
public:
    HashMapValueIterator(sgi::hash_map<K, V, _HashFcn, _EqualKey> const & src_) :
        src(src_), current(src_.begin()) {
    }

    /* Are there any more items to be iterated */
    virtual bool hasNext() const {
        return current != src.end();
    }

    /* Peeks at the next object without moving the iterator ahead */
    virtual V peek() const {
        return current->second;
    }

    /* Returns the next object in the iterator */
    virtual V next() {
        return (current++)->second;
    }

    /* Resets the iterator to beginning */
    virtual void reset() {
        current = src.begin();
    }

private:
    typedef sgi::hash_map<K, V, _HashFcn, _EqualKey> type;
    type const & src;
    typename type::const_iterator current;
};

/*
 * Iterator for std::hash_map keys
 */
template<class K, class V, class _HashFcn = sgi::hash<K>,
        class _EqualKey = std::equal_to<K> >
class HashMapKeyIterator: public Iterator<K> {
public:
    HashMapKeyIterator(sgi::hash_map<K, V, _HashFcn, _EqualKey> const & src_) :
        src(src_), current(src_.begin()) {
    }

    /* Are there any more items to be iterated */
    virtual bool hasNext() const {
        return current != src.end();
    }

    /* Peeks at the next object without moving the iterator ahead */
    virtual K peek() const {
        return current->first;
    }

    /* Returns the next object in the iterator */
    virtual K next() {
        return (current++)->first;
    }

    /* Resets the iterator to beginning */
    virtual void reset() {
        current = src.begin();
    }

private:
    typedef sgi::hash_map<K, V, _HashFcn, _EqualKey> type;
    type const & src;
    typename type::const_iterator current;
};

}

#endif // !defined(UTIL_ITERATOR_HPP)
