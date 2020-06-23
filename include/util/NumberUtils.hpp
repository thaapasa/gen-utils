#if !defined(UTIL_NUMBERUTILS_HPP)
#define UTIL_NUMBERUTILS_HPP

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502884197
#endif // M_PI
/*
 * Provides basic numerical utilities
 * 
 * @author Tuukka Haapasalo
 * @created 2004-07-27
 */

namespace util {

/*
 * Tests for double "near-equality", that is, the values must be
 * near each other (with given precision)
 */
extern bool equals(double a, double b, double const precision = 0.000001);

/*
 * Converts degrees to radians
 */
inline double degreesToRadians(double angle) {
    return (angle * M_PI) / 180.0;
}

/*
 * Converts radians to degrees
 */
inline double radiansToDegrees(double angle) {
    return (angle * 180.0) / M_PI;
}

/*
 * Returns the angle of the given point (from origo) in degrees
 * 0 is towards +x, 90 towards +y, 180 towards -x and 270 towards -y
 */
extern double angleDegrees(double x, double y);

/* Returns the angle of the given point (from origo) in degrees */
extern double angleRadians(double x, double y);

/*
 * Returns the value that is congruent with the given value modulo maximum
 * so that 0 <= result < maximum (or would be, if the same mathematical
 * laws would apply to decimal numbers).
 */
extern double congruent(double value, double maximum);
/*
 * Returns the value that is congruent with the given value so that
 * minimum <= value < maximum.
 */
extern double congruent(double value, double minimum, double maximum);

/*
 * Returns the value that is congruent with the given value modulo maximum
 * so that 0 <= result < maximum.
 */
extern int congruent(int value, int maximum);
/*
 * Returns the value that is congruent with the given value so that
 * minimum <= value < maximum.
 */
extern int congruent(int value, int minimum, int maximum);

}
;

#endif // !defined(UTIL_NUMBERUTILS_HPP)
