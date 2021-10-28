/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "sendfile.h"

#include "../error.h"
#include "../filter.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>

using entity::SendFile;

namespace {
    off_t size_of(int fd)
    {
	struct stat st;
	if(fstat(fd, &st)) throw EntityError{errno};
	return st.st_size;
    }
}

SendFile::SendFile(const Content& content, int fd, const char* mime)
    : Entity{content},
      src(fd),
      st_size(size_of(src)),
      mime(mime)
{}

SendFile::~SendFile()
{
    close(src);
}

std::ostream& SendFile::headers(std::ostream& os) const
{
    os << "Content-Type: " << mime << "\r\n"
       << "Content-Length: " << st_size << "\r\n"
       << "Last-Modified: Mon, 04 Aug 2014 22:05:06 GMT\r\n";

    return os;
}

template<class Filter>
bool SendFile::tick(int fd, Filter& filter)
{
    const size_t max = 40960;
    size_t len = std::min(max, st_size - n);
    ssize_t wlen = filter.sendfile(fd, src, len);
    if (wlen==-1) throw EntityError{errno};
    n += wlen;
    return size_t(wlen)==len;
}

template bool SendFile::tick(int fd, Filter::P& filter);
