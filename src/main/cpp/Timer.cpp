#include "util/Timer.hpp"
#include "util/Log.hpp"

/*
 * Provides a timer that calculates time from its initialization and
 * last call to it
 * 
 * @author Tuukka Haapasalo
 * @created 2004-07-28
 * 
 * $Id: Timer.cpp,v 1.6 2004-11-02 11:35:52 tuska Exp $
 */

#include <cmath>

using namespace util;

#if !defined(WIN32)

namespace util {
inline double tv_diff_secs(struct timeval * start, struct timeval * end) {
    time_t secs = end->tv_sec - start->tv_sec;
    // end must be >= start
    ASSERT(secs >= 0);
    if (secs == 0) {
        ASSERT(end->tv_usec >= start->tv_usec);
        // usec = microseconds
        return (end->tv_usec - start->tv_usec) / (double) 1000000.0;
    }
    return (secs - 1) + // The whole seconds without the first second
            (((1000000 - start->tv_usec) + // the leftovers from first second
                    end->tv_usec) // the last second's leftovers
                    / (double) 1000000.0); // .. microseconds scaled to seconds
}
}

#endif // !defined(WIN32)

Timer::Timer() :
    paused(true), accumulated(0) {
    start();
    paused = true;
}

Timer::~Timer() {
}

/*
 * (Re-)starts the timer
 */
void Timer::start() {
#if defined(WIN32)
    startTime = timeGetTime();
    lastTime = startTime;
#else // defined(WIN32)
    gettimeofday(&startTime, 0);
    lastTime = startTime;
#endif // defined(WIN32)
    paused = false;
}

/*
 * Pauses the timer
 */
void Timer::pause() {
    if (paused)
        return;
    accumulated = timeFromStart();
    paused = true;
}

/*
 * Returns the time from the start of the timer without including pauses.
 */
double Timer::timeFromStart() {
    if (paused) {
        return accumulated;
    }

#if defined(WIN32)

    return accumulated + fabs((double)(timeGetTime() - startTime)
            / (double)1000.0);

#else // defined(WIN32)
    struct timeval current;
    gettimeofday(&current, 0);
    return accumulated + tv_diff_secs(&startTime, &current);

#endif // defined(WIN32)
}

/*
 * Returns the time from the last (absolute) invocation of this method. 
 * Pauses are not handled so they do not affect the outcome of this 
 * method.
 */
double Timer::timeFromLast() {
#if defined(WIN32)

    // Windows
    DWORD current = timeGetTime();
    DWORD diff = current - lastTime;
    lastTime = current;
    return fabs((double)diff / (double)1000.0);

#else // defined(WIN32)
    struct timeval current;
    gettimeofday(&current, 0);
    double diff = tv_diff_secs(&lastTime, &current);
    lastTime = current;
    return diff;

#endif // defined(WIN32)
}
