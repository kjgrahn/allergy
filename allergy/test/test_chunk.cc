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
	}
    }
}
