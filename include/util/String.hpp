#if !defined(UTIL_STRING_HPP)
#define UTIL_STRING_HPP

/*
 * A basic string class. Adds a couple of functions to std::string
 *
 * @author Tuukka Haapasalo
 * @created 2004-06-22
 */

#include <string>
#include <vector>
#include <algorithm>
#include <cstdarg>
#include <sstream>

namespace util {

class String: public std::string {
public:

    /* String constructors */
    String() :
        std::string() {
    }
    String(char const * const str) :
        std::string(str) {
    }
    String(char const * const str, int const size) :
        std::string(str, size) {
    }
    String(std::string const & str) :
        std::string(str) {
    }

    /* Converts the string to lower case  */
    String toUpperCase() const;
    /* Converts the string to lower case  */
    String toLowerCase() const;

    /* Trims trailing and preceding whitespace */
    String trim() const;
    /* Trims preceding whitespace */
    String trimLeft() const;
    /* Trims trailing whitespace */
    String trimRight() const;

    /* Converts string to float */
    float toFloat() const;
    /* Converts string to double */
    double toDouble() const;
    /* Converts string to int */
    int toInt() const;
    /* Converts string to long */
    long long toLong() const;
    /* Converts string to char */
    char toChar() const;

    static String toString(float const value);
    static String toString(double const value);
    static String toString(int const value);
    static String toString(long long const value);
    static String toString(char const value);

    static String format(char const * const , ...);
    static String format(char const * const , va_list ap);

    /* 
     * Returns true if the given character is considered to be white
     * space
     */
    static bool isWhitespace(char const c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    /* Returns true if the given character is a letter */
    static bool isLetter(char const c);

    /* Returns the first character (or 0 if the string is empty) */
    char firstChar() const;

    /* Returns the last character (or 0 if the string is empty) */
    char lastChar() const;

    /* Checks whether the string starts with another string */
    bool startsWith(String const &) const;

    /* Checks whether the string starts with a given character */
    bool startsWith(char const) const;

    /* Checks whether the string ends with another string */
    bool endsWith(String const &) const;

    /* Checks whether the string ends with a given character */
    bool endsWith(char const) const;

    /* Checks whether the string contains another string */
    bool contains(String const &) const;

    /* 
     * Returns the string after the last separator, or the whole
     * string if separator is not found
     */
    String afterLast(String const & separator) const;

    /* Splits the string to a list of strings */
    std::vector<String> split(String const & delim, bool const emptyResults =
            false) const;

    /* Splits the string to a list of strings */
    std::vector<String>
    split(char const delim, bool const emptyResults = false) const;

public:

    static String const linefeed;
    static char const * const clf;

};

}

#define TOSTRING(x) dynamic_cast<std::stringstream &>(std::stringstream().seekp(0) << x).str()

//class ToString {
//public:
//    ToString() {
//    }
//};
//
//extern util::ToString const toString;
//
//}
//
//inline std::string operator<<(std::ostream & str, util::ToString const &) {
//    std::ostream * oPtr = &str;
//    std::stringstream * strPtr = dynamic_cast<std::stringstream *> (oPtr);
//    if (strPtr != 0) {
//        return strPtr->str();
//    }
//    return "";
//}

#endif // !defined(UTIL_STRING_HPP)
