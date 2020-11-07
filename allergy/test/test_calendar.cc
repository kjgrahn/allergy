/*
 * Copyright (C) 2020 J�rgen Grahn.
 * All rights reserved.
 */
#include <allergy/calendar.h>

#include <orchis.h>

namespace {
}

namespace allergy {

    using orchis::TC;

    namespace year {

	using allergy::Year;

	void assert_invalid(const char* s)
	{
	    const Year val(s);
	    orchis::assert_false(val);
	}

	void simple(TC)
	{
	    const Year val("2020");
	    orchis::assert_eq(val, val);
	    orchis::assert_true(val);
	}

	void invalid(TC)
	{
	    const Year y;
	    orchis::assert_false(y);

	    assert_invalid("");
	    assert_invalid("  ");
	    assert_invalid("20201");
	    assert_invalid("202");
	}

	void invalid2(TC)
	{
	    assert_invalid("202b");
	}
    }

    namespace month {

	using allergy::Month;

	void assert_invalid(const char* s)
	{
	    const Month val(s);
	    orchis::assert_false(val);
	}

	void simple(TC)
	{
	    const Month val("2020-11");
	    orchis::assert_eq(val, val);
	    orchis::assert_true(val);
	}

	void invalid(TC)
	{
	    const Month y;
	    orchis::assert_false(y);

	    assert_invalid("");
	    assert_invalid("  ");
	    assert_invalid("2020-111");
	    assert_invalid("2020-1");
	}

	void invalid2(TC)
	{
	    assert_invalid("2020-0b");
	    assert_invalid("2020-00");
	    assert_invalid("2020-13");
	}
    }

    namespace day {

	using allergy::Day;

	void assert_invalid(const char* s)
	{
	    const Day val(s);
	    orchis::assert_false(val);
	}

	void simple(TC)
	{
	    const Day val("2020-11-07");
	    orchis::assert_eq(val, val);
	    orchis::assert_true(val);
	}

	void invalid(TC)
	{
	    const Day y;
	    orchis::assert_false(y);

	    assert_invalid("");
	    assert_invalid("  ");
	    assert_invalid("2020-11-071");
	    assert_invalid("2020-11-0");
	    assert_invalid("2020:11:07");
	}

	void invalid2(TC)
	{
	    assert_invalid("2020-11-ab");
	    assert_invalid("2020-11-00");
	    assert_invalid("2020-11-40");
	}
    }
}
