/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "stat.h"
#include "../error.h"

#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

using entity::Stat;

Stat::Stat(int fd)
{
    struct stat st;
    if(fstat(fd, &st)) throw EntityError{errno};

    size = st.st_size;
    mtime = st.st_mtim;
}
