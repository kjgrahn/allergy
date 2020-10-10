/* Copyright (c) 2020 J�rgen Grahn
 * All rights reserved.
 *
 */
#include "photo.h"
#include "../root.h"

#include <cctype>


using allergy::Photo;

namespace {
    const char* eat_digits(const char* a)
    {
	while (std::isdigit(static_cast<unsigned char>(*a))) a++;
	return a;
    }

    template <class It>
    unsigned strtou(It a, It b)
    {
	unsigned n = 0;
	while (a!=b) {
	    n = 10*n + (*a++ - '0');
	}
	return n;
    }

    /**
     * The quarter 1--4, or 0.  Assumes an otherwise validated file
     * name.
     */
    unsigned quarter_of(const std::string& s)
    {
	const char* a = s.c_str();
	const char* c = eat_digits(a);
	c = (c-a == 4) ? a+5 : a+2;
	unsigned n = strtou(c, c+2);
	unsigned q = 1 + (n-1) / 3;
	if (q > 4) q = 0;
	return q;
    }

    unsigned year_of(const std::string& s)
    {
	const char* a = s.c_str();
	const char* c = eat_digits(a);
	if (c-a == 4) return strtou(a, a+4);
	return 2000 + strtou(a, a+2);
    }
}

Photo::Photo(const std::string& s)
    : val {s},
      quarter {0}
{
    const char* a = val.c_str();
    const char* const b = a + val.size();
    const char* c;

    c = eat_digits(a);
    if (c-a == 4) {
	if (*c != '-') return;
	a = c+1;

	c = eat_digits(a);
	if (c-a != 2) return;
	if (*c != '-') return;
	a = c+1;

	c = eat_digits(a);
	if (c-a != 2) return;
    }
    else if (c-a != 6) {
	return;
    }

    if (*c != '_') return;
    a = c+1;

    c = eat_digits(a);
    if (c-a < 1) return;
    if (*c != '.') return;
    a = c+1;
    if (std::string(a, b) != "jpg") return;

    quarter = quarter_of(val);
}

Photo::Photo(const char* a, const char* const b)
    : Photo {std::string{a, b}}
{}

std::string Photo::dir() const
{
    if (!quarter) return {};

    return std::to_string(year_of(val)) + '.' + std::to_string(quarter);
}

/**
 * The (relative) path to the photo in the file system.
 */
std::string Photo::path() const
{
    return dir() + "/" + val;
}

int Photo::open(const Root& root) const
{
    return root.open(path());
}

/**
 * Open 'p' relative to the root. The photo needs to be valid().
 */
int allergy::open(const Root& r, const Photo& p)
{
    return p.open(r);
}
