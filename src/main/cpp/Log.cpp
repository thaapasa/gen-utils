#include "util/Log.hpp"
#include <sstream>

using namespace std;
using namespace util;

static stringstream nullStream;

Log & Log::instance() {
    static Log _the_log;
    return _the_log;
}

ostream & Log::getStream(Level level, int line, char const * file,
        char const * levelName) {
    ostream * stream = &clog;
    if (level >= Warn) {
        stream = &cerr;
    }
    if (logLevel <= level) {
        *stream << levelName << " " << file << ":" << line << "  ";
        return *stream;
    } else {
        nullStream.flush();
        return nullStream;
    }
}

ostream & Log::debug(int line, char const * file) {
    Log & log = Log::instance();
    return log.getStream(Debug, line, file, "DEBUG");
}

ostream & Log::info(int line, char const * file) {
    Log & log = Log::instance();
    return log.getStream(Info, line, file, "INFO ");
}

ostream & Log::warn(int line, char const * file) {
    Log & log = Log::instance();
    return log.getStream(Warn, line, file, "WARN ");
}

ostream & Log::error(int line, char const * file) {
    Log & log = Log::instance();
    return log.getStream(Error, line, file, "ERROR");
}

ostream & Log::bug(int line, char const * file) {
    Log & log = Log::instance();
    return log.getStream(Bug, line, file, "BUG  ");
}
