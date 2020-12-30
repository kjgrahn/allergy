/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_INDEX_H
#define ALLERGY_INDEX_H

#include "entry.h"
#include "keyword.h"

#include <iosfwd>
#include <vector>
#include <map>

class Files;

namespace allergy {

    /**
     * A parsed set of allergy(5) files: named and timestamped photos,
     * tagged with keywords.
     */
    class Index {
    public:
	Index(std::ostream& err, Files& in);
	Index(const Index&) = delete;
	Index& operator= (const Index&) = delete;

	bool valid() const { return true; }

	using iterator = std::vector<Entry>::const_iterator;
	iterator begin() const { return entries.begin(); }
	iterator end() const { return entries.end(); }

	bool has(const Photo& p) const;

	std::vector<Entry> all() const;

	std::vector<Entry> in(const Year& key) const;
	std::vector<Entry> in(const Month& key) const;
	std::vector<Entry> on(const Day& key) const;

	std::vector<Entry> key(const Key& k) const;

	struct {
	    Year begin;
	    Year end;
	} years;

    private:
	std::vector<Entry> entries;
	struct {
	    std::map<Photo, unsigned> name;
	    std::map<Key, std::vector<unsigned>> key;
	} by;
    };
}
#endif
