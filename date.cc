/* Copyright (c) 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "date.h"

#include <cstring>

namespace {

    static const char weekdays[] = "Sun Mon Tue Wed Thu Fri Sat";
    static const char months[] =
	"Jan Feb Mar Apr May Jun "
	"Jul Aug Sep Oct Nov Dec";

    void wdd(char* p, unsigned n)
    {
	p[0] = '0' + n/10;
	p[1] = '0' + n%10;
    }

    void wdddd(char* p, unsigned n)
    {
	wdd(p, n/100);
	wdd(p + 2, n%100);
    }
}


/**
 * Format 't' according to RFC 1123:
 * "Sun, 06 Nov 1994 08:49:37 GMT".
 *
 * Similar to strftime(..., "%a, %d %b %Y %H:%M:%S GMT")
 * but not broken by locale settings and, in my environment,
 * more than twice as fast.
 */
std::string DateConv::format(const time_t t)
{
    std::string s;

#if 0
    char buf[30];
    s.assign(buf, format(buf, t));
#else

    Cal::const_iterator i = calendar.lower_bound(t);
    if(i!=calendar.end() && t - i->first < 24*3600) {
	unsigned since_midnight = t - i->first;
	s = i->second;
	char* p = &s[17];
	wdd(p, since_midnight/3600); p += 3;
	wdd(p, (since_midnight/60) % 60); p += 3;
	wdd(p, since_midnight % 60);
    }
    else {
	struct tm tm;
	gmtime_r(&t, &tm);
	char buf[30];
	s.assign(buf, format(buf, tm));

	unsigned since_midnight = (tm.tm_hour*60 + tm.tm_min)*60 + tm.tm_sec;
	calendar[t - since_midnight] = s;
    }
#endif

    return s;
}


char* DateConv::format(char* const buf, time_t t)
{
    struct tm tm;
    gmtime_r(&t, &tm);
    return format(buf, tm);
}


char* DateConv::format(char* buf, const struct tm& tm)
{
    char* p = buf;

    std::copy(weekdays + 4*tm.tm_wday,
	      weekdays + 4*tm.tm_wday + 3, p); p += 3;
    *p++ = ',';
    *p++ = ' ';
    wdd(p, tm.tm_mday); p += 2;
    *p++ = ' ';
    std::copy(months + 4*tm.tm_mon,
	      months + 4*tm.tm_mon + 3, p); p += 3;
    *p++ = ' ';
    wdddd(p, tm.tm_year + 1900); p += 4;
    *p++ = ' ';
    wdd(p, tm.tm_hour); p += 2;
    *p++ = ':';
    wdd(p, tm.tm_min); p += 2;
    *p++ = ':';
    wdd(p, tm.tm_sec); p += 2;
    std::copy(" GMT", " GMT" + 4, p); p += 4;
    return p;
}
