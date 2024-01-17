#if 0
// Функция 2
// Дана функция CheckDateTimeValidity. Оцените, насколько удобно ей пользоваться, тестировать, поддерживать, и выберите наиболее
// подходящий способ рефакторинга. Реализуйте выбранный способ и сохраните решение в файл CheckDateTimeValidity.cpp.
void CheckDateTimeValidity(const DateTime& dt) {
    if (dt.year < 1) {
        throw domain_error("year is too small"s);
    }
    if (dt.year > 9999) {
        throw domain_error("year is too big"s);
    }

    if (dt.month < 1) {
        throw domain_error("month is too small"s);
    }
    if (dt.month > 12) {
        throw domain_error("month is too big"s);
    }

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (dt.day < 1) {
        throw domain_error("day is too small"s);
    }
    if (dt.day > month_lengths[dt.month - 1]) {
        throw domain_error("day is too big"s);
    }

    if (dt.hour < 0) {
        throw domain_error("hour is too small"s);
    }
    if (dt.hour > 23) {
        throw domain_error("hour is too big"s);
    }

    if (dt.minute < 0) {
        throw domain_error("minute is too small"s);
    }
    if (dt.minute > 59) {
        throw domain_error("minute is too big"s);
    }

    if (dt.second < 0) {
        throw domain_error("second is too small");
    }
    if (dt.second > 59) {
        throw domain_error("second is too big"s);
    }
}
#endif

#include <string>
#include <stdexcept>
#include <array>
#include <vector>

using namespace std;
using namespace std::literals;

// o - object (year, month, ...)
// min < o < max - true, else throw
#define __CHECK_DATE_TIME_MIN_MAX(o, min, max) if(o < min) throw domain_error(#o" is too small"s); if(o > max) throw domain_error(#o" is too big"s)

struct DateTime {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

    void CheckDateTimeValidity() const;
    bool IsLeapYear() const;
    vector<int> MonthLength() const;
};

void DateTime::CheckDateTimeValidity() const {
    __CHECK_DATE_TIME_MIN_MAX(year  , 1, 9999);
    __CHECK_DATE_TIME_MIN_MAX(month , 1, 12);
    __CHECK_DATE_TIME_MIN_MAX(day   , 1, 31);
    __CHECK_DATE_TIME_MIN_MAX(hour  , 0, 23);
    __CHECK_DATE_TIME_MIN_MAX(minute, 0, 59);
    __CHECK_DATE_TIME_MIN_MAX(second, 0, 59);
}

bool DateTime::IsLeapYear() const {
    return (year % 4 == 0) && !(year % 100 == 0 && year % 400 != 0);
}

vector<int> DateTime::MonthLength() const {
    return {31, 28 + IsLeapYear() ? 1 : 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
}
