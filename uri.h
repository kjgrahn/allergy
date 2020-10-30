/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_URI_H
#define ALLERGY_URI_H

#include "blob.h"

#include <string>
#include <array>
#include <iosfwd>

/**
 * A HTTP Request-URI [5.1.2] as received in a request.
 *
 * Doesn't own its data (typically a Request does).
 *
 * Only what's needed for our purposes: assumed to be on the form
 * "/a/b/c/d/..." and we look at segments between slashes (or after
 * the last slash; an URI which ends with a slash ends with a segment
 * which happens to be empty).
 *
 * Supports simple matches like /foo/bar/T, where T is a user-defined
 * type.
 */
class Uri {
public:
    Uri(const char* a, const char* b);
    Uri(const std::string& s) : Uri{s.data(), s.data() + s.size()} {}
    bool operator== (const std::string& s) const;

    const char* begin() const { return a; }
    const char*   end() const { return b; }

    size_t size() const { return b - a; }
    bool  empty() const { return !size(); }

    bool segments(size_t n) const { return n+1==vn; }
    bool segment(const char* a, const char* b, size_t n) const;
    bool segment(const char* s, size_t n) const { return segment(s, s + std::strlen(s), n); }

    template <class T>
    T make(size_t n) const
    {
	if (n+1 >= vn) return nil<T>();
	auto c = begin() + v[n] + 1;
	auto d = begin() + v[n+1];
	return T {c, d};
    }

    template <class T>
    T nil() const { const char* a = {}; return T {a, a}; }

private:
    const char* const a;
    const char* const b;

    std::array<unsigned, 5> v;
    unsigned vn;
};

std::ostream& operator<< (std::ostream& os, const Uri& val);

/**
 * Returns the T in /T, or else T{}.
 */
template <class T>
T match(const Uri& uri)
{
    if (!uri.segments(1)) return uri.nil<T>();
    return uri.make<T>(0);
}

template <>
bool match<bool>(const Uri& uri);

/**
 * Returns the T in /foo/T, or else T{}.
 * Doesn't match /foo/T/ or /foo/T/bar.
 */
template <class T>
T match(const Uri& uri, const char* a)
{
    if (!uri.segments(2)) return uri.nil<T>();
    if (!uri.segment(a, 0)) return uri.nil<T>();
    return uri.make<T>(1);
}

template <> inline
bool match<bool>(const Uri& uri, const char* a)
{
    if (!uri.segments(1)) return false;
    if (!uri.segment(a, 0)) return false;
    return true;
}

/**
 * Returns the T in /foo/bar/T, or else T{}.
 */
template <class T>
T match(const Uri& uri, const char* a, const char* b)
{
    if (!uri.segments(3)) return uri.nil<T>();
    if (!uri.segment(a, 0)) return uri.nil<T>();
    if (!uri.segment(b, 1)) return uri.nil<T>();
    return uri.make<T>(2);
}

template <> inline
bool match<bool>(const Uri& uri, const char* a, const char* b)
{
    if (!uri.segments(2)) return false;
    if (!uri.segment(a, 0)) return false;
    if (!uri.segment(b, 1)) return false;
    return true;
}

#endif
