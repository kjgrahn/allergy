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
 * XXX Description is for the earlier design.
 *
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
    Response() = default;
    virtual ~Response() = default;
    Response(const Response&) = delete;
    Response& operator= (const Response&) = delete;

    /**
     * Assuming !done and 'fd' is writable, push towards completion.
     * Returns true if the fd goes blocked and the backlog is now
     * non-empty.
     */
    virtual bool tick(int fd) = 0;
    bool done = false;
};


namespace response {

    /**
     *
     */
    struct Headers {
	template <class Content>
	explicit Headers(Backlog& backlog, const Content& c)
	    : text(""),
	      filter(backlog)
	{
	    std::ostringstream oss;
	    oss << c.status_code
		<< date()
		<< c.encoding
		<< c.type
		<< c.len;
	    text = entity::String{oss};
	}

	bool tick(int fd);
	bool done() const;

	entity::String text;
	Filter::P filter;

    private:
	const char* date() const { return "XXX"; }
    };

    /**
     *
     */
    template <class E, class F>
    struct Content {
	template <class C>
	Content(Backlog& backlog, C arg)
	    : entity(arg),
	      filter(backlog)
	{}

	bool tick(int fd);
	bool done() const;

	const char* status_code;
	const char* encoding;
	const char* type;
	const char* len;

	E entity;
	F filter;
    };

}

#endif
