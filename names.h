/* -*- c++ -*-
 *
 * Copyright (c) 2013 J�rgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTND_NAMES_H
#define OUTND_NAMES_H

#include "request.h"

struct Names {
    Names();
    Request::Property lookup(const char* a, const char* b);
};

#endif

