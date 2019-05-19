/* Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "string.h"

using entity::String;

String::String(const char* s)
    : s(s),
      blob(s)
{}

String::String(const std::string& s)
    : s(s),
      blob(s)
{}

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
	size_t n = std::min(8192u, blob.size());
	blob = {a+n, b};
	return {a, a+n};
    }
}

Blob String::tick()
{
    return consume(blob);
}
