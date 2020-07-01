/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_IMAGE_H_
#define GB_ENTITY_IMAGE_H_

#include "file.h"

namespace entity {

    /**
     * An image file, presumably, given by an fd opened for reading.
     */
    class Image : public File {
    public:
	explicit Image(int fd)
	    : File(fd, "image/jpeg")
	{}
    };
}

#endif
