/* Copyright (C) 2021 Jörgen Grahn.
 * All rights reserved.
 */
#include <hostnames.h>
#include <blob.h>

#include <orchis.h>

namespace hostnames {

    using orchis::TC;

    void assert_match(const Hostnames& hh, const char* s)
    {
	orchis::assert_true(hh.match(Blob{s}));
    }

    void assert_mismatch(const Hostnames& hh, const char* s)
    {
	orchis::assert_false(hh.match(Blob{s}));
    }

    void empty(TC)
    {
	const Hostnames hh;
	assert_match(hh, "foo");
	assert_match(hh, "bar");
	assert_match(hh, "foobar");
	assert_match(hh, "foobar:80");
    }

    void match(TC)
    {
	Hostnames hh;
	hh.add("foo");
	hh.add("bar");

	assert_match(hh, "foo");
	assert_match(hh, "bar");
	assert_mismatch(hh, "foobar");
	assert_mismatch(hh, "fo");
	assert_mismatch(hh, "");
    }

    void port(TC)
    {
	Hostnames hh;
	hh.add("foo");
	hh.add("bar");
	hh.add("[::1]");

	assert_match(hh, "foo");
	assert_match(hh, "foo:80");
	assert_match(hh, "[::1]");
	assert_match(hh, "[::1]:80");
    }
}
