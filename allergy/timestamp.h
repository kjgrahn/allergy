/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_TIMESTAMP_H
#define ALLERGY_TIMESTAMP_H

#include "calendar.h"

#include <string>
#include <iosfwd>

namespace allergy {

    /**
     * A timestamp from an allergy(5) picture information file.
     * Fairly free-form, but must at least start yyyy-mm-dd,
     * and the rest should be time-of-day.
     */
    class Timestamp {
    public:
	Timestamp() = default;
	explicit Timestamp(const std::string& s);
	Timestamp& operator= (const Timestamp& other) = default;

	bool valid() const { return bool{day}; }
	bool operator== (const Timestamp& other) const;
	bool operator<  (const Timestamp& other) const;

	Day day;
	std::string time;
    };

    inline
    bool Timestamp::operator== (const Timestamp& other) const
    {
	if (day != other.day) return false;
	return time==other.time;
    }

    inline
    bool Timestamp::operator< (const Timestamp& other) const
    {
	if (day==other.day) return time < other.time;
	return day < other.day;
    }

    std::ostream& operator<< (std::ostream& os, const Timestamp& val);
}

#endif
