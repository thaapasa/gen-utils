#if !defined(UTIL_VECTOR2D_HPP)
#define UTIL_VECTOR2D_HPP

/*
 * A simple 2D vector template. The angles are stored as radians.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-10-21
 */

#include "util/NumberUtils.hpp"
#include "util/Log.hpp"
#include <cmath>

#define TWOPI (M_PI * 2.0)

namespace util {

template<typename T>
class Vector2D {
public:
    /* Creates a vector pointing at the origin */
    Vector2D();
    /* 
     * Creates a vector pointing given coordinates. Coordinates are by
     * default given in the cartesian form, but if the third parameter
     * is false, they are interpreted as polar coordinates with the 
     * angle given as radians.
     * 
     * When given as polar coordinates, the first coordinate is angle,
     * and the second is radius.
     */
    Vector2D(T p1, T p2, bool cartesian = true);
    /* Copies a vector */
    Vector2D(Vector2D const & vector);
    /* Assigns a vector */
    Vector2D const & operator=(Vector2D const & vector);
    /* Destroys a vector */
    ~Vector2D();

    /* Compares two vector for equality */
    bool operator==(Vector2D const & vector) const;
    /* Compares two vector for inequality */
    bool operator!=(Vector2D const & vector) const;

    /* Multiplies the vector with a given scalar */
    Vector2D operator*(T scalar) const;
    /* Multiplies this vector with a given scalar */
    Vector2D const & operator*=(T scalar);

    /* Divides the vector with a given scalar */
    Vector2D operator/(T scalar) const;
    /* Divides this vector with a given scalar */
    Vector2D const & operator/=(T scalar);

    /* Adds the vector with another vector */
    Vector2D operator+(Vector2D const & vec) const;
    /* Adds given vector to this vector */
    Vector2D const & operator+=(Vector2D const & vec);

    /* Subtracts the vector from another vector */
    Vector2D operator-(Vector2D const & vec) const;
    /* Subtracts given vector to this vector */
    Vector2D const & operator-=(Vector2D const & vec);

    /* Takes the vector that is opposite to this vector */
    Vector2D operator-() const;

    /* Sets the polar form of the vector in radians */
    void setPolarRadians(double const angle, double const radius);
    /* Sets the polar form of the vector in degrees */
    void setPolarDegrees(double const angle, double const radius);
    /* Sets the cartesian form of the vector */
    void setCartesian(double const x, double const y);

    /* Returns the x-coordinate of the vector */
    T x() const;
    /* Returns the y-coordinate of the vector */
    T y() const;

    /* Returns the angle of the (polar) vector in radians */
    double angleRadians() const;
    /* Returns the angle of the (polar) vector in degrees */
    double angleDegrees() const;
    /* Returns the radius of the (polar) vector */
    double radius() const;

    /* 
     * Returns the angle towards another vector in degrees.
     * Leftwards on x-axis is 0, upwards on y-axis is 90, and so on
     */
    double angleTowardsDegrees(Vector2D<T> const & other) const;
    /* 
     * Returns the angle towards another vector in radians.
     * Leftwards on x-axis is 0, upwards on y-axis is PI/2, and so on
     */
    double angleTowardsRadians(Vector2D<T> const & other) const;

    /* Calculates the distance to the given vector */
    double distance(Vector2D<T> const & c) const;

    /* Scales the length of the vector to the given value */
    Vector2D<T> scaleRadiusTo(double radius) const;

private:
    /* Calculates the cartesian form of the vector from the polar form */
    void toCartesian() const;
    /* Calculates the polar form of the vector from the cartesian form */
    void toPolar() const;

private:
    mutable bool isCartesian;
    mutable double xCo;
    mutable double yCo;
    mutable bool isPolar;
    mutable double ang;
    mutable double rad;

};

}

/*
 *-----------------------------------------------------------------------------
 * Template method definitions
 *-----------------------------------------------------------------------------
 */

/* 
 * Creates a vector pointing at the origin
 */
template<typename T>
util::Vector2D<T>::Vector2D() :
    isCartesian(true), xCo(0), yCo(0), isPolar(true), ang(0), rad(0) {
}

/* 
 * Creates a vector pointing given coordinates. Coordinates are by
 * default given in the cartesian form, but if the third parameter
 * is false, they are interpreted as polar coordinates with the 
 * angle given as radians.
 */
template<typename T>
util::Vector2D<T>::Vector2D(T p1, T p2, bool cartesian) :
    isCartesian(false), xCo(0), yCo(0), isPolar(false), ang(0), rad(0) {
    if (cartesian) {
        // p1 is X-coordinate
        // p2 is Y-coordinate
        setCartesian(p1, p2);
    } else {
        // p1 is angle in radians
        // p2 is radius
        // If radius is negative, flip the vector
        if (p2 < 0) {
            p2 = -p2;
            p1 += M_PI;
        }
        setPolarRadians(p1, p2);
    }
}

/* 
 * Copies a vector 
 */
template<typename T>
util::Vector2D<T>::Vector2D(Vector2D<T> const & vector) :
    isCartesian(vector.isCartesian), xCo(vector.xCo), yCo(vector.yCo), isPolar(
            vector.isPolar), ang(vector.ang), rad(vector.rad) {
}

/* 
 * Assigns a vector 
 */
template<typename T>
util::Vector2D<T> const & util::Vector2D<T>::operator=(Vector2D const & vector) {
    isCartesian = vector.isCartesian;
    xCo = vector.xCo;
    yCo = vector.yCo;
    isPolar = vector.isPolar;
    ang = vector.ang;
    rad = vector.rad;
    return *this;
}

/* 
 * Destroys a vector 
 */
template<typename T>
util::Vector2D<T>::~Vector2D() {
}

/* 
 * Compares two vector for equality 
 */
template<typename T>
bool util::Vector2D<T>::operator==(Vector2D const & vector) const {
    if (isCartesian) {
        if (!vector.isCartesian)
            vector.toCartesian();
        return xCo == vector.xCo && yCo == vector.yCo;
    }
    ASSERT(isPolar);
    if (vector.isPolar)
        return rad == vector.rad && ang == vector.ang;
    ASSERT(vector.isCartesian);
    toCartesian();
    return xCo == vector.xCo && yCo == vector.yCo;
}

/* 
 * Compares two vector for inequality 
 */
template<typename T>
bool util::Vector2D<T>::operator!=(Vector2D const & vector) const {
    return !(*this == vector);
}

/* 
 * Multiplies the vector with a given scalar 
 */
template<typename T>
util::Vector2D<T> util::Vector2D<T>::operator*(T scalar) const {
    Vector2D a(*this);
    a *= scalar;
    return a;
}

/* 
 * Multiplies this vector with a given scalar
 */
template<typename T>
util::Vector2D<T> const & util::Vector2D<T>::operator*=(T scalar) {
    if (isCartesian) {
        xCo *= scalar;
        yCo *= scalar;
        isPolar = false;
    } else {
        ASSERT(isPolar);
        rad *= scalar;
        isCartesian = false;
    }
    return *this;
}

/* 
 * Divides the vector with a given scalar 
 */
template<typename T>
util::Vector2D<T> util::Vector2D<T>::operator/(T scalar) const {
    Vector2D a(*this);
    a /= scalar;
    return a;
}

/* 
 * Divides this vector with a given scalar
 */
template<typename T>
util::Vector2D<T> const & util::Vector2D<T>::operator/=(T scalar) {
    if (isCartesian) {
        xCo /= scalar;
        yCo /= scalar;
        isPolar = false;
    } else {
        ASSERT(isPolar);
        rad /= scalar;
        isCartesian = false;
    }
    return *this;
}

/* 
 * Adds the vector with another vector 
 */
template<typename T>
util::Vector2D<T> util::Vector2D<T>::operator+(Vector2D const & vec) const {
    Vector2D a(*this);
    a += vec;
    return a;
}

/* 
 * Adds given vector to this vector 
 */
template<typename T>
util::Vector2D<T> const & util::Vector2D<T>::operator+=(Vector2D const & vec) {
    if (!isCartesian)
        toCartesian();
    if (!vec.isCartesian)
        vec.toCartesian();
    xCo += vec.xCo;
    yCo += vec.yCo;
    isPolar = false;
    return *this;
}

/* 
 * Substracts another vector from this vector 
 */
template<typename T>
util::Vector2D<T> util::Vector2D<T>::operator-(Vector2D const & vec) const {
    Vector2D a(*this);
    a -= vec;
    return a;
}

/* 
 * Subtracts given vector from this vector 
 */
template<typename T>
util::Vector2D<T> const & util::Vector2D<T>::operator-=(Vector2D const & vec) {
    if (!isCartesian)
        toCartesian();
    if (!vec.isCartesian)
        vec.toCartesian();
    xCo -= vec.xCo;
    yCo -= vec.yCo;
    isPolar = false;
    return *this;
}

/* 
 * Takes the vector that is opposite to this vector 
 */
template<typename T>
util::Vector2D<T> util::Vector2D<T>::operator-() const {
    Vector2D<T> a = *this;
    if (a.isCartesian) {
        a.setCartesian(-a.x(), -a.y());
        return a;
    } else {
        ASSERT(a.isPolar);
        a.setPolarDegrees(a.angleRadians() + M_PI, a.radius());
        return a;
    }
}

/* 
 * Sets the polar form of the vector, with angle given in radians
 */
template<typename T>
void util::Vector2D<T>::setPolarRadians(double angle, double radius) {
    ang = util::congruent(angle, TWOPI);
    rad = radius;
    isPolar = true;
    isCartesian = false;
}

/* 
 * Sets the polar form of the vector, with angle given in degrees
 */
template<typename T>
void util::Vector2D<T>::setPolarDegrees(double angle, double radius) {
    setPolarRadians(degreesToRadians(angle), radius);
}

/* 
 * Sets the cartesian form of the vector 
 */
template<typename T>
void util::Vector2D<T>::setCartesian(double x, double y) {
    xCo = x;
    yCo = y;
    isPolar = false;
    isCartesian = true;
}

/* 
 * Returns the x-coordinate of the vector 
 */
template<typename T>
T util::Vector2D<T>::x() const {
    if (!isCartesian)
        toCartesian();
    return xCo;
}

/* 
 * Returns the y-coordinate of the vector 
 */
template<typename T>
T util::Vector2D<T>::y() const {
    if (!isCartesian)
        toCartesian();
    return yCo;
}

/* 
 * Returns the angle of the (polar) vector in radians 
 */
template<typename T>
double util::Vector2D<T>::angleRadians() const {
    if (!isPolar)
        toPolar();
    return ang;
}

/* 
 * Returns the angle of the (polar) vector in degrees 
 */
template<typename T>
double util::Vector2D<T>::angleDegrees() const {
    if (!isPolar)
        toPolar();
    return radiansToDegrees(ang);
}

/* 
 * Returns the radius of the (polar) vector 
 */
template<typename T>
double util::Vector2D<T>::radius() const {
    if (!isPolar)
        toPolar();
    return rad;
}

/* 
 * Returns the angle towards another vector in degrees.
 * Leftwards on x-axis is 0, upwards on y-axis is 90, and so on
 */
template<typename T>
double util::Vector2D<T>::angleTowardsDegrees(Vector2D<T> const & other) const {
    return (other - *this).angleDegrees();
}

/* 
 * Returns the angle towards another vector in radians.
 * Leftwards on x-axis is 0, upwards on y-axis is PI/2, and so on
 */
template<typename T>
double util::Vector2D<T>::angleTowardsRadians(Vector2D<T> const & other) const {
    return (other - *this).angleRadians();
}

/* 
 * Calculates the distance to the given vector 
 */
template<typename T>
double util::Vector2D<T>::distance(Vector2D<T> const & c) const {
    return sqrt((x() - c.x()) * (x() - c.x()) + (y() - c.y()) * (y() - c.y()));
}

/* 
 * Scales the length of the vector to the given value 
 */
template<typename T>
util::Vector2D<T> util::Vector2D<T>::scaleRadiusTo(double rad) const {
    double len = radius();
    if (len == rad)
        return *this;
    return *this * (rad / len);
}

/* 
 * Calculates the cartesian form of the vector from the polar form 
 */
template<typename T>
void util::Vector2D<T>::toCartesian() const {
    ASSERT(isPolar);
    ASSERT(!isCartesian);
    xCo = cos(ang) * rad;
    yCo = sin(ang) * rad;
    isCartesian = true;
}

/* 
 * Calculates the polar form of the vector from the cartesian form 
 */
template<typename T>
void util::Vector2D<T>::toPolar() const {
    ASSERT(!isPolar);
    ASSERT(isCartesian);
    ang = util::angleRadians(xCo, yCo);
    rad = ::sqrt(xCo * xCo + yCo * yCo);
    isPolar = true;
}

/*
 * Multiplication the other way around
 */
template<typename T>
util::Vector2D<T> operator*(T scalar, util::Vector2D<T> const & vector) {
    return vector * scalar;
}

#endif // !defined(UTIL_VECTOR2D_HPP)
