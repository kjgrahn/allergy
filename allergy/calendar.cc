/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "calendar.h"

#include <cstring>

namespace {

    /**
     * Valid at least 1970--2050.
     */
    bool leapyear(unsigned year)
    {
	return year % 4 == 0;
    }

    /**
     * The number of days in a certain month (1--12).
     */
    unsigned days(unsigned year, unsigned mm)
    {
	switch (mm) {
	case 11:
	case 4:
	case 6:
	case 9:
	    // "tretti dagar har november; april, juni och september"
	    return 30;
	case 2:
	    return 28 + leapyear(year);
	default:
	    return 31;
	}
    }

    template <class It>
    bool match(It a, const It b, const char* p)
    {
	if (std::distance(a, b) != std::strlen(p)) return false;
	while (a!=b) {
	    if (*p=='n') {
		if (!std::isdigit(static_cast<unsigned char>(*a))) return false;
	    }
	    else {
		if (*p != *a) return false;
	    }
	    p++;
	    a++;
	}
	return true;
    }

    /**
     * Is 'val' in [a..b]?  Note the inclusive borders;
     * 'a' and 'b' are in [a..b].
     */
    template <class C, C a, C b>
    bool in(C val)
    {
	if (val < a) return false;
	if (b < val) return false;
	return true;
    }

    /**
     * Find an unsigned decimal number at the beginning of [a..b);
     * update a to point to the first non-digit; return 0 unless
     * val < limit.
     *
     * Because std::strtoul() only handles C strings, and std::stou()
     * throws exceptions at random, and I don't have std::string_view.
     */
    template <class It>
    unsigned strtou(It& a, const It b, unsigned limit)
    {
	unsigned acc = 0;

	while (a!=b && in<char, '0', '9'>(*a)) {
	    char ch = *a++;
	    acc = 10*acc + (ch - '0');
	}

	if (acc < limit) return acc;
	return 0;
    }

    /**
     * Shorthand, because the compilers complain.
     */
    unsigned short narrow(unsigned n)
    {
	return static_cast<unsigned short>(n);
    }
}

using allergy::Day;
using allergy::Month;
using allergy::Year;

Day::Day(const char* a, const char* b)
{
    if (match(a, b, "nnnn-nn-nn")) {
	val[0] = strtou(a, b, 3000);
	a++;
	val[1] = strtou(a, b, 13);
	a++;
	val[2] = strtou(a, b, 1 + days(val[0], val[1]));
    }
}

Day::Day(const std::string& s)
    : Day {s.c_str(), s.c_str() + s.size()}
{}

Day::Day(unsigned year, unsigned mm, unsigned dd)
    : val {narrow(year),
	   narrow(mm),
	   narrow(dd)}
{
    if (!in<unsigned, 1, 12>(mm)) val.fill(0);
    if (dd==0 || dd > days(year, mm)) val.fill(0);
}

Year Day::year() const { return Year {val[0]}; }

Month Day::month() const { return Month {val[0], val[1]}; }

std::ostream& Day::put(std::ostream& os) const
{
    char buf[11];
    std::snprintf(buf, sizeof buf, "%4hu-%02hu-%02hu",
		  val[0], val[1], val[2]);
    return os << buf;
}

Month::Month(const char* a, const char* b)
{
    if (match(a, b, "nnnn-nn")) {
	val[0] = strtou(a, b, 3000);
	a++;
	val[1] = strtou(a, b, 13);
    }
}

Month::Month(const std::string& s)
    : Month {s.c_str(), s.c_str() + s.size()}
{}

Month::Month(unsigned year, unsigned mm)
    : val {narrow(year),
	   narrow(mm)}
{
    if (!in<unsigned, 1, 12>(mm)) val.fill(0);
}

Month Month::operator-- (int)
{
    auto prev = *this;
    if (val[1]==1) {
	val[0]--;
	val[1] = 12;
    }
    else {
	val[1]--;
    }
    return prev;
}

Month Month::operator++ (int)
{
    auto prev = *this;
    if (val[1]==12) {
	val[0]++;
	val[1] = 1;
    }
    else {
	val[1]++;
    }
    return prev;
}

const char* Month::name() const
{
    switch (val[1]) {
    case  1: return "jan";
    case  2: return "feb";
    case  3: return "mar";
    case  4: return "apr";
    case  5: return "maj";
    case  6: return "jun";
    case  7: return "jul";
    case  8: return "aug";
    case  9: return "sep";
    case 10: return "okt";
    case 11: return "nov";
    case 12: return "dec";
    default: return nullptr;
    }
}

Year Month::year() const { return Year {val[0]}; }

std::string Month::url() const
{
    char buf[9];
    std::snprintf(buf, sizeof buf, "/%4hu-%02hu",
		  val[0], val[1]);
    return buf;
}

std::ostream& Month::put(std::ostream& os) const
{
    char buf[8];
    std::snprintf(buf, sizeof buf, "%4hu-%02hu",
		  val[0], val[1]);
    return os << buf;
}

Year::Year(const char* a, const char* b)
{
    if (match(a, b, "nnnn")) {
	val = strtou(a, b, 3000);
    }
}

Year::Year(const std::string& s)
    : Year {s.c_str(), s.c_str() + s.size()}
{}

std::array<Month, 12> Year::months() const
{
    std::array<Month, 12> res;
    for (unsigned n=0; n<12; n++) res[n] = {val, n+1};
    return res;
}

std::string Year::url() const
{
    char buf[6];
    std::snprintf(buf, sizeof buf, "/%4hu", val);
    return buf;
}

std::ostream& Year::put(std::ostream& os) const
{
    char buf[5];
    std::snprintf(buf, sizeof buf, "%4hu", val);
    return os << buf;
}

namespace {

    /**
     * The weekday of year-01-01.
     */
    unsigned weekday(const unsigned year)
    {
	unsigned n = 3;
	for (unsigned y = 1970; y < year; y++) {
	    n += 1 + leapyear(y);
	}
	return n % 7;
    }

    /**
     * The weekday of year-mm-01, from 0 (Monday) to 6 (Sunday). My
     * weeks start on Monday.
     */
    unsigned weekday(unsigned year, unsigned mm)
    {
	static const std::array<unsigned char, 12> offs {0,3,3,6, 1,4,6,2, 5,0,3,5};
	static const std::array<unsigned char, 12> leap {0,3,4,0, 2,5,0,3, 6,1,4,6};
	unsigned n = leapyear(year) ? leap[mm-1] : offs[mm-1];
	return (weekday(year) + n) % 7;
    }
}

using allergy::Calendar;

Calendar::Calendar(unsigned year, unsigned mm, unsigned months)
    : mm {mm},
      dd {1},
      year {year},
      mend {mm+months}
{}

/**
 * Get the next week, or return false.
 */
bool Calendar::get(std::array<Day, 7>& week)
{
    if (mm==mend) return false;

    auto it = begin(week);
    week.fill({});

    if (dd==1) {
	it += weekday(year, mm);
    }

    while (it!=end(week)) {
	*it++ = {year, mm, dd};
	dd++;
	if (dd > days(year, mm)) {
	    dd = 1;
	    mm++;
	    if (mm==mend) break;
	}
    }

    return true;
}
