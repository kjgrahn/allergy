/*
 * Copyright (c) 2013, 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "log.h"

#include "times.h"

#include <iostream>
#include <array>
#include <ctime>
#include <cstdio>

#include <sys/uio.h>
#include <sys/time.h>

namespace {

    /**
     * Current time as just time of day, e.g. "06:02:00.999 ".
     * Note the milliseconds and the trailing space: it's just
     * for debug logging.
     *
     * Truncates to ms instead of rounding: 06:02:00.9999 becomes
     * 06:02:00.999, not 06:02:01.000.
     */
    auto timestamp()
    {
	std::array<char, 13> v;
	const timespec ts = now();
	const struct tm tm = *std::localtime(&ts.tv_sec);
	const unsigned ms = ts.tv_nsec / 1000 / 1000;
	std::snprintf(v.data(), v.size(), "%02d:%02d:%02d.%03u",
		      tm.tm_hour, tm.tm_min, tm.tm_sec, ms);
	v.back() = ' ';
	return v;
    }
}

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
	auto header = timestamp();
	size_t len = pptr() - pbase();
	iovec v[3] = {{header.data(), header.size()},
		      {pbase(), len},
		      {&nl, 1}};
	writev(1, v, 3);
    }
    char_type* p = &v[0];
    setp(p, p + v.size()-1);
}

void Syslog::activate()
{
    use_syslog = true;
}
