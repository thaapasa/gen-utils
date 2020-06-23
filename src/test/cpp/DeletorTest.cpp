#include "util/Deletor.hpp"
#include "util/Log.hpp"
#include <iostream>

using namespace util;
using namespace std;

int globalV = 0;

class StackDTest {
public:
    StackDTest(int value) {
        v = value;
    }
    ~StackDTest() {
        ASSERT(globalV == v);
        --globalV;
    }
private:
    int v;
};

class VectorDTest {
public:
    VectorDTest(int value) {
        v = value;
    }
    ~VectorDTest() {
        ASSERT(globalV == v);
        ++globalV;
    }
private:
    int v;
};

void testStackDeletor() {
    StackDeletor<StackDTest> * st = new StackDeletor<StackDTest> ();

    StackDTest * st1 = new StackDTest(1);
    StackDTest * st2 = new StackDTest(2);
    StackDTest * st3 = new StackDTest(3);
    StackDTest * st4 = new StackDTest(4);
    StackDTest * st5 = new StackDTest(5);

    st->add(st1);
    st->add(st2);
    st->add(st3);
    st->add(st4);
    st->add(st5);

    globalV = 5;
    delete st;
    ASSERT(globalV == 0);
}

void testVectorDeletor() {
    VectorDeletor<VectorDTest> st;

    VectorDTest * st1 = new VectorDTest(1);
    VectorDTest * st2 = new VectorDTest(2);
    VectorDTest * st3 = new VectorDTest(3);
    VectorDTest * st4 = new VectorDTest(4);
    VectorDTest * st5 = new VectorDTest(5);

    st.add(st1);
    st.add(st2);
    st.add(st3);
    st.add(st4);
    st.add(st5);

    globalV = 1;
    st.purge();
    ASSERT(globalV == 6);
}

int main(int argc, char ** argv) {
    cout << "Testing Deletors........" << endl;
    cout << "-------------------------" << endl;

    cout << "Stack deletor ........ ";
    testStackDeletor();
    cout << "ok" << std::endl;

    cout << "Vector deletor ....... ";
    testVectorDeletor();
    cout << "ok" << std::endl;

    cout << "-------------------------" << endl;
    cout << "All tests passed!" << endl;
    return 0;
}
