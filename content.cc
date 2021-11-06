/* Copyright (c) 2017, 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "content.h"

#include "root.h"
#include "hostnames.h"
#include "times.h"
#include "request.h"
#include "response.h"
#include "status.h"
#include "log.h"
#include "allergy/thumbnail.h"
#include "allergy/page.h"

#include <iostream>

namespace {

    template <class ErrorPage>
    Response* open(const Request& req, const Content& c)
    {
	int fd = c.lib.open(ErrorPage::status::file);
	if (fd==-1) {
	    return new response::Error<typename ErrorPage::status>(c, req.T);
	}
	return new ErrorPage{c, req.T, fd};
    }

    template <unsigned code>
    Response* resp(const Request& req, const Content& c)
    {
	return open<response::ErrorPage<Status<code>>>(req, c);
    }

    /* Trivial open() overload, for thumbnail().
     */
    int open(const Root&, int fd)
    {
	return fd;
    }

    /**
     * fd = open(root, T) and then R(fd), or a thrown Status<nnn> if the
     * file cannot be opened.
     */
    template <class R, class T, class ... Args>
    Response* open(const Request& req,
		   const Content& c,
		   const Root& root,
		   const T& path, Args&& ... argv)
    {
	int fd = open(root, path);
	if (fd==-1) switch (errno) {
	    case EACCES: // don't give away that the file exists
	    case ENOENT:
	    case ENOTDIR:
		throw Status<404> {};
	    default:
		throw Status<500> {};
	}
	return new R{c, req.T, fd, argv ...};
    }
}

Content::Content(std::ostream& err,
		 const Hostnames& hosts,
		 const allergy::Index& index,
		 const std::string& root_)
    : hosts{hosts},
      index{index},
      epoch{now()},
      lib{"lib"},
      thumb{"thumb"},
      root{root_}
{
    auto whine = [&err] (const std::string& name) mutable {
		     err << "error: cannot open directory " << name
			 << ": " << std::strerror(errno) << '\n';
		 };
    if (!root.valid()) whine(root_);
    else if (!thumb.valid()) whine("thumb");
    else if (!lib.valid()) whine("lib");
}

Content::~Content() = default;

bool Content::valid() const
{
    return lib.valid() && thumb.valid() && root.valid();
}

Response* Content::response_of(const Request& req) const
{
    if(req.method != Request::Property::GET) return resp<501>(req, *this);

    const auto& host = req.header(Request::Property::Host);
    if(!hosts.match(host)) return resp<400>(req, *this);

    const auto& uri = req.request_uri();

    try {
	if (match<bool>(uri, ""))           return frontpage(req);
	if (match<bool>(uri, "index.html")) return frontpage(req);

	if (match<bool>(uri, "by-date")) return by_date(req);

	if (auto year  = match<allergy::Year>(uri))  return calendar(req, year);
	if (auto month = match<allergy::Month>(uri)) return calendar(req, month);
	if (auto day   = match<allergy::Day>(uri))   return calendar(req, day);

	if (auto name = match<allergy::Photo>(uri))          return photo(req, name);
	if (auto name = match<allergy::Photo>(uri, "thumb")) return thumbnail(req, name);

	const auto ps = match<allergy::Day, allergy::Serial>(uri);
	if (ps.first) return photopage(req, ps.first, ps.second);

	if (match<bool>(uri, "keywords")) return redirect(req, "/key/");
	if (match<bool>(uri, "key"))      return redirect(req, "/key/");
	if (match<bool>(uri, "key", ""))  return keywords(req);

	if (auto key = match<allergy::Key>(uri, "key")) return keyword(req, key);

	if (match<bool>(uri, "css"))         return css(req);
	if (match<bool>(uri, "robots.txt"))  return robots(req);
	if (match<bool>(uri, "icon"))        return favicon(req);
    }
    catch (Status<404>) {
	return resp<404>(req, *this);
    }
    catch (Status<500>) {
	return resp<500>(req, *this);
    }

    return resp<404>(req, *this);
}

namespace {

    template <class Page, class ... Args>
    Response* generated(const Request& req, const Content& c, Args&& ... argv)
    {
	if (req.version==Request::Property::HTTP10) {
	    return new response::Generated<Page, Filter::P> {c, req.T, argv ... };
	}

	return new response::Generated<Page, Filter::C> {c, req.T, argv ... };
    }
}

Response* Content::frontpage(const Request& req) const
{
    return generated<allergy::page::Frontpage>(req, *this);
}

Response* Content::by_date(const Request& req) const { return resp<404>(req, *this); }

Response* Content::calendar(const Request& req, const allergy::Year& yyyy) const
{
    return generated<allergy::page::Year>(req, *this, yyyy);
}

Response* Content::calendar(const Request& req, const allergy::Month& month) const
{
    return generated<allergy::page::Month>(req, *this, month);
}

Response* Content::calendar(const Request& req, const allergy::Day& day) const
{
    return generated<allergy::page::Day>(req, *this, day);
}

Response* Content::redirect(const Request& req, const std::string& uri) const
{
    return new response::Redirect {*this, req.T, uri};
}

Response* Content::photo(const Request& req, const allergy::Photo& p) const
{
    const allergy::Entry& e = index.get(p);
    return open<response::Image>(req, *this, root, e);
}

/**
 * Requesting a thumbnail. Like requesting a photo, except if it
 * doesn't exist we try to create it and try again.
 *
 * From a whole-server perspective, this is the only "heavy" request.
 * Everything else is bounded by disk I/O. Acceptable, I hope: every
 * thumbnail should be generated at most once.
 */
Response* Content::thumbnail(const Request& req, const allergy::Photo& p) const
{
    const allergy::Entry& e = index.get(p);

    const int fd = open(thumb, e);
    if (fd==-1 && errno==ENOENT) {
	if (!allergy::thumbnail(root, thumb, e)) {
	    Warning(Syslog::log) << p << ": failed to thumbnail";
	    throw Status<404> {};
	}
	Info(Syslog::log) << p << ": thumbnailed";
	return open<response::Image>(req, *this, thumb, e);
    }
    return open<response::Image>(req, *this, thumb, fd);
}

Response* Content::photopage(const Request& req,
			     const allergy::Day& day,
			     const allergy::Serial& serial) const
{
    return generated<allergy::page::Photo>(req, *this, day, serial);
}

Response* Content::keywords(const Request& req) const { return resp<404>(req, *this); }

Response* Content::keyword(const Request& req, const allergy::Key& key ) const
{
    return generated<allergy::page::Keyword>(req, *this, key);
}

Response* Content::robots(const Request& req) const
{
    return open<response::File>(req, *this,
				lib, "robots.txt", "text/plain");
}

Response* Content::css(const Request& req) const
{
    return open<response::File>(req, *this,
				lib, "css", "text/css");
}

Response* Content::favicon(const Request& req) const
{
    return open<response::File>(req, *this,
				lib,  "icon", "image/svg+xml");
}
