/* -*- c++ -*-
 *
 * Copyright (c) 2012, 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ERROR_H_
#define GB_ERROR_H_

/**
 * A TCP connection or HTTP session is too messed up to continue.
 */
struct SessionError {};

struct WriteError : public SessionError {
    explicit WriteError(int err = 0) : err(err) {}
    int err;
};

/**
 * Surprising error originating in the entity, including I/O error
 * after deciding to return it in a 200 OK.
 */
struct EntityError : public SessionError {
    explicit EntityError(int err = 0) : err(err) {}
    int err;
};


/**
 * An error which means death to the whole server.
 */
struct FatalError {};

#endif
