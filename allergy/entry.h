/* -*- c++ -*-
 *
 * Copyright (c) 2020 J�rgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_ENTRY_H
#define ALLERGY_ENTRY_H

#include "keys.h"
#include "photo.h"
#include "timestamp.h"
#include "keyword.h"

#include <string>
#include <vector>
#include <set>
#include <iosfwd>

namespace allergy {

    /**
     * An entry from an allergy(1) picture information file:
     * - filename
     * - an optional timestamp
     * - optional descriptive text
     * - keywords from the text
     */
    class Entry {
    public:
	template <class It>
	Entry(const std::string& filename,
	      It begin,
	      It end,
	      const Timestamp& timestamp,
	      const Entry* prev);

	Photo filename;
	Timestamp timestamp;
	std::string text;
	std::set<Key> keywords;
    };

    template <class It>
    Entry::Entry(const std::string& filename,
		 It begin,
		 It end,
		 const Timestamp& timestamp,
		 const Entry* const prev)
	: filename(filename),
	  timestamp(timestamp)
    {
	for(It i=begin; i!=end; i++) {
	    const Keys keys(*i);
	    if(i!=begin) text += '\n';
	    text += keys.str();
	    for(const std::string& key : keys) {
		keywords.emplace(key);
	    }
	}
	if (text=="ibid" && prev) {
	    text = prev->text;
	    keywords = prev->keywords;
	}
    }

    std::ostream& operator<< (std::ostream& os, const Entry& val);
}

#endif
