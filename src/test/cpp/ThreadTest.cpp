#include "util/Thread.hpp"
#include "util/Log.hpp"
#include "util/Random.hpp"
#include <iostream>

using namespace util;
using namespace std;

class TesterThread: public Runnable {
public:
    TesterThread(int i) :
        val(i) {
        cout << "Created " << val << endl;
    }
    virtual ~TesterThread() {
    }

    virtual void stop() {
    }

    virtual int run() {
        for (int i = 0; i < val * 10; ++i) {
            cout << val;
            if (Random::getInstance()->getRandom() > 0.5)
                Thread::yield();
        }

        INFO("Waiting for " << val << " sec");
        Thread::sleep(val);
        INFO("Waited " << val << " sec");

        return Thread::ExitCodeOK;
    }

private:
    int val;
};

void testThreads() {
    TesterThread tt1(1);
    Thread t1(&tt1);
    TesterThread tt2(2);
    Thread t2(&tt2);
    TesterThread tt3(3);
    Thread t3(&tt3);

    ASSERT(t1.start());
    ASSERT(t2.start());
    ASSERT(t3.start());

    t1.waitForExit();
    t2.waitForExit();
    t3.waitForExit();
}

int main(int argc, char ** argv) {
    cout << "Testing Thread class....." << endl;
    cout << "-------------------------" << endl;

    cout << "Threads ............. ";
    testThreads();
    cout << " ok" << std::endl;

    cout << "-------------------------" << endl;
    cout << "All tests passed!" << endl;
    return 0;
}
