/*
 * Copyright (C) 2016 Jörgen Grahn.
 * All rights reserved.
 */
#include <blob.h>

#include <orchis.h>

using orchis::TC;

namespace blob {

    using orchis::assert_true;
    using orchis::assert_false;
    using orchis::assert_eq;
    using orchis::assert_neq;

    void empty(TC)
    {
	const Blob b;
	assert_true(b.empty());
	assert_false(b);
	assert_eq(b.size(), 0);
	assert_eq(b, b);
    }

    const std::string s("Hello, world!");

    void string(TC)
    {
	const Blob b(s);
	assert_false(b.empty());
	assert_true(b);
	assert_eq(b.size(), s.size());
	assert_eq(b, Blob(s));
	assert_neq(b, Blob{});
    }

    void other(TC)
    {
	const char* const p = s.data();
	const size_t n = s.size();
	const Blob b0(s);

	assert_eq(b0, Blob(p, n));
	assert_eq(b0, Blob(p, p + n));
	assert_eq(b0, Blob(p, p + n));
	assert_eq(b0, Blob(p));
    }
}
