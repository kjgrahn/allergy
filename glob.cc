/* Copyright (c) 2017 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "glob.h"

#include <glob.h>

/**
 * Like glob(3), but without error reporting and with:
 * - GLOB_NOSORT
 * - GLOB_BRACE (support {foo,bar} expansion, like most shells)
 * - GLOB_TILDE_CHECK (tilde expansion)
 * and without
 * - GLOB_ERR (continue on error instead)
 * - GLOB_NOCHECK (return nothing instead of the pattern)
 *
 * Nothing special is done to prevent upwards directory traversal
 * or following symlinks.
 *
 * No error reporting means you cannot tell the difference between
 * no matches and e.g. I/O errors.
 */
std::vector<std::string> util::glob(const std::string& pattern)
{
    std::vector<std::string> v;

    const int flags = GLOB_NOSORT | GLOB_BRACE | GLOB_TILDE_CHECK;
    glob_t g;
    const int err = ::glob(pattern.c_str(), flags, nullptr, &g);
    if(!err) {
	v = {g.gl_pathv, g.gl_pathv + g.gl_pathc};
    }
    ::globfree(&g);
    return v;
}
