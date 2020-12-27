/* Copyright (c) 2016 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "timestamp.h"

#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cstring>

using allergy::Timestamp;
using allergy::Day;
using allergy::Month;
using allergy::Year;

namespace {
    unsigned strtou(const char* p, const char** end)
    {
	char* e;
	unsigned n = std::strtoul(p, &e, 10);
	*end = e;
	return n;
    }

    bool in_range(unsigned a, unsigned n, unsigned b)
    {
	return a<=n && n<=b;
    }

    bool isspace(const char ch)
    {
	unsigned char n = ch;
	return std::isspace(n);
    }
}

Timestamp::Timestamp(const std::string& s)
{
    const char* const p0 = &s[0];
    const char* p = p0;
    const char* const q = p + s.size();

    const char* end;
    const unsigned n = strtou(p, &end);
    if(*end!='-' || !in_range(1800, n, 3000)) return;
    year = {p0, end};

    p = end+1;
    const unsigned mm = strtou(p, &end);
    if(*end!='-' || !in_range(1, mm, 12)) return;
    month = {p0, end};

    p = end+1;
    const unsigned dd = strtou(p, &end);
    if(!in_range(1, dd, 31)) return;
    if(*end && !isspace(*end)) return;
    date = {p0, end};

    str = s;
}

std::ostream& allergy::operator<< (std::ostream& os, const Timestamp& val)
{
    return os << val.str;
}

namespace {

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
}

Day::Day(const char* a, const char* b)
    : val{a, b}
{
    if (!match(a, b, "nnnn-nn-nn")) val.clear();
}

Day::Day(const std::string& s)
    : Day {s.c_str(), s.c_str() + s.size()}
{}

Month::Month(const char* a, const char* b)
    : val{a, b}
{
    if (!match(a, b, "nnnn-nn")) val.clear();
}

Month::Month(const std::string& s)
    : Month {s.c_str(), s.c_str() + s.size()}
{}

Year::Year(const char* a, const char* b)
    : val{a, b}
{
    if (!match(a, b, "nnnn")) val.clear();
}

Year::Year(const std::string& s)
    : Year {s.c_str(), s.c_str() + s.size()}
{}
