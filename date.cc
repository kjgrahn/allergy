/* Copyright (c) 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "date.h"

namespace {

    static const tm NIL_TM = { 0 };

};


/**
 * Parse a HTTP date.  [3.3.1] lists these examples, where the first
 * is the recommended:
 *
 *    Sun, 06 Nov 1994 08:49:37 GMT  ; RFC 822, updated by RFC 1123
 *    Sunday, 06-Nov-94 08:49:37 GMT ; RFC 850, obsoleted by RFC 1036
 *    Sun Nov  6 08:49:37 1994       ; ANSI C's asctime() format
 *
 */
time_t DateConv::parse(const std::string& s)
{
    const char* const p = s.c_str();
    struct tm tm = NIL_TM;

    const char* q = strptime(p, "%a, %d %b %Y %T GMT", &tm);
    if(!q) {
	q = strptime(p, "%a, %d-%b-%y %T GMT", &tm);
    }
    if(!q) {
	q = strptime(p, "%a %b %d %T %Y", &tm);
    }

    return q? mktime(&tm) : 0;
}
