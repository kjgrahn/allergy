/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
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
	String(const char* s, const char* mime);
	String(const std::string& s, const char* mime);

	String& operator= (String&&) = default;

	std::ostream& headers(std::ostream& os) const;

	bool done() const { return blob.empty(); }

	template<class Filter>
	bool tick(int fd, Filter& filter);

    private:
	const std::string s;
	Blob blob;
	const char* const mime;
    };
}

#endif
