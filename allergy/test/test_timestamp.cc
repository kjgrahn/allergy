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
	    orchis::assert_eq(ts.str, "2016-08-14 09:18");
	    orchis::assert_eq(ts.year, allergy::Year{"2016"});
	    orchis::assert_eq(ts.month, allergy::Month{"2016-08"});
	    orchis::assert_eq(ts.day, allergy::Day{"2016-08-14"});
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
    }
}
