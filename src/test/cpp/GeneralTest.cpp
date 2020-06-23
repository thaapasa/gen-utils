#include "util/String.hpp"
#include "util/Log.hpp"
#include "util/Interval.hpp"
#include "util/NumberUtils.hpp"
#include "util/Collections.hpp"

#include <iostream>

using namespace util;
using namespace std;

void testNumberUtils() {
    ASSERT(congruent(0, 5) == 0);
    ASSERT(congruent(2, 5) == 2);
    ASSERT(congruent(5, 5) == 0);
    ASSERT(congruent(4, 5) == 4);
    ASSERT(congruent(6, 5) == 1);
    ASSERT(congruent(8, 5) == 3);
    ASSERT(congruent(12, 5) == 2);

    ASSERT(congruent(112, 5) == 2);

    ASSERT(congruent(-1, 5) == 4);
    ASSERT(congruent(-3, 5) == 2);
    ASSERT(congruent(-4, 5) == 1);
    ASSERT(congruent(-5, 5) == 0);
    ASSERT(congruent(-6, 5) == 4);
    ASSERT(congruent(-10, 5) == 0);

    ASSERT(congruent(-20, 5) == 0);
    ASSERT(congruent(-21, 5) == 4);
    ASSERT(congruent(-19, 5) == 1);

    ASSERT(congruent(15, 7) == 1);

    ASSERT(congruent(3, 5, 10) == 8);
    ASSERT(congruent(8, 5, 10) == 8);
    ASSERT(congruent(5, 5, 10) == 5);
    ASSERT(congruent(4, 5, 10) == 9);
    ASSERT(congruent(9, 5, 10) == 9);
    ASSERT(congruent(10, 5, 10) == 5);
    ASSERT(congruent(12, 5, 10) == 7);
    ASSERT(congruent(15, 5, 10) == 5);
    ASSERT(congruent(14, 5, 10) == 9);

    ASSERT(congruent(-1, 5, 10) == 9);
    ASSERT(congruent(0, 5, 10) == 5);

    ASSERT(congruent(0.0, 5.0) == 0.0);
    ASSERT(congruent(2.0, 5.0) == 2.0);
    ASSERT(congruent(5.0, 5.0) == 0.0);
    ASSERT(congruent(4.0, 5.0) == 4.0);
    ASSERT(congruent(6.0, 5.0) == 1.0);
    ASSERT(congruent(8.0, 5.0) == 3.0);
    ASSERT(congruent(12.0, 5.0) == 2.0);

    ASSERT(congruent(-1.0, 5.0) == 4.0);
    ASSERT(congruent(-3.0, 5.0) == 2.0);
    ASSERT(congruent(-4.0, 5.0) == 1.0);
    ASSERT(congruent(-5.0, 5.0) == 0.0);
    ASSERT(congruent(-6.0, 5.0) == 4.0);
    ASSERT(congruent(-10.0, 5.0) == 0.0);

    ASSERT(congruent(-20.0, 5.0) == 0.0);
    ASSERT(congruent(-21.0, 5.0) == 4.0);
    ASSERT(congruent(-19.0, 5.0) == 1.0);

    ASSERT(congruent(15.0, 7.0) == 1.0);

    ASSERT(congruent(3.0, 5.0, 10.0) == 8.0);
    ASSERT(congruent(8.0, 5.0, 10.0) == 8.0);
    ASSERT(congruent(5.0, 5.0, 10.0) == 5.0);
    ASSERT(congruent(4.0, 5.0, 10.0) == 9.0);
    ASSERT(congruent(9.0, 5.0, 10.0) == 9.0);
    ASSERT(congruent(10.0, 5.0, 10.0) == 5.0);
    ASSERT(congruent(12.0, 5.0, 10.0) == 7.0);
    ASSERT(congruent(15.0, 5.0, 10.0) == 5.0);
    ASSERT(congruent(14.0, 5.0, 10.0) == 9.0);

    ASSERT(congruent(-1.0, 5.0, 10.0) == 9.0);
    ASSERT(congruent(0.0, 5.0, 10.0) == 5.0);

    ASSERT(util::equals(congruent(3.4, 0.5), 0.4));
    ASSERT(util::equals(congruent(2.2, 0.5), 0.2));
    ASSERT(util::equals(congruent(1.4, 0.7), 0.0));
    ASSERT(util::equals(congruent(0.2, 0.5), 0.2));
    ASSERT(util::equals(congruent(0.9, 0.9), 0.0));
    ASSERT(util::equals(congruent(1.0, 0.9), 0.1));

    ASSERT(util::equals(congruent(5.5, 2.6), 0.3));

    ASSERT(util::equals(congruent(5.55, 2.6, 2.7), 2.65));

}

/*
 * int wrapper for testing
 */
class Integer {
public:
    Integer(int i) :
        value(i) {
    }
    Integer(String a) :
        value(a.toInt()) {
    }
    operator int() const {
        return value;
    }
    operator String() const {
        return String::toString(value);
    }

    int getValue() const {
        return value;
    }

private:
    int value;
};

void testConversions() {
    Integer c = String("26");
    int d = c;
    ASSERT(d == 26);
    String s = Integer(48);
    ASSERT(s == "48");
}

void testIntervals() {
    typedef Interval<double> DInterval;
    DInterval a(6, 3);
    DInterval b(6, 7);
    ASSERT(b.intersects(a));
    ASSERT(a.intersects(b));

    DInterval c(6.5, 8);
    ASSERT(b.intersects(c));
    ASSERT(c.intersects(b));

    ASSERT(!a.intersects(c));
    ASSERT(!c.intersects(a));

    ASSERT(a + b == DInterval(3, 7));
    ASSERT(b + a == DInterval(3, 7));
    ASSERT(DInterval(3, 7) == DInterval(7, 3));
    ASSERT(a + c == a);
    ASSERT(c + a == c);

    ASSERT(DInterval(3, 6).contains(DInterval(4, 5)));
    ASSERT(!DInterval(3, 6).contains(DInterval(3, 6.1)));
}

void resetTestSet(set<int> & a) {
    a.clear();
    a.insert(1);
    a.insert(4);
    a.insert(8);
    a.insert(10);
}

void testCollections() {
    set<int> a;
    resetTestSet(a);
    ASSERTEQUALS((size_t) 4, a.size());
    Collections::removeLessThan(a, 4);
    ASSERTEQUALS((size_t) 3, a.size());
    ASSERTEQUALS(4, *(a.begin()));

    resetTestSet(a);
    Collections::removeLessThan(a, 3);
    ASSERTEQUALS((size_t) 3, a.size());
    ASSERTEQUALS(4, *(a.begin()));

    resetTestSet(a);
    Collections::removeLessThan(a, 1);
    ASSERTEQUALS((size_t) 4, a.size());
    ASSERTEQUALS(1, *(a.begin()));

    resetTestSet(a);
    Collections::removeLessThan(a, 2);
    ASSERTEQUALS((size_t) 3, a.size());
    ASSERTEQUALS(4, *(a.begin()));

    resetTestSet(a);
    Collections::removeLessThan(a, 5);
    ASSERTEQUALS((size_t) 2, a.size());
    ASSERTEQUALS(8, *(a.begin()));

    resetTestSet(a);
    Collections::removeLessThan(a, 10);
    ASSERTEQUALS((size_t) 1, a.size());
    ASSERTEQUALS(10, *(a.begin()));

    resetTestSet(a);
    Collections::removeLessThan(a, 11);
    ASSERTEQUALS((size_t) 0, a.size());
    ASSERT(a.empty());

    resetTestSet(a);
    ASSERT(Collections::makeSet(4, 1, 4, 10, 8) == a);
    ASSERTEQUALS(Collections::makeSet(4, 1, 8, 10, 4), a);
    ASSERT(Collections::makeSet(3, 1, 4, 8) != a);

    set<unsigned int> b;
    b.insert(1);
    b.insert(5);
    b.insert(6);
    ASSERTEQUALS(Collections::makeSet(3, 1u, 5, 6), b);
    ASSERTEQUALS(Collections::makeSet(3, 6u, 1, 5), b);
}

int main(int argc, char ** argv) {
    cout << "Testing General Stuff...." << endl;
    cout << "-------------------------" << endl;

    cout << "Number utilities .... ";
    testNumberUtils();
    cout << " ok" << std::endl;

    cout << "Conversions ......... ";
    testConversions();
    cout << " ok" << std::endl;

    cout << "Intervals ........... ";
    testIntervals();
    cout << " ok" << std::endl;

    cout << "Collections utils ... ";
    testCollections();
    cout << " ok" << std::endl;

    cout << "-------------------------" << endl;
    cout << "All tests passed!" << endl;
    return 0;
}
