/* -*- c++ -*-
 *
 * Copyright (c) 2012, 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ERROR_H_
#define GB_ERROR_H_

#include <iosfwd>

/**
 * A TCP connection or HTTP session is too messed up to continue.
 */
struct SessionError {
    virtual void put(std::ostream& os) const = 0;
};

inline
std::ostream& operator<< (std::ostream& os, const SessionError& val)
{
    val.put(os);
    return os;
}

struct WriteError final: public SessionError {
    explicit WriteError(int err = 0) : err(err) {}
    int err;
    void put(std::ostream& os) const;
};

/**
 * Surprising error originating in the entity, including I/O error
 * after deciding to return it in a 200 OK.
 */
struct EntityError final: public SessionError {
    explicit EntityError(int err = 0) : err(err) {}
    int err;
    void put(std::ostream& os) const;
};


/**
 * An error which means death to the whole server.
 */
struct FatalError {};

#endif
