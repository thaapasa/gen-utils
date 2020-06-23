#include "util/exception/Exception.hpp"

/*
 * Generic base class for exceptions
 * 
 * @author Tuukka Haapasalo
 * @created 2004-12-03
 * 
 * $Id: Exception.cpp,v 1.2 2004-12-03 17:35:13 tuska Exp $
 */

#include <cstdarg>

using namespace util;
using namespace std;

/* 
 * Creates an exception with a printf-formatted message 
 */
Exception::Exception(char const * message_, va_list & list) {
    message = String::format(message_, list);
}

/* 
 * Creates an exception with a printf-formatted message 
 */
Exception::Exception(char const * message_, ...) {
    va_list ap;
    va_start(ap, message_);
    message = String::format(message_, ap);
    va_end(ap);
}

/* 
 * Creates an exception with the predefined message 
 */
Exception::Exception(String const & message_) :
    message(message_) {
}

/* 
 * Copies the exception 
 */
Exception::Exception(Exception const & exception) :
    message(exception.message) {
}

/* 
 * Assigns the exception 
 */
Exception const & Exception::operator=(Exception const & exception) {
    message = exception.message;
    return *this;
}

/* 
 * Destroys the exception 
 */
Exception::~Exception() {
}

/* 
 * Returns the message of this exception 
 */
String const & Exception::getMessage() const {
    return message;
}

/* 
 * Sets the message 
 */
void Exception::setMessage(String const & message_) {
    message = message_;
}

/*
 * Creates an exception with a printf-formatted message
 */
Error::Error(char const * message_, va_list & list) {
    message = String::format(message_, list);
}

/*
 * Creates an exception with a printf-formatted message
 */
Error::Error(char const * message_, ...) {
    va_list ap;
    va_start(ap, message_);
    message = String::format(message_, ap);
    va_end(ap);
}

/*
 * Creates an exception with the predefined message
 */
Error::Error(string const & message_) :
    message(message_) {
}

/*
 * Destroys the bug
 */
Error::~Error() {
}


/*
 * Creates an exception with a printf-formatted message
 */
Bug::Bug(char const * message_, ...) {
    va_list ap;
    va_start(ap, message_);
    String message = String::format(message_, ap);
    va_end(ap);
    setMessage(message);
}

/*
 * Creates an exception with the predefined message
 */
Bug::Bug(string const & message_) :
    Error(message_) {
}

/*
 * Destroys the bug
 */
Bug::~Bug() {
}
