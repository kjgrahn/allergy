/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_GENERATED_H_
#define GB_ENTITY_GENERATED_H_

namespace entity {

    class Generated {
    public:
	template <class F>
	explicit Generated(const F& f);

	Generated(const Generated&) = delete;
	Generated& operator= (const Generated&) = delete;

	bool done() const;
	template<class Filter>
	bool tick(int fd, Filter& filter);
    };
}

#endif
