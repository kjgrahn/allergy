/* Copyright (c) 2017 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "join.h"

#include <sstream>

/**
 * Like perl's join function.
 */
std::string util::join(const char * const delim,
		       const std::vector<std::string>& ss)
{
    std::ostringstream oss;
    join_to(oss, delim, ss.begin(), ss.end());
    return oss.str();
}
