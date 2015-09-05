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


/**
 * A fixed 404 Not found response.
 */
class NotFound : public Response {
public:
    NotFound() : n(0) {}
    bool write(int fd);
    bool done() const { return "XXX"; }

private:
    unsigned n;
    Filter::P out;
};


/* When we get here, we may be in either of several
 * valid states:
 *
 * src    filter     effect
 * ------ ------     ------
 *   data      -     write data to filter
 *   data   data     write data to filter
 *      -      -     write eof
 *      -   data     write eof
 *      -   data+eof flush
 */
bool NotFound::write(int fd)
{
    const bool first = !n++;

    if(first) {

	std::ostringstream oss;
	oss << "HTTP/1.1 404 Not Found\r\n"
	    << "Date: " << now() << "\r\n"
	    << "Server: outnumbered/0\r\n"
	    << "Content-Type: text/html; charset=iso-8859-1\r\n"
	    << "Content-length: 366\r\n"
	    << "\r\n";

	oss << "<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.01//EN' "
	       "'http://www.w3.org/TR/html4/strict.dtd'>\r\n"
	    << "<html>\r\n"
	    << "<head>\r\n"
	    << "<meta http-equiv='Content-Type' content='text/html; charset=iso-8859-1'>\r\n"
	    << "<meta http-equiv='Content-Language' content='en'>\r\n"
	    << "<link rel='stylesheet' href='css' type='text/css'>\r\n"
	    << "<title>Not found</title>\r\n"
	    << "</head>\r\n"
	    << "<body>\r\n"
	    << "<h1>Not found</h1>\r\n"
	    << "</body>\r\n"
	    << "</html>\r\n";

	return ::write(out, fd, oss);
    }

    return out.write(fd);
}


Response* response_of(const Request& request)
{
    return new NotFound;
}
