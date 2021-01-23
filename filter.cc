/*
 * Copyright (c) 2012, 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "filter.h"
#include "error.h"

#include <cstdio>
#include <cassert>
#include <errno.h>
#include <sys/uio.h>
#include <sys/sendfile.h>


using Filter::Plain;

ssize_t Plain::sendfile(int fd, int src, size_t n)
{
    return ::sendfile(fd, src, nullptr, n);
}


using Filter::Chunked;

namespace {
    const Blob crlf("\r\n", 2);
}


template<class Next>
bool Chunked<Next>::write(int fd, const Blob& a)
{
    char buf[8+2+1];
    const int n = std::sprintf(buf, "%x\r\n", unsigned(a.size()));
    return next.write(fd,
		      Blob(buf, n),
		      a,
		      crlf);
}

template<class Next>
bool Chunked<Next>::write_end(int fd, const Blob& a)
{
    char buf[8+2+1];
    const int n = std::sprintf(buf, "%x\r\n", unsigned(a.size()));
    return next.write(fd,
		      Blob(buf, n),
		      a,
		      Blob {"\r\n"
			    "0\r\n"
			    "\r\n", 7});
}


template<class Next>
bool Chunked<Next>::end(int fd)
{
    return next.write(fd, Blob("0\r\n"
			       "\r\n"));
}


using Filter::Zlib;


template<class Next>
bool Zlib<Next>::write(int fd)
{
    assert(!ending);
    return next.write(fd);
}


template<class Next>
bool Zlib<Next>::write(int fd, const Blob& a)
{
    assert(!ending);
    compress.push(a);
    const Blob out = compress.front();
    if(out.empty()) return true;

    bool r = next.write(fd, out);
    compress.pop();
    return r;
}


template<class Next>
bool Zlib<Next>::write(int fd, const Blob& a, const Blob& b)
{
    assert(!ending);
    compress.push(a);
    return write(fd, b);
}


template<class Next>
bool Zlib<Next>::end(int fd)
{
    if(!ending) {
	ending = true;
	compress.finish();
	const Blob out = compress.front();
	bool r = out.empty() || next.write(fd, out);
	compress.pop();

	if(!r) return r;
    }

    return next.end(fd);
}


/* Explicit template instantiation for the interesting cases.
 */
namespace Filter {

    template class Zlib<Plain>;
    template class Chunked<Plain>;
    template class Zlib<Chunked<Plain>>;
}
