/* -*- c++ -*-
 *
 * Copyright (c) 2015 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_H_
#define GB_ENTITY_H_

#include <array>

class Blob;

namespace entity {

    /**
     * A fixed string as a HTML entity: a size, and a way to emit its
     * content in reasonably-sized chunks.
     */
    class String {
    public:
	explicit String(const char* s);
	size_t size() const { return s.size(); }
	Blob tick();
    private:
	const std::string s;
	size_t i;
    };


    /**
     * A file's contents, given an fd open for reading.
     */
    class File {
    public:
	explicit File(int fd);
	size_t size() const;
	Blob tick();
    private:
	const int fd;
	const size_t statsize;
	size_t acc;
	std::array<uint8_t, 8192> v;
    };
}

#endif
