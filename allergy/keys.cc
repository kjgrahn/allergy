/* Copyright (c) 2014 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "keys.h"

using allergy::Keys;


namespace {

    char right(char ch)
    {
	switch(ch) {
	case '[': return ']';
	case '{': return '}';
	default: return '\0';
	}
    }

    /**
     * Find matching bracket; ] or } depending on what *a is.
     * Otherwise return b.  Never returns a, unless a==b.
     */
    template<class It> It match_bracket(It a, It b)
    {
	if(a!=b) {
	    const char ch = *a++;
	    const char ech = right(ch);
	    unsigned n = 1;
	    while(a!=b) {
		if(*a==ch) n++;
		else if(*a==ech) n--;
		if(!n) break;
		a++;
	    }
	}
	return a;
    }

    std::string parse(const char* a, const char* const b)
    {
	std::string s;

	while(a!=b) {
	    if(*a=='[') {
		const char* c = match_bracket(a, b);
		if(c!=b) {
		    s.append(parse(a+1, c));
		    a = c+1;
		    continue;
		}
	    }
	    s.push_back(*a++);
	}
	return s;
    }

    std::string parse(const std::string& s)
    {
	const char* p = s.data();
	return parse(p, p + s.size());
    }
}


Keys::Keys(const std::string& s)
    : str(parse(s))
{}

namespace {
    static const std::vector<std::string> dummy;
}

Keys::const_iterator Keys::begin() const
{
    return dummy.begin();
}

Keys::const_iterator Keys::end() const
{
    return dummy.end();
}

bool Keys::empty() const
{
    return dummy.empty();
}
