/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "error.h"

#include <string>
#include <cstring>
#include <iostream>

namespace {
    void message(std::ostream& os, int err, const char* kind)
    {
	if (err) {
	    os << kind << " error: " << std::strerror(err);
	}
	else {
	    os << "unspecified " << kind << " error";
	}
    }
}

void WriteError::put(std::ostream& os) const { message(os, err, "socket"); }

void EntityError::put(std::ostream& os) const { message(os, err, "entity"); }
