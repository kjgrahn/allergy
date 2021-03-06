/* -*- c++ -*-
 *
 * Copyright (c) 2012, 2013 J?rgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_TIME_H_
#define GB_TIME_H_

#include <time.h>

timespec now();
timespec operator- (const timespec& a, const timespec& b);
bool operator> (const timespec& a, unsigned b);


/**
 * Simple utility for keeping track of periodic work.
 *   +---+---+---+---+---+---+---+---+???
 *   t0
 * You simply ask it "have I passed at least one deadline since last
 * time I asked?"
 *
 * It's really just meant for crude tasks, specifically scheduling
 * garbage collection of idle sessions, a few times a minute or so.
 */
class Periodic {
public:
    Periodic(const timespec& t, unsigned dt);
    bool check(const timespec& t);

private:
    timespec deadline;
    const unsigned dt;
};

#endif
