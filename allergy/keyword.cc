/* Copyright (c) 2021 J�rgen Grahn
 * All rights reserved.
 *
 */
#include "keyword.h"

using allergy::Key;

std::ostream& Key::put(std::ostream& os) const
{
    return os << val;
}
