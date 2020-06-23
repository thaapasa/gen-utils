#include "util/NumberUtils.hpp"

/*
 * Provides basic numerical utilities
 * 
 * @author Tuukka Haapasalo
 * @created 2004-07-27
 * 
 * $Id: NumberUtils.cpp,v 1.5 2005-04-21 13:16:41 tuska Exp $
 */

#include "util/Log.hpp"
#include <cmath>

#define MIN_DOUBLE_PRECISION 1e-10
#define ABS(a) ((a) < 0 ? -(a) : (a))

/*
 * Tests for double "near-equality", that is, the values must be
 * near each other (with given precision)
 */
bool util::equals(double const a, double const b, double const precision) {
    bool result = true;

    if (a == b) {
        return true;
    } else if (ABS(a) < precision || ABS(b) < precision) {
        return ABS(a - b) < precision;
    } else {
        double eps = ABS(a * precision);
        if (eps < MIN_DOUBLE_PRECISION)
            eps = MIN_DOUBLE_PRECISION;
        ASSERT(eps > 0);
        double dist = ABS(a - b);
        result = dist == 0 || dist <= eps;
    }

    if (!result) {
        LOG(a << " != " << b << " (diff: " << fabs(a - b) << ", precision: " <<
                fabs(a * precision));
    }
    return result;
}

/* 
 * Returns the angle of the given point (from origo) in degrees
 * 0 is towards +x, 90 towards +y, 180 towards -x and 270 towards -y
 */
double util::angleDegrees(double x, double y) {
    return radiansToDegrees(angleRadians(x, y));
}

/* 
 * Returns the angle of the given point (from origo) in degrees 
 */
double util::angleRadians(double x, double y) {
    if (y == 0)
        return x == 0 ? 0 : ((x > 0) ? 0 : M_PI);

    if (x >= 0 && y >= 0)
        return atan(y / x);
    else if (x < 0 && y >= 0)
        return M_PI + atan(y / x);
    else if (x < 0 && y < 0)
        return M_PI + atan(y / x);
    else if (x >= 0 && y < 0)
        return M_PI * 2.0 + atan(y / x);

        else
        ASSERT(false);

    // Not reached
    return 0;
}

/*
 * Returns the value that is congruent with the given value modulo maximum
 * so that 0 <= result < maximum (or would be, if the same mathematical 
 * laws would apply to decimal numbers).
 */
double util::congruent(double value, double maximum) {
    if (value < 0) {
        value += ((int) (-(value / maximum)) + 1) * maximum;
        ASSERT(value >= 0);
    }
    if (value >= maximum) {
        value -= ((int) (value / maximum)) * maximum;
        ASSERT(value < maximum);
    }
    return value;
}

/*
 * Returns the value that is congruent with the given value so that
 * minimum <= value < maximum.
 */
double util::congruent(double value, double minimum, double maximum) {
    if (minimum > maximum) {
        return congruent(value, maximum, minimum);
    }
    return congruent(value - minimum, maximum - minimum) + minimum;
}

/*
 * Returns the value that is congruent with the given value modulo maximum
 * so that 0 <= result < maximum.
 */
int util::congruent(int value, int maximum) {
    if (value < 0) {
        value += (-(value / maximum) + 1) * maximum;
        ASSERT(value >= 0);
    }
    if (value >= maximum) {
        value %= maximum;
        ASSERT(value < maximum);
    }
    return value;
}

/*
 * Returns the value that is congruent with the given value so that
 * minimum <= value < maximum.
 */
int util::congruent(int value, int minimum, int maximum) {
    if (minimum > maximum) {
        return congruent(value, maximum, minimum);
    }
    return congruent(value - minimum, maximum - minimum) + minimum;
}

