#include "util/Date.hpp"
#include <iomanip>

using namespace std;
using namespace util;

Date::Date() :
    rawtime() {
    time(&rawtime);
    struct tm * tp = localtime(&rawtime);
    timeinfo = *tp;
}

Date::~Date() {
}

/* Returns the weekday */
Weekday Date::weekday() const {
    switch (timeinfo.tm_wday) {
    case 0:
        return Sunday;
    case 1:
        return Monday;
    case 2:
        return Tuesday;
    case 3:
        return Wednesday;
    case 4:
        return Thusday;
    case 5:
        return Friday;
    case 6:
        return Saturday;
    }
    return InvalidDay;
}

std::ostream & operator<<(std::ostream & str, util::Date const & date) {
    str << setfill('0') << setw(4) << date.year() << "-" << setw(2)
            << date.month() << "-" << setw(2) << date.day() << " " << setw(2)
            << date.hours() << ":" << setw(2) << date.minute() << ":"
            << setw(2) << date.second();
    return str;
}
