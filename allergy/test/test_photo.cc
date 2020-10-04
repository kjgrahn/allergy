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

	void valid(TC)
	{
	    auto assert_valid = [] (const char* s) {
				    orchis::assert_true(Photo(s).valid());
				};

	    assert_valid("2020-07-24_0001.jpg");
	    assert_valid("2020-07-24_01.jpg");
	    assert_valid("2020-07-24_0.jpg");
	    assert_valid("1900-09-01_0001.jpg");
	}

	void quarter(TC)
	{
	    auto assert_quarter = [] (const char* q, const char* s) {
				      const Photo p {s};
				      orchis::assert_true(p.valid());
				      orchis::assert_eq(p.dir(), q);
				  };

	    assert_quarter("2020.1", "2020-01-01_0001.jpg");
	    assert_quarter("2020.1", "2020-03-31_0001.jpg");
	    assert_quarter("2020.2", "2020-04-01_0001.jpg");
	    assert_quarter("2020.2", "2020-06-30_0001.jpg");
	    assert_quarter("2020.3", "2020-07-01_0001.jpg");
	    assert_quarter("2020.3", "2020-07-31_0001.jpg");
	    assert_quarter("2020.4", "2020-10-01_0001.jpg");
	    assert_quarter("2020.4", "2020-12-31_0001.jpg");
	}

	void invalid(TC)
	{
	    auto assert_invalid = [] (const char* s) {
				      orchis::assert_false(Photo(s).valid());
				  };

	    assert_invalid("");
	    assert_invalid(".");
	    assert_invalid("2020-07-24.jpg");
	    assert_invalid("2020-07-24_abcd.jpg");
	    assert_invalid("../2020-07-24.jpg");
	    assert_invalid("2020-17-24_0001.jpg");
	    assert_invalid("2020-a7-24_0001.jpg");

	    assert_invalid("2020-07-24_0001.gif");
	}

	namespace legacy {

	    void valid(TC)
	    {
		auto assert_valid = [] (const char* s) {
		    orchis::assert_true(Photo(s).valid());
		};

		assert_valid("200724_0001.jpg");
		assert_valid("200724_01.jpg");
		assert_valid("200724_0.jpg");
		assert_valid("000901_0001.jpg");
	    }

	    void quarter(TC)
	    {
		auto assert_quarter = [] (const char* q, const char* s) {
		    const Photo p {s};
		    orchis::assert_true(p.valid());
		    orchis::assert_eq(p.dir(), q);
		};

		assert_quarter("2020.1", "200101_0001.jpg");
		assert_quarter("2020.1", "200331_0001.jpg");
		assert_quarter("2020.2", "200401_0001.jpg");
		assert_quarter("2020.2", "200630_0001.jpg");
		assert_quarter("2020.3", "200701_0001.jpg");
		assert_quarter("2020.3", "200731_0001.jpg");
		assert_quarter("2020.4", "201001_0001.jpg");
		assert_quarter("2020.4", "201231_0001.jpg");
	    }

	    void invalid(TC)
	    {
		auto assert_invalid = [] (const char* s) {
		    orchis::assert_false(Photo(s).valid());
		};

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
