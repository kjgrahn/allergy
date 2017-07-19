/*
 * $Id: test_util.cc,v 1.3 2009-01-07 19:22:30 grahn Exp $
 *
 * Copyright (C) 2007 Jörgen Grahn.
 * All rights reserved.
 */
#include <join.h>

#include <sstream>
#include <orchis.h>

namespace util {

    void test_join() {
	static const char * ss[] = {"foo", "bar", "baz"};
	std::ostringstream oss;
	join_to(oss, "++", ss, ss);
	orchis::assert_eq(oss.str(), "");
	join_to(oss, "++", ss, ss+1);
	orchis::assert_eq(oss.str(), "foo");
	join_to(oss, "++", ss, ss+3);
	orchis::assert_eq(oss.str(), "foofoo++bar++baz");
    }
}
