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

#include "entity/string.h"
#include "entity/image.h"
#include "entity/generated.h"

#include <sstream>


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
     * The response headers for a certain Body, filtered and connected to
     * the Backlog.
     */
    struct Headers {
	template <class B>
	explicit Headers(Backlog& backlog, const B& b)
	    : text(""),
	      filter(backlog)
	{
	    std::ostringstream oss;
	    oss << b.status_code
		<< date()
		<< b.encoding
		<< b.type
		<< b.len;
	    text = entity::String{oss};
	}

	bool tick(int fd);
	bool done() const;

    private:
	entity::String text;
	Filter::P filter;

	const char* date() const { return "XXX"; }
    };

    /**
     * The combination of entity, and the filter applied to it.
     */
    template <class E, class F>
    struct Body {
	template <class C>
	Body(Backlog& backlog, C arg)
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

    /**
     *
     */
    struct Error : public Response {
	explicit Error(const char* s)
	    : body(backlog, s),
	      headers(backlog, body)
	{}

	bool tick(int fd) override;

    private:
	Backlog backlog;
	Body<entity::String, Filter::P> body;
	Headers headers;
    };

    /**
     *
     */
    struct Image : public Response {
	explicit Image(int fd)
	    : body(backlog, fd),
	      headers(backlog, body)
	{}

	bool tick(int fd) override;

    private:
	Backlog backlog;
	Body<entity::Image, Filter::P> body;
	Headers headers;
    };

    /**
     *
     */
    struct Generated : public Response {
	template <class F>
	explicit Generated(const F& f)
	    : body(backlog, f),
	      headers(backlog, body)
	{}

	bool tick(int fd) override;

    private:
	Backlog backlog;
	Body<entity::Generated, Filter::P> body;
	Headers headers;
    };
}

#endif
