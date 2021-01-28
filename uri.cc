/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "uri.h"

#include <iostream>
#include <algorithm>

namespace {

    unsigned nybble(char ch)
    {
	auto in = [ch] (char a, char b) {
	    if (ch < a) return false;
	    if (b < ch) return false;
	    return true;
	};

	if (in('0', '9')) return ch - '0';
	if (in('A', 'F')) return ch - 'A' + 10;
	if (in('a', 'f')) return ch - 'a' + 10;
	return 16; // error
    }

    template <class It>
    char eat(It& p, const It pe)
    {
	if (p==pe) return 0;
	const char ch = *p++;
	if (ch!='%') return ch;

	if (p+2 > pe) return 0;
	unsigned a = nybble(*p++);
	unsigned b = nybble(*p++);
	if ((a|b) > 15) return 0;

	char pch = (a << 4) | b;
	return pch;
    }

    /**
     * String equality, allowing for URI percent-encoding in [p..pe),
     * so that "AC/DC" and "AC%2FDC" compare equal.
     *
     * May give some false positives for illegal percent-encodings,
     * but this doesn't matter for our use case.
     */
    template <class It>
    bool eq_percent(It a, const It ae,
		    It p, const It pe)
    {
	while (a!=ae) {
	    const char ch = *a++;
	    const char pch = eat(p, pe);
	    if (ch!=pch) return false;
	}
	return p==pe;
    }

    /**
     * Verbatim string equality. no percent-encoding.
     */
    template <class It>
    bool eq(It a, const It ae,
	    It b, const It be)
    {
	if (std::distance(a, ae) != std::distance(b, be)) return false;
	return std::equal(a, ae, b);
    }
}

Uri::Uri(const char* a, const char* b)
    : a{a}, b{b},
      percent(std::count(a, b, '%'))
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
    if (percent) return eq_percent(sa, sb, c, d);
    return eq(sa, sb, c, d);
}

/**
 * Decode percent-encoding in [a, b).
 */
std::string Uri::decode(const char* a, const char* const b)
{
    std::string s;
    s.reserve(std::distance(a, b));

    while (a!=b) {
	s.push_back(eat(a, b));
    }
    return s;
}

std::ostream& operator<< (std::ostream& os, const Uri& val)
{
    val.put(os);
    return os;
}
