#if !defined(UTIL_SCHEDULER_HPP)
#define UTIL_SCHEDULER_HPP

/*
 * The scheduler is a very simple scheduler that executes a given task
 * at specific intervals.
 * 
 * @author Tuukka Haapasalo
 * @created 2005-04-27
 */

namespace util {

class Timer;
class Task;

class Scheduler {
public:

    /**
     * Creates a scheduler that will run the given task at the given intervals
     */
    Scheduler(Task & task, double interval);
    ~Scheduler();

    /** Starts the scheduler */
    void start();
    /** Stops the scheduler */
    void stop();

protected:
    Task & task;
    double interval;
    bool running;
    Timer * timer;

};

}

#endif // !defined(UTIL_SCHEDULER_HPP)
