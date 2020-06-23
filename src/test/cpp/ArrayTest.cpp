#include "util/Array.hpp"
#include "util/Log.hpp"
#include <iostream>

using namespace util;
using namespace std;

class CountDeletions {
public:
    CountDeletions() {
    }
    ~CountDeletions() {
        counter++;
    }
    static size_t counter;
};

size_t CountDeletions::counter = 0;

void testArrays() {
    Array<int> a(5);
    a[0] = 0;
    a[1] = 1;
    a[3] = 6;
    a[4] = 4;

    ASSERTEQUALS((size_t) 5, a.size());

    ASSERTEQUALS(0, a[0]);
    ASSERTEQUALS(1, a[1]);
    ASSERTEQUALS(0, a[2]);
    ASSERTEQUALS(6, a[3]);
    ASSERTEQUALS(4, a[4]);

    cout << endl << a;

    CountDeletions::counter = 0;
    Array<CountDeletions> * delar = new Array<CountDeletions> (15);
    ASSERTEQUALS((size_t) 0, CountDeletions::counter);
    delete delar;
    ASSERTEQUALS((size_t) 15, CountDeletions::counter);

    Array<CountDeletions*> * delarp = new Array<CountDeletions*> (15);
    ASSERTEQUALS((size_t) 15, CountDeletions::counter);
    delete delarp;
    ASSERTEQUALS((size_t) 15, CountDeletions::counter);

    CountDeletions *cd = new CountDeletions();
    delarp = new Array<CountDeletions*> (15);
    ASSERTEQUALS((size_t) 15, CountDeletions::counter);
    (*delarp)[0] = cd;
    (*delarp)[1] = cd;
    ASSERTEQUALS((*delarp)[0], delarp->element(0));
    delete delarp;
    ASSERTEQUALS((size_t) 15, CountDeletions::counter);

    delete cd;
    ASSERTEQUALS((size_t) 16, CountDeletions::counter);
}

void testDynArrays() {
    Array<int> a;
    a[0] = 0;
    a[1] = 1;
    a[3] = 0;
    a[4] = 4;

    ASSERTEQUALS((size_t) 5, a.size());

    a = Array<int> (10);
    ASSERTEQUALS((size_t) 10, a.size());
    ASSERTEQUALS((size_t) 10, a.capacity());

    a[0];
    a[1];
    a[4];
    a[12] = 125;

    ASSERTEQUALS((size_t) 13, a.size());
    ASSERTEQUALS((size_t) 20, a.capacity());
    ASSERTEQUALS(125, a[12]);
    a[11];

    ASSERTEQUALS((size_t) 13, a.size());
    a.add(22);
    ASSERTEQUALS((size_t) 14, a.size());
    ASSERTEQUALS(125, a[12]);
    ASSERTEQUALS(22, a[13]);
}

int main(int argc, char ** argv) {
    cout << "Testing Arrays .........." << endl;
    cout << "-------------------------" << endl;
    cout << "Arrays ............. ";
    testArrays();
    cout << " ok" << std::endl;
    cout << "Dynamic arrays ..... ";
    testDynArrays();
    cout << " ok" << std::endl;
    cout << "-------------------------" << endl;
    cout << "All tests passed!" << endl;
    return 0;
}
