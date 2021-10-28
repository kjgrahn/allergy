/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_FILE_H_
#define GB_ENTITY_FILE_H_

#include "entity.h"

#include <iosfwd>

namespace entity {

    /**
     * A generic entity read from file.
     *
     * Trusts fstat(2) for the size, and never writes more than what
     * was discovered that way -- it's preferable to send a broken
     * image compared to breaking the session.
     */
    class File : public Entity {
    public:
	File(const Content& content, int fd, const char* mime);
	~File();

	std::ostream& headers(std::ostream& os) const;

	bool done() const { return n == st_size; }
	template<class Filter>
	bool tick(int fd, Filter& filter);

    private:
	const int src;
	const size_t st_size;
	const char* const mime;
	size_t n = 0;
    };
}

#endif
