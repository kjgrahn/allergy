/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_PAGE_H
#define ALLERGY_PAGE_H

#include "index.h"
#include "chunk.h"

#include <iosfwd>

namespace allergy {

    namespace page {

	/**
	 * The main index page, /.
	 */
	class Frontpage {
	public:
	    explicit Frontpage(const allergy::Index& ix) : ix{ix} {}
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
