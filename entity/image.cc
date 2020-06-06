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

#include <iostream>

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
    : src(fd),
      st_size(size_of(src))
{}

Image::~Image()
{
    close(src);
}

std::ostream& Image::headers(std::ostream& os) const
{
    os << "Content-Type: image/jpeg\r\n"
       << "Content-Length: " << st_size << "\r\n"
       << "Last-Modified: Mon, 04 Aug 2014 22:05:06 GMT\r\n";

    return os;
}

template<class Filter>
bool Image::tick(int fd, Filter& filter)
{
    char buf[8192];
    size_t len = std::min(sizeof buf, st_size - n);
    ssize_t rlen = read(src, buf, len);
    if (rlen==-1) throw EntityError{errno};
    if (rlen != ssize_t(len)) throw EntityError{};

    n += len;
    if (done()) return filter.end(fd, Blob{buf, len});
    return filter.write(fd, Blob{buf, len});
}

template bool Image::tick(int fd, Filter::P& filter);
