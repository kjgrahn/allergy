/* Copyright (C) 2021 Jörgen Grahn.
 * All rights reserved.
 */
#include <utf8.h>

#include <orchis.h>

namespace utf8 {

    using orchis::TC;

    void assert_encodes(const char* s, const char* ref)
    {
	orchis::assert_eq(encode(s), ref);
	orchis::assert_eq(encode(std::string{s}), ref);
    }

    void empty(TC)
    {
	assert_encodes("", "");
    }

    void ascii(TC)
    {
	assert_encodes("foo", "foo");
	assert_encodes("Foo\nFoo\nFoo\nFoo\nFoo\nFoo\nFoo\nFoo\n",
		       "Foo\nFoo\nFoo\nFoo\nFoo\nFoo\nFoo\nFoo\n");
    }

    void utf8(TC)
    {
	assert_encodes("r\xe4va", "r\xc3\xa4va");
    }
}
