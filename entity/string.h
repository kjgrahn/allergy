/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_STRING_H_
#define GB_ENTITY_STRING_H_

#include <string>

class Blob;

namespace entity {

    /**
     * A fixed string as a HTML entity: a size, and a way to emit its
     * content in reasonably-sized chunks.
     */
    class String {
    public:
	explicit String(const char* s);
	explicit String(const std::string& s);
	template <class C>
	explicit String(const C& c)
	    : String(c.str())
	{}

	size_t size() const { return s.size(); }
	Blob tick();
    private:
	std::string s;
    };
}

#endif
