/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_IMAGE_H_
#define GB_ENTITY_IMAGE_H_

#include "entity.h"
#include "../blob.h"

#include <array>
#include <iosfwd>

namespace entity {

    /**
     * An image file, presumably, given by an fd opened for reading.
     *
     * Trusts fstat(2) for the size, and never writes more than what
     * was discovered that way -- it's preferable to send a broken
     * image compared to breaking the session.
     */
    class Image : public Entity {
    public:
	explicit Image(int fd);

	std::ostream& headers(std::ostream& os) const;

	bool done() const { return n == st_size; }
	template<class Filter>
	bool tick(int fd, Filter& filter);

    private:
	const int fd;
	const size_t st_size;
	size_t n = 0;
    };
}

#endif
