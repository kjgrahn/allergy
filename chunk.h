/* -*- c++ -*-
 *
 * Copyright (c) 2016 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_CHUNK_H_
#define OUTN_CHUNK_H_

#include <string>
#include <iostream>
#include <streambuf>
#include <vector>

/**
 * Formatted output, plus a way to find out if it's sizeable
 * and in that case flush it.  Exists because we need formatted
 * output (for headers and generated entities) but
 *
 * a) don't want it for many other entities which need no formatting
 *    or intermediate buffering
 *
 * b) don't need to format the whole entity before flushing it
 *
 * c) on the other hand, don't want to flush tiny pieces either,
 *    since that would make filter::C, filter::Z and the actual
 *    socket I/O inefficient.
 *
 * Much like a std::stringstream, except optimized for a few kilobytes
 * of content.  Relies on us being able to take all of the content and
 * dump responsibility on a filter or backlog.
 */
class Chunk : private std::basic_streambuf<char> {
public:
    Chunk();
    ~Chunk();
    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;

    bool empty() const { return size()==0; }
    size_t size() const { return pptr() - pbase(); }

    std::ostream& ostream() { return os_; }
    void put_terminator();

    int flush(int fd);

    std::string str() const;

private:
    typedef std::vector<char_type> Vec;
    Vec vec_;
    char_type* z_;

    void grow_buf();
    void clear_buf();

    virtual int_type overflow(int_type c = traits_type::eof());

    std::ostream os_;
};

template <class T>
inline
std::ostream& operator<< (Chunk& buf, const T& t)
{
    return buf.ostream() << t;
}

#endif
