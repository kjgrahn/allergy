/*
 * Copyright (C) 2020 Jörgen Grahn.
 * All rights reserved.
 */
#include <allergy/photo.h>

#include <orchis.h>

namespace allergy {

    namespace photo {

	using orchis::TC;

	using orchis::assert_eq;

	void assert_valid(const char* s)
	{
	    orchis::assert_true(Photo(s).valid());
	    orchis::assert_true(Photo(s));
	}

	void assert_invalid(const char* s)
	{
	    orchis::assert_false(Photo(s).valid());
	    orchis::assert_false(Photo(s));
	}

	void valid(TC)
	{
	    assert_valid("2020-07-24_0001.jpg");
	    assert_valid("2020-07-24_01.jpg");
	    assert_valid("2020-07-24_0.jpg");
	    assert_valid("1900-09-01_0001.jpg");
	}

	void invalid(TC)
	{
	    assert_invalid("");
	    assert_invalid(".");
	    assert_invalid("2020-07-24.jpg");
	    assert_invalid("2020-07-24_abcd.jpg");
	    assert_invalid("../2020-07-24.jpg");
	    assert_invalid("2020-17-24_0001.jpg");
	    assert_invalid("2020-a7-24_0001.jpg");

	    assert_invalid("2020-07-24_0001.gif");
	}

	void url(TC)
	{
	    const Photo p {"2020-07-24_0001.jpg"};
	    orchis::assert_true(p.valid());
	    assert_eq(p.url(), "/2020-07-24_0001.jpg");
	    assert_eq(p.pageurl(), "/2020-07-24/0001");
	    assert_eq(p.thumburl(), "/thumb/2020-07-24_0001.jpg");
	}

	namespace legacy {

	    void valid(TC)
	    {
		assert_valid("200724_0001.jpg");
		assert_valid("200724_01.jpg");
		assert_valid("200724_0.jpg");
		assert_valid("000901_0001.jpg");
	    }

	    void equality(TC)
	    {
		const Photo a {"2020-07-24_0001.jpg"};
		const Photo b {"200724_0001.jpg"};
		assert_eq(a, b);
	    }

	    void url(TC)
	    {
		const Photo p {"210515_0001.jpg"};
		orchis::assert_true(p.valid());
		assert_eq(p.url(), "/2021-05-15_0001.jpg");
		assert_eq(p.pageurl(), "/2021-05-15/0001");
		assert_eq(p.thumburl(), "/thumb/2021-05-15_0001.jpg");
	    }

	    void invalid(TC)
	    {
		assert_invalid("200724.jpg");
		assert_invalid("200724_abcd.jpg");
		assert_invalid("../200724.jpg");
		assert_invalid("201724_0001.jpg");
		assert_invalid("20a724_0001.jpg");

		assert_invalid("200724_0001.gif");
	    }
	}
    }
}
