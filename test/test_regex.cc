/*
 * Copyright (C) 2017 Jörgen Grahn.
 * All rights reserved.
 */
#include <regex>

#include <orchis.h>

namespace regex {

    using orchis::TC;

    void simple(TC) {
	const std::regex re{"f.o"};

	orchis::assert_true(regex_match("foo", re));
	orchis::assert_true(regex_match("fno", re));

	orchis::assert_false(regex_match("xfoo", re));
	orchis::assert_false(regex_match("foox", re));

	orchis::assert_true(regex_search("xfoo", re));
	orchis::assert_true(regex_search("foox", re));
    }
}
