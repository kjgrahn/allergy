/*
 * Copyright (C) 2013, 2016 Jörgen Grahn.
 * All rights reserved.
 */
#include <filter.h>
#include "pipe.h"

#include <orchis.h>

namespace {
    template<class Filter>
    size_t write(Filter& filter, Pipe& pipe,
		 const std::string& s)
    {
	return filter.write(pipe.fd(), Blob(s));
    }

    template<class Filter>
    size_t write(Filter& filter, Pipe& pipe,
		 const std::string& s,
		 const std::string& t)
    {
	return filter.write(pipe.fd(), Blob(s), Blob(t));
    }

    template<class Filter>
    size_t write(Filter& filter, Pipe& pipe,
		 const std::string& s,
		 const std::string& t,
		 const std::string& u)
    {
	return filter.write(pipe.fd(), Blob(s), Blob(t), Blob(u));
    }
}


namespace filter {

    static const Blob karen("But, I do understand what you're saying;\n"
			    "it's just that it doesn't make any sense!\n");

    using orchis::assert_eq;
    using orchis::assert_gt;
    using orchis::TC;

    namespace plain {

	void simple(TC)
	{
	    Pipe p;
	    Backlog backlog;
	    Filter::P f(backlog);
	    assert_eq(write(f, p, "foo"), true);
	    p.assert_read("foo");
	    p.assert_empty();
	    assert_eq(write(f, p, "fo", "o"), true);
	    p.assert_read("foo");
	    p.assert_empty();
	    assert_eq(write(f, p, "f", "o", "o"), true);
	    p.assert_read("foo");
	    p.assert_empty();
	}

	void block(TC)
	{
	    Pipe p;
	    Backlog backlog;
	    Filter::P f(backlog);

	    size_t n = 0;
	    while(f.write(p.fd(), karen, karen)) {
		n += 2*karen.size();
	    }
	    assert_gt(n, 0);

	    while(n) {
		p.assert_read(karen);
		n -= karen.size();
	    }
	    p.assert_empty();

	    /* now a new write will pass through, along with
	     * the failed one above
	     */
	    assert_eq(f.write(p.fd(), karen), true);
	    p.assert_read(karen);
	    p.assert_read(karen);
	    p.assert_read(karen);
	    p.assert_empty();
	}

	void end(TC)
	{
	    Pipe p;
	    Backlog backlog;
	    Filter::P f(backlog);

	    size_t n = 0;
	    while(f.write(p.fd(), karen, karen, karen)) {
		n += 3*karen.size();
	    }
	    assert_gt(n, 0);

	    assert_eq(f.end(p.fd()), false);
	    assert_eq(f.end(p.fd()), false);

	    while(n) {
		p.assert_read(karen);
		n -= karen.size();
	    }
	    p.assert_empty();

	    assert_eq(f.end(p.fd()), true);
	}
    }

    namespace chunked {

	void simple(TC)
	{
	    Pipe p;
	    Backlog backlog;
	    Filter::C f(backlog);
	    assert_eq(write(f, p, "foo"), true);
	    p.assert_read("3\r\n"
			  "foo\r\n");
	    p.assert_empty();
	    assert_eq(write(f, p, "foo bar baz"), true);
	    assert_eq(write(f, p, "bat"), true);
	    p.assert_read("b\r\n"
			  "foo bar baz\r\n"
			  "3\r\n"
			  "bat\r\n");
	    p.assert_empty();
	    assert_eq(f.end(p.fd()), true);
	    p.assert_read("0\r\n"
			  "\r\n");
	    p.assert_empty();
	}

	void block(TC)
	{
	    Pipe p;
	    Backlog backlog;
	    Filter::C f(backlog);

	    size_t n = 0;
	    while(f.write(p.fd(), karen)) {
		n += 2 + 2 + 0x53 + 2;
	    }
	    assert_gt(n, 0);

	    while(n) {
		p.assert_read("53\r\n");
		p.assert_read(karen);
		p.assert_read("\r\n");
		n -= 2 + 2 + 0x53 + 2;
	    }
	    p.assert_empty();

	    assert_eq(f.end(p.fd()), true);
	    p.assert_read("53\r\n");
	    p.assert_read(karen);
	    p.assert_read("\r\n");
	    p.assert_read("0\r\n"
			  "\r\n");
	    p.assert_empty();
	}
    }

    namespace zlib {

	std::string random_string(size_t len)
	{
	    std::string s(len, 'x');
	    for(unsigned i=0; i<len; i++) {
		s[i] = std::rand();
	    }
	    return s;
	}

	void simple(TC)
	{
	    Pipe p;
	    Backlog backlog;
	    Filter::Z f(backlog);

	    while(f.write(p.fd(), Blob(random_string(40)))) ;
	    while(p.drain(10000)) ;
	    while(!f.end(p.fd())) ;
	    while(p.drain(10000)) ;
	}
    }
}
