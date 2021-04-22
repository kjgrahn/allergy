/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_UTF8_H_
#define ALLERGY_UTF8_H_

#include <string>
#include <cstring>

namespace utf8 {

    std::string encode(const char* a, const char* b);

    inline std::string encode(const std::string& s)
    {
	const char* a = s.data();
	return encode(a, a + s.size());
    }

    inline std::string encode(const char* s)
    {
	const size_t n = std::strlen(s);
	return encode(s, s+n);
    }
}

#endif
