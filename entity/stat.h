/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_STAT_H_
#define GB_ENTITY_STAT_H_

#include <iosfwd>

#include <sys/types.h>
#include <sys/stat.h>

namespace entity {

    /**
     * The parts of struct stat for a file which map to its entity
     * headers.
     */
    class Stat {
    public:
	explicit Stat(int fd);

	size_t size;
	timespec mtime;
    };

    std::ostream& operator<< (std::ostream& os, const timespec& val);
}

#endif
