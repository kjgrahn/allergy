/* Copyright (C) 2021 Jörgen Grahn.
 * All rights reserved.
 */
#include <allergy/photo.h>

#include <orchis.h>

namespace allergy {

    namespace serial {

	using orchis::TC;

	using orchis::assert_eq;
	using orchis::assert_lt;
	using orchis::assert_true;
	using orchis::assert_false;

	template <class C>
	void assert_formats(const C& val, const std::string& ref)
	{
	    std::ostringstream oss;
	    oss << val;
	    orchis::assert_eq(oss.str(), ref);
	}

	void defaulted(TC)
	{
	    const Serial sr;
	    assert_eq(sr, sr);
	    assert_false(sr);
	}

	void invalid(TC)
	{
	    for (std::string s : {"",
				  "foo",
				  " 42",
				  "42a"}) {
		const Serial sr {s};
		assert_false(sr);
	    }
	}

	void single(TC)
	{
	    const Serial a {"0"};
	    const Serial b {"1"};
	    assert_true(a); assert_true(b);
	    assert_eq(a, a);
	    assert_lt(a, b);
	    assert_false(a < a);
	    assert_false(a == b);
	    assert_formats(a, "0");
	    assert_formats(b, "1");
	}

	void bits(TC)
	{
	    const Serial a {"0112"};
	    assert_formats(a, "0112");
	    assert_eq(a.digits, 4);
	    assert_eq(a.n, 112);
	}

	void six(TC)
	{
	    const Serial a {"000112"};
	    const Serial b {"039456"};
	    assert_true(a); assert_true(b);
	    assert_eq(a, a);
	    assert_lt(a, b);
	    assert_false(a < a);
	    assert_false(a == b);
	    assert_formats(a, "000112");
	    assert_formats(b, "039456");
	}
    }
}
