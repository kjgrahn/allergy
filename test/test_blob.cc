/*
 * Copyright (C) 2016 Jörgen Grahn.
 * All rights reserved.
 */
#include <blob.h>

#include <map>

#include <orchis.h>

using orchis::TC;

namespace blob {

    using orchis::assert_true;
    using orchis::assert_false;
    using orchis::assert_eq;
    using orchis::assert_lt;
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

    void consume(const unsigned n)
    {
	const std::string s(n, 'x');
	Blob b {s};
	std::map<unsigned, unsigned> ss;
	Blob c;
	while (true) {
	    c = consume(b);
	    if (c.empty()) break;
	    ss[c.size()]++;
	}

	assert_true(b.empty());

	if (ss.size()==1) {
	    // n = m * K
	    const auto i = ss.begin();
	    assert_eq(i->first * i->second, n);
	    return;
	}

	// n = m * K + r, where r < K
	assert_eq(ss.size(), 2);
	auto i = ss.begin();
	const unsigned r = i->first;
	assert_eq(i->second, 1);
	i++;

	const unsigned K = i->first;
	const unsigned m = i->second;
	assert_eq(K*m + r, n);
	assert_lt(r, K);
    }

    void consume_empty(TC)
    {
	Blob b;
	Blob c = consume(b);
	assert_true(c.empty());
    }

    void consume(TC)
    {
	consume(1);
	consume(2);
	consume(10);
	consume(100);
	consume(1000);
	consume(8191);
	consume(8192);
	consume(8193);
	consume(10000);
	consume(80000);
    }
}
