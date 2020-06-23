#if !defined(UTIL_PRODUCER_HPP)
#define UTIL_PRODUCER_HPP

/*
 * Base class for a producer that produces items.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-07-22
 */

#include "util/String.hpp"
#include <istream>

namespace util {

template<typename T>
class Producer {
public:
    virtual ~Producer() {
    }

    /*
     * Tests whether more strings are available
     */
    virtual bool available() const = 0;

    /*
     * Returns the next item from the producer
     */
    virtual T produce() = 0;

    /*
     * Provides a textual description of the producer
     */
    virtual String toString() const = 0;

};

/*
 * An std::istream implementation of Producer. Reads strings
 * from standard input
 */
template<typename T>
class InputStreamProducer: public Producer<T> {
public:
    InputStreamProducer(std::istream &);

    /*
     * Tests whether more strings are available
     */
    virtual bool available() const;

    /*
     * Returns the next string from the Producer
     */
    virtual T produce();

    /*
     * Provides a textual description of the producer
     */
    virtual String toString() const;

private:
    std::istream & stream;

};
}

/*
 * Template method definitions
 */

/*
 * Template method definitions for InputStreamProducer
 */
template<typename T>
util::InputStreamProducer<T>::InputStreamProducer(std::istream & stream_) :
    stream(stream_) {
}

/*
 * Tests whether more strings are available
 */
template<typename T>
bool util::InputStreamProducer<T>::available() const {
    return !stream.eof();
}

/*
 * Returns the next string from the Producer
 */
template<typename T>
T util::InputStreamProducer<T>::produce() {
    String item;
    getline(stream, item);
    return item;
}

/*
 * Provides a textual description of the producer
 */
template<typename T>
util::String util::InputStreamProducer<T>::toString() const {
    return "InputStreamProvider";
}

#endif // !defined(UTIL_PRODUCER_HPP)
