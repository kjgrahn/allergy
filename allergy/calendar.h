/* Copyright (c) 2020 J�rgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_CALENDAR_H
#define ALLERGY_CALENDAR_H

#include <string>
#include <iostream>

namespace allergy {

    /**
     * A certain day (e.g. "2020-11-06").
     */
    struct Day {
	Day() = default;
	explicit Day(const std::string&);
	Day(const char*, const char*);
	explicit operator bool() const { return !val.empty(); }
	bool operator== (const Day& other) const { return val==other.val; }
	bool operator<  (const Day& other) const { return val<other.val; }

	std::string val;
    };

    /**
     * A certain month (e.g. "2020-09") parsed from a string, or else
     * invalid.
     */
    struct Month {
	Month() = default;
	explicit Month(const std::string&);
	Month(const char*, const char*);
	explicit operator bool() const { return !val.empty(); }
	bool operator== (const Month& other) const { return val==other.val; }
	bool operator<  (const Month& other) const { return val<other.val; }

	std::string val;
    };

    /**
     * A year (e.g. "2020") parsed from a string, or else invalid.
     */
    struct Year {
	Year() = default;
	explicit Year(const std::string&);
	Year(const char*, const char*);
	explicit operator bool() const { return !val.empty(); }
	bool operator== (const Year& other) const { return val==other.val; }
	bool operator<  (const Year& other) const { return val<other.val; }

	std::string val;
    };

    inline std::ostream& operator<< (std::ostream& os, const Day& val)   { return os << val.val; }
    inline std::ostream& operator<< (std::ostream& os, const Month& val) { return os << val.val; }
    inline std::ostream& operator<< (std::ostream& os, const Year& val)  { return os << val.val; }
}
#endif
