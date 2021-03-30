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

/**
 * Print the URI verbatim.
 */
void Uri::put(std::ostream& os) const
{
    os.write(a, b-a);
}

/**
 * True if there is a segment 'n' matching [sa, sb).
 */
bool Uri::segment(const char* const sa, const char* const sb,
		  const size_t n) const
{
    if (n+1 >= vn) return false;
    auto c = a + v[n] + 1;
    auto d = a + v[n+1];
    if (std::distance(sa, sb) != std::distance(c, d)) return false;
    return std::equal(sa, sb, c);
}

std::ostream& operator<< (std::ostream& os, const Uri& val)
{
    val.put(os);
    return os;
}
