/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "hostnames.h"
#include "blob.h"

#include <algorithm>

void Hostnames::add(const char* name)
{
    val.emplace_back(name);
}

namespace {

    template <class It>
    It findr(It a, It b, char ch)
    {
	while (a!=b) {
	    b--;
	    if (*b==ch) break;
	}
	return b;
    }

    /**
     * The interesting cases are:
     * - empty
     * - host
     * - host:port
     * - [ipv6]
     * - [ipv6]:port
     */
    Blob trim_port(const Blob& blob)
    {
	if (blob.empty()) return blob;

	const auto a = blob.begin();
	const auto b = blob.end();
	const auto c = findr(a, b, ':');
	const auto d = std::find(c, b, ']');
	if (c==a) return {a, b};
	if (d!=b) return {a, d+1};
	return {a, c};
    }
}

/**
 * See if host part of a Host: host:port header matches.
 * If the Hostnames are empty(), anything matches.
 */
bool Hostnames::match(const Blob& s) const
{
    if (val.empty()) return true;

    const auto host = trim_port(s);
    for (const auto& v : val) {
	if (Blob(v) == host) return true;
    }
    return false;
}
