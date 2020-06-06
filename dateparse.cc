/* Copyright (c) 2013, 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "date.h"

#include "blob.h"
#include <cstring>

namespace {

    static const char months[] =
	"Jan Feb Mar Apr May Jun "
	"Jul Aug Sep Oct Nov Dec";

    /**
     * Parse a three-letter month name in a[0..2] as Jan--Dec,
     * into 0--11, or -1.
     */
    int month(const char* a)
    {
	int n = -1;
	for(int i=0; i<12; i++) {
	    if(std::strncmp(a, months+4*i, 3)==0) n = i;
	}
	return n;
    }


    unsigned dd(const char* p)
    {
	return 10*(p[0] - '0') + (p[1] - '0');
    }

    unsigned xd(const char* p)
    {
	if(p[0]==' ') return p[1] - '0';
	return dd(p);
    }

    unsigned dddd(const char* p)
    {
	return 100*dd(p) + dd(p+2);
    }


    bool parse_rfc850(const char* a, const char* const b,
		      struct tm& t)
    {
	/*   Sunday, 06-Nov-94 08:49:37 GMT
	 * Saturday, 06-Nov-94 08:49:37 GMT
	 *           ^cut here
	 */
	if(b-a < 30) return false;
	while(a!=b && *a!= ' ') a++;
	if(a==b) return false;
	a++;
	if(a[ 2] != '-' ||
	   a[ 6] != '-' ||
	   a[ 9] != ' ' ||
	   a[12] != ':' ||
	   a[15] != ':' ||
	   a[18] != ' ') return false;
	t.tm_mday = dd(a + 0);
	t.tm_mon  = month(a + 3);
	t.tm_year = dd(a + 7);
	/* This sucks, but she should have used a four-digit format: */
	if(t.tm_year<70) t.tm_year += 100;
	t.tm_hour = dd(a + 10);
	t.tm_min  = dd(a + 13);
	t.tm_sec  = dd(a + 16);
	return true;
    }

    bool parse_asctime(const char* const a, const char* const b,
		       struct tm& t)
    {
	/* Sun Nov  6 08:49:37 1994 */
	if(b-a != 24) return false;
	if(a[ 3] != ' ' ||
	   a[ 7] != ' ' ||
	   a[10] != ' ' ||
	   a[13] != ':' ||
	   a[16] != ':' ||
	   a[19] != ' ') return false;
	t.tm_mon  = month(a + 4);
	t.tm_mday = xd(a + 8);
	t.tm_hour = dd(a + 11);
	t.tm_min  = dd(a + 14);
	t.tm_sec  = dd(a + 17);
	t.tm_year = dddd(a + 20) - 1900;
	return true;
    }


    /**
     * Parse a date, while filling in a broken-down representation.
     */
    bool parse(const char* const a, const char* const b,
	       struct tm& t)
    {
	/* Sun, 06 Nov 1994 08:49:37 GMT */
	if(b-a != 29) return parse_rfc850(a, b, t) || parse_asctime(a, b, t);
	if(a[ 3] != ',' ||
	   a[ 4] != ' ' ||
	   a[ 7] != ' ' ||
	   a[11] != ' ' ||
	   a[16] != ' ' ||
	   a[19] != ':' ||
	   a[22] != ':' ||
	   a[25] != ' ') return false;

	t.tm_mday = dd(a + 5);
	t.tm_mon  = month(a + 8);
	t.tm_year = dddd(a + 12) - 1900;
	t.tm_hour = dd(a + 17);
	t.tm_min  = dd(a + 20);
	t.tm_sec  = dd(a + 23);
	return true;
    }
}


/**
 * Parse a HTTP date.
 *
 */
time_t DateConv::parse(const std::string& s)
{
    return parse(Blob(s));
}


time_t DateConv::parse(const Blob& s)
{
    time_t t = 0;
    struct tm tm;
    const char* p = reinterpret_cast<const char*>(s.begin());
    if(::parse(p, p + s.size(), tm)) {
	tm.tm_isdst = 0;
	t = timegm(&tm);
    }
    return t;
}
