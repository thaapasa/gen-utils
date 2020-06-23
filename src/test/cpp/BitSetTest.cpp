#include "util/BitSet.hpp"
#include "util/Log.hpp"
#include <iostream>

using namespace util;
using namespace std;

void testBitSets() {
    BitSet s;
    s[1] = true;
    s[2] = true;
    s[30] = true;
    ASSERT(!s[0]);
    ASSERT(s[1]);
    ASSERT(s[2]);
    ASSERT(!s[3]);
    ASSERTEQUALS(false, (bool) s[0]);
    ASSERTEQUALS(true, (bool) s[1]);
    ASSERTEQUALS(true, (bool) s[2]);
    ASSERTEQUALS(false, (bool) s[3]);
    ASSERT(s[30]);
    ASSERT(!s[31]);

    BitSet d = s;

    d[2] = false;
    ASSERT(!d[0]);
    ASSERT(d[1]);
    ASSERT(!d[2]);
    ASSERT(!d[3]);
    ASSERT(d[30]);
    ASSERT(!d[31]);
    cout << endl << s << endl << d;
}

int main(int argc, char ** argv) {
    cout << "Testing BitSets ........." << endl;
    cout << "-------------------------" << endl;
    cout << "BitSets ............ ";
    testBitSets();
    cout << " ok" << std::endl;
    cout << "-------------------------" << endl;
    cout << "All tests passed!" << endl;
    return 0;
}
