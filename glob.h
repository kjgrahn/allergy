/* -*- c++ -*-
 *
 * Copyright (c) 2017 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_GLOB_H_
#define OUTN_GLOB_H_

#include <string>
#include <vector>

namespace util {

    std::vector<std::string> glob(const std::string& pattern);
}

#endif
