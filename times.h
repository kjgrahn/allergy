/* -*- c++ -*-
 *
 * Copyright (c) 2012, 2013 J�rgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_TIME_H_
#define GB_TIME_H_

#include <time.h>

timespec now();
timespec operator- (const timespec& a, const timespec& b);
bool operator> (const timespec& a, unsigned b);

#endif
