#include "util/ArgumentReader.hpp"
#include "util/Log.hpp"
#include "util/Iterator.hpp"
#include "util/Consumer.hpp"
#include <iostream>
#include <cstring>

using namespace util;
using namespace std;

void testArguments() {
    ArgumentReader args(false, 0);
    args.addArgument("test-key", "t");

    ASSERT(args.isKey("test-key"));
    ASSERT(args.isKey("TEST-key"));
    ASSERT(args.isShortKey("t"));
    ASSERT(args.isShortKey("T"));

    char ** t = new char *[6];
    for (int i = 0; i < 6; i++) {
        t[i] = new char[20];
        memset(t[i], 0, 20);
    }
    strcpy(t[0], "--apina");
    strcpy(t[1], "mo");
    strcpy(t[2], "-t");
    strcpy(t[3], "heps");
    strcpy(t[4], "-t");
    strcpy(t[5], "kro");

    Log::instance().setLogLevel(Log::Error);

    args.read(6, t);

    ASSERT(!args.has("t"));
    ASSERT(!args.has("apina"));
    ASSERT(args.get("apina") == "");
    ASSERT(args.countValues("apina") == 0);
    ASSERT(args.has("test-key"));
    ASSERT(args.get("test-key") == "heps");
    ASSERT(args.countValues("test-key") == 2);

    Iterator < String > *it = args.getValues("test-key");
    ASSERT(it != 0);
    ASSERT(it->hasNext());
    ASSERT(it->next() == "heps");
    ASSERT(it->hasNext());
    ASSERT(it->next() == "kro");
    ASSERT(!it->hasNext());

    it->reset();
    ASSERT(it->hasNext());
    ASSERT(it->next() == "heps");
    ASSERT(it->hasNext());
    ASSERT(it->next() == "kro");
    ASSERT(!it->hasNext());

    for (int i = 0; i < 6; i++)
        delete[] t[i];
    delete[] t;

    Log::instance().setLogLevel(Log::Info);
}

int main(int argc, char ** argv) {
    cout << "Testing ArgumentReader..." << endl;
    cout << "-------------------------" << endl;

    cout << "Argument parsing ..... ";
    testArguments();
    cout << "ok" << std::endl;

    cout << "-------------------------" << endl;
    cout << "All tests passed!" << endl;
    return 0;
}
