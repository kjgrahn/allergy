/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "blob.h"

bool Blob::operator== (const Blob& other) const
{
    return size()==other.size() &&
	std::equal(begin(), end(), other.begin());
}

/**
 * Consume a sizeable chunk off a blob.
 */
Blob consume(Blob& blob)
{
    auto a = std::begin(blob);
    auto b = std::end(blob);
    size_t n = std::min(size_t{8192}, blob.size());
    blob = {a+n, b};
    return {a, a+n};
}
