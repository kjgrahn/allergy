/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_PAGE_H
#define ALLERGY_PAGE_H

#include "index.h"

#include <iosfwd>

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

	/**
	 * The main index page, /.
	 */
	class Index {
	public:
	    explicit Index(const allergy::Index& ix) : ix{ix} {}
	    using Chunk = page::Chunk<1>;

	    void put(std::ostream& os, Chunk n) const;

	private:
	    const allergy::Index& ix;
	};

	/**
	 * A year page, /yyyy.
	 */
	class Year {
	public:
	    Year(const allergy::Index& ix, allergy::Year yyyy)
		: ix{ix},
		  yyyy {yyyy}
	    {}
	    using Chunk = page::Chunk<4, 1>;

	    void put(std::ostream& os, Chunk n) const;

	private:
	    const allergy::Index& ix;
	    const allergy::Year yyyy;
	};
    }
}

#endif
