#if !defined(UTIL_LOG_HPP)
#define UTIL_LOG_HPP

/*
 * Defines basic logging functions. The log level can be set at run-time.
 * 
 * @author Tuukka Haapasalo
 * @created 2008-05-29
 */

#include "util/String.hpp"
#include "util/exception/Exception.hpp"
#include <string>
#include <sstream>
#include <iostream>

static std::string emptyString("");

#ifdef ERROR
#undef ERROR
#endif

#ifdef assert
#undef assert
#endif // assert
#ifdef ASSERT
#undef ASSERT
#endif // ASSERT
#define LOGATLEVEL(level, func, x) { if (util::Log::instance().logAtLevel(level)) { func(__LINE__, __FILE__) << x << std::endl << std::flush; } }

#define LOG(x) LOGATLEVEL(util::Log::Debug, util::Log::debug, x)
#define INFO(x) LOGATLEVEL(util::Log::Info, util::Log::info, x)
#define WARN(x) LOGATLEVEL(util::Log::Warn, util::Log::warn, x)
#define ERROR(x) { \
    std::stringstream _e_msg; \
    _e_msg << x; \
    util::Log::error(__LINE__, __FILE__) << _e_msg.str() << std::endl; \
    throw util::Bug(_e_msg.str()); \
}
#define BUG(x) { \
    std::stringstream _b_msg; \
    _b_msg << x; \
    util::Log::bug(__LINE__, __FILE__) << _b_msg.str() << std::endl; \
    throw util::Bug(_b_msg.str()); \
}
#define BUG_LF(line, file, x) { \
    std::stringstream _b_msg; \
    _b_msg << x; \
    util::Log::bug(line, file) << _b_msg.str() << std::endl; \
    throw util::Bug(_b_msg.str()); \
}

namespace util {

class Log {
public:
    static Log & instance();

    typedef enum {
        Debug, Info, Warn, Error, Bug
    } Level;

    static std::ostream & debug(int line, char const * file);
    static std::ostream & info(int line, char const * file);
    static std::ostream & warn(int line, char const * file);
    static std::ostream & error(int line, char const * file);
    static std::ostream & bug(int line, char const * file);

    void setLogLevel(Level level) {
        logLevel = level;
    }
    Level getLogLevel() {
        return logLevel;
    }
    bool logAtLevel(Level level) {
        return logLevel <= level;
    }

    static void assert(int line, char const * file, bool condition,
            char const * condStr, std::string const & extraMessage) {
        if (!condition) {
            BUG_LF(line, file, "Assertion failed: " << condStr
                    << (!extraMessage.empty() ? TOSTRING("; " << extraMessage) : emptyString));
        }
    }

    template<typename T>
    static void assertEquals(int line, char const * file, T const & expected,
            T const & actual, char const * expectedName,
            char const * actualName, std::string const & extraMessage) {
        if (!(expected == actual)) {
            BUG_LF(line, file, "Assertion failed: " << expectedName << " != " << actualName
                    << " (expected " << expected << ", got " << actual << ")"
                    << (!extraMessage.empty() ? TOSTRING("; " << extraMessage) : emptyString));
        }
    }

private:
    Log() :
        logLevel(Info) {
    }

    std::ostream & getStream(Level level, int line, char const * file,
            char const * levelName);

private:
    // Variables
    Level logLevel;

};
}

#define ASSERT(x) util::Log::assert(__LINE__, __FILE__, x, #x, emptyString);
#define ASSERTM(x, msg) { if (!(x)) { util::Log::assert(__LINE__, __FILE__, false, #x, TOSTRING(msg)); } }
#define ASSERTEQUALS(expected, actual) util::Log::assertEquals(__LINE__, __FILE__, expected, actual, #expected, #actual, emptyString);
#define ASSERTEQUALSM(expected, actual, msg) { if ((expected) != (actual)) { util::Log::assertEquals(__LINE__, __FILE__, true, false, #expected, #actual, TOSTRING(msg)); } }

#endif // !defined(UTIL_LOG_HPP)
