/*
 * $Id: test_response.cc,v 1.7 2011-07-02 10:19:44 grahn Exp $
 *
 * Copyright (C) 2010, 2011 J?rgen Grahn.
 * All rights reserved.
 */
#include <sstream>

#include <orchis.h>

/*
 * XXX These tests don't relate to anything remotely interesting.
 */


namespace {

    template<class T>
    std::string fmt(const T& resp)
    {
	std::ostringstream oss;
	oss << resp;
	const std::string s = oss.str();
	orchis::assert_eq(s.empty(), false);
	return s;
    }

    void assert_startswith(const std::string& haystack,
			   const std::string& needle)
    {
	std::ostringstream message;
	if(haystack.find(needle)!=0) {
	    message << '\'' << needle << '\''
		    << " is not a prefix of "
		    << '\'' << haystack << '\'';
	    throw orchis::Failure(message);
	}
    }

    void assert_endswith(const std::string& haystack,
			 const std::string& needle)
    {
	std::ostringstream message;
	if(haystack.size() < needle.size()
	   || haystack.rfind(needle) != haystack.size() - needle.size()) {
	    message << '\'' << needle << '\''
		    << " is not a suffix of "
		    << '\'' << haystack << '\'';
	    throw orchis::Failure(message);
	}
    }

    /**
     * Assert that 'resp' formats to exactly 's'.
     */
    template<class T>
    void assert_fmt(const T& resp, const std::string& s)
    {
	const std::string r = fmt(resp);
	orchis::assert_eq(r, s);
    }

    /**
     * Assert that 'resp' formats to something that
     * begins with 'a' and ends with 'b'.
     */
    template<class T>
    void assert_fmt(const T& resp,
		    const std::string& a,
		    const std::string& b)
    {
	std::ostringstream message;
	const std::string r = fmt(resp);
	assert_startswith(r, a);
	assert_endswith(r, b);
    }
}


namespace starts_endswith {

    void test_start()
    {
	assert_startswith("foo", "foo");
	assert_startswith("foo", "f");
	assert_startswith("foo", "");
    }

    void test_end()
    {
	assert_endswith("foo", "");
	assert_endswith("foo", "o");
	assert_endswith("foo", "foo");
    }
}


namespace response {

    namespace single {

	void test111()
	{
	    assert_fmt(Date(3600),
		       "111 19700101010000\r\n");
	}

	void test200()
	{
	    assert_fmt(Mode(true, "service available, posting allowed"),
		       "200 ", "\r\n");
	}

	void test201()
	{
	    assert_fmt(Mode(false, "service available, posting prohibited"),
		       "201 ", "\r\n");
	}

	void test205()
	{
	    assert_fmt(Quit("goodbye!"),
		       "205 goodbye!\r\n");
	}

	void test211()
	{
	    assert_fmt(Group(42, 100, 200, "misc.test"),
		       "211 42 100 200 misc.test selected\r\n");	    
	}

	void test223()
	{
	    assert_fmt(Next(Number(421), MsgId("foo@example.org")),
		       "223 421 <foo@example.org>\r\n");	    
	}

	void test240();
	void test340();
    }

    namespace multi {

	void test100();
	void test101();
	void test211();
	void test215();
	void test220();
	void test221();
	void test222();
	void test224();
	void test225();
	void test230();
	void test231();
    }

    namespace error {

	void test400();
	void test401();
	void test403();
	void test411();
	void test412();
	void test420();
	void test421();
	void test422();
	void test423();
	void test430();
	void test440();
	void test441();
	void test480();
	void test483();
	void test500();
	void test501();
	void test502();
	void test503();
    }
}
