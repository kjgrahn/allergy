/*
 * Copyright (C) 2013 Jörgen Grahn.
 * All rights reserved.
 */
#include <date.h>

#include <testicle.h>
#include <string>

namespace {

    /* date --date=@1368092101 -u
     * Thu May  9 09:35:01 UTC 2013
     */
    const time_t may9 = 1368092101;
}


namespace date {

    using testicle::assert_eq;

    void test_simple()
    {
	DateConv dc;
	assert_eq(dc.parse("Thu, 09 May 2013 09:35:01 GMT"), may9);
	assert_eq(dc.parse("Thursday, 09-May-13 09:35:01 GMT"), may9);
	assert_eq(dc.parse("Thu May  9 09:35:01 2013"), may9);
    }
}
