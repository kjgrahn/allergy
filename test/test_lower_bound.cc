/* Copyright (C) 2021 Jörgen Grahn.
 * All rights reserved.
 */
#include <lower_bound.h>

#include <orchis.h>

using orchis::TC;

namespace bound {

    void test(const std::string& s)
    {
	const auto a = begin(s);
	const auto b = end(s);
	auto c = ::lower_bound(a, b, [] (char ch) { return ch!=':'; });
	if (c==b) {
	    if (c==a) return;
	    c--;
	    orchis::assert_eq(*c, '.');
	}
	else {
	    orchis::assert_eq(*c, ':');
	    if (c==a) return;
	    c--;
	    orchis::assert_eq(*c, '.');
	}
    }

    void empty(TC)
    {
	test("");
    }

    void simple(TC)
    {
	test(".:");
    }

    void none(TC)
    {
	test(".");
	test("..");
	test("...");
	test("....");
	test("......................");
    }

    void all(TC)
    {
	test(":");
	test("::");
	test(":::");
	test("::::");
	test("::::::::::::::::::::::");
    }

    void two(TC)
    {
	test("..");
	test(".:");
	test("::");
    }

    void three(TC)
    {
	test("...");
	test("..:");
	test(".::");
	test(":::");
    }

    void four(TC)
    {
	test("....");
	test("...:");
	test("..::");
	test(".:::");
	test("::::");
    }

    void longer(TC)
    {
	test(".................................:::::::::::::::::::::::::::");
	test("..................................::::::::::::::::::::::::::");
	test("...................................:::::::::::::::::::::::::");
    }
}
