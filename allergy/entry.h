/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_ENTRY_H
#define ALLERGY_ENTRY_H

#include "keys.h"
#include "photo.h"
#include "timestamp.h"
#include "keyword.h"

#include "../join.h"

#include <string>
#include <vector>
#include <set>
#include <iosfwd>

class Root;

namespace allergy {

    /**
     * An entry from an allergy(1) picture information file:
     * - the Photo, i.e. a (date, serial) unique identifier
     * - filename
     * - timestamp
     * - optional descriptive text
     * - keywords from the text
     *
     * Entries are ordered by timestamp. The Photo is synthesized from
     * the file name, and the Day part of the Photo should match the
     * Day part of the timestamp, but the Entries are not necessarily
     * also ordered by Photo.
     */
    class Entry {
    public:
	template <class It>
	Entry(const std::string& filename,
	      It begin,
	      It end,
	      const Timestamp& timestamp,
	      const Entry* prev);

	explicit operator bool() const { return bool {photo}; }
	bool operator< (const Entry& other) const { return timestamp < other.timestamp; }

	Photo photo;
	std::string filename;
	Timestamp timestamp;
	std::string text;
	std::set<Key> keywords;
    };

    std::ostream& operator<< (std::ostream& os, const Entry& val);

    inline std::string url(const Entry& e) { return e.photo.url(); }
    inline std::string thumburl(const Entry& e) { return e.photo.thumburl(); }
    inline std::string dir(const Entry& e) { return e.photo.dir(); }
    std::string path(const Entry&);
}

int open(const Root& r, const allergy::Entry&);

#endif
