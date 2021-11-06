/* Copyright (c) 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "date.h"

#include <algorithm>
#include <cstring>

template <unsigned Epoch, unsigned N>
Calendar<Epoch, N>::Calendar()
{
    unsigned n = 0;
    for (unsigned y=Epoch; y < Epoch+N; y++) {

	for (unsigned m=0; m < 12; m++) {

	    struct tm tm = {};
	    tm.tm_year = y - 1900;
	    tm.tm_mon  = m;
	    tm.tm_mday = 1;

	    val[n] = timegm(&tm);
	    wal[n] = tm.tm_wday;
	    n++;
	}
    }
}

/**
 * Find the largest calendar entry <= t (but don't find the last one).
 * Otherwise return end().
 *
 * This is a way to quickly find the year and month of a time_t for
 * certain years, and thus avoid leap year and weekday calculations.
 */
template <unsigned Epoch, unsigned N>
unsigned Calendar<Epoch, N>::find(time_t t) const
{
    const auto a = val.begin();
    const auto e = val.end();
    auto p = std::upper_bound(a, e, t);
    if (p==a) return end();
    if (p==e) return end();
    p--;
    return std::distance(a, p);
}

/**
 * The offset from calendar entry 'n' to time 't', from 0 seconds to
 * 31 days. Undefined result unless 't' is in the calendar.
 */
template <unsigned Epoch, unsigned N>
unsigned Calendar<Epoch, N>::dt(unsigned n, time_t t) const
{
    return t - val[n];
}

DateConv::DateConv() = default;

namespace {

    /**
     * n % m, and then divide n by m.
     */
    unsigned chop(unsigned& n, unsigned m)
    {
	unsigned mod = n % m;
	n = n / m;
	return mod;
    }
}

/**
 * Format 't' according to RFC 1123:
 * "Sun, 06 Nov 1994 08:49:37 GMT".
 *
 * Similar to strftime(..., "%a, %d %b %Y %H:%M:%S GMT")
 * but not broken by locale settings and, in my environment,
 * a bit faster if you hit the precalculated table.
 */
std::string DateConv::format(const time_t t) const
{
    char buf[30];

    const unsigned n = cal.find(t);
    if (n==cal.end()) {
	return {buf, format(buf, t)};
    }

    /* cal[n] is the time_t for yyyy-mm-01 00:00, for the month
     * containing t. Then the rest of the calculations become easy:
     * the table removed all leap year issues and since we're in UTC
     * there is no DST.
     */
    struct tm tm;
    tm.tm_year = cal.year(n) - 1900;
    tm.tm_mon  = cal.mon(n);
    unsigned dt = cal.dt(n, t);
    tm.tm_sec  = chop(dt, 60);
    tm.tm_min  = chop(dt, 60);
    tm.tm_hour = chop(dt, 24);
    tm.tm_mday = 1 + dt;
    tm.tm_wday = (cal.wday(n) + dt) % 7;

    return {buf, format(buf, tm)};
}


char* DateConv::format(char* const buf, time_t t)
{
    struct tm tm;
    gmtime_r(&t, &tm);
    return format(buf, tm);
}

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
