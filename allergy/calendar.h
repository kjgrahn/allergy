/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_CALENDAR_H
#define ALLERGY_CALENDAR_H

#include <string>
#include <iostream>
#include <array>

namespace allergy {

    struct Month;
    struct Year;

    /**
     * A certain day (e.g. "2020-11-06").
     */
    struct Day {
	Day() = default;
	explicit Day(const std::string&);
	Day(const char*, const char*);
	Day(unsigned year, unsigned mm, unsigned dd);

	explicit operator bool() const { return val[0] && val[1] && val[2]; }
	bool operator== (const Day& other) const { return val==other.val; }
	bool operator!= (const Day& other) const { return val!=other.val; }
	bool operator<  (const Day& other) const { return val<other.val; }
	bool operator<= (const Day& other) const { return !(other < *this); }

	Year year() const;
	Month month() const;
	std::string quarter() const;
	unsigned mday() const { return val[2]; }
	bool first() const { return *this && mday()==1; }

	std::string url() const;
	std::ostream& put_pretty(std::ostream& os) const;
	std::ostream& put(std::ostream& os) const;
	std::ostream& put_short(std::ostream& os) const;

    private:
	std::array<unsigned short, 3> val = {0, 0, 0};
    };

    /**
     * A certain month (e.g. "2020-09") parsed from a string, or else
     * invalid.
     */
    struct Month {
	Month() = default;
	explicit Month(const std::string&);
	Month(unsigned year, unsigned mm);
	Month(const char*, const char*);

	explicit operator bool() const { return val[0] && val[1]; }
	bool operator== (const Month& other) const { return val==other.val; }
	bool operator<  (const Month& other) const { return val<other.val; }
	bool operator<= (const Month& other) const { return !(other < *this); }
	Month operator-- (int);
	Month operator++ (int);

	const char* name() const;
	Year year() const;
	std::string quarter() const;
	unsigned num() const { return val[1]; }

	std::string url() const;
	std::ostream& put(std::ostream& os) const;

    private:
	std::array<unsigned short, 2> val = {0, 0};
    };

    inline Month prev(Month val) { val--; return val; }
    inline Month next(Month val) { val++; return val; }

    /**
     * A year (e.g. "2020") parsed from a string, or else invalid.
     */
    struct Year {
	Year() = default;
	explicit Year(const std::string&);
	explicit Year(unsigned short yyyy) : val {yyyy} {};
	Year(const char*, const char*);

	explicit operator bool() const { return val; }
	bool operator== (const Year& other) const { return val==other.val; }
	bool operator<  (const Year& other) const { return val<other.val; }
	bool operator<= (const Year& other) const { return !(other < *this); }
	Year operator-- (int) { auto prev = *this; val--; return prev; }
	Year operator++ (int) { auto prev = *this; val++; return prev; }
	std::array<Month, 12> months() const;

	unsigned value() const { return val; }
	std::string url() const;
	std::ostream& put(std::ostream& os) const;

    private:
	unsigned short val = 0;
    };

    inline Year prev(Year val) { val--; return val; }
    inline Year next(Year val) { val++; return val; }

    inline std::ostream& operator<< (std::ostream& os, const Day& val)   { return val.put(os); }
    inline std::ostream& operator<< (std::ostream& os, const Month& val) { return val.put(os); }
    inline std::ostream& operator<< (std::ostream& os, const Year& val)  { return val.put(os); }

    /**
     * A week-by-week calendar, starting at a month boundary and
     * lasting a number of months.  Kind of like cal(1) with the
     * -A option.
     *
     * Don't choose weird years, accept that weeks start on Monday,
     * and don't try to span New Year.
     */
    class Calendar {
    public:
	Calendar(unsigned year, unsigned mm, unsigned months);

	bool get(std::array<Day, 7>& week);

    private:
	unsigned mm;
	unsigned dd;
	const unsigned year;
	const unsigned mend;
    };
}
#endif
