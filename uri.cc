/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "uri.h"

#include <iostream>
#include <algorithm>

Uri::Uri(const char* a, const char* b)
    : a{a}, b{b}
{
    auto vi = v.begin();
    auto q = a;

    while (vi!=v.end()) {
	q = std::find(q, b, '/');
	*vi++ = q - a;
	if (q==b) break;
	q++;
    }
    vn = std::distance(v.begin(), vi);
}

bool Uri::operator== (const std::string& s) const
{
    return size() == s.size() && std::equal(begin(), end(), s.begin());
}

/**
 * True if there is a segment 'n' matching 's'.
 */
bool Uri::segment(const char* const a, const char* const b,
		  const size_t n) const
{
    if (n+1 >= vn) return false;
    auto c = begin() + v[n] + 1;
    auto d = begin() + v[n+1];
    if (std::distance(a, b) != std::distance(c, d)) return false;
    return std::equal(a, b, c);
}

std::ostream& operator<< (std::ostream& os, const Uri& val)
{
    return os.write(val.begin(), val.size());
}
