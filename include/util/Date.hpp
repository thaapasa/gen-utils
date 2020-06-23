#if !defined(UTIL_DATE_HPP)
#define UTIL_DATE_HPP

#include <ctime>
#include <iostream>

/*
 * A storage class for storing and formatting date time objects; like
 * Java's date + parts of calendar.
 * 
 * @author Tuukka Haapasalo
 * @created 2010-12-10
 */

namespace util {

enum Weekday {
    Monday, Tuesday, Wednesday, Thusday, Friday, Saturday, Sunday, InvalidDay
};

class Date {
public:
Date();
~Date();

    int second() const { return timeinfo.tm_sec; }
    int minute() const { return timeinfo.tm_min; }
    int hours() const { return timeinfo.tm_hour; }

    /* Returns the day of the month, range 1-31 */
    int day() const { return timeinfo.tm_mday; }
    /* Returns the month, range 1-12 */
    int month() const { return timeinfo.tm_mon + 1; }
    int year() const { return timeinfo.tm_year + 1900; }

    /* Returns the weekday */
    util::Weekday weekday() const;

    /* Returns the day of the year, range 1-366 */
    int dayOfYear() const { return timeinfo.tm_yday + 1; }

private:
    time_t rawtime;
    struct tm timeinfo;

};

}

std::ostream & operator<<(std::ostream & str, util::Date const & date);

#endif // !defined(UTIL_DATE_HPP)
