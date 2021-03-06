/* $Id: responsebuf.cc,v 1.14 2011-07-03 08:27:21 grahn Exp $
 *
 * Copyright (c) 2012, 2013 J?rgen Grahn
 * All rights reserved.
 *
 */
#include "times.h"


/**
 * Shorthand for clock_gettime(CLOCK_REALTIME).
 */
timespec now()
{
    timespec ts;
    (void)clock_gettime(CLOCK_REALTIME, &ts);
    return ts;
}


/**
 * t - t0.  Undefined unless 't' is 't0' or a later time.
 * Undefined if 't' or 't0' is not normalized.
 */
timespec operator- (const timespec& t, const timespec& t0)
{
    timespec n = t;
    n.tv_sec -= t0.tv_sec;
    if(n.tv_nsec >= t0.tv_nsec) {
	n.tv_nsec -= t0.tv_nsec;
    }
    else {
	n.tv_nsec = 1000*1000*1000 - (t0.tv_nsec - n.tv_nsec);
	n.tv_sec--;
    }
    return n;
}


/**
 * True if a delta timespec 'a' is greater than 'b' seconds.
 */
bool operator> (const timespec& a, unsigned b)
{
    time_t n = b;
    return a.tv_sec > n || (a.tv_sec == n && a.tv_nsec);
}


namespace {
    timespec operator+ (timespec t, unsigned s)
    {
	t.tv_sec += s;
	return t;
    }

    bool operator< (const timespec& a, const timespec& b)
    {
	if(a.tv_sec < b.tv_sec) return true;
	if(b.tv_sec < a.tv_sec) return false;
	return a.tv_nsec < b.tv_nsec;
    }
}


/**
 * Create a periodic work with deadlines at absolute times
 * t+dt, t+2dt, t+3dt, ...
 * The interval is expressed in seconds.
 */
Periodic::Periodic(const timespec& t, unsigned dt)
    : deadline(t+dt),
      dt(dt)
{}


/**
 * True if we have passed at least one deadline since the last check
 * (or since creation).
 */
bool Periodic::check(const timespec& t)
{
    unsigned n = 0;
    while (deadline < t) {
	deadline = deadline + dt;
	n++;
    }
    return n;
}
