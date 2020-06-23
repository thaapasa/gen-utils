#include "util/Log.hpp"
#include "util/File.hpp"
#include <iostream>

using namespace util;
using namespace std;

static char const * utilsTestFileName = "utilstestfile";

void testName() {
    File file("patterns/mobydick");
    ASSERTEQUALS(String("mobydick"), file.getName());
#ifdef WIN32
    cout << endl;
    cout << "Win32 tests ..........";
    File file2("patterns\\mobydick");
    ASSERTEQUALS(String("mobydick"), file2.getName());
#endif
}


void testFiles() {
    File file(utilsTestFileName);

    // Delete test file if is has been left around by a failed test
    file.deleteFile();
    ASSERT(!file.exists());

    ASSERT(file.writeString("moi ja poro"));
    ASSERT(file.exists());
    string a = file.readAsString();
    ASSERTEQUALS(string("moi ja poro"), a);
    ASSERT(file.exists());
    ASSERT(file.deleteFile());
    ASSERT(!file.exists());
    ASSERT(!file.deleteFile());
    ASSERT(!file.exists());
}

int main(int argc, char ** argv) {
    cout << "File utils .............." << endl;
    cout << "-------------------------" << endl;

    cout << "Path name ............ ";
    testName();
    cout << "ok" << std::endl;

    cout << "File handling ........ ";
    testFiles();
    cout << "ok" << std::endl;

    cout << "-------------------------" << endl;
    cout << "All tests passed!" << endl;
    return 0;
}
