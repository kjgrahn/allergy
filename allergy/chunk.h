/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_ALLERGY_CHUNK_H
#define ALLERGY_ALLERGY_CHUNK_H

#include "../range.h"

#include <vector>
#include <cstdlib>

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

	namespace container {

	    /**
	     * For pages which mostly show sequences, e.g. a gallery. Lets
	     * you decide a chunk size and divides the sequence into
	     * chunks.
	     *
	     * E.g. with chunk size 10 and 23 entries you get three chunks:
	     * 0--9, 10--19, 20--22.
	     */
	    struct Chunk {
		template <class C>
		Chunk(unsigned size, const C& c)
		    : Chunk {size, c.size()}
		{}

		Chunk operator++(int) { auto old = *this; i++; return old; }
		bool end() const { return i==n; }
		bool first() const { return i==0; }
		bool last() const  { return i+1==n; }

		template <class C>
		std::vector<typename C::value_type> val(const C& c) const;

		template <class C>
		Range<C> val(Range<C> range) const;

	    private:
		Chunk(unsigned size, size_t seq_size);

		const unsigned size;
		const unsigned n;
		unsigned i;
	    };

	    template <class C>
	    std::vector<typename C::value_type> Chunk::val(const C& c) const
	    {
		auto a = std::begin(c) + i * size;
		auto b = last() ? std::end(c) : a + size;
		return {a, b};
	    }

	    template <class C>
	    Range<C> Chunk::val(Range<C> range) const
	    {
		auto a = std::begin(range) + i * size;
		auto b = last() ? std::end(range) : a + size;
		return {a, b};
	    }
	}
    }
}

#endif
