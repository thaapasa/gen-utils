#include "util/BinaryData.hpp"
#include "util/Log.hpp"
#include <iostream>

using namespace util;
using namespace std;

void testStringManipulation() {
    {
        BinaryData data(25);
        ASSERT(data.getCapacity() == 32);
        ASSERT(data.getSize() == 0);
        data.write("This is the test string\n", 24);
        ASSERT(data.getSize() == 24);
        ASSERT(data.getCapacity() == 32);
        ASSERT(data.getString() == "This is the test string\n");
        data.write("That", 4);
        ASSERT(data.getSize() == 24);
        ASSERT(data.getCapacity() == 32);
        ASSERT(data.getString() == "That is the test string\n");
        data.write(8, "not", 3);
        ASSERT(data.getSize() == 24);
        ASSERT(data.getCapacity() == 32);
        ASSERT(data.getString() == "That is not test string\n");

        data.write(23, ", or so they say...", 19);

        ASSERT(data.getSize() == 42);
        ASSERT(data.getCapacity() == 64);
        ASSERT(data.getString() == "That is not test string, or so they say...");
    }
    {
        BinaryData data("T‰ss‰p‰ nakki");
        ASSERT(data.getSize() == 13);
        ASSERT(data.getCapacity() == 16);
        ASSERT(data.getString() == "T‰ss‰p‰ nakki");
    }
    {
        BinaryData data("g");
        ASSERT(data.getSize() == 1);
        ASSERT(data.getCapacity() == BinaryData::MinimumCapacity);
        ASSERT(data.getString() == "g");
    }
}

void testAssignment() {
    BinaryData d1("Aapo");
    ASSERT(d1.getSize() == 4);
    ASSERT(d1.getString() == "Aapo");

    BinaryData d2("Aapo");
    ASSERT(d2.getSize() == 4);
    ASSERT(d2.getString() == "Aapo");
    ASSERT(d1.getSize() == 4);
    ASSERT(d1.getString() == "Aapo");

    BinaryData d3("Kellopeliappelsiini");
    ASSERT(d3.getSize() == 19);
    ASSERT(d3.getString() == "Kellopeliappelsiini");

    d3 = d1;
    ASSERT(d3.getSize() == 4);
    ASSERT(d3.getString() == "Aapo");
    ASSERT(d1.getSize() == 4);
    ASSERT(d1.getString() == "Aapo");

}

void testDataArray() {
    BinaryData data("Kellokauppias");
    ASSERT(String(data.getData()) == "Kellokauppias");
    ASSERT(String(data.getData(5)) == "kauppias");
    ASSERT(String(data.getDataForWriting()) == "Kellokauppias");
    ASSERT(String(data.getDataForWriting(5)) == "kauppias");
}

void testAppend() {
    BinaryData data("Kala", 4);
    ASSERT(data.getString() == "Kala");
    data.append("kukko", 5);
    ASSERT(data.getString() == "Kalakukko");
    data.append(" on hyv‰‰!", 10);
    ASSERT(data.getString() == "Kalakukko on hyv‰‰!");
}

int main(int argc, char ** argv) {
    cout << "Testing Binary data......" << endl;
    cout << "-------------------------" << endl;

    cout << "String manipulation .. ";
    testStringManipulation();
    cout << "ok" << std::endl;

    cout << "Assignment and copy .. ";
    testAssignment();
    cout << "ok" << std::endl;

    cout << "Data array pointer ... ";
    testDataArray();
    cout << "ok" << std::endl;

    cout << "Appending ............ ";
    testAppend();
    cout << "ok" << std::endl;

    cout << "-------------------------" << endl;
    cout << "All tests passed!" << endl;
    return 0;
}
