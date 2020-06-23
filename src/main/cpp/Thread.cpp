#ifndef WIN32
#include <pthread.h>
#endif // WIN32
#include "util/Thread.hpp"

/*
 * Threads and an interface for Runnable objects (that can be run 
 * in Threads)
 * 
 * @author Tuukka Haapasalo
 * @created 2004-10-19
 * 
 * $Id: Thread.cpp,v 1.12 2005-04-29 09:41:03 tuska Exp $
 */

#include "util/Log.hpp"

using namespace util;

#if defined(WIN32)

/*
 * Entry point for Windows threads. The Thread instance is passed as 
 * the parameter.
 */
DWORD WINAPI DoStartThread(void * param)
{
    Thread * thread = reinterpret_cast<Thread *>(param);
    int result = thread->doStart();
    return result;
}

#else // defined(WIN32)
#include <sys/wait.h>

/*
 * Entry point f
 ts.tv_nsec = (long int)((seconds - or POSIX threads.
 */
void * DoStartThread(void * param) {
    Thread * thread = reinterpret_cast<Thread *> (param);
    thread->doStart();
    pthread_exit( NULL);
    return NULL;
}

#endif // defined(WIN32)
/*
 * Virtual destructor. Although declared as zero, must be defined to
 * satisfy linker.
 */
Runnable::~Runnable() {
}

/* 
 * Creates a Thread with an attached Runnable object 
 */
Thread::Thread(Runnable * runnable_) :
    started(false),
#if defined(WIN32)
            threadId(0),
            handle(0),
#endif //defined(WIN32)
            runnable(runnable_) {
}

/* 
 * Creates a Thread with no Runnable object attached (for subclass use). 
 */
Thread::Thread() :
    started(false),
#if defined(WIN32)
            threadId(0),
            handle(0),
#endif //defined(WIN32)
            runnable(0) {
}

/* 
 * Virtual destructor.
 */
Thread::~Thread() {
#if defined(WIN32) 
    if (handle != 0) {
        CloseHandle(handle);
        handle = 0;
    }
#endif // defined(WIN32)
}

/* 
 * Starts the Thread running. 
 */
bool Thread::start() {
    if (started)
        return false;

#if defined(WIN32)
    if (handle != 0) return false;
    SECURITY_ATTRIBUTES attr;
    attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    attr.lpSecurityDescriptor = 0;
    attr.bInheritHandle = TRUE;
    handle = CreateThread(&attr, 0, ::DoStartThread, this, 0, &threadId);

    if (handle == NULL) return false;

#else // defined(WIN32)
    int res = pthread_create(&thread, 0, ::DoStartThread, this);

    if (res != 0)
        return false;

#endif // defined(WIN32)
    started = true;
    return true;
}

/* 
 * Called at Thread startup. Runs the attached Runnable object. 
 */
int Thread::doStart() {
    if (runnable != 0) {
        return runnable->run();
    } else {
        return ExitCodeOK;
    }
}

/* 
 * Yields the currently running thread execution 
 */
void Thread::yield() {
#if defined(WIN32)
    Sleep(0);
#else // defined(WIN32)
    pthread_yield();
#endif // defined(WIN32)
}

/* 
 * Sleeps for a given amount of second. 
 */
void Thread::sleep(double seconds) {
#if defined(WIN32)
    Sleep((DWORD)(seconds * 1000));
#else // defined(WIN32)
    struct timespec ts;
    int wholes = (int) seconds;
    ts.tv_sec = wholes;
    ts.tv_nsec = (long int) ((seconds - (double) wholes) * 1000000000);
    nanosleep(&ts, NULL);
#endif // defined(WIN32)
}

/* 
 * Waits until the Thread has terminated. 
 */
bool Thread::waitForExit() {
#if defined(WIN32)
    return WaitForSingleObject(handle, INFINITE) == WAIT_OBJECT_0;
#else // defined(WIN32)
    return pthread_join(thread, 0) == 0;
#endif // defined(WIN32)
}
