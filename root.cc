/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "root.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

Root::Root(const std::string& path)
    : dirfd {::open(path.c_str(), O_DIRECTORY | O_PATH)}
{}

Root::~Root()
{
    if (valid()) close(dirfd);
}

/**
 * Open a file read-only relative to this root.
 *
 * No check that they don't try to circumvent the root.
 */
int Root::open(const std::string& relpath) const
{
    return openat(dirfd, relpath.c_str(), O_RDONLY);
}

/**
 * Unlink 'name' relative to this root. Returns true if this succeeds
 * (so it fails if there is nothing to unlink).
 */
bool Root::unlink(const std::string& name) const
{
    return unlinkat(dirfd, name.c_str(), 0)==0;
}

/**
 * Create directory 'name' relative to this root. Returns true if it
 * exists afterwards, like 'mkdir -p'.
 */
bool Root::mkdir(const std::string& name) const
{
    int err = mkdirat(dirfd, name.c_str(), 0777);
    if (err) return errno==EEXIST;
    return true;
}

/**
 * Change directory to here.  Returns success.
 */
bool Root::chdir() const
{
    return fchdir(dirfd)==0;
}
