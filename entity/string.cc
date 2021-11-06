/* Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "string.h"

#include "../filter.h"
#include "../crlf.h"

#include <iostream>

using entity::String;

String::String(const Content& content, const char* s, const char* mime)
    : Entity(content),
      s(s),
      blob(s),
      mime(mime)
{}

std::ostream& String::headers(std::ostream& os) const
{
    os << "Content-Type: " << mime << crlf
       << "Content-Language: sv" << crlf
       << "Content-Length: " << s.size() << crlf
       << "Last-Modified: " << content.format(content.epoch) << crlf;

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
