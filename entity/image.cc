/* Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "image.h"

#include "../error.h"
#include "../filter.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


using entity::Image;

namespace {
    off_t size_of(int fd)
    {
	struct stat st;
	if(fstat(fd, &st)) throw EntityError{errno};
	return st.st_size;
    }
}

Image::Image(int fd)
    : fd(fd),
      st_size(size_of(fd))
{}

template<class Filter>
bool Image::tick(int fd, Filter& filter)
{
    char buf[8192];
    size_t len = std::min(sizeof buf, st_size - n);
    ssize_t rlen = read(fd, buf, len);
    if (rlen==-1) throw EntityError{errno};
    if (rlen != ssize_t(len)) throw EntityError{};

    n += len;
    if (done()) return filter.end(fd, Blob{buf, len});
    return filter.write(fd, Blob{buf, len});
}

template bool Image::tick(int fd, Filter::P& filter);
