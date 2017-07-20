/*
 * Copyright (C) 2017 Jörgen Grahn.
 * All rights reserved.
 */
#include <glob.h>

#include <sstream>
#include <orchis.h>

namespace {

    void assert_globs(const char* pattern, const std::string& res)
    {
	const auto v = util::glob(pattern);
	orchis::assert_eq(v.size(), 1);
	orchis::assert_eq(v.front(), res);
    }
}

namespace glob {

    using orchis::TC;
    using orchis::assert_eq;

    void simple(TC) {
	assert_globs("/dev/null",
		     "/dev/null");
    }

    void one_match(TC) {
	assert_globs("/dev/[n]ull",
		     "/dev/null");
    }

    void one_dir_match(TC) {
	assert_globs("/[d]ev/null",
		     "/dev/null");
    }

    void no_match(TC) {
	auto v = util::glob("/tmp/59c74258409b418*b62faf54508938e1");
	orchis::assert_true(v.empty());
    }

    void several_match(TC) {
	auto v = util::glob("/dev/*");
	orchis::assert_gt(v.size(), 1);
    }

    void tilde(TC) {
	auto v = util::glob("~root");
	orchis::assert_eq(v.size(), 1);
    }

    void braces(TC) {
	assert_globs("/{dev,59c74258409b418fb62faf54508938e1}/null",
		     "/dev/null");
    }
}
