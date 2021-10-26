/* -*- c++ -*-
 *
 * Copyright (c) 2012 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_BLOB_H_
#define GB_BLOB_H_

#include <stdint.h>
#include <cstdlib>
#include <vector>
#include <array>
#include <string>
#include <cstring>


/**
 * Just my usual range-of-octets utility type.
 * Of course, sensitive to changes in the underlying object.
 */
class Blob {
public:
    Blob() : a(0), n(0) {}
    Blob(const char* a, size_t n)
	: a(reinterpret_cast<const uint8_t*>(a)),
	  n(n)
    {}
    Blob(const char* a, const char* b)
	: Blob(a, b-a)
    {}
    Blob(const uint8_t* a, size_t n) : a(a), n(n) {}
    Blob(const uint8_t* a,
	 const uint8_t* b)
	: Blob(a, b-a)
    {}
    explicit Blob(const std::vector<uint8_t>& v)
	: Blob(v.data(), v.size())
    {}
    template<std::size_t n>
    explicit Blob(const std::array<uint8_t, n>& v)
	: a(v.data()),
	  n(v.size())
    {}
    explicit Blob(const std::string& s)
	: Blob(s.data(), s.size())
    {}
    explicit Blob(const char* s)
	: Blob(s, std::strlen(s))
    {}

    const uint8_t* begin() const { return a; }
    const uint8_t* end() const { return a + n; }

    bool empty() const { return !n; }
    size_t size() const { return n; }
    operator const void*() const { return empty()? 0: this; }
    bool operator== (const Blob& other) const;

private:
    const uint8_t* a;
    size_t n;
};

Blob consume(Blob& blob);

#endif
