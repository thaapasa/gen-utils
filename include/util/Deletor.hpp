#if !defined(UTIL_DELETOR_HPP)
#define UTIL_DELETOR_HPP

/*
 * Base class for a deletor that manages a bunch of pointers, 
 * deleting them when it itself is deleted. The order of deletion
 * is determined by subclasses (for example, VectorDeletor<T>
 * and StackDeletor<T>).
 * 
 * @author Tuukka Haapasalo
 * @created 2004-08-22
 */

#include <stack>
#include <vector>

namespace util {

template<class T>
class Deletor {
public:
    /*
     * Destructor
     */
    virtual ~Deletor() {
    }

    /*
     * Adds the pointer to be deleted
     */
    virtual void add(T *) = 0;

    /*
     * Empties the deletion buffer. Called automatically by the destructor.
     */
    virtual void purge() = 0;

};

/*
 * A stack-based LIFO-implementation of the Deletor
 */
template<class T>
class StackDeletor: public Deletor<T> {
public:
    virtual ~StackDeletor() {
        purge();
    }
    /*
     * Adds the pointer to be deleted
     */
    inline virtual void add(T *);

    /*
     * Empties the deletion buffer. Called automatically by the destructor.
     */
    inline virtual void purge();

private:
    typename std::stack<T *> hold;

};

/*
 * A vector-based FIFO-implementation of the Deletor
 */
template<class T>
class VectorDeletor: public Deletor<T> {
public:
    virtual ~VectorDeletor() {
        purge();
    }

    /*
     * Adds the pointer to be deleted
     */
    inline virtual void add(T *);

    /*
     * Empties the deletion buffer. Called automatically by the destructor.
     */
    inline virtual void purge();

private:
    typename std::vector<T *> hold;

};

}

/*
 * Adds the pointer to be deleted
 */
template<class T>
void util::StackDeletor<T>::add(T * ptr) {
    hold.push(ptr);
}

/*
 * Empties the deletion buffer. Called automatically by the destructor.
 */
template<class T>
void util::StackDeletor<T>::purge() {
    while (!hold.empty()) {
        delete hold.top();
        hold.pop();
    }
}

/*
 * Adds the pointer to be deleted
 */
template<class T>
void util::VectorDeletor<T>::add(T * ptr) {
    hold.push_back(ptr);
}

/*
 * Empties the deletion buffer. Called automatically by the destructor.
 */
template<class T>
void util::VectorDeletor<T>::purge() {
    for (typename std::vector<T *>::const_iterator i = hold.begin(); i
            != hold.end(); ++i) {
        delete *i;
    }
    hold.clear();
}

#endif // !defined(UTIL_DELETOR_HPP)
