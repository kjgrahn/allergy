/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_LOWER_BOUND_H
#define ALLERGY_LOWER_BOUND_H

#include <iterator>

/**
 * Assuming pred(elem) goes from true to false somewhere as you move
 * forward in [a, b), return the leftmost iterator c so that !pred(*c).
 * If none exist, return b.
 *
 * This is equivalent tp std::lower_bound, but put differently and
 * with the predicate expressed differently, now that we have lambdas
 * and it's easy to provide the function.
 */
template <class It, class Pred>
It lower_bound(It a, It b, Pred pred)
{
    while (a!=b) {
	auto c = std::next(a, std::distance(a, b)/2);
	if (pred(*c)) {
	    a = c;
	    a++;
	}
	else {
	    b = c;
	}
    }
    return a;
}

#endif
