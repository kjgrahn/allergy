/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "file.h"

#include "../error.h"
#include "../filter.h"

#include <unistd.h>

#include <iostream>

using entity::File;

File::File(const Content& content, int fd, const char* mime)
    : Entity{content},
      src{fd},
      stat{fd},
      mime{mime}
{}

File::~File()
{
    close(src);
}

std::ostream& File::headers(std::ostream& os) const
{
    os << "Content-Type: " << mime << "\r\n"
       << "Content-Length: " << stat.size << "\r\n"
       << "Last-Modified: " << stat.mtime << "\r\n";

    return os;
}

template<class Filter>
bool File::tick(int fd, Filter& filter)
{
    char buf[8192];
    size_t len = std::min(sizeof buf, stat.size - n);
    ssize_t rlen = read(src, buf, len);
    if (rlen==-1) throw EntityError{errno};
    if (rlen != ssize_t(len)) throw EntityError{};

    n += len;
    if (done()) return filter.end(fd, Blob{buf, len});
    return filter.write(fd, Blob{buf, len});
}

template bool File::tick(int fd, Filter::P& filter);
