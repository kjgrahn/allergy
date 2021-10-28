/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_SENDFILE_H_
#define GB_ENTITY_SENDFILE_H_

#include "entity.h"

#include <iosfwd>

namespace entity {

    /**
     * Like File, but uses Linux sendfile(2) to avoid copying through
     * userspace.
     *
     * All entities are emitted through a filter; this one can only be
     * used with Filter::Plain.
     */
    class SendFile : public Entity {
    public:
	SendFile(const Content& content, int fd, const char* mime);
	~SendFile();

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
