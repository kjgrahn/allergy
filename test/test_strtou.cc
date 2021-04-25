/* Copyright (C) 2021 Jörgen Grahn.
 * All rights reserved.
 */
#include <strtou.h>

#include <orchis.h>
#include <string>
#include <list>

namespace strto {

    using orchis::TC;
    using orchis::assert_eq;
    using orchis::assert_true;
    using orchis::assert_false;

    void assert_to(const char* s, const unsigned n)
    {
	const std::string ss {s};
	const std::list<char> sl {begin(ss), end(ss)};
	const auto e = s + ss.size();
	const unsigned limit = n+10;

	auto as = s;
	assert_eq(strtou(as, e, limit), n);
	assert_true(as==e);

	auto ass = begin(ss);
	assert_eq(strtou(ass, end(ss), limit), n);
	assert_true(ass==end(ss));

	auto asl = begin(sl);
	assert_eq(strtou(asl, end(sl), limit), n);
	assert_true(asl==end(sl));
    }

    void simple(TC)
    {
	assert_to(      "0", 0);
	assert_to( "000000", 0);
	assert_to(      "1", 1);
	assert_to("0000001", 1);
	assert_to(     "10", 10);
	assert_to(    "010", 10);
	assert_to(    "019", 19);
	assert_to("1234567", 1234567);
    }

    void trailer(TC)
    {
	auto assert_to = [] (const std::string& s,
			     const unsigned n)
	{
	    auto as = begin(s);
	    const unsigned limit = n+10;

	    assert_eq(strtou(as, end(s), limit), n);
	    assert_false(as==end(s));
	    assert_eq(*as, '.');
	};

	assert_to(      "0.", 	 0);
	assert_to( "000000.", 	 0);
	assert_to(      "1.", 	 1);
	assert_to(      "1.234", 1);
	assert_to("0000001.", 	 1);
	assert_to(     "10.", 	 10);
	assert_to(    "010.", 	 10);
	assert_to(    "019.", 	 19);
	assert_to("1234567.", 	 1234567);
    }

    void limit(TC)
    {
	const std::string s {"123"};
	auto a = begin(s);
	assert_eq(strtou(a, end(s), 1000), 123);
	a = begin(s);
	assert_eq(strtou(a, end(s), 100), 0);
    }

    void no_limit(TC)
    {
	const std::string s {"123"};
	auto a = begin(s);
	assert_eq(strtou(a, end(s)), 123);
	assert_true(a==end(s));
    }
}
