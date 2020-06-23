#if !defined(UTIL_INTERVAL_HPP)
#define UTIL_INTERVAL_HPP

/*
 * Represents a closed intervals of numbers.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-08-28
 */

#include "util/String.hpp"
#include <sstream>

namespace util {

template<typename T>
class Interval {
public:
    /* Constructs an interval from point1 to point2 */
    inline Interval(T point1, T point2);
    /* Copies the interval */
    inline Interval(Interval<T> const & interval);
    /* Assigns the interval */
    inline Interval const & operator=(Interval<T> const & interval);

    /* Compares two intervals */
    inline bool operator==(Interval<T> const & interval) const;
    inline bool operator!=(Interval<T> const & interval) const;

    /* 
     * Adds another interval to this interval. 
     * If the intervals do not intersect, does not modify the interval.
     */
    inline Interval const & operator+=(Interval<T> const & interval);
    /* 
     * Adds the intervals together.
     * If the intervals do not intersect, returns this interval.
     */
    inline Interval operator+(Interval<T> const & interval) const;

    /* Returns the beginning of the interval */
    inline T begin() const;
    /* Returns the end of the interval */
    inline T end() const;

    /* Checks whether the interval contains the given value */
    inline bool contains(T const & value) const;

    /* Checks whether the interval wholly contains the given interval */
    inline bool contains(Interval<T> const & interval) const;

    /* Checks whether the interval intersects with the given interval */
    inline bool intersects(Interval<T> const & interval) const;

    /* Provides a textual representation of the interval */
    String toString() const;

private:
    T p1;
    T p2;

};
}

/*
 * Template method definitions
 */

/* 
 * Constructs an interval from point1 to point2 
 */
template<typename T>
util::Interval<T>::Interval(T point1, T point2) {
    if (point1 <= point2) {
        p1 = point1;
        p2 = point2;
    } else {
        p1 = point2;
        p2 = point1;
    }
}

/* 
 * Copies the interval 
 */
template<typename T>
util::Interval<T>::Interval(Interval<T> const & interval) :
    p1(interval.p1), p2(interval.p2) {
}

/* 
 * Assigns the interval 
 */
template<typename T>
util::Interval<T> const & util::Interval<T>::operator=(
        Interval<T> const & interval) {
    p1 = interval.p1;
    p2 = interval.p2;
}

/* 
 * Compares two intervals 
 */
template<typename T>
bool util::Interval<T>::operator==(Interval<T> const & interval) const {
    return p1 == interval.p1 && p2 == interval.p2;
}
template<typename T>
bool util::Interval<T>::operator!=(Interval<T> const & interval) const {
    return !(*this == interval);
}

/* 
 * Adds another interval to this interval. 
 * If the intervals do not intersect, does not modify the interval.
 */
template<typename T>
util::Interval<T> const & util::Interval<T>::operator+=(
        Interval<T> const & interval) {
    if (!intersects(interval))
        return *this;
    p1 = p1 < interval.p1 ? p1 : interval.p1;
    p2 = p2 > interval.p2 ? p2 : interval.p2;
    return *this;
}

/* 
 * Adds the intervals together.
 * If the intervals do not intersect, returns this interval.
 */
template<typename T>
util::Interval<T> util::Interval<T>::operator+(Interval<T> const & interval) const {
    Interval a(*this);
    a += interval;
    return a;
}

/* 
 * Returns the beginning of the interval 
 */
template<typename T>
T util::Interval<T>::begin() const {
    return p1;
}

/* 
 * Returns the end of the interval 
 */
template<typename T>
T util::Interval<T>::end() const {
    return p2;
}

/* 
 * Checks whether the interval contains the given value 
 */
template<typename T>
bool util::Interval<T>::contains(T const & value) const {
    return value >= p1 && value <= p2;
}

/* 
 * Checks whether the interval wholly contains the given interval 
 */
template<typename T>
bool util::Interval<T>::contains(Interval<T> const & interval) const {
    return p1 <= interval.p1 && p2 >= interval.p2;
}

/* 
 * Checks whether the interval intersects with the given interval 
 */
template<typename T>
bool util::Interval<T>::intersects(Interval<T> const & interval) const {
    return contains(interval.p1) || contains(interval.p2) || interval.contains(
            p1) || interval.contains(p2);
}

/*
 * Provides a textual description of the producer
 */
template<typename T>
util::String util::Interval<T>::toString() const {
    std::stringstream str;
    str << "[" << p1 << ", " << p2 << "]";
    return str.str();
}

#endif // !defined(UTIL_INTERVAL_HPP)
