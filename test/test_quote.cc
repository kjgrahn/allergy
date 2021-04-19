/* Copyright (C) 2021 Jörgen Grahn.
 * All rights reserved.
 */
#include <quote.h>

#include <sstream>
#include <orchis.h>

namespace quot {

    using orchis::TC;

    void assert_quotes(const std::string& s,
		       const std::string& ref,
		       const std::string& refa)
    {
	std::ostringstream oss;
	oss << quote(s) << attr::quote(s);
	orchis::assert_eq(oss.str(), ref + refa);
    }

    void empty(TC)
    {
	assert_quotes("", {}, {});
    }

    void nop(TC)
    {
	assert_quotes("foobar",
		      "foobar",
		      "foobar");
    }

    void single(TC)
    {
	assert_quotes("&", "&amp;", "&");
	assert_quotes("'", "'", "&#39;");
    }

    void multi(TC)
    {
	assert_quotes("<<<<<",
		      "&lt;&lt;&lt;&lt;&lt;",
		      "<<<<<");
    }

    void mid(TC)
    {
	assert_quotes("y&z",
		      "y&amp;z",
		      "y&z");
    }
}
