/* -*- c++ -*-
 *
 * Copyright (c) 2012, 2013, 2016 J�rgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_FILTER_H_
#define GB_FILTER_H_

#include "backlog.h"
#include "deflate.h"

#include <vector>


/**
 * Response writing filters.
 *
 * These are chainable, because you must be able to write:
 * (a) headers
 * (b) data (entity-body) as-is
 * (c) compressed data
 * (d) chunked data
 * (e) compressed and chunked data
 *
 * The thing is, what's at the bottom is a nonblocking socket
 * which may only partially accept a write(), but we need something
 * inbetween which will buffer the remainder *and* at the same time
 * signal the original writer to back off until the socket becomes
 * writable again.
 *
 * Writing through a filter means a series of write(fd, data) calls
 * followed by one or more end(fd) calls.  The latter is used when the
 * actual data has run out, and serves to (a) write any EOF markers
 * mandated by the filter or underlying filters, such as the zero
 * chunk length of a Chunked encoding, and (b) to flush the buffered
 * data to the fd.
 *
 * Both write(fd, data) and end(fd) have the semantics:
 * true:  All data was successfully written to the fd, the backlog
 *        is empty, and you may continue writing.
 * false: Some data was /not/ written successfully, and was stored
 *        in the backlog. You may not write again (or count the
 *        response as complete, in the end() case) unless the fd
 *        goes writable.
 * All other situations map to an exception being thrown.
 *
 */
namespace Filter {

    /**
     * Plain or identity filter; write data verbatim.
     */
    class Plain {
    public:
	explicit Plain(Backlog& backlog)
	    : backlog(backlog)
	{}
	Plain(const Plain&) = delete;
	Plain& operator= (const Plain&) = delete;

	bool write(int fd) {
	    return backlog.write(fd)==0;
	}
	bool write(int fd, const Blob& a) {
	    return backlog.write(fd, a)==0;
	}
	bool write(int fd, const Blob& a, const Blob& b) {
	    return backlog.write(fd, a, b)==0;
	}
	bool write(int fd, const Blob& a, const Blob& b, const Blob& c) {
	    return backlog.write(fd, a, b, c)==0;
	}
	bool end(int fd) {
	    return backlog.write(fd)==0;
	}

    private:
	Backlog& backlog;
    };


    /**
     * Write using chunked transfer coding [RFC�2616 3.6.1].
     * Each write is exactly one chunk.
     */
    template<class Next>
    class Chunked {
    public:
	explicit Chunked(Backlog& backlog)
	    : next(backlog)
	{}
	Chunked(const Chunked&) = delete;
	Chunked& operator= (const Chunked&) = delete;

	bool write(int fd) { return next.write(fd); }
	bool write(int fd, const Blob& a);
	bool end(int fd);

    private:
	Next next;
    };


    /**
     * Write using zlib compression [RFC�2616 3.5, 3.6].
     */
    template<class Next>
    class Zlib {
    public:
	explicit Zlib(Backlog& backlog)
	    : next(backlog),
	      ending(false)
	{}
	Zlib(const Zlib&) = delete;
	Zlib& operator= (const Zlib&) = delete;

	bool write(int fd);
	bool write(int fd, const Blob& a);
	bool write(int fd, const Blob& a, const Blob& b);
	bool end(int fd);

    private:
	Next next;
	Deflate compress;
	bool ending;
    };


    /* Shortcuts.
     *
     *   P <--+--- C <--- CZ
     *        |
     *        +--- Z
     */
    typedef Plain P;
    typedef Zlib<Plain> Z;
    typedef Chunked<Plain> C;
    typedef Zlib<C> CZ;
}

#endif
