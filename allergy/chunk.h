/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_ALLERGY_CHUNK_H
#define ALLERGY_ALLERGY_CHUNK_H

namespace allergy {

    namespace page {

	/**
	 * Pages come in N pieces, and you can ask them to render any of them.
	 * To avoid dealing with an integer, and when N is fixed, a Page class
	 * can use this template.
	 *
	 * As an example, Chunk<4, 1>:  1, 2, 3, 4, end
	 * with 1 being first and 4 last.
	 *
	 */
	template <unsigned N, unsigned offset = 0>
	struct Chunk {
	    Chunk operator++(int) { auto old = *this; n++; return old; }
	    bool end() const { return n == N; }
	    bool first() const { return !n; }
	    bool last() const { return n+1 == N; }
	    unsigned val() const { return n + offset; }
	private:
	    unsigned n = 0;
	};
    }
}

#endif
