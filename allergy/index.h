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
#include <unordered_map>

class Files;

namespace allergy {

    /**
     * A parsed set of allergy(5) files: named and timestamped photos,
     * tagged with keywords.
     */
    class Index {
    public:
	explicit Index(Files& in);

	using const_iterator = std::vector<Entry>::const_iterator;
	const_iterator begin() const { return entries.begin(); }
	const_iterator end() const { return entries.end(); }

    private:
	std::vector<Entry> entries;
	std::unordered_map<std::string, std::vector<unsigned>> keys;
    };
}
#endif
