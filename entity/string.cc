/* Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "string.h"

#include "../filter.h"

#include <iostream>

using entity::String;

String::String(const char* s)
    : s(s),
      blob(s)
{}

String::String(const std::string& s)
    : s(s),
      blob(s)
{}

std::ostream& String::headers(std::ostream& os) const
{
    os << "Content-Type: foo\r\n"
       << "Content-Language: sv\r\n"
       << "Content-Length: " << s.size() << "\r\n"
       << "Last-Modified: Mon, 04 Aug 2014 22:05:06 GMT\r\n";

    return os;
}

namespace {

    /**
     * Consume a sizeable chunk off the blob.
     *
     * Should be placed elsewhere.
     */
    Blob consume(Blob& blob)
    {
	auto a = std::begin(blob);
	auto b = std::end(blob);
	size_t n = std::min({8192}, blob.size());
	blob = {a+n, b};
	return {a, a+n};
    }
}

template<class Filter>
bool String::tick(int fd, Filter& filter)
{
    const Blob b = consume(blob);
    if (blob.empty()) return filter.end(fd, b);
    return filter.write(fd, b);
}

template bool String::tick(int fd, Filter::P& filter);
