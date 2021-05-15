/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_RANGE_H
#define ALLERGY_RANGE_H

#include <cstddef>
#include <iterator>

/**
 * An [a, b) pair of (random-access) iterators.
 */
template <class It>
class Range {
public:
    Range(It a, It b) : a {a}, b {b} {}

    It begin() const { return a; }
    It end()   const { return b; }
    std::size_t size() const { return std::distance(a, b); }
    bool empty() const { return !size(); }

private:
    It a; It b;
};

#endif
