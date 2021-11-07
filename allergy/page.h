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
	    using Chunk = page::Chunk<2>;

	    Chunk begin() const { return {}; }
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

	    Chunk begin() const { return {}; }
	    void put(std::ostream& os, Chunk n) const;

	private:
	    const allergy::Index& ix;
	    const allergy::Year yyyy;
	};

	/**
	 * A month page, /yyyy-mm.
	 */
	class Month {
	public:
	    Month(const allergy::Index& ix, allergy::Month month);

	    using Chunk = page::container::Chunk;

	    Chunk begin() const;
	    void put(std::ostream& os, Chunk n) const;

	private:
	    const allergy::Index::Range ee;
	    const allergy::Month month;
	};

	/**
	 * A day page, /yyyy-mm-dd.
	 */
	class Day {
	public:
	    Day(const allergy::Index& ix, allergy::Day day);

	    using Chunk = page::container::Chunk;

	    Chunk begin() const;
	    void put(std::ostream& os, Chunk n) const;

	private:
	    const allergy::Index::Range ee;
	    const allergy::Day day;
	    const allergy::Day prev;
	    const allergy::Day next;
	};

	/**
	 * A per-photo page, /yyyy-mm-dd/serial.
	 */
	class Photo {
	public:
	    Photo(const Index& ix, allergy::Day day, const Serial& serial);

	    using Chunk = page::Chunk<2>;

	    Chunk begin() const { return {}; }
	    void put(std::ostream& os, Chunk n) const;

	private:
	    const allergy::Entry e;
	};

	/**
	 * A keyword page, /key/keyword.
	 */
	class Keyword {
	public:
	    Keyword(const allergy::Index& ix, const allergy::Key& key);

	    using Chunk = page::container::Chunk;

	    Chunk begin() const;
	    void put(std::ostream& os, Chunk n) const;

	private:
	    const std::vector<allergy::Entry> ee;
	    const allergy::Key key;
	};

	/**
	 * The "all keywords" page, /key/.
	 */
	class Words {
	public:
	    explicit Words(const allergy::Index& ix) : ix{ix} {}

	    using Chunk = page::Chunk<25>;

	    Chunk begin() const { return {}; }
	    void put(std::ostream& os, Chunk n) const;

	private:
	    const allergy::Index& ix;
	};
    }
}

#endif
