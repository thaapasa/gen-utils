#include "util/Log.hpp"
#include <iostream>

using namespace util;
using namespace std;

class LogTest {
public:
    int a;
    LogTest() :
        a(0) {
    }
};

ostream & operator<<(ostream & os, LogTest & l) {
    l.a++;
    os << l.a;
    cout << endl << "PROCESSING" << endl;
    return os;
}

void testLog() {
    Log::Level level = Log::instance().getLogLevel();

    LogTest l;
    ASSERT(l.a == 0);
    Log::instance().setLogLevel(Log::Debug);
    LOG("T[" << l << "]");
    ASSERT(l.a == 1);
    INFO("T[" << l << "]");
    ASSERT(l.a == 2);
    WARN("T[" << l << "]");
    ASSERT(l.a == 3);

    l.a = 0;
    ASSERT(l.a == 0);
    Log::instance().setLogLevel(Log::Warn);
    LOG("T[" << l << "]");
    ASSERT(l.a == 0);
    INFO("T[" << l << "]");
    ASSERT(l.a == 0);
    WARN("T[" << l << "]");
    ASSERT(l.a == 1);

    Log::instance().setLogLevel(level);
}

void testAssertions() {
    ASSERT(1 == 1);
    ASSERT('a' == (char) 97);
    ASSERTEQUALS(1, 1);
    string s1("moi");
    ASSERTEQUALS(string("moi"), s1);
    ASSERTEQUALS('a', (char) 97);
}

void testErrors() {
    int i = 1;
    char const * msg = "Message";
    bool success = false;
    try {
        BUG("Testing bug reporting " << i << ": " << msg);
    } catch (Bug b) {
        LOG("Bug thrown, message is " << b.getMessage());
        ASSERTEQUALS(b.getMessage(), string("Testing bug reporting 1: Message"));
        success = true;
    }
    ASSERT(success);

    ++i;
    success = false;
    try {
        throw Bug("Testing %d: %s", i, msg);
    } catch (Bug b) {
        LOG("Bug thrown, message is " << b.getMessage());
        ASSERTEQUALS(b.getMessage(), string("Testing 2: Message"));
        success = true;
    }
    ASSERT(success);

    ++i;
    success = false;
    try {
        ERROR("Testing error reporting " << i << ": " << msg);
    } catch (Error e) {
        LOG("Error thrown, message is " << e.getMessage());
        ASSERTEQUALS(e.getMessage(), string("Testing error reporting 3: Message"));
        success = true;
    }
    ASSERT(success);

    ++i;
    success = false;
    try {
        throw Error("Error-testing %d: %s", i, msg);
    } catch (Error e) {
        LOG("Error thrown, message is " << e.getMessage());
        ASSERTEQUALS(e.getMessage(), string("Error-testing 4: Message"));
        success = true;
    }
    ASSERT(success);
}

int main(int argc, char ** argv) {
    if (argc > 1 && string("-d") == argv[1]) {
        cout << "Setting log level to Debug" << endl;
        Log::instance().setLogLevel(Log::Debug);
    }
    cout << "Testing logging ........." << endl;
    cout << "-------------------------" << endl;

    cout << "Log processing ....... " << endl;
    testLog();
    cout << "                       ok" << std::endl;

    cout << "Assertions ........... ";
    testAssertions();
    cout << "ok" << std::endl;

    cout << "Errors and bugs. ..... " << endl;
    testErrors();
    cout << "                       ok" << std::endl;

    cout << "-------------------------" << endl;
    cout << "All tests passed!" << endl;
    return 0;
}
