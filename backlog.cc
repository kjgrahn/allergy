/*
 * Copyright (c) 2012, 2013, 2015 J�rgen Grahn
 * All rights reserved.
 *
 */
#include "backlog.h"
#include "error.h"

#include <cstdio>
#include <cassert>
#include <errno.h>
#include <sys/uio.h>


namespace {

    iovec iov(const std::vector<char>& v)
    {
	iovec tmp = { (void*)&v[0], v.size() };
	return tmp;
    }

    iovec iov(const Blob& a)
    {
	iovec tmp = { (void*)a.begin(), a.size() };
	return tmp;
    }

    /**
     * Insert the contents of the iovec at the end of 'acc', except
     * for the first 'n', which presumably have been written already.
     * For convenient mixing with writev(2), 'n' may be -1.
     *
     * Returns the number of octets inserted.
     */
    size_t salvage(std::vector<char>& acc,
		   const iovec* vec, int count,
		   ssize_t n)
    {
	if(n==-1) n = 0;
	size_t written = 0;

	for(int i=0; i<count; i++) {
	    const char* const p = (const char*)vec[i].iov_base;
	    const size_t len = vec[i].iov_len;
	    if(len <= size_t(n)) {
		n -= len;
	    }
	    else {
		acc.insert(acc.end(), p+n, p+len);
		written += (len - n);
		n = 0;
	    }
	}

	return written;
    }
}


/**
 * Write the backlog to 'fd', and return the number of octets still
 * unwritten afterwards.  This means writing an empty Backlog is
 * indistinguishable from being successful.
 *
 * Throws an exception on real errors.
 */
size_t Backlog::write(int fd)
{
    std::vector<char> tmp;
    tmp.swap(v);
    const iovec ovec = iov(tmp);

    ssize_t n = writev(fd, &ovec, 1);
    if(n==-1 && errno!=EAGAIN) {
	throw WriteError(errno);
    }

    return salvage(v, &ovec, 1, n);
}


/**
 * Write the backlog (if any) to 'fd', followed by 'a'.
 * Otherwise similar to write(fd).
 */
size_t Backlog::write(int fd, const Blob& a)
{
    std::vector<char> tmp;
    tmp.swap(v);
    const iovec ovec[2] = { iov(tmp),
			    iov(a) };

    ssize_t n = writev(fd, ovec, 2);
    if(n==-1 && errno!=EAGAIN) {
	throw WriteError(errno);
    }

    return salvage(v, ovec, 2, n);
}


/**
 * Write the backlog (if any) to 'fd', followed by 'a' and 'b'.
 * Otherwise similar to write(fd).
 */
size_t Backlog::write(int fd, const Blob& a, const Blob& b)
{
    std::vector<char> tmp;
    tmp.swap(v);
    const iovec ovec[3] = { iov(tmp),
			    iov(a),
			    iov(b) };

    ssize_t n = writev(fd, ovec, 3);
    if(n==-1 && errno!=EAGAIN) {
	throw WriteError(errno);
    }

    return salvage(v, ovec, 3, n);
}


size_t Backlog::write(int fd, const Blob& a, const Blob& b, const Blob& c)
{
    std::vector<char> tmp;
    tmp.swap(v);
    const iovec ovec[4] = { iov(tmp),
			    iov(a),
			    iov(b),
			    iov(c) };

    ssize_t n = writev(fd, ovec, 4);
    if(n==-1 && errno!=EAGAIN) {
	throw WriteError(errno);
    }

    return salvage(v, ovec, 4, n);
}
