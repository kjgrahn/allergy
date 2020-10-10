/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_INDEX_H
#define ALLERGY_INDEX_H

#include "entry.h"

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
	explicit Index(Files& in);

	using iterator = std::vector<Entry>::const_iterator;
	iterator begin() const { return entries.begin(); }
	iterator end() const { return entries.end(); }

	bool has(const Photo& p) const;

	std::vector<Entry> all() const;

	std::vector<Entry> year(const std::string& s) const;
	std::vector<Entry> month(const std::string& s) const;
	std::vector<Entry> day(const std::string& s) const;

	std::vector<Entry> key(const std::string& s) const;

    private:
	std::vector<Entry> entries;
	struct {
	    std::map<Photo, unsigned> name;
	    std::map<std::string, std::vector<unsigned>> key;
	} by;
    };
}
#endif
