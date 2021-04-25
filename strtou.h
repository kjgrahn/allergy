/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_STRTOU_H_
#define ALLERGY_STRTOU_H_

/**
 * Find an unsigned decimal number at the beginning of [a..b);
 * update a to point to the first non-digit; return 0 unless
 * val < limit.
 *
 * Because std::strtoul() only handles C strings, and std::stou()
 * throws exceptions at random, and I don't have std::string_view.
 */
template <class It>
unsigned strtou(It& a, const It b, unsigned limit = 0)
{
    auto isdigit = [] (char ch) { return '0' <= ch && ch <= '9'; };

    unsigned acc = 0;

    while (a!=b && isdigit(*a)) {
	char ch = *a++;
	acc = 10*acc + (ch - '0');
    }

    if (!limit || acc < limit) return acc;
    return 0;
}

#endif
