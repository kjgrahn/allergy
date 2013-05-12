/* -*- c++ -*-
 *
 * Copyright (c) 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_DATE_H
#define OUTN_DATE_H

#include "blob.h"

#include <time.h>


/**
 * Date conversion between HTTP dates (the three forms in [3.3])
 * and time_t.  There is some caching going on to avoid calling
 * strptime(3) in the most common scenarios.
 */
class DateConv {
public:
    time_t parse(const std::string& s);
    time_t parse(const Blob& s);
};

#endif
