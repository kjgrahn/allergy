/* Copyright (c) 2016 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "chunk.h"

/**
 * The actual streambuf is implemented in an infinitely growing
 * vector<char> where the buffering takes place:
 *
 * v.begin()                                   v.end()
 * pbase             z            pptr         epptr
 * |:::::::::::::::::|::::::::::::|............|
 *
 * When queueing, 'z' marks the end of dot-stuffed data; [z, pptr) contains no
 * complete multi-line responses.
 *
 * When flushing, [pbase .. pptr) is the area which remains
 * to write.
 */


Chunk::Chunk()
    : vec_(4096),
      os_(this)
{
    clear_buf();
}

Chunk::~Chunk()
{}

/**
 * Reset the StreamBuf, so it's fully usable for queuing again.
 * The vector is not resized.
 */
void Chunk::clear_buf()
{
    char_type* p = &vec_[0];
    setp(p, p + vec_.size());
    z_ = p;
}

Chunk::int_type Chunk::overflow(Chunk::int_type c)
{
    grow_buf();
    sputc(c);
    return '!';
}

/**
 * Grow the buffer with 50% or so, i.e. with at least 2K.
 */
void Chunk::grow_buf()
{
    const size_t dz = z_ - pbase();
    const size_t dp = pptr() - pbase();

    vec_.resize(vec_.size()*3/2);
    char_type* const p = &vec_[0];
    setp(p, p + vec_.size());
    z_ = p + dz;
    pbump(dp);
}

/**
 * Pull out the contents of the buffer as a string.
 * For test purposes only.
 */
std::string Chunk::str() const
{
    return std::string(pbase(), pptr());
}
