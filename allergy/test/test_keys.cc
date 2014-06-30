/*
 * Copyright (C) 2014 Jörgen Grahn.
 * All rights reserved.
 */
#include <allergy/keys.h>

#include <testicle.h>

namespace {
    void eq(const allergy::Keys& k,
	    const char* s)
    {
	testicle::assert_eq(k.str(), s);
	allergy::Keys::const_iterator i = k.begin();
	testicle::assert_(i==k.end());
    }

    void eq(const allergy::Keys& k,
	    const char* s,
	    const char* k1)
    {
	testicle::assert_eq(k.str(), s);
	allergy::Keys::const_iterator i = k.begin();
	testicle::assert_(i!=k.end());
	testicle::assert_eq(*i++, k1);
	testicle::assert_(i==k.end());
    }

    void eq(const allergy::Keys& k,
	    const char* s,
	    const char* k1,
	    const char* k2)
    {
	testicle::assert_eq(k.str(), s);
	allergy::Keys::const_iterator i = k.begin();
	testicle::assert_(i!=k.end());
	testicle::assert_eq(*i++, k1);
	testicle::assert_(i!=k.end());
	testicle::assert_eq(*i++, k2);
	testicle::assert_(i==k.end());
    }
}

namespace allergy_keys {

    using allergy::Keys;
    using testicle::assert_eq;
    using testicle::assert_;


    void construct(testicle::TC)
    {
	Keys k("foo");
    }

    void empty(testicle::TC)
    {
	Keys k("");
	eq(k, "");
    }

    void unkeyed(testicle::TC)
    {
	Keys k("foo");
	eq(k, "foo");
    }

    namespace keyed {

	void a(testicle::TC)
	{
	    const Keys k("foo [bar] baz");
	    eq(k, "foo bar baz",
	       "bar");
	}

	void b(testicle::TC)
	{
	    const Keys k("foo [bar] baz [bat]");
	    eq(k, "foo bar baz bat",
	       "bar", "bat");
	}

	void c(testicle::TC)
	{
	    const Keys k("foo [bar] baz [bat] fred");
	    eq(k, "foo bar baz bat fred",
	       "bar", "bat");
	}

	void d(testicle::TC)
	{
	    const Keys k("foo[]bar");
	    eq(k, "foobar",
	       "");
	}

	void e(testicle::TC)
	{
	    const Keys k("foo[][]bar");
	    eq(k, "foobar",
	       "", "");
	}

	void f(testicle::TC)
	{
	    const Keys k("[foo]");
	    eq(k, "foo", "foo");
	}
    }

    void nested(testicle::TC)
    {
	const Keys k("[Vartofta-[Asaka]]");
	eq(k, "Vartofta-Asaka",
	   "Asaka", "Vartofta-Asaka");
    }

    void misnested(testicle::TC)
    {
	const Keys k("foo] bar [b [az]");
	eq(k, "foo] bar b az",
	   "az", "b az");
    }

    namespace curly {

	void a(testicle::TC)
	{
	    const Keys k("{foo}");
	    eq(k, "", "foo");
	}

	void b(testicle::TC)
	{
	    const Keys k("{foo}{bar}");
	    eq(k, "", "foo", "bar");
	}

	void c(testicle::TC)
	{
	    const Keys k("[foo]{bar}");
	    eq(k, "foo", "foo", "bar");
	}
    }
}
