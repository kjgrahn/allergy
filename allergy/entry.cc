/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "entry.h"

#include <iostream>

using allergy::Entry;

std::ostream& operator<< (std::ostream& os, const Entry& val)
{
    os << val.filename << '\n';
    if(val.timestamp.valid()) {
	os << val.timestamp << '\n';
    }
    return os << val.text;
}
