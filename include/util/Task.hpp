#if !defined(UTIL_TASK_HPP)
#define UTIL_TASK_HPP

/*
 * Task defines the interface for a general task that can be executed.
 * 
 * @author Tuukka Haapasalo
 * @created 2005-04-27
 */

namespace util {

class Task {
public:
    virtual ~Task() {
    }

    /**
     * Call operator() to execute the task.
     */
    virtual bool operator()() = 0;
};

}

#endif // !defined(UTIL_TASK_HPP)
