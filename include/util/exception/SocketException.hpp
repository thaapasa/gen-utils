#if !defined(UTIL_SOCKETEXCEPTION_HPP)
#define UTIL_SOCKETEXCEPTION_HPP

/*
 * Socket exceptions
 * 
 * @author Tuukka Haapasalo
 * @created 2004-10-12
 */

#include "util/exception/Exception.hpp"
#include <cstdarg>

namespace util {

class SocketException: public Exception {
protected:
    /* Creates an exception with an empty message */
    SocketException();
    /* Creates an exception with a printf-formatted message */
    SocketException(char const * message, va_list & list);
public:
    /* Creates a socket exception with a printf-formatted message */
    SocketException(char const * message, ...);
    /* Creates a socket exception with the predefined message */
    SocketException(String const & message);
};

class ConnectionClosedException: public SocketException {
public:
    /* Creates a connection closed exception with a printf-formatted message */
    ConnectionClosedException(char const * message, ...);
    /* Creates a connection closed exception with the predefined message */
    ConnectionClosedException(String const & message);
};

}

#endif // !defined(UTIL_SOCKETEXCEPTION_HPP)
