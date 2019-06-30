/* Copyright (c) 2012, 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "response.h"

#include "request.h"
#include "filter.h"
#include "date.h"

#include <cassert>
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


    class DateConv dateconv;

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

    template <class Body>
    bool tick(int fd, Backlog& backlog,
	      response::Headers& headers,
	      Body& body)
    {
	if(!backlog.empty()) return backlog.write(fd);
	if(!headers.done()) return headers.tick(fd);
	return body.tick(fd);
    }
}

bool response::Headers::tick(int fd)
{
    return text.tick(fd, filter);
}

bool response::Headers::done() const
{
    // ok because the filter is always Plain.
    return text.done();
}

bool response::Error::tick(int fd)
{
    bool unblocked = ::tick(fd, backlog, headers, body);
    done = body.done();
    return unblocked;
}

bool response::Image::tick(int fd)
{
    bool unblocked = ::tick(fd, backlog, headers, body);
    done = body.done();
    return unblocked;
}

bool response::Generated::tick(int fd)
{
    bool unblocked = ::tick(fd, backlog, headers, body);
    done = body.done();
    return unblocked;
}
