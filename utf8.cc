/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "utf8.h"

#include <algorithm>

namespace {

    /* Append the two octets to 's' which are the UTF-8 encoding of 'n'.
     */
    void encode_to(std::string& s, unsigned n)
    {
	unsigned char cc[2];
	cc[0] = 0xc0 | (n>>6);
	cc[1] = 0x80 | (n & 0x3f);
	s.append(reinterpret_cast<char*>(cc), 2);
    }
}

/**
 * Encode (or transcode) a string assumed to be ASCII or ISO 8859-1 into
 * UTF-8.  Worst case, this doubles its size.
 */
std::string utf8::encode(const char* a, const char* b)
{
    std::string s;
    s.reserve(b - a);

    auto high = [] (unsigned n) { return n >= 0x80; };

    while (a!=b) {
	auto c = std::find_if(a, b, high);
	s.append(a, c);
	if (c==b) break;
	a = c;
	unsigned char n = *a++;
	encode_to(s, n);
    }
    return s;
}
