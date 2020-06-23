#include "util/Random.hpp"
#include "util/Log.hpp"
#include <iostream>

#define VALUE_TEST_COUNT 5000
#define SEED_TABLE_SIZE 100000

using namespace util;
using namespace std;

inline void testRange(Random * random, double const min, double const max) {
    if (min > max) {
        testRange(random, max, min);
        return;
    }
    double value = random->getRandom(min, max);
    ASSERT(value >= min);
    ASSERT(value < max);
}

void testValueRange(Random * random) {
    for (int i = 0; i < VALUE_TEST_COUNT; i++) {
        testRange(random, -100000, 100000);
        testRange(random, 0, 1);
        testRange(random, -1, 0);
    }
}

void testReSeed(Random * random) {
    volatile double * originals = new double[SEED_TABLE_SIZE];
    random->seed();
    int const seed = (int) (random->getRandom() * 1000000);

    // Seed the generator
    random->seed(seed);
    // Get original values
    for (int i = 0; i < SEED_TABLE_SIZE; i++) {
        originals[i] = random->getRandom();
    }

    // Re-seed
    random->seed(seed);
    // Assure equality
    for (int i = 0; i < SEED_TABLE_SIZE; i++) {
        volatile double newValue = random->getRandom();
        ASSERT(originals[i] == newValue);
    }

    delete[] originals;
}

void test(Random * random) {
    cout << "Value range .......... ";
    testValueRange(random);
    cout << "ok" << std::endl;
    cout << "After re-seeding ..... ";
    testReSeed(random);
    cout << "ok" << std::endl;
}

int main(int argc, char ** argv) {
    {
        cout << "Testing Native random ..." << endl;
        cout << "-------------------------" << endl;
        Random * random = new NativeRandom();
        test(random);
        delete random;
        cout << "-------------------------" << endl;
        cout << endl;
    }
    {
        cout << "Testing Table random ...." << endl;
        cout << "-------------------------" << endl;
        Random * random = new TableRandom();
        test(random);
        delete random;
        cout << "-------------------------" << endl;
        cout << "All tests passed!" << endl;
    }
    return 0;
}
