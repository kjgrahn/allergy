/* Copyright (c) 2012, 2013, 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "response.h"

#include "request.h"
#include "filter.h"
#include "date.h"

#include <cctype>
#include <sstream>


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


    DateConv dateconv;

    std::string now()
    {
	return dateconv.format(time(0));
    }


    template <class Filter>
    bool write(Filter& out, int fd, std::ostringstream& oss)
    {
	std::string s = oss.str();
	return out.write(fd, Blob(s));
    }
}

bool response::Headers::tick(int fd)
{
    return text.tick(fd, filter);
}

bool response::Headers::done() const
{
    return text.done();
}

bool response::Image::tick(int fd)
{
    bool blocked = response::tick(fd, backlog, headers, body);
    done = body.done();
    return blocked;
}
