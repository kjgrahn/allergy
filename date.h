/* -*- c++ -*-
 *
 * Copyright (c) 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_DATE_H
#define OUTN_DATE_H

#include <string>
#include <map>
#include <time.h>

class Blob;

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
    time_t parse(const std::string& s);
    time_t parse(const Blob& s);

    std::string format(time_t t);

private:
    typedef std::map<time_t, std::string> Cal;
    Cal calendar;

    static char* format(char* buf, time_t t);
};

#endif
