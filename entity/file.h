/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_FILE_H_
#define GB_ENTITY_FILE_H_

#include "entity.h"
#include "../blob.h"

#include <array>
#include <iosfwd>

namespace entity {

    /**
     * A generic entity read from file.
     *
     */
    class File : public Entity {
    public:
	explicit File(int fd);
	~File();

	std::ostream& headers(std::ostream& os) const;

	bool done() const { return n == st_size; }
	template<class Filter>
	bool tick(int fd, Filter& filter);

    private:
	const int src;
	const size_t st_size;
	size_t n = 0;
    };
}

#endif
