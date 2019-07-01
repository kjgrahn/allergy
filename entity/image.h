/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_IMAGE_H_
#define GB_ENTITY_IMAGE_H_

#include "../blob.h"
#include <array>

namespace entity {

    /**
     * An image file, presumably, given by an fd opened for reading.
     */
    class Image {
    public:
	explicit Image(int fd);
	Image(const Image&) = delete;
	Image& operator= (const Image&) = delete;

	size_t size() const;
	bool done() const;
	template<class Filter>
	bool tick(int fd, Filter& filter);

    private:
	const int fd;
	Blob tick();
	std::array<char, 8192> buf;
	size_t n;
    };
}

#endif
