#include "util/String.hpp"
#include "util/Log.hpp"
#include "util/NumberUtils.hpp"
#include <iostream>
#include <sstream>

using namespace util;
using namespace std;

void testConversions() {
    String s1 = "256";
    ASSERT(s1.toInt() == 256);
    ASSERT(s1.toLong() == 256);

    String s2 = "952630395923";
    //  ASSERT(s2.toInt() == -852343789); // Too big for int
    ASSERT(s2.toLong() == 952630395923ll);

    String s3 = "13513513.243e12";
    ASSERT(util::equals(s3.toDouble(), 13513513.243e12));
    ASSERT(util::equals(s3.toFloat(), 13513513.243e12));

    ASSERT(String::toString(34) == "34");
    ASSERT(String::toString(26.7) == "26.7");
}

void testFormat() {
    ASSERT(String("aapeli45ko") == String::format("aa%s%dko", "peli", 45));
    ASSERT(String("aapeli45kokelipelikelipelikelipelikelipelikelipelikelipeli"
                    "kelipelikelipelikelipelikelipelikelipelikelipeli") == String::format(
                    "aa%s%dko%s", "peli", 45,
                    "kelipelikelipelikelipelikelipelikelipelikelipeli"
                    "kelipelikelipelikelipelikelipelikelipelikelipeli"));
}

void testStartingEnding() {
    ASSERT(String("Kaurapuurokala").endsWith("la"));
    ASSERT(!String("Makaro").endsWith("garo"));
    ASSERT(!String("Makaro").endsWith(" Makaro"));
    ASSERT(String("Makaro").endsWith("Makaro"));
    ASSERT(String("Makaro").endsWith("karo"));
    ASSERT(String("Makaro").endsWith(""));
    ASSERT(String("Torni1").endsWith("i1"));
    ASSERT(String("Torni1").startsWith("Torni1"));
    ASSERT(String("Torni1").startsWith(""));
    ASSERT(String("Torni1").startsWith("Tor"));
    ASSERT(String("Torni1").startsWith("Torni"));
    ASSERT(String("Torni1").startsWith("T"));
}

template <typename T>
struct lval {
    T t;
    T & get() { return t; }
};

void testStreams() {
    ASSERTEQUALS(string("ko12po"), TOSTRING("ko" << 12 << "po"));
    ASSERTEQUALS(string("ko12po"), TOSTRING("ko" << 12 << "po"));
}

int main(int argc, char ** argv) {
    cout << "Testing String class....." << endl;
    cout << "-------------------------" << endl;

    cout << "Conversions ..........";
    testConversions();
    cout << " ok" << std::endl;

    cout << "Formatting ...........";
    testFormat();
    cout << " ok" << std::endl;

    cout << "Starting and ending ..";
    testStartingEnding();
    cout << " ok" << std::endl;

    cout << "String streams .......";
    testStreams();
    cout << " ok" << std::endl;

    cout << "-------------------------" << endl;
    cout << "All tests passed!" << endl;
    return 0;
}
