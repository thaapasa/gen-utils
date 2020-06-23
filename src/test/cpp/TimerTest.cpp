#include "util/Timer.hpp"
#include "util/NumberUtils.hpp"
#include "util/Log.hpp"
#include "util/Thread.hpp"
#include <iostream>

using namespace util;
using namespace std;

void testStartTime() {
    cout << endl;
    Timer timer;
    Timer timer2;
    double accum = 0;
    timer.start();
    timer2.start();
    timer2.pause();
    for (int i = 0; i < 10; i++) {
        Thread::sleep(0.1);
        accum += timer.timeFromLast();
    }
    timer2.start();
    double total2 = timer2.timeFromStart();
    double total = timer.timeFromStart();
    ASSERT(!equals(total, 0));
    ASSERT(total2 < 0.0001);
    LOG("Total " << total << ", accum " << accum);
    ASSERT(equals(total, accum, 0.0001));
    cout << "..................... ";
}

int main(int argc, char ** argv) {
    cout << "Testing Timer ..........." << endl;
    cout << "-------------------------" << endl;
    cout << "Time from start ..... ";
    testStartTime();
    cout << " ok" << std::endl;
    cout << "-------------------------" << endl;
    cout << "All tests passed!" << endl;
    return 0;
}
