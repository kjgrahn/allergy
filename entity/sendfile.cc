/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "sendfile.h"

#include "../error.h"
#include "../filter.h"

#include <unistd.h>

#include <iostream>

using entity::SendFile;

SendFile::SendFile(const Content& content, int fd, const char* mime)
    : Entity{content},
      src{fd},
      stat{fd},
      mime{mime}
{}

SendFile::~SendFile()
{
    close(src);
}

std::ostream& SendFile::headers(std::ostream& os) const
{
    os << "Content-Type: " << mime << "\r\n"
       << "Content-Length: " << stat.size << "\r\n"
       << "Last-Modified: " << stat.mtime << "\r\n";

    return os;
}

template<class Filter>
bool SendFile::tick(int fd, Filter& filter)
{
    const size_t max = 40960;
    size_t len = std::min(max, stat.size - n);
    ssize_t wlen = filter.sendfile(fd, src, len);
    if (wlen==-1) throw EntityError{errno};
    n += wlen;
    return size_t(wlen)==len;
}

template bool SendFile::tick(int fd, Filter::P& filter);
