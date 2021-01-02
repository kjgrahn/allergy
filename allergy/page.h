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
	 * The main index page, /.
	 */
	class Index {
	public:
	    explicit Index(const allergy::Index& ix) : ix{ix} {}

	    void put(std::ostream& os) const;

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

	    void put(std::ostream& os) const;

	private:
	    const allergy::Index& ix;
	    const allergy::Year yyyy;
	};
    }
}

#endif
