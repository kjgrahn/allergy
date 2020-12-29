/* Copyright (c) 2016 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "timestamp.h"

#include <iostream>
#include <cctype>
#include <cstdlib>

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
