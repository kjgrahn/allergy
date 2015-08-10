/*
 * Copyright (C) 2013 Jörgen Grahn.
 * All rights reserved.
 */
#include <date.h>

#include <orchis.h>
#include <string>

namespace {

    /* date --date=@1368092101 -u
     * Thu May  9 09:35:01 UTC 2013
     */
    const time_t may9 = 1368092101;

    void assert_parses(const char* s, time_t t)
    {
	DateConv dc;
	orchis::assert_eq(dc.parse(s), t);
    }

    void assert_formats(const char* s, time_t t)
    {
	DateConv dc;
	orchis::assert_eq(dc.format(t), s);
    }
}


namespace date {

    using orchis::assert_eq;
    using orchis::TC;

    namespace parse {

	void simple(TC)
	{
	    DateConv dc;
	    assert_eq(dc.parse("Thu, 09 May 2013 09:35:01 GMT"), may9);
	    assert_eq(dc.parse("Thursday, 09-May-13 09:35:01 GMT"), may9);
	    assert_eq(dc.parse("Thu May  9 09:35:01 2013"), may9);
	}

	void ignore_wd(TC)
	{
	    DateConv dc;
	    assert_eq(dc.parse("Mon, 09 May 2013 09:35:01 GMT"), may9);
	    assert_eq(dc.parse("Monday, 09-May-13 09:35:01 GMT"), may9);
	    assert_eq(dc.parse("Mon May  9 09:35:01 2013"), may9);
	}

	void leading_zero_asctime(TC)
	{
	    DateConv dc;
	    assert_eq(dc.parse("Thu May  9 09:35:01 2013"), may9);
	    assert_eq(dc.parse("Thu May 09 09:35:01 2013"), may9);
	}

	void various(TC)
	{
	    assert_parses("Thu, 01 Jan 1970 00:00:00 GMT", 0);
	    assert_parses("Thu, 01 Jan 1970 00:00:01 GMT", 1);
	    assert_parses("Thu, 01 Jan 1970 00:01:00 GMT", 60);
	    assert_parses("Thu, 01 Jan 1970 01:00:00 GMT", 3600);
	    assert_parses("Thu, 01 Jan 1970 06:02:00 GMT", 21720);
	    assert_parses("Thu, 01 Jan 1970 23:00:00 GMT", 82800);
	    assert_parses("Sat, 31 Jan 1970 06:02:00 GMT", 2613720);
	    assert_parses("Fri, 31 Dec 1999 06:02:00 GMT", 946620120);
	    assert_parses("Sat, 01 Jan 2000 06:02:00 GMT", 946706520);
	    assert_parses("Mon, 28 Feb 2000 06:02:00 GMT", 951717720);
	    assert_parses("Tue, 29 Feb 2000 06:02:00 GMT", 951804120);
	    assert_parses("Wed, 01 Mar 2000 06:02:00 GMT", 951890520);
	    assert_parses("Thu, 01 Jun 2000 06:02:00 GMT", 959839320);
	    assert_parses("Tue, 19 Jan 2038 03:10:00 GMT", 2147483400);
	}

	void time_t_wraparound(TC)
	{
	    assert_parses("Sun, 01 Mar 2048 06:02:00 GMT", 2466655320);
	}
    }

    namespace format {

	void simple(TC)
	{
	    DateConv dc;
	    assert_formats("Thu, 09 May 2013 09:35:01 GMT", may9);
	}

	void various(TC)
	{
	    assert_formats("Thu, 01 Jan 1970 00:00:00 GMT", 0);
	    assert_formats("Thu, 01 Jan 1970 00:00:01 GMT", 1);
	    assert_formats("Thu, 01 Jan 1970 00:01:00 GMT", 60);
	    assert_formats("Thu, 01 Jan 1970 01:00:00 GMT", 3600);
	    assert_formats("Thu, 01 Jan 1970 06:02:00 GMT", 21720);
	    assert_formats("Thu, 01 Jan 1970 23:00:00 GMT", 82800);
	    assert_formats("Sat, 31 Jan 1970 06:02:00 GMT", 2613720);
	    assert_formats("Fri, 31 Dec 1999 06:02:00 GMT", 946620120);
	    assert_formats("Sat, 01 Jan 2000 06:02:00 GMT", 946706520);
	    assert_formats("Mon, 28 Feb 2000 06:02:00 GMT", 951717720);
	    assert_formats("Tue, 29 Feb 2000 06:02:00 GMT", 951804120);
	    assert_formats("Wed, 01 Mar 2000 06:02:00 GMT", 951890520);
	    assert_formats("Thu, 01 Jun 2000 06:02:00 GMT", 959839320);
	    assert_formats("Tue, 19 Jan 2038 03:10:00 GMT", 2147483400);
	}

	void time_t_wraparound(TC)
	{
	    assert_formats("Sun, 01 Mar 2048 06:02:00 GMT", 2466655320);
	}

	void benchmark(TC)
	{
	    DateConv dc;
	    for(unsigned n=0; n<50; n++) {
		for(time_t t = 946620000; t < 1900000000; t += 13*3600) {
		    const std::string s = dc.format(t);
		    orchis::assert_eq(s.size(), 29);
		}
	    }
	}
    }
}
