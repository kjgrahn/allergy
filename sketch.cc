#include <sstream>

#include "filter.h"

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


namespace entity {

    class String {
    public:
	explicit String(const char* s);
	explicit String(const std::string& s);
	template <class C>
	explicit String(const C& c)
	    : String(c.str())
	{}
    };

    class Image {
    public:
	explicit Image(int fd);
    };

    class Generated {
    public:
	template <class F>
	explicit Generated(const F& f);
    };
}


/**
 *
 */
struct Headers {
    template <class C>
    explicit Headers(Backlog& backlog, const C& c)
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


namespace {
    template <class Content>
    bool tick(int fd, Backlog& backlog,
	      Headers& headers,
	      Content& content)
    {
	if(!backlog.empty()) return backlog.write(fd);
	if(!headers.done()) return headers.tick(fd);
	return content.tick(fd);
    }
}


/**
 *
 */
struct Error : public Response {
    explicit Error(const char* s)
	: content(backlog, s),
	  headers(backlog, content)
    {}

    bool tick(int fd) override
    {
	bool unblocked = ::tick(fd, backlog, headers, content);
	done = content.done();
	return unblocked;
    }

    Backlog backlog;

    Content<entity::String, Filter::P> content;
    Headers headers;
};


/**
 *
 */
struct Image : public Response {
    explicit Image(int fd)
	: content(backlog, fd),
	  headers(backlog, content)
    {}

    bool tick(int fd) override
    {
	bool unblocked = ::tick(fd, backlog, headers, content);
	done = content.done();
	return unblocked;
    }

    Backlog backlog;

    Content<entity::Image, Filter::P> content;
    Headers headers;
};


/**
 *
 */
struct Generated : public Response {
    template <class F>
    explicit Generated(const F& f)
	: content(backlog, f),
	  headers(backlog, content)
    {}

    bool tick(int fd) override
    {
	bool unblocked = ::tick(fd, backlog, headers, content);
	done = content.done();
	return unblocked;
    }

    Backlog backlog;

    Content<entity::Generated, Filter::P> content;
    Headers headers;
};
