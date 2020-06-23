#if !defined(UTIL_COLLECTIONS_HPP)
#define UTIL_COLLECTIONS_HPP

#include <set>
#include <vector>
#include <list>
#include <deque>
#include <map>

namespace util {

class Collections {
public:
    template<typename T>
    static void removeLessThan(std::set<T> & set, T const & x);

    template<typename T>
    static void ensureSize(std::set<T> & collection, size_t size);

    template<typename T>
    static void ensureSize(std::list<T> & collection, size_t size);

    template<typename T>
    static void ensureSize(std::vector<T> & collection, size_t size);

    template<typename T>
    static bool contains(std::set<T> const & set, T const & x);

    template<typename T>
    static bool contains(std::list<T> const & list, T const & x);

    template<typename K, typename V>
    static bool contains(std::map<K, V> const & list, K const & x);

    template<typename T>
    static std::set<T> makeSet(size_t numItems, T firstElement, ...);

};

} // End of namespace util

//template<typename T>
//std::ostream & operator<<(std::ostream & os, std::vector<T> const & ar);
//
//template<typename T>
//std::ostream & operator<<(std::ostream & os, std::deque<T> const & ar);
//
//template<typename T>
//std::ostream & operator<<(std::ostream & os, std::list<T> const & li);
//
//template<typename T>
//std::ostream & operator<<(std::ostream & os, std::set<T> const & st);
//
//template<typename T1, typename T2>
//std::ostream & operator<<(std::ostream & os, std::pair<T1, T2> const & pair);

#include "util/CollectionsTempl.hpp"

#endif // !defined(UTIL_COLLECTIONS_HPP)
