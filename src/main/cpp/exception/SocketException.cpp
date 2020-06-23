#include "util/exception/SocketException.hpp"

/*
 * Socket exceptions
 * 
 * @author Tuukka Haapasalo
 * @created 2004-12-03
 * 
 * $Id: SocketException.cpp,v 1.1 2004-12-03 17:35:13 tuska Exp $
 */

#include <cstdarg>
 
using namespace util;

/*************************
 * Base socket exception *
 *************************/


/* 
 * Creates an exception with an empty message 
 */
SocketException::SocketException()
: Exception()
{
}

/* 
 * Creates an exception with a printf-formatted message 
 */
SocketException::SocketException(char const * message, va_list & list)
: Exception(message, list)
{
}

/* 
 * Creates a socket exception with a printf-formatted message 
 */
SocketException::SocketException(char const * message, ...)
{
  va_list ap;
  va_start(ap, message);
  setMessage(String::format(message, ap));
  va_end(ap);
}

/* 
 * Creates a socket exception with the predefined message 
 */
SocketException::SocketException(String const & message)
: Exception(message)
{
}


/*******************************
 * Connection closed exception *
 *******************************/

/* 
 * Creates a connection closed exception with a printf-formatted message 
 */
ConnectionClosedException::ConnectionClosedException(char const * message, ...)
: SocketException()
{
  va_list ap;
  va_start(ap, message);
  setMessage(String::format(message, ap));
  va_end(ap); 
}

/* 
 * Creates a connection closed exception with the predefined message 
 */
ConnectionClosedException::ConnectionClosedException(String const & message)
: SocketException(message)
{
}
