/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "quote.h"

#include <iostream>
#include <algorithm>
#include <functional>

namespace {

    bool special(char ch)
    {
	switch(ch) {
	case '<':
	case '&':
	    return true;
	default:
	    return false;
	}
    }

    bool special_attr(char ch)
    {
	switch(ch) {
	case '\'':
	case '"':
	    return true;
	default:
	    return false;
	}
    }

    /**
     * Put [a, b) onto 'os', but escape characters matching 'f'.
     */
    void quote_to(const std::function<bool(char)> f, std::ostream& os,
		  const char* a, const char* const b)
    {
	auto c = std::find_if(a, b, f);
	os.write(a, c-a);
	a = c;

	while (a!=b) {
	    char ch = *a++;
	    switch(ch) {
	    case '<':  os << "&lt;"; break;
	    case '&':  os << "&amp;"; break;
	    case '\'': os << "&#39;"; break;
	    case '"':  os << "&quot;"; break;
	    }

	    auto c = std::find_if(a, b, f);
	    os.write(a, c-a);
	    a = c;
	}
    }
}

std::ostream& operator<< (std::ostream& os, const quote& val)
{
    const char* a = val.s.c_str();
    quote_to(special, os, a, a + val.s.size());
    return os;
}

std::ostream& attr::operator<< (std::ostream& os, const quote& val)
{
    const char* a = val.s.c_str();
    quote_to(special_attr, os, a, a + val.s.size());
    return os;
}
