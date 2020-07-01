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

#include "status.h"
#include "entity/string.h"
#include "entity/file.h"
#include "entity/image.h"
#include "entity/generated.h"

#include <cassert>
#include <sstream>


/**
 * A complete, specific HTTP response in the form of a state machine.
 * tick(fd) pushes it towards completion, until done() turns true.
 *
 * tick() returns true if the fd goes blocked and the backlog is now non-empty.
 *
 *   +---------+     +--------+     +------+     +-----+
 *   | Headers |---->| Filter |-----|------|---->|     |
 *   +---------+     +--------+     | Back |     | fd  |
 *   | Body    +---->| Filter +-----| log -|---->|     |
 *   |         |     |        |     |      |     |     |
 *   +---------+     +--------+     +------+     +-----+
 *
 * What's interesting from the Response user's point of view is
 * whether the response is done (written to the fd) and if not,
 * whether it's blocked and we need to wait for the fd to become
 * writable before tick()ing the state machine again.
 *
 * Internally
 *
 * - The Backlog buffers the fd, so that any single write succeeds, by
 *   placing the failed tail in the backlog. However we must not
 *   continue tick()ing after that happens. And when the socket
 *   becomes writeable again, we must begin by draining the backlog;
 *   we can't let it grow indefinitely.
 *
 * - The Headers and Body can both be seen as an Entity passed through
 *   a Filter, even though the headers have a pass-through filter.
 *   Entities are drained stepwise through the filter. Filters can
 *   buffer: you may put a long text into a filter::Z and nothing
 *   comes out. When an entity hits EOF, the filter must be flushed so
 *   we don't get an extra state: the entity being empty but the
 *   filter being not.
 *
 * So when the fd becomes writable, the strategy to safely advance the
 * state machine becomes one of the three:
 *
 * - If the backlog isn't empty, drain it.
 * - If the headers aren't empty, tick() them.
 * - If the body isn't empty, tick() it.
 *
 * If none of these things can be done, there's a programming error.
 *
 * I/O errors go on top of all this in the form of exceptions, and are
 * fatal for the whole Session.
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
	template <class B, class Status>
	explicit Headers(Backlog& backlog, const B& body, const Status status)
	    : text(""),
	      filter(backlog)
	{
	    std::ostringstream oss;
	    oss << "HTTP/1.1 " << status.text << "\r\n";
	    general_headers(oss);
	    response_headers(oss);
	    body.entity_headers(oss) << "\r\n";
	    text = entity::String{oss};
	}

	bool tick(int fd);
	bool done() const;

    private:
	entity::String text;
	Filter::P filter;

	std::ostream& general_headers(std::ostream& oss)
	{
	    return oss << date();
	}

	std::ostream& response_headers(std::ostream& oss)
	{
	    return oss << "Server: allergy\r\n";
	}

	const char* date() const { return "Date: Mon, 04 Aug 2014 22:05:06 GMT\r\n"; }
    };

    /**
     * The combination of entity, and the filter applied to it.
     */
    template <class E, class F>
    struct Body {
	template <class ... Args>
	Body(Backlog& backlog, Args&& ... argv)
	    : entity(argv ...),
	      filter(backlog)
	{}

	bool tick(int fd) { return entity.tick(fd, filter); }
	bool done() const { return entity.done(); }

	std::ostream& entity_headers(std::ostream& oss) const
	{
	    return entity.headers(oss);
	}

    private:
	E entity;
	F filter;
    };

    /**
     * tick() a combination of headers, body and backlog; returns true
     * iff the fd goes blocked. Undefined result if the whole sequence
     * was already done.
     */
    template <class Body>
    bool tick(int fd, Backlog& backlog,
	      response::Headers& headers,
	      Body& body)
    {
	if(!backlog.empty()) return backlog.write(fd);
	if(!headers.done()) return headers.tick(fd);
	assert(!body.done());
	return body.tick(fd);
    }

    /**
     * An error (like 404 Not Found) with a minimal body.
     */
    template <class Status>
    struct Error : public Response {
	Error()
	    : body(backlog, Status::text),
	      headers(backlog, body, Status{})
	{}

	bool tick(int fd) override
	{
	    bool blocked = response::tick(fd, backlog, headers, body);
	    done = body.done();
	    return blocked;
	}

    private:
	Backlog backlog;
	Body<entity::String, Filter::P> body;
	Headers headers;
    };

    /**
     * Like Error, but with a static HTML body read from file.
     */
    template <class Status>
    struct ErrorPage : public Response {
	using status = Status;

	explicit ErrorPage(int fd)
	    : body(backlog, fd, "text/html"),
	      headers(backlog, body, Status{})
	{}
	bool tick(int fd) override
	{
	    bool blocked = response::tick(fd, backlog, headers, body);
	    done = body.done();
	    return blocked;
	}

    private:
	Backlog backlog;
	Body<entity::File, Filter::P> body;
	Headers headers;
    };

    /**
     * A JPEG image, read from file.
     */
    struct Image : public Response {
	explicit Image(int fd)
	    : body(backlog, fd),
	      headers(backlog, body, Status<200>{})
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
	      headers(backlog, body, Status<200>{})
	{}

	bool tick(int fd) override;

    private:
	Backlog backlog;
	Body<entity::Generated, Filter::P> body;
	Headers headers;
    };
}

#endif
