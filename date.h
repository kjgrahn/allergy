/* -*- c++ -*-
 *
 * Copyright (c) 2013, 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_DATE_H
#define OUTN_DATE_H

#include <array>
#include <string>
#include <time.h>

class Blob;

/**
 * A precalculated table of months and weekdays.
 */
template <unsigned Epoch, unsigned N>
class Calendar {
public:
    Calendar();
    unsigned find(time_t) const;
    unsigned end() const { return N*12; }

    unsigned year(unsigned n) const { return Epoch + n/12; }
    unsigned mon(unsigned n)  const { return n % 12; }
    unsigned wday(unsigned n) const { return wal[n]; }
    unsigned dt(unsigned n, time_t t) const;

private:
    std::array<time_t, N*12> val;
    std::array<char, N*12> wal;
};

/**
 * Date conversion between HTTP dates (the three forms in [3.3])
 * and time_t.
 *
 *    Sun, 06 Nov 1994 08:49:37 GMT  ; RFC 1123
 *    Sunday, 06-Nov-94 08:49:37 GMT ; RFC 850
 *    Sun Nov  6 08:49:37 1994       ; asctime(3)
 *
 * We generate the RFC 1123 format, exactly as above. And for parsing:
 * - All three are accepted.
 * - Case is significant and so are leading zeros and the exact number of
 *   spaces, as per the RFC.
 * - The weekday is not checked for correctness (a relaxation).
 * - "Sun Nov 06 ..." is an acceptable form of "Sun Nov  6 ..." (possibly
 *   a relaxation too).
 *
 * In practice, we're even more relaxed here: in the context of HTTP
 * there's nothing to gain by sneaking in a malformed date, and
 * performance takes priority.
 *
 * - Should also describe what year ranges are tolerated -- there's
 *   nothing wrong with a date from 1914, yet time_t cannot reliably
 *   represent it.
 * - Should describe what we do about 31th of February etc.
 */
class DateConv {
public:
    DateConv();
    time_t parse(const std::string& s);
    time_t parse(const Blob& s);

    std::string format(time_t t) const;

private:
    const Calendar<1980, 50> cal;

    static char* format(char* buf, time_t t);
    static char* format(char* buf, const struct tm& tm);
};

#endif
