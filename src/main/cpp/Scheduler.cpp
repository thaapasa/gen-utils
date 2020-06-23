#include "util/Scheduler.hpp"

/*
 * The scheduler is a very simple scheduler that executes a given task
 * at specific intervals.
 * 
 * @author Tuukka Haapasalo
 * @created 2005-04-27
 * 
 * $Id: Scheduler.cpp,v 1.1 2005-04-28 08:51:22 tuska Exp $
 */

#include "util/Task.hpp"
#include "util/Timer.hpp"
#include "util/Thread.hpp"
#include "util/Log.hpp"

using namespace util;

/**
 * Creates a scheduler that will run the given task at the given intervals
 */
Scheduler::Scheduler(Task & task_, double interval_) :
    task(task_), interval(interval_), running(false), timer(new Timer()) {
}

Scheduler::~Scheduler() {
}

/** 
 * Starts the scheduler 
 */
void Scheduler::start() {
    timer->start();
    running = true;
    double fromLast = 0;
    while (running) {
        double rem = interval - fromLast;
        if (rem > 0) {
            Thread::sleep(rem);
        } else {
            WARN("Scheduling failed, task is " << (-rem) << " seconds late");
        }
        // Clear the timer
        timer->timeFromLast();
        task();
        fromLast = timer->timeFromLast();
    }
}

/** 
 * Stops the scheduler 
 */
void Scheduler::stop() {
    running = false;
}

