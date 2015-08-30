/* Copyright (c) 2012, 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "response.h"

#include "request.h"

#include <cassert>
#include <cctype>

namespace {

    std::string rstrip(const std::string& s)
    {
	std::string r = s;

	const char* const p = s.c_str();
	const char* q = p + s.size();
	while(p!=q) {
	    if(!isspace(*(q-1))) break;
	    q--;
	}
	r.resize(q-p);
	return r;
    }
}


