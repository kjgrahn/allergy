/* -*- c++ -*-
 *
 * Copyright (c) 2014 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_KEYS_H
#define ALLERGY_KEYS_H

#include <string>
#include <vector>

namespace allergy {

    /**
     * From string 's', extract [[bracketed] keys, even nested
     * ones]. Also extract and remove keys in {angle brackets}.
     */
    class Keys {
    public:
	explicit Keys(const std::string& s);

	typedef std::vector<std::string>::const_iterator const_iterator;
	const_iterator begin() const;
	const_iterator end() const;
	bool empty() const;

	const std::string str;
    };
}

#endif
