#include "util/String.hpp"

/*
 * A basic string class. Adds a couple of functions to std::string
 *
 * @author Tuukka Haapasalo
 * @created 2004-06-22
 * 
 * $Id: String.cpp,v 1.8 2008-04-28 13:12:45 tuska Exp $
 */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <cstring>

using namespace util;

String const String::linefeed("\n");
char const * const String::clf = "\n";

//namespace util {
//ToString const toString;
//}

/* 
 * Converts the string to upper case 
 */
String String::toUpperCase() const {
    String res = "";
    for (const_iterator i = begin(); i != end(); i++) {
        res += toupper(*i);
    }
    return res;
}

/* 
 * Converts the string to lower case 
 */
String String::toLowerCase() const {
    String res = "";
    for (const_iterator i = begin(); i != end(); i++) {
        res += tolower(*i);
    }
    return res;
}

/* 
 * Trims trailing and preceding whitespace 
 */
String String::trim() const {
    return trimLeft().trimRight();
}

/* 
 * Trims preceding whitespace 
 */
String String::trimLeft() const {
    size_t i = 0;
    size_t len = length();
    while (i < len && (isWhitespace(at(i)) || at(i) == 0))
        i++;
    return substr(i, len);
}

/*
 * Trims trailing whitespace 
 */
String String::trimRight() const {
    size_t len = length();
    int i = len - 1;
    while (i >= 0 && (isWhitespace(at(i)) || at(i) == 0))
        i--;
    return substr(0, i + 1);
}

/* 
 * Converts string to float 
 */
float String::toFloat() const {
    return atof(c_str());
}

/* 
 * Converts string to double 
 */
double String::toDouble() const {
    return atof(c_str());
}

/* 
 * Converts string to int 
 */
int String::toInt() const {
    return atoi(c_str());
}

/* 
 * Converts string to long 
 */
long long String::toLong() const {
#if defined(unix) && !defined(linux)
    return atol(c_str());
#else
    return atoll(c_str());
#endif
}

/* 
 * Converts string to char 
 */
char String::toChar() const {
    if (length() < 1)
        return 0;
    return (*this)[0];
}

/* 
 * Returns true if the given character is a letter 
 */
bool String::isLetter(char const c) {
    return toupper(c) != tolower(c);
    /*
     return
     (c >= 'A' && c <= 'Z') ||
     (c >= 'a' && c <= 'z') ||
     c == 'å' || c == 'Å' ||
     c == 'ä' || c == 'Ä' ||
     c == 'ö' || c == 'Ö';
     */
}

/*
 * Converts a float to string
 */
String String::toString(float const value) {
    std::stringstream sr;
    sr << value;
    return sr.str();
}

/*
 * Converts a double to string
 */
String String::toString(double const value) {
    std::stringstream sr;
    sr << value;
    return sr.str();
}

/*
 * Converts an integer to string
 */
String String::toString(int const value) {
    std::stringstream sr;
    sr << value;
    return sr.str();
}

/*
 * Converts a long to string
 */
String String::toString(long long const value) {
    std::stringstream sr;
    sr << value;
    return sr.str();
}

/*
 * Converts a character to string
 */
String String::toString(char const value) {
    std::stringstream sr;
    sr << value;
    return sr.str();
}

/*
 * Formats a printf-styled string given as a variable parameter list
 */
String String::format(char const * const src, ...) {
    va_list ap;
    va_start(ap, src);
    String msg = String::format(src, ap);
    va_end(ap);

    return msg;
}

/*
 * Formats a printf-styled string given as a variable parameter list
 */
String String::format(char const * const src, va_list ap) {
    // Guess size
    int size = strlen(src) + 30;
    String result;

    va_list aq;
    bool success = false;
    while (!success) {
        char * buffer = new char[size];
        if (buffer <= 0) {
            return "[could not allocate space for string formatting]";
        }

        va_copy(aq, ap);
        int printed = vsnprintf(buffer, size, src, aq);
        va_end(aq);

        if (printed < 0) {
            // Output error
            result = String("");
            success = true;
        } else if (printed >= size) {
            size = printed + 1;
        } else {
            result = String(buffer);
            success = true;
        }
        delete[] buffer;
    }
    return result;
}

/* 
 * Returns the first character (or 0 if the string is empty) 
 */
char String::firstChar() const {
    if (length() < 1)
        return 0;
    return at(0);
}

/* 
 * Returns the last character (or 0 if the string is empty) 
 */
char String::lastChar() const {
    size_t len = length();
    if (len == 0)
        return 0;
    return at(len - 1);
}

/* 
 * Checks whether the string starts with another string 
 */
bool String::startsWith(String const & other) const {
    if (other.length() > length())
        return false;
    return substr(0, other.length()) == other;
}

/* 
 * Checks whether the string starts with a given character 
 */
bool String::startsWith(char const character) const {
    if (length() < 1)
        return false;
    return at(0) == character;
}

/* 
 * Checks whether the string ends with another string 
 */
bool String::endsWith(String const & other) const {
    if (other.length() > length())
        return false;
    return substr(length() - other.length()) == other;
}

/* 
 * Checks whether the string ends with a given character 
 */
bool String::endsWith(char const character) const {
    if (length() < 1)
        return false;
    return at(length() - 1) == character;
}

/* 
 * Checks whether the string contains another string 
 */
bool String::contains(String const & other) const {
    return find(other) != npos;
}

/* 
 * Splits the string to a list of strings 
 */
std::vector<String> String::split(char const delim, bool const emptyResults) const {
    String a = String(&delim, 1);
    return split(a, emptyResults);
}

/* 
 * Splits the string to a list of strings 
 */
std::vector<String> String::split(String const & delim, bool const emptyResults) const {
    std::vector<String> results;
    String::size_type start = 0;
    String::size_type pos = find(delim);
    while (pos != npos) {
        if (!emptyResults) {
            while (pos == start) {
                start = pos + delim.length();
                pos = find(delim, start);
                if (pos == npos)
                    return results;
            }
        }
        results.push_back(substr(start, pos - start));
        start = pos + delim.length();
        pos = find(delim, start);
    }
    String last = substr(start);
    if (emptyResults || last != "")
        results.push_back(last);
    return results;
}

/* 
 * Returns the string after the last separator, or the whole
 * string if separator is not found
 */
String String::afterLast(String const & separator) const {
    String::size_type last = rfind(separator);
    return last != npos ? String(substr(last + separator.length())) : *this;
}
