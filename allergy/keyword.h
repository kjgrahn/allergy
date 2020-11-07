/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_KEY_H
#define ALLERGY_KEY_H

#include <string>

namespace allergy {

    struct Key {

	explicit Key(const std::string& s) : val{s} {}
	Key(const char* a, const char* b) : val{a, b} {}
	explicit operator bool() const { return !val.empty(); }
	bool operator== (const Key& other) const { return val==other.val; }
	bool operator<  (const Key& other) const { return val<other.val; }

    private:
	std::string val;
    };
}
#endif
