#if !defined(UTIL_EXCEPTION_HPP)
#define UTIL_EXCEPTION_HPP

/*
 * A generic base class for exceptions
 * 
 * @author Tuukka Haapasalo
 * @created 2004-12-03
 */

#include "util/String.hpp"
#include <cstdarg>

namespace util {

class Exception {
protected:
    /* Creates an exception with an empty message */
    Exception() {
    }
    /* Creates an exception with a printf-formatted message */
    Exception(char const * message, va_list & list);

public:
    /* Creates an exception with a printf-formatted message */
    Exception(char const * message, ...);
    /* Creates an exception with the predefined message */
    Exception(String const & message);
    /* Copies the exception */
    Exception(Exception const & exception);
    /* Assigns the exception */
    Exception const & operator=(Exception const & exception);
    /* Destroys the exception */
    virtual ~Exception();

    /* Returns the message of this exception */
    String const & getMessage() const;

protected:
    /* Sets the message */
    void setMessage(String const & message);

private:
    /* The message */
    String message;

};

class IndexOutOfBounds: Exception {
private:
    int index;
    size_t maxSize;
public:
    IndexOutOfBounds(int index_, size_t maxSize_) :
        Exception("Index out of bounds: %d, max is %d", index_, maxSize_),
                index(index_), maxSize(maxSize_) {
    }
};

class Error {
protected:
    /* Creates an exception with an empty message */
    Error() {
    }
    /* Creates an exception with a printf-formatted message */
    Error(char const * message, va_list & list);

public:
    /* Creates an exception with a printf-formatted message */
    Error(char const * message, ...);
    /* Creates an exception with the predefined message */
    Error(std::string const & message);
    ~Error();

    std::string const & getMessage() const {
        return message;
    }

protected:
    void setMessage(std::string const & message_) {
        message = message_;
    }

private:
    /* The message */
    std::string message;

};

class Bug: public Error {
public:
    /* Creates an exception with a printf-formatted message */
    Bug(char const * message, ...);
    /* Creates an exception with the predefined message */
    Bug(std::string const & message);
    ~Bug();
};

}

#endif // !defined(UTIL_EXCEPTION_HPP)
