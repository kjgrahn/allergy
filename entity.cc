/*
 * Copyright (c) 2015 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "entity.h"
#include "blob.h"
#include "error.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


Entity::String::String(const char* s)
    : s(s),
      i(0)
{}

/**
 * Return the next chunk, or an empty one if there's nothing more to
 * read.
 */
Blob Entity::String::tick()
{
    size_t i0 = i;
    i = std::min(i + 8192, s.size());
    const char* p = s.data();
    return Blob(p + i0, p + i);
}

namespace {
    size_t stat_size(int fd)
    {
	struct stat buf;
	int err = fstat(fd, &buf);
	if(err) throw SessionError();
	// XXX narrowing off_t
	return buf.st_size;
    }
}

Entity::File::File(int fd)
    : fd(fd),
      statsize(stat_size(fd)),
      acc(0)
{}

Blob Entity::File::tick()
{
    ssize_t rc = read(fd, v.data(), v.size());
    if(rc==-1) throw SessionError();

    size_t n = rc;
    acc += n;
    /* It's important for HTTP (with Content-Length: header) that
     * size() matches what the series of tick() eventually reads.  For
     * a file which increases in size while being read, or shrinks,
     * the response must fail, and probably the whole HTTP session.
     */
    if(n==0 && acc != statsize) throw SessionError();
    return Blob(v.data(), n);
}
