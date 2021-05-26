/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_KEY_H
#define ALLERGY_KEY_H

#include <string>
#include <iosfwd>

namespace allergy {

    struct Key {

	explicit Key(const std::string& s) : val{s} {}
	Key(const char* a, const char* b) : val{a, b} {}
	explicit operator bool() const { return !val.empty(); }
	bool operator== (const Key& other) const { return val==other.val; }
	bool operator<  (const Key& other) const { return val<other.val; }
	std::ostream& put(std::ostream& os) const;

	struct Url { const Key& val; };

	const std::string val;
    };

    inline
    Key::Url url(const Key& val) { return {val}; }

    inline
    std::ostream& operator<< (std::ostream& os, const Key& val) { return val.put(os); }

    std::ostream& operator<< (std::ostream& os, const Key::Url& val);
}
#endif
