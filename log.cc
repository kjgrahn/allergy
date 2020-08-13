/*
 * Copyright (c) 2013 J�rgen Grahn
 * All rights reserved.
 *
 */
#include "log.h"

Syslog Syslog::log;


Syslog::Syslog()
    : v(500),
      os(this)
{
    char_type* p = &v[0];
    setp(p, p + v.size()-1);
    openlog("allergyd", 0, LOG_DAEMON);
}


Syslog::~Syslog()
{
    closelog();
}


void Syslog::flush(int prio)
{
    *pptr() = '\0';
    syslog(prio, "%s", pbase());
    char_type* p = &v[0];
    setp(p, p + v.size()-1);
}
