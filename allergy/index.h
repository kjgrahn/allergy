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
#include "../range.h"

#include <iosfwd>
#include <vector>
#include <map>

class Files;

namespace allergy {

    /**
     * A parsed set of allergy(5) files: named and timestamped photos,
     * tagged with keywords.
     *
     * It's mainly an array of entries:
     * - sorted by timestamp
     * - with Photo as key (Photo is derived from timestamp and serial)
     */
    class Index {
    public:
	Index(std::ostream& err, Files& in);
	Index(const Index&) = delete;
	Index& operator= (const Index&) = delete;

	bool valid() const { return true; }

	using Range = ::Range<std::vector<Entry>::const_iterator>;

	bool has(const Photo& p) const;
	const Entry& get(const Photo& p) const;

	Range all() const;
	Range in(const Year& key) const;
	Range in(const Month& key) const;
	Range on(const Day& key) const;

	Range cluster(const Photo& p) const;

	Day prev(const Day& day) const;
	Day next(const Day& day) const;

	Photo prev(const Photo& p) const;
	Photo next(const Photo& p) const;

	std::vector<Entry> key(const Key& k) const;

	struct {
	    Year begin;
	    Year end;
	} years;

    private:
	std::vector<Entry> entries;
	struct {
	    std::map<Key, std::vector<unsigned>> key;
	} by;
    };
}
#endif
