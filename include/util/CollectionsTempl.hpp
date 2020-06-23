#if !defined(UTIL_COLLECTIONS_TEMPLATES_HPP)
#define UTIL_COLLECTIONS_TEMPLATES_HPP

#include <cstdarg>

// --------------------------------------------------------------------
// General collection functions
// --------------------------------------------------------------------

namespace util {

template<typename T>
void Collections::removeLessThan(std::set<T> & st, T const & x) {
    if (st.empty())
        return;
    typename std::set<T>::iterator it = st.lower_bound(x);
    if (it == st.end()) {
        // No item was greater or equal, clear whole set
        st.clear();
        return;
    } else if (it == st.begin()) {
        // Nothing to remove
        ASSERT(*it >= x);
        return;
    } else {
        ASSERT(*it >= x);
        st.erase(st.begin(), it);
        return;
    }
}

template<typename T>
void Collections::ensureSize(std::set<T> & collection, size_t size) {
    if (collection.size() < size) {
        collection.resize(size);
    }
}

template<typename T>
void Collections::ensureSize(std::list<T> & collection, size_t size) {
    if (collection.size() < size) {
        collection.resize(size);
    }
}

template<typename T>
void Collections::ensureSize(std::vector<T> & collection, size_t size) {
    if (collection.size() < size) {
        collection.resize(size);
    }
}

template<typename T>
bool Collections::contains(std::set<T> const & set, T const & x) {
    return set.find(x) != set.end();
}

template<typename T>
bool Collections::contains(std::list<T> const & list, T const & x) {
    return list.find(x) != list.end();
}

template<typename K, typename V>
bool Collections::contains(std::map<K, V> const & map, K const & x) {
    return map.find(x) != map.end();
}

template<typename T>
std::set<T> Collections::makeSet(size_t numItems, T firstElement, ...) {
    std::set<T> set;
    if (numItems < 1)
        return set;
    set.insert(firstElement);
    va_list ap;
    va_start(ap, firstElement);
    for (; numItems > 1; --numItems) {
        T item = va_arg(ap, T);
        set.insert(item);
    }
    va_end(ap);
    return set;
}

} // End of namespace util


// --------------------------------------------------------------------
// Printing functions
// --------------------------------------------------------------------

template<typename T>
std::ostream & operator<<(std::ostream & os, std::vector<T> const & ar) {
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
std::ostream & operator<<(std::ostream & os, std::deque<T> const & ar) {
    os << "[";
    bool first = true;
    for (typename std::deque<T>::const_iterator it = ar.begin(); it != ar.end(); it++) {
        if (!first)
            os << ", ";
        os << *it;
        first = false;
    }
    os << "]";
    return os;
}

template<typename T>
std::ostream & operator<<(std::ostream & os, std::list<T> const & li) {
    os << "[";
    bool first = true;
    for (typename std::list<T>::const_iterator it = li.begin(); it != li.end(); it++) {
        if (!first)
            os << ", ";
        os << *it;
        first = false;
    }
    os << "]";
    return os;
}

template<typename T>
std::ostream & operator<<(std::ostream & os, std::set<T> const & st) {
    os << "{";
    bool first = true;
    for (typename std::set<T>::const_iterator it = st.begin(); it != st.end(); it++) {
        if (!first)
            os << ", ";
        os << *it;
        first = false;
    }
    os << "}";
    return os;
}

template<typename T1, typename T2>
std::ostream & operator<<(std::ostream & os, std::pair<T1, T2> const & pair) {
    os << "<" << pair.first << ", " << pair.second << ">";
    return os;
}

#endif // UTIL_COLLECTIONS_TEMPLATES_HPP
