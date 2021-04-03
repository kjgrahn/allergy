/*
 * Copyright (C) 2014, 2021 Jörgen Grahn.
 * All rights reserved.
 */
#include <allergy/keys.h>

#include <orchis.h>

namespace {

    template <class It>
    std::string join(const char* sep, It a, It b)
    {
	std::string s;
	for (It i=a; i!=b; i++) {
	    if (i!=a) s += sep;
	    s += *i;
	}
	return s;
    }

    void eq(const allergy::Keys& k,
	    const char* s,
	    const char* kk = "")
    {
	orchis::assert_eq(k.str(), s);
	orchis::assert_eq(join("/", k.begin(), k.end()), kk);
    }
}

namespace allergy {

    namespace keys {

	using orchis::TC;

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

	void linebreaks(TC)
	{
	    Keys k("foo \n"
		   " bar \n"
		   "baz\n");
	    eq(k,
	       "foo\n"
	       "bar\n"
	       "baz");
	}

	namespace keyed {

	    void a(TC)
	    {
		const Keys k("foo [bar] baz");
		eq(k, "foo bar baz", "bar");
	    }

	    void b(TC)
	    {
		const Keys k("foo [bar] baz [bat]");
		eq(k, "foo bar baz bat", "bar/bat");
	    }

	    void c(TC)
	    {
		const Keys k("foo [bar] baz [bat] fred");
		eq(k, "foo bar baz bat fred", "bar/bat");
	    }

	    void d(TC)
	    {
		const Keys k("foo[]bar");
		eq(k, "foobar", "");
	    }

	    void e(TC)
	    {
		const Keys k("foo[][]bar");
		eq(k, "foobar", "/");
	    }

	    void f(TC)
	    {
		const Keys k("[foo]");
		eq(k, "foo", "foo");
	    }

	    void nested(TC)
	    {
		const Keys k("[Vartofta-[Asaka]]");
		eq(k, "Vartofta-Asaka", "Asaka/Vartofta-Asaka");
	    }

	    void misnested(TC)
	    {
		const Keys k("foo] bar [b [az]");
		eq(k, "foo] bar b az", "az/b az");
	    }

	    void curly(TC)
	    {
		const Keys k("foo [bar{baz}]");
		eq(k, "foo bar{baz}", "bar{baz}");
	    }
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
		eq(k, "", "foo/bar");
	    }

	    void c(TC)
	    {
		const Keys k("[foo]{bar}");
		eq(k, "foo", "foo/bar");
	    }

	    namespace whitespace {

		void a(TC)
		{
		    const Keys k("foo {bar}");
		    eq(k, "foo", "bar");
		}

		void b(TC)
		{
		    const Keys k("{bar} foo");
		    eq(k, "foo", "bar");
		}

		void c(TC)
		{
		    const Keys k("foo {bar} baz");
		    eq(k, "foo baz", "bar");
		}

		void d(TC)
		{
		    const Keys k("foo {bar} {baz} bat");
		    eq(k, "foo bat", "bar/baz");
		}

		void e(TC)
		{
		    const Keys k("foo\n"
				 "{bar} {baz}\n"
				 "{bat}\n");
		    eq(k, "foo", "bar/baz/bat");
		}

		void f(TC)
		{
		    const Keys k("foo {bar}\n"
				 " {baz} bat\n");
		    eq(k, "foo\nbat", "bar/baz");
		}

		void g(TC)
		{
		    const Keys k("{ b ar} foo");
		    eq(k, "foo", " b ar");
		}

	    }
	}
    }
}
