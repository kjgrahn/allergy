/* -*- c++ -*-
 *
 * Copyright (c) 2012, 2013, 2015 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_RESPONSE_H_
#define GB_RESPONSE_H_

#include "backlog.h"
#include "filter.h"
#include "entity.h"


/**
 * A complete, specific HTTP response in the form of a state machine.
 * tick(fd) pushes it towards completion, until done() turns true.
 *
 * tick() returns true if the fd goes blocked and the backlog is now non-empty.
 *
 *   +---------+                             +--------+
 *   | headers |-----------------+---------->| fd     |
 *   +---------+     +--------+  |           |        |
 *   | Entity  +---->| Filter +----+-------->|        |
 *   |         |     |        |  v v         |        |
 *   +---------+     +--------+ +------+     |        |
 *                              | Back +---->|        |
 *                              | log  |     |        |
 *                              +------+     +--------+
 *
 * Or put differently:
 *
 *     headers --> filter::P -+---> backlog ---> fd
 *     entity ---> filter ----'
 *
 * At different points in time, there may be data in:
 * - the backlog
 * - the headers
 * - the entity
 * - the entity's filter (e.g if it's filter::Z)
 * If they're all empty, the request is done().
 *
 */
class Response {
public:
    Response(const Response&) = delete;
    Response& operator= (const Response&) = delete;
    virtual ~Response() = default;

    virtual bool tick(int fd) = 0;
    virtual bool done() const = 0;

protected:
    Backlog backlog;
};

#if 0
template <class E, class F>
bool Response<E, F>::tick(int fd)
{
    if(!backlog.empty()) {
	backlog.write(fd);
    }
    else if(!headers.empty()) {
    }

    return !backlog.empty();
}
#endif

namespace response {

    class String: public Response {
    public:
	String();

	bool tick(int fd) override;
	virtual bool done() const override;

    protected:
	Filter::P hfilter;
	Filter::P efilter;

	entity::String headers;
	entity::String entity;
    };

    class Error : public String {
    public:
	explicit Error(const char* status);
    };
}

#endif
