/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "photo.h"

#include <cctype>


using allergy::Photo;

namespace {
    const char* eat_digits(const char* a)
    {
	while (std::isdigit(static_cast<unsigned char>(*a))) a++;
	return a;
    }

    unsigned quarter_of(const char* p)
    {
	unsigned n = std::strtoul(p, nullptr, 10);
	if (n < 1 || n > 12) return 0;
	return 1 + (n-1) / 3;
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
    if (c-a != 4) return;
    if (*c != '-') return;
    a = c+1;

    c = eat_digits(a);
    if (c-a != 2) return;
    if (*c != '-') return;
    const char* const q = a;
    a = c+1;

    c = eat_digits(a);
    if (c-a != 2) return;
    if (*c != '_') return;
    a = c+1;

    c = eat_digits(a);
    if (c-a < 1) return;
    if (*c != '.') return;
    a = c+1;
    if (std::string(a, b) != "jpg") return;

    quarter = quarter_of(q);
}

Photo::Photo(const char* a, const char* const b)
    : Photo {std::string{a, b}}
{}
