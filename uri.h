/* Copyright (c) 2020, 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_URI_H
#define ALLERGY_URI_H

#include <string>
#include <array>
#include <iosfwd>
#include <utility>
#include <cstring>

/**
 * A HTTP Request-URI [5.1.2] as received in a request.
 *
 * Doesn't own its data (typically a Request does).
 *
 * Only what's needed for our purposes: supports simple string matches
 * and extraction like /foo/bar/T, where T is a user-defined type with
 * a T(begin, end) constructor.
 *
 * Unlike Uri, T must own its data.
 *
 * While doing this, we don't expose any percent-encoding [RFC 3986, 2.1]:
 * calling code sees "AC/DC", not "AC%2FDC".
 *
 * The URI is assumed to be on the form "/a/b/c/d/..." and we look at
 * segments between slashes (or after the last slash; an URI which
 * ends with a slash ends with a segment which happens to be empty).
 *
 */
class Uri {
public:
    Uri(const char* a, const char* b);
    Uri(const std::string& s) : Uri{s.data(), s.data() + s.size()} {}

    bool empty() const { return b==a; }

    void put(std::ostream& os) const;

    // true iff there are n segments
    bool segments(size_t n) const { return n+1==vn; }

    // true iff the nth segment is [a..b)
    bool segment(const char* a, const char* b, size_t n) const;
    bool segment(const char* s, size_t n) const { return segment(s, s + std::strlen(s), n); }

    template <class T>
    T make(size_t n) const
    {
	if (n+1 >= vn) return nil<T>();
	auto c = a + v[n] + 1;
	auto d = a + v[n+1];
	if (!percent) return T {c, d};

	const auto s = decode(c, d);
	c = s.data();
	d = c + s.size();
	return T {c, d};
    }

    template <class T>
    T nil() const { const char* a = {}; return T {a, a}; }

private:
    const char* const a;
    const char* const b;
    const bool percent;

    std::array<unsigned, 5> v;
    unsigned vn;

    static std::string decode(const char* a, const char* b);
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

/**
 * Returns the A and B in /A/B, or else {}, {}.
 */
template <class A, class B>
std::pair<A, B> match(const Uri& uri)
{
    const auto nil = std::make_pair(uri.nil<A>(),
				    uri.nil<B>());
    if (!uri.segments(2)) return nil;
    auto m = std::make_pair(uri.make<A>(0),
			    uri.make<B>(1));
    if (m.first && m.second) return m;
    return nil;
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
