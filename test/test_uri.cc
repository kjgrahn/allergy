/*
 * Copyright (C) 2020 Jörgen Grahn.
 * All rights reserved.
 */
#include <uri.h>

#include <orchis.h>
#include <string>

namespace uri {

    using orchis::assert_true;
    using orchis::assert_false;
    using orchis::assert_eq;
    using orchis::TC;

    void create(TC)
    {
	const std::string s = "/foo/bar";
	const Uri uri {s};
	assert_false(uri.empty());
    }

    namespace m {

	struct Seg {
	    Seg(const char* a, const char* b) : s{a, b} {}
	    explicit operator bool() const { return !s.empty(); }
	    template <class T> bool operator== (const T& other) const { return s==other; }
	    template <class T> bool operator!= (const T& other) const { return s!=other; }
	    std::string s;
	};

	std::ostream& operator<< (std::ostream& os, const Seg& val) { return os << val.s; }

	void assert_seg(const Uri& uri, const char* seg)
	{
	    auto m = match<Seg>(uri);
	    assert_true(m);
	    assert_eq(m, seg);
	}

	void assert_seg(const Uri& uri, const char* a, const char* seg)
	{
	    auto m = match<Seg>(uri, a);
	    assert_true(m);
	    assert_eq(m, seg);
	}

	void assert_seg(const Uri& uri, const char* a, const char* b, const char* seg)
	{
	    auto m = match<Seg>(uri, a, b);
	    assert_true(m);
	    assert_eq(m, seg);
	}

	void empty(TC)
	{
	    const std::string s = "/";
	    const Uri uri {s};

	    assert_true(match<bool>(uri, ""));
	    assert_false(match<Seg>(uri));

	    assert_false(match<bool>(uri, "foo"));
	    assert_false(match<bool>(uri, "foo", "bar"));

	    assert_false(match<Seg>(uri, "foo"));
	}

	void foo(TC)
	{
	    const std::string s = "/foo";
	    const Uri uri {s};

	    assert_true(match<bool>(uri, "foo"));
	    assert_seg(uri, "foo");

	    assert_false(match<bool>(uri, "bar"));
	    assert_false(match<bool>(uri, "foo", ""));
	}

	void foo_s(TC)
	{
	    const std::string s = "/foo/";
	    const Uri uri {s};

	    assert_true(match<bool>(uri, "foo", ""));

	    assert_false(match<bool>(uri, "foo"));
	    assert_false(match<bool>(uri, "foo", "bar"));
	}

	void foobar(TC)
	{
	    const std::string s = "/foo/bar";
	    const Uri uri {s};

	    assert_true(match<bool>(uri, "foo", "bar"));
	    assert_seg(uri, "foo", "bar");

	    assert_false(match<bool>(uri, "foo", "bat"));
	}

	void foo_baz(TC)
	{
	    const std::string s = "/foo//baz";
	    const Uri uri {s};

	    assert_seg(uri, "foo", "", "baz");

	    assert_false(match<bool>(uri, "foo", ""));
	}

	namespace percent {

	    void simple(TC)
	    {
		const std::string s = "/f%6Fo";
		const Uri uri {s};
		assert_true(match<bool>(uri, "foo"));
		assert_seg(uri, "foo");
	    }

	    void full(TC)
	    {
		const std::string s = "/%66%6F%6F";
		const Uri uri {s};
		assert_true(match<bool>(uri, "foo"));
		assert_seg(uri, "foo");
	    }

	    void lower(TC)
	    {
		const std::string s = "/f%6fo";
		const Uri uri {s};
		assert_true(match<bool>(uri, "foo"));
		assert_seg(uri, "foo");
	    }

	    void slash(TC)
	    {
		const std::string s = "/f%2Foo/b%2Far";
		const Uri uri {s};
		assert_true(match<bool>(uri, "f/oo", "b/ar"));
		assert_seg(uri, "f/oo", "b/ar");
	    }

	    void longseg(TC)
	    {
		const std::string s {"/123456789%20abc%20def"
		                     "/123456789%20abc%20def"
		                     "/123456789%20abc%20def"};
		const Uri uri {s};
		assert_seg(uri,
			   "123456789 abc def",
			   "123456789 abc def",
			   "123456789 abc def");
	    }

	    void utf8(TC)
	    {
		const std::string s = "/w/index.php?title=%F0%9F%92%BE&redirect=no";
		const Uri uri {s};
		auto seg = match<std::string>(uri, "w");
		return;
		assert_seg(uri, "w", "index.php?title=\xF0\x9F\x92\xBE&redirect=no");
	    }

	    void broken(TC)
	    {
		for (std::string s: {"/f%",
				     "/f%6",
				     "/f%6g",
				     "/f%%%"}) {
		    /* These don't have to have well-defined semantics,
		     * apart from keeping to the Uri interface.
		     */
		    const Uri uri {s};
		    const std::string seg = match<std::string>(uri);
		}
	    }
	}

	namespace unsupported {

	    void empty(TC)
	    {
		const std::string s = "";
		const Uri uri {s};
		assert_false(match<bool>(uri, "foo"));
	    }

	    void unslashed(TC)
	    {
		const std::string s = "foobar foobar foobar";
		const Uri uri {s};
		assert_false(match<bool>(uri, "foo"));
	    }

	    void foobar_s(TC)
	    {
		const std::string s = "/foo/bar/";
		const Uri uri {s};
		assert_false(match<bool>(uri, "foo", "bar"));
	    }

	    void foobarbaz(TC)
	    {
		const std::string s = "/foo/bar/baz";
		const Uri uri {s};
		assert_false(match<bool>(uri, "foo", "bar"));
	    }

	    void foobarbaz_(TC)
	    {
		const std::string s = "/foo/bar/baz/";
		const Uri uri {s};
		assert_false(match<bool>(uri, "foo", "bar"));
	    }

	    void foobarbazbat(TC)
	    {
		const std::string s = "/foo/bar/baz/bat";
		const Uri uri {s};
		assert_false(match<bool>(uri, "foo", "bar"));
	    }

	    void slashy(TC)
	    {
		const std::string s = "////////////////////";
		const Uri uri {s};
		assert_false(match<bool>(uri, "foo"));
	    }
	}
    }
}
