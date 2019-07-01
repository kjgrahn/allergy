/* Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "image.h"

#include "../error.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


using entity::Image;

namespace {
    off_t size_of(int fd)
    {
	struct stat st;
	if(fstat(fd, &st)) throw EntityError(errno);
	return st.st_size;
    }
}

Image::Image(int fd)
    : fd(fd),
      n(0)
{}
