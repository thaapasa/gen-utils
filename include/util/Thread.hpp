#if !defined(UTIL_THREAD_HPP)
#define UTIL_THREAD_HPP

/*
 * Threads and an interface for Runnable objects (that can be run 
 * in Threads)
 * 
 * @author Tuukka Haapasalo
 * @created 2004-10-19
 */

#if defined(WIN32)
#include <windows.h>
#else // defined(WIN32)
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#endif // !defined(WIN32)

#if defined(WIN32)
// Definition for the entry point for Windows threads
DWORD WINAPI DoStartThread(void * param);
#else // defined(WIN32)
void * DoStartThread(void * param);
#endif // defined(WIN32)
namespace util {

/*
 * Interface for Runnable classes. Runnables can be started inside
 * Thread instances.
 */
class Runnable {
public:
    /* Virtual destructor */
    virtual ~Runnable() = 0;

    /* Starts the execution of the runnable task */
    virtual int run() = 0;

    /* Signals the runnable program that it should stop */
    virtual void stop() = 0;

};

/*
 * Thread implementation. Does the actual thread spawning and runs an
 * attached Runnable object. You can also directly subclass Thread
 * and implement functionality in doStart().
 */
class Thread {
public:
    static int const ExitCodeOK = 0;
    static int const ExitCodeFailure = 1;

    /* Creates a Thread with an attached Runnable object */
    Thread(Runnable * runnable);
    /* Virtual destructor. */
    virtual ~Thread();

    /* Starts the Thread running. */
    bool start();

    /* Yields the currently running thread execution. */
    static void yield();

    /* Sleeps for a given amount of second. */
    static void sleep(double seconds);

    /* Returns the attached Runnable object. */
    Runnable * getRunnable() const {
        return runnable;
    }

    /* Waits until the Thread has terminated. */
    bool waitForExit();

protected:
    /* Creates a Thread with no Runnable object attached (for subclass use). */
    Thread();

protected:
    /* Called at Thread startup. Runs the attached Runnable object. */
    virtual int doStart();

private:
    bool started;
#if defined(WIN32)
    // Windows thread ID
    DWORD threadId;
    // Windows thread handle
    HANDLE handle;
#else // defined(WIN32) 
    pthread_t thread;
#endif // defined(WIN32)
    // The attached runnable
    Runnable * runnable;

#if defined(WIN32)
    /* Entry point for Windows threads must be friend to Thread */
    friend DWORD WINAPI ::DoStartThread(void * param);
#else // defined(WIN32)
    /* Entry point for POSIX threads must be friend to Thread */
    friend void * ::DoStartThread(void * param);
#endif // defined(WIN32)
};

}

#endif // !defined(UTIL_THREAD_HPP)
