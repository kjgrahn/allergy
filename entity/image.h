/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_IMAGE_H_
#define GB_ENTITY_IMAGE_H_

#include "sendfile.h"

namespace entity {

    /**
     * An image file, presumably, given by an fd opened for reading.
     */
    class Image : public SendFile {
    public:
	Image(const Content& content, int fd)
	    : SendFile(content, fd, "image/jpeg")
	{}
    };
}

#endif
