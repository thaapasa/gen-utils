#include "util/Random.hpp"

/*
 * Provides a native and a sequential random number generator
 *
 * @author Tuukka Haapasalo
 * @created 2004-07-21
 * 
 * $Id: Random.cpp,v 1.7 2005-01-10 09:33:03 tuska Exp $
 */

#if defined(WIN32)
#include <windows.h>
#else // defined(WIN32)
#include <sys/time.h>
#endif // defined(WIN32)
#include "util/Log.hpp"
#include "util/String.hpp"

#include <sstream>

using namespace util;

/*************************
 * Base Random interface *
 *************************/

/* Latest created instance of Random */
Random * Random::instance = 0;

/* 
 * Protected constructor for subclasses 
 */
Random::Random() {
}

/* 
 * Virtual destructor 
 */
Random::~Random() {
}

/* 
 * Returns the static Random instance 
 */
Random * Random::getInstance() {
    // If no instance of Random is set, create a new NativeRandom
    if (instance == 0) {
        instance = new NativeRandom();
    }
    ASSERT(instance != 0);
    return instance;
}

/* 
 * Manually sets the static Random instance 
 */
void Random::setInstance(Random * random) {
    instance = random;
}

/* 
 * Returns a random number between [min..max[ 
 */
double Random::getRandom(double const min, double const max) {
    if (min == max) {
        WARN("getRandom called with identical min & max values (" << min << " and " << max << ")");
        return min;
    }

    if (min > max)
        return getRandom(max, min);

    double interval = max - min;
    double rnd = getRandom() * interval + min;
    // Check for upper boundary (getRandom() must not return max)
    if (rnd >= max)
        return getRandom(min, max);
    return rnd;
}

/* 
 * Seeds the random number generator with current time 
 */
void Random::seed() {
#if defined(WIN32)
    seed((int)(timeGetTime() * 4.2436523));
#else // defined(WIN32)
    struct timeval tv;
    gettimeofday(&tv, 0);
    seed(tv.tv_sec * 1648634 + tv.tv_usec * 13156);
#endif // defined(WIN32)
}

/**********************************
 * Native random number generator *
 **********************************/

/*
 * Creates a native random generator, seeding it with a number fetched
 * from system time
 */
NativeRandom::NativeRandom() :
    Random() {
    Random::seed();
}

/* 
 * Creates a native random generator, seeding it with given number 
 */
NativeRandom::NativeRandom(int const number) {
    seed(number);
}

/* 
 * Destroys the native random number generator 
 */
NativeRandom::~NativeRandom() {
    if (instance == this) {
        instance = 0;
    }
}

/* Seeds the generator with given number */
void NativeRandom::seed(int const number) {
    srand(number);
}

/* 
 * Returns a random number between [0..1[ 
 */
double NativeRandom::getRandom() {
    double rnd = ((double) rand() / (double) RAND_MAX);
    // Check for upper boundary (getRandom() must not return 1)
    if (rnd >= 1)
        return getRandom();
    return rnd;
}

/***************************************
 * Table-based random number generator *
 ***************************************/

/*
 * Creates a table-based random generator, seeding it with a number fetched
 * from system time
 */
TableRandom::TableRandom() :
    Random() {
    Random::seed();
}

/* 
 * Creates a table-based random generator, seeding it with given number 
 */
TableRandom::TableRandom(int const number) :
    Random() {
    seed(number);
}

/* 
 * Destroys the table-based random number generator 
 */
TableRandom::~TableRandom() {
    if (instance == this) {
        instance = 0;
    }
}

/* 
 * Returns a random number between [0..1[ 
 */
double TableRandom::getRandom() {
    int i = data[98];
    int j = data[99];

    int t = 0;
    if ((t = data[i] - data[j]) < 0)
        t += 1000000000L;

    data[i] = t;

    data[98]--;
    data[99]--;
    if (data[98] < 0)
        data[98] = 55;
    if (data[99] < 0)
        data[99] = 55;

    int k = data[100] % 42 + 56;
    data[100] = data[k];
    data[k] = t;
    return (double) data[100] / 1000000000.0;
}

/* 
 * Seeds the generator with given number 
 */
void TableRandom::seed(int const number) {
    std::stringstream seedStr;
    seedStr << "aG3t5!FDd%d" << number << "l#dQas%öÄS";
    int c = 1;
    while (seedStr.str().size() < 101) {
        seedStr << number * c;
        c *= -2;
    }
    String seeder = seedStr.str().substr(0, 101);
    ASSERT(seeder.size() == 101);

    char const * buf = seeder.c_str();
    for (int i = 0; i < 101; i++)
        data[i] = buf[i] * 8171717 + i * 997;

    int i = 97;
    int j = 12;
    for (int k = 1; k < 998; k++) {
        data[i] -= data[j];
        if (data[i] < 0)
            data[i] += 1000000000L;

        i--;
        j--;
        if (i == 0)
            i = 97;
        if (j == 0)
            j = 97;
    }

    data[98] = 55;
    data[99] = 24;
    data[100] = 77;
}

