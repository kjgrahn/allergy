/* Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "string.h"

#include "../filter.h"

#include <iostream>

using entity::String;

String::String(const char* s, const char* mime)
    : s(s),
      blob(s),
      mime(mime)
{}

std::ostream& String::headers(std::ostream& os) const
{
    os << "Content-Type: " << mime << "\r\n"
       << "Content-Language: sv\r\n"
       << "Content-Length: " << s.size() << "\r\n"
       << "Last-Modified: Mon, 04 Aug 2014 22:05:06 GMT\r\n";

    return os;
}

template<class Filter>
bool String::tick(int fd, Filter& filter)
{
    const Blob b = consume(blob);
    if (blob.empty()) return filter.end(fd, b);
    return filter.write(fd, b);
}

template bool String::tick(int fd, Filter::P& filter);
