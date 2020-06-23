#if !defined(UTIL_CONSUMER_HPP)
#define UTIL_CONSUMER_HPP

/*
 * Base class for a consumer that consumes something (for example,
 * Strings - outputing them to screen or appending to a file).
 * 
 * @author Tuukka Haapasalo
 * @created 2004-08-02
 */

#include "util/String.hpp"
#include "util/Cloneable.hpp"
#include <string>

namespace util {

template<typename T>
class Consumer {
public:
    virtual ~Consumer() {
    }

    /*
     * Consumes the item
     */
    virtual void consume(T const & src) = 0;

    /*
     * Provides a textual description of the consumer
     */
    virtual String toString() const = 0;

};

/*
 * An std::ostream implementation of the Consumer. Pipes the
 * consumed things to the output stream.
 */
template<typename T>
class OutputStreamConsumer: public Consumer<T> {
public:
    OutputStreamConsumer(std::ostream &);

    /*
     * Consumes the string
     */
    virtual void consume(T const & src);

    /*
     * Provides a textual description of the consumer
     */
    virtual String toString() const;

private:
    std::ostream & stream;

};

}

/*
 * Template method definitions
 */

template<typename T>
util::OutputStreamConsumer<T>::OutputStreamConsumer(std::ostream & str) :
    stream(str) {
}

/*
 * Consumes the item
 */
template<typename T>
void util::OutputStreamConsumer<T>::consume(T const & src) {
    stream << src;
}

/*
 * Provides a textual description of the consumer
 */
template<typename T>
util::String util::OutputStreamConsumer<T>::toString() const {
    return "OutputStreamConsumer";
}

#endif // !defined(UTIL_CONSUMER_HPP)
