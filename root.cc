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
    if (dirfd!=-1) close(dirfd);
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
 * Change directory to here.  Returns success.
 */
bool Root::chdir() const
{
    return fchdir(dirfd)==0;
}
