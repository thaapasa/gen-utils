#if !defined(UTIL_TIMER_HPP)
#define UTIL_TIMER_HPP

/*
 * Provides a timer that calculates time from its initialization and
 * last call to it
 * 
 * @author Tuukka Haapasalo
 * @created 2004-07-28
 */

#if defined(WIN32)
#include <windows.h>
#else // defined(WIN32)
#include <sys/time.h>
#endif // defined(WIN32)
namespace util {

class Timer {
public:
    Timer();
    ~Timer();

    /*
     * Starts the timer
     */
    void start();

    /*
     * Pauses the timer
     */
    void pause();

    /*
     * Returns the time from the start of the timer without including pauses.
     */
    double timeFromStart();

    /*
     * Returns the time from the last (absolute) invocation of this method. 
     * Pauses are not handled so they do not affect the outcome of this 
     * method.
     */
    double timeFromLast();

private:
    bool paused;
    double accumulated;

#if defined(WIN32)
    DWORD startTime;
    DWORD lastTime;
#else // defined(WIN32)
    struct timeval startTime;
    struct timeval lastTime;
#endif // defined(WIN32)
};

}

#endif // !defined(UTIL_TIMER_HPP)
