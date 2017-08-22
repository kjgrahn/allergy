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
}


response::Error::Error(const char* status)
    : entity(status)
{
    std::ostringstream oss;
    const char crlf[] = "\r\n";
    oss << "HTTP/1.1 " << status << crlf
	<< "Date: " << now() << crlf
	<< "Server: allergy" << crlf
	<< "Content-Length: " << entity.size() << crlf
	<< "Content-Type: text/plain; charset=iso-8859-1" << crlf
	<< crlf;
    headers = entity::String(oss.str());
}

namespace {
    template <class A, class B>
    Blob tick(A& a, B& b)
    {
	Blob blob = a.tick();
	if(blob.size()) return blob;
	return b.tick();
    }
}

bool response::Error::tick(int fd)
{
    if(backlog.empty()) {
	Blob b = ::tick(headers, entity);
    }
    else {
	return backlog.write(fd);
    }
}
