#include <sstream>

#include "filter.h"

#include "entity/string.h"
#include "entity/image.h"
#include "entity/generated.h"

/**
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

    entity::String text;
    Filter::P filter;

private:
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


namespace {
    template <class Body>
    bool tick(int fd, Backlog& backlog,
	      Headers& headers,
	      Body& body)
    {
	if(!backlog.empty()) return backlog.write(fd);
	if(!headers.done()) return headers.tick(fd);
	return body.tick(fd);
    }
}


/**
 *
 */
struct Error : public Response {
    explicit Error(const char* s)
	: body(backlog, s),
	  headers(backlog, body)
    {}

    bool tick(int fd) override
    {
	bool unblocked = ::tick(fd, backlog, headers, body);
	done = body.done();
	return unblocked;
    }

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

    bool tick(int fd) override
    {
	bool unblocked = ::tick(fd, backlog, headers, body);
	done = body.done();
	return unblocked;
    }

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

    bool tick(int fd) override
    {
	bool unblocked = ::tick(fd, backlog, headers, body);
	done = body.done();
	return unblocked;
    }

    Backlog backlog;

    Body<entity::Generated, Filter::P> body;
    Headers headers;
};
