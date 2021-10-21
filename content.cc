/* Copyright (c) 2017, 2021 J�rgen Grahn
 * All rights reserved.
 *
 */
#include "content.h"

#include "root.h"
#include "hostnames.h"
#include "request.h"
#include "response.h"
#include "status.h"
#include "log.h"
#include "allergy/thumbnail.h"
#include "allergy/page.h"

#include <iostream>

namespace {

    template <class ErrorPage>
    Response* open(const Request& req, const Root& lib)
    {
	int fd = lib.open(ErrorPage::status::file);
	if (fd==-1) {
	    return new response::Error<typename ErrorPage::status>(req.T);
	}
	return new ErrorPage{req.T, fd};
    }

    Response* resp400(const Request& req, const Root& lib) { return open<response::ErrorPage<Status<400>>>(req, lib); }
    Response* resp404(const Request& req, const Root& lib) { return open<response::ErrorPage<Status<404>>>(req, lib); }
    Response* resp500(const Request& req, const Root& lib) { return open<response::ErrorPage<Status<500>>>(req, lib); }
    Response* resp501(const Request& req, const Root& lib) { return open<response::ErrorPage<Status<501>>>(req, lib); }

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
	return new R{req.T, fd, argv ...};
    }
}

Content::Content(std::ostream& err,
		 const Hostnames& hosts,
		 const allergy::Index& index,
		 const std::string& root_)
    : hosts{hosts},
      index{index},
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
    if(req.method != Request::Property::GET) return resp501(req, lib);

    const auto& host = req.header(Request::Property::Host);
    if(!hosts.match(host)) return resp400(req, lib);

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
	return resp404(req, lib);
    }
    catch (Status<500>) {
	return resp500(req, lib);
    }

    return resp404(req, lib);
}

namespace {

    template <class Page, class ... Args>
    Response* generated(const Request& req, Args&& ... argv)
    {
	if (req.version==Request::Property::HTTP10) {
	    return new response::Generated<Page, Filter::P> {req.T, argv ... };
	}

	return new response::Generated<Page, Filter::C> {req.T, argv ... };
    }
}

Response* Content::frontpage(const Request& req) const
{
    return generated<allergy::page::Frontpage>(req, index);
}

Response* Content::by_date(const Request& req) const { return resp404(req, lib); }

Response* Content::calendar(const Request& req, const allergy::Year& yyyy) const
{
    return generated<allergy::page::Year>(req, index, yyyy);
}

Response* Content::calendar(const Request& req, const allergy::Month& month) const
{
    return generated<allergy::page::Month>(req, index, month);
}

Response* Content::calendar(const Request& req, const allergy::Day& day) const
{
    return generated<allergy::page::Day>(req, index, day);
}

Response* Content::redirect(const Request& req, const std::string& uri) const
{
    return new response::Redirect {req.T, uri};
}

Response* Content::photo(const Request& req, const allergy::Photo& p) const
{
    const allergy::Entry& e = index.get(p);
    return open<response::Image>(req, root, e);
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
	return open<response::Image>(req, thumb, e);
    }
    return open<response::Image>(req, thumb, fd);
}

Response* Content::photopage(const Request& req,
			     const allergy::Day& day,
			     const allergy::Serial& serial) const
{
    return generated<allergy::page::Photo>(req, index, day, serial);
}

Response* Content::keywords(const Request& req) const { return resp404(req, lib); }

Response* Content::keyword(const Request& req, const allergy::Key& key ) const
{
    return generated<allergy::page::Keyword>(req, index, key);
}

Response* Content::robots(const Request& req) const { return open<response::File>(req, lib, "robots.txt", "text/plain"); }
Response* Content::css(const Request& req) const { return open<response::File>(req, lib, "css", "text/css"); }
Response* Content::favicon(const Request& req) const { return open<response::File>(req, lib,  "icon", "image/svg+xml"); }
