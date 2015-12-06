/* -*- c++ -*-
 *
 * Copyright (c) 2015 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_BACKLOG_H_
#define GB_BACKLOG_H_

#include "blob.h"

/**
 * Slightly funny name for a thing used to write to a nonblocking
 * socket, and which stores unwritten parts until the next write.
 */
class Backlog {
public:
    Backlog() = default;
    Backlog(const Backlog&) = delete;
    Backlog& operator= (const Backlog&) = delete;

    bool empty() const { return v.empty(); }

    size_t write(int fd);
    size_t write(int fd, const Blob& a);
    size_t write(int fd, const Blob& a, const Blob& b);
    size_t write(int fd, const Blob& a, const Blob& b, const Blob& c);

private:
    std::vector<char> v;
};


#endif
