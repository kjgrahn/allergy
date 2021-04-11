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
