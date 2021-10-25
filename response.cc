/* Copyright (c) 2012, 2013, 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "response.h"

#include "date.h"
#include "crlf.h"


using response::Headers;

bool Headers::tick(int fd)
{
    const Blob b = consume(blob);
    if (blob.empty()) return filter.end(fd, b);
    return filter.write(fd, b);
}

std::ostream& Headers::general_headers(std::ostream& oss,
				       const timespec& ts,
				       bool chunked) const
{
    static DateConv dateconv;
    oss << "Date: " << dateconv.format(ts.tv_sec) << crlf;
    if (chunked) {
	oss << "Transfer-Encoding: chunked" << crlf;
    }
    return oss;
}

/**
 * Render response headers [6.2] to 'oss'. For the extra ones in 'hh',
 * there are two list elements per header: the name and the value.
 */
std::ostream& Headers::response_headers(std::ostream& oss,
					std::initializer_list<const std::string> hh) const
{
    oss << "Server: allergy" << crlf;
    auto p = begin(hh);
    while (p!=end(hh)) {
	const auto& name = *p++;
	const auto& val = *p++;
	oss << name << ": " << val << crlf;
    }
    return oss;
}

bool response::Headers::done() const
{
    return blob.empty();
}

bool response::tick(int fd, Backlog& backlog,
		    response::Headers& headers)
{
    if(!backlog.empty()) return backlog.write(fd);
    assert(!headers.done());
    return headers.tick(fd);
}

response::Redirect::Redirect(const timespec& ts, const std::string& uri)
    : Response(Status<301>{}),
      headers {ts, backlog, Status<301>{},
	       {"Location", uri,
		"Content-Length", "0"}}
{}

bool response::Redirect::tick(int fd)
{
    bool blocked = response::tick(fd, backlog, headers);
    done = headers.done();
    return blocked;
}

bool response::File::tick(int fd)
{
    bool blocked = response::tick(fd, backlog, headers, body);
    done = body.done();
    return blocked;
}

bool response::Image::tick(int fd)
{
    bool blocked = response::tick(fd, backlog, headers, body);
    done = body.done();
    return blocked;
}
