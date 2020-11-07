/* -*- c++ -*-
 *
 * Copyright (c) 2016 Jörgen Grahn
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
     * Fairly free-form, but must at least start yyyy-mm-dd.
     */
    class Timestamp {
    public:
	Timestamp() = default;
	explicit Timestamp(const std::string& s);
	bool valid() const { return !str.empty(); }

	bool operator== (const Timestamp& other) const { return str==other.str; }
	bool operator<  (const Timestamp& other) const { return str<other.str; }

	Year  year;
	Month month;
	Day   date;
	std::string str;
    };

    std::ostream& operator<< (std::ostream& os, const Timestamp& val);
}

#endif
