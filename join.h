/* -*- c++ -*-
 *
 * Copyright (c) 2007, 2008, 2009, 2017 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_JOIN_H_
#define OUTN_JOIN_H_

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

namespace util {

    std::string join(const char * delim,
		     const std::vector<std::string>& ss);

    /**
     * Like perl's join function, but generalized to
     * ostreams and iterators.
     */
    template <class Iter>
    std::ostream& join_to(std::ostream& os,
			  const char * const delim,
			  const Iter first, const Iter last)
    {
	for(Iter i = first; i!=last; ++i) {
	    if(i!=first) os << delim;
	    os << *i;
	}
	return os;
    }

    template <class Iter>
    std::string join(const char * const delim,
		     const Iter first, const Iter last)
    {
	std::ostringstream oss;
	join_to(oss, delim, first, last);
	return oss.str();
    }
}

#endif
