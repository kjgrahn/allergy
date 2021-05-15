/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "timestamp.h"

#include <iostream>
#include <algorithm>

using allergy::Timestamp;

namespace {

    allergy::Day day_of(const std::string& s)
    {
	const auto a = s.data();
	const auto b = a + s.size();
	const auto c = std::find(a, b, ' ');
	return {a, c};
    }

    std::string rest_of(const std::string& s)
    {
	auto a = s.data();
	const auto b = a + s.size();
	a = std::find(a, b, ' ');
	while (a!=b && *a==' ') a++;
	return {a, b};
    }
}

Timestamp::Timestamp(const std::string& s)
    : day {day_of(s)},
      time {rest_of(s)}
{}

std::ostream& allergy::operator<< (std::ostream& os, const Timestamp& val)
{
    return os << val.day << ' ' << val.time;
}
