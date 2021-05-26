/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "keyword.h"
#include "../quote.h"

#include <iostream>

using allergy::Key;

std::ostream& Key::put(std::ostream& os) const
{
    return os << val;
}

std::ostream& allergy::operator<< (std::ostream& os, const Key::Url& val)
{
    return os << "/key/" << quote(val.val.val);
}
