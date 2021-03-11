/* Copyright (C) 2021 Jörgen Grahn.
 * All rights reserved.
 */
#include <allergy/chunk.h>

#include <orchis.h>

namespace allergy {

    namespace page {

	namespace chunk {

	    using orchis::TC;
	    using orchis::assert_true;
	    using orchis::assert_false;
	    using orchis::assert_eq;

	    template <class Chunk>
	    void assert_pos(Chunk c, bool first, bool last, unsigned val)
	    {
		assert_false(c.end());
		assert_eq(c.first(), first);
		assert_eq(c.last(), last);
		assert_eq(c.val(), val);
	    }

	    void single(TC)
	    {
		Chunk<1> c;
		assert_pos(c, true, true, 0); c++;
		assert_true(c.end());
	    }

	    void simple(TC)
	    {
		Chunk<5> c;
		assert_pos(c, true,  false, 0); c++;
		assert_pos(c, false, false, 1); c++;
		assert_pos(c, false, false, 2); c++;
		assert_pos(c, false, false, 3); c++;
		assert_pos(c, false, true,  4); c++;
		assert_true(c.end());
	    }

	    void offset(TC)
	    {
		Chunk<5, 10> c;
		assert_pos(c, true,  false, 10); c++;
		assert_pos(c, false, false, 11); c++;
		assert_pos(c, false, false, 12); c++;
		assert_pos(c, false, false, 13); c++;
		assert_pos(c, false, true,  14); c++;
		assert_true(c.end());
	    }

	    namespace cont {

		template <class Chunk, class Cont>
		void assert_pos(Chunk c, const Cont& cont,
				bool first, bool last, const std::string& val)
		{
		    assert_false(c.end());
		    assert_eq(c.first(), first);
		    assert_eq(c.last(), last);
		    const auto v = c.val(cont);
		    const std::string s {begin(v), end(v)};
		    assert_eq(s, val);
		}

		void empty(TC)
		{
		    const std::string cont = "";
		    container::Chunk c {3, cont};
		    assert_pos(c, cont, true, true, ""); c++;
		    assert_true(c.end());
		}

		void short_single(TC)
		{
		    const std::string cont = "ab";
		    container::Chunk c {3, cont};
		    assert_pos(c, cont, true, true, "ab"); c++;
		    assert_true(c.end());
		}

		void single(TC)
		{
		    const std::string cont = "abc";
		    container::Chunk c {3, cont};
		    assert_pos(c, cont, true, true, "abc"); c++;
		    assert_true(c.end());
		}

		void two(TC)
		{
		    const std::string cont = "abc" "d";
		    container::Chunk c {3, cont};
		    assert_pos(c, cont, true, false, "abc"); c++;
		    assert_pos(c, cont, false, true, "d"); c++;
		    assert_true(c.end());
		}

		void three(TC)
		{
		    const std::string cont = "abc" "def" "gh";
		    container::Chunk c {3, cont};
		    assert_pos(c, cont, true, false, "abc"); c++;
		    assert_pos(c, cont, false, false, "def"); c++;
		    assert_pos(c, cont, false, true, "gh"); c++;
		    assert_true(c.end());
		}

		void four(TC)
		{
		    const std::string cont = "abc" "def" "ghi" "xyz";
		    container::Chunk c {3, cont};
		    assert_pos(c, cont, true, false, "abc"); c++;
		    assert_pos(c, cont, false, false, "def"); c++;
		    assert_pos(c, cont, false, false, "ghi"); c++;
		    assert_pos(c, cont, false, true, "xyz"); c++;
		    assert_true(c.end());
		}
	    }
	}
    }
}
