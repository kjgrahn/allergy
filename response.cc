/* Copyright (c) 2012, 2013, 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "response.h"

#include "date.h"


using response::Headers;

bool Headers::tick(int fd)
{
    return text.tick(fd, filter);
}

std::ostream& Headers::general_headers(std::ostream& oss,
				       const timespec& ts) const
{
    static DateConv dateconv;
    return oss << "Date: " << dateconv.format(ts.tv_sec) << "\r\n";
}

std::ostream& Headers::response_headers(std::ostream& oss) const
{
    return oss << "Server: allergy\r\n";
}

bool response::Headers::done() const
{
    return text.done();
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
