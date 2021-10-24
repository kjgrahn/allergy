/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_GENERATED_H_
#define GB_ENTITY_GENERATED_H_

#include <iostream>
#include <sstream>

#include "../content.h"

namespace entity {

    /**
     * HTML generated piece by piece by a <Page> object.
     */
    template <class Page>
    class Generated : public Entity {
    public:
	template <class ... Args>
	Generated(const Content& content, Args&& ... argv)
	    : Entity {content},
	      src {content.index, argv ...},
	      chunk {src.begin()}
	{}

	std::ostream& headers(std::ostream& os) const;

	bool done() const;
	template<class Filter>
	bool tick(int fd, Filter& filter);

    private:
	const Page src;
	typename Page::Chunk chunk;
    };

    template <class Page>
    std::ostream& Generated<Page>::headers(std::ostream& os) const
    {
	return os << "Content-Type: text/html; charset=utf-8" << "\r\n"
		  << "Last-Modified: " << content.epoch << "\r\n";
    }

    template <class Page>
    bool Generated<Page>::done() const
    {
	return chunk.end();
    }

    template <class Page> template <class Filter>
    bool Generated<Page>::tick(int fd, Filter& filter)
    {
	std::ostringstream os;
	src.put(os, chunk++);
	if (chunk.end()) {
	    return filter.write_end(fd, Blob {os.str()});
	}
	else {
	    return filter.write(fd, Blob {os.str()});
	}
    }
}

#endif
