/*
 * Copyright (C) 2014 Jörgen Grahn.
 * All rights reserved.
 */
#include <allergy/timestamp.h>

#include <orchis.h>

namespace {
    void assert_invalid(const char* s)
    {
	const allergy::Timestamp ts(s);
	orchis::assert_false(ts.valid());
    }
}

namespace allergy {

    namespace timestamp {

	using orchis::TC;

	using allergy::Timestamp;

	void simple(TC)
	{
	    const Timestamp ts("2016-08-14 09:18");
	    orchis::assert_true(ts.valid());
	    orchis::assert_eq(ts.day, allergy::Day{"2016-08-14"});
	    orchis::assert_eq(ts.time, "09:18");
	}

	void compare(TC)
	{
	    const Timestamp a("2016-08-14 09:18");
	    const Timestamp b("2016-08-14 09:20");
	    const Timestamp c("2016-08-17 06:02");

	    orchis::assert_eq(a, a);
	    orchis::assert_eq(b, b);
	    orchis::assert_eq(c, c);

	    orchis::assert_lt(a, b);
	    orchis::assert_lt(b, c);
	    orchis::assert_lt(a, c);
	}

	void invalid(TC)
	{
	    assert_invalid("");
	    assert_invalid("  ");
	    assert_invalid("foo");
	    assert_invalid("2016-08-34 09:18");
	    assert_invalid("2016-18-14 09:18");
	    assert_invalid("2016-08");
	}

	void format(TC)
	{
	    const std::string s {"2016-08-14 09:18"};
	    std::ostringstream oss;
	    oss << Timestamp {s};
	    orchis::assert_eq(oss.str(), s);
	}
    }
}
