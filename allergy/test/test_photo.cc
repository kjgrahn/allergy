/*
 * Copyright (C) 2020 Jörgen Grahn.
 * All rights reserved.
 */
#include <allergy/photo.h>

#include <orchis.h>

namespace photo {

    using orchis::TC;

    using allergy::Photo;
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
    }
}
