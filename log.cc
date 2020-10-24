/*
 * Copyright (c) 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "log.h"

#include <sys/uio.h>

Syslog Syslog::log;

Syslog::Syslog()
    : os(this)
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
    if (use_syslog) {
	*pptr() = '\0';
	syslog(prio, "%s", pbase());
    }
    else {
	char nl = '\n';
	iovec v[2] = {{pbase(), pptr() - pbase()},
		      {&nl, 1}};
	writev(1, v, 2);
    }
    char_type* p = &v[0];
    setp(p, p + v.size()-1);
}

void Syslog::activate()
{
    use_syslog = true;
}
