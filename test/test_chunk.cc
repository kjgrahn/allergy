/*
 * Copyright (C) 2016 Jörgen Grahn.
 * All rights reserved.
 */
#include <chunk.h>

#include <orchis.h>

namespace chunk {

    using namespace orchis;


    void test_construct()
    {
	Chunk buf;
	assert_eq(buf.empty(), true);
	assert_eq(buf.str(), "");
    }

    void test_simple1()
    {
	Chunk buf;
	buf << "foo\r\n";
	assert_eq(buf.empty(), false);
	assert_eq(buf.str(), "foo\r\n");
    }

    void test_simple2()
    {
	Chunk buf;
	buf.ostream().put('f').write("oo\r\n", 4);
	assert_eq(buf.empty(), false);
	assert_eq(buf.str(), "foo\r\n");
    }

    void test_flush()
    {
	Chunk buf;
	std::ostream& os = buf.ostream();
	os.put('f');
	os.flush();
	os.write("oo\r\n", 4);
	assert_eq(buf.empty(), false);
	assert_eq(buf.str(), "foo\r\n");
    }

    namespace huge {

	void test(size_t n)
	{
	    Chunk buf;
	    std::ostringstream oss;
	    for(size_t i=0; i<n; ++i) {
		buf << "Foo bar baz " << i << "\r\n";
		oss << "Foo bar baz " << i << "\r\n";
	    }

	    assert_eq(buf.size(), oss.str().size());
	    assert_eq(buf.str(), oss.str());
	}

	void test() { test(50e3); }
    }
}
