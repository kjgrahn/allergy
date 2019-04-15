/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_STRING_H_
#define GB_ENTITY_STRING_H_

namespace entity {

    class String {
    public:
	explicit String(const char* s);
	explicit String(const std::string& s);
	template <class C>
	explicit String(const C& c)
	    : String(c.str())
	{}
    };
}

#endif
