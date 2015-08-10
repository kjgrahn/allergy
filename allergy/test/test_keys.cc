/*
 * Copyright (C) 2014 Jörgen Grahn.
 * All rights reserved.
 */
#include <allergy/keys.h>

#include <orchis.h>

namespace {
    void eq(const allergy::Keys& k,
	    const char* s)
    {
	orchis::assert_eq(k.str(), s);
	allergy::Keys::const_iterator i = k.begin();
	orchis::assert_(i==k.end());
    }

    void eq(const allergy::Keys& k,
	    const char* s,
	    const char* k1)
    {
	orchis::assert_eq(k.str(), s);
	allergy::Keys::const_iterator i = k.begin();
	orchis::assert_(i!=k.end());
	orchis::assert_eq(*i++, k1);
	orchis::assert_(i==k.end());
    }

    void eq(const allergy::Keys& k,
	    const char* s,
	    const char* k1,
	    const char* k2)
    {
	orchis::assert_eq(k.str(), s);
	allergy::Keys::const_iterator i = k.begin();
	orchis::assert_(i!=k.end());
	orchis::assert_eq(*i++, k1);
	orchis::assert_(i!=k.end());
	orchis::assert_eq(*i++, k2);
	orchis::assert_(i==k.end());
    }
}

namespace allergy_keys {

    using orchis::TC;

    using allergy::Keys;
    using orchis::assert_eq;
    using orchis::assert_;


    void construct(TC)
    {
	Keys k("foo");
    }

    void empty(TC)
    {
	Keys k("");
	eq(k, "");
    }

    void unkeyed(TC)
    {
	Keys k("foo");
	eq(k, "foo");
    }

    namespace keyed {

	void a(TC)
	{
	    const Keys k("foo [bar] baz");
	    eq(k, "foo bar baz",
	       "bar");
	}

	void b(TC)
	{
	    const Keys k("foo [bar] baz [bat]");
	    eq(k, "foo bar baz bat",
	       "bar", "bat");
	}

	void c(TC)
	{
	    const Keys k("foo [bar] baz [bat] fred");
	    eq(k, "foo bar baz bat fred",
	       "bar", "bat");
	}

	void d(TC)
	{
	    const Keys k("foo[]bar");
	    eq(k, "foobar",
	       "");
	}

	void e(TC)
	{
	    const Keys k("foo[][]bar");
	    eq(k, "foobar",
	       "", "");
	}

	void f(TC)
	{
	    const Keys k("[foo]");
	    eq(k, "foo", "foo");
	}
    }

    void nested(TC)
    {
	const Keys k("[Vartofta-[Asaka]]");
	eq(k, "Vartofta-Asaka",
	   "Asaka", "Vartofta-Asaka");
    }

    void misnested(TC)
    {
	const Keys k("foo] bar [b [az]");
	eq(k, "foo] bar b az",
	   "az", "b az");
    }

    namespace curly {

	void a(TC)
	{
	    const Keys k("{foo}");
	    eq(k, "", "foo");
	}

	void b(TC)
	{
	    const Keys k("{foo}{bar}");
	    eq(k, "", "foo", "bar");
	}

	void c(TC)
	{
	    const Keys k("[foo]{bar}");
	    eq(k, "foo", "foo", "bar");
	}
    }
}
