/* -*- c++ -*-
 *
 * Copyright (c) 2019 J�rgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_STRING_H_
#define GB_ENTITY_STRING_H_

#include <string>
#include <iosfwd>

#include "entity.h"
#include "../blob.h"

namespace entity {

    /**
     * A fixed string as a HTML entity: a size, and a way to emit its
     * content in reasonably-sized chunks.
     */
    class String : public Entity {
    public:
	explicit String(const char* s);
	explicit String(const std::string& s);
	template <class C>
	explicit String(const C& c)
	    : String(c.str())
	{}

	String& operator= (String&&) = default;

	std::ostream& headers(std::ostream& os) const;

	bool done() const { return blob.empty(); }

	template<class Filter>
	bool tick(int fd, Filter& filter);

    private:
	std::string s;
	Blob blob;
    };
}

#endif
