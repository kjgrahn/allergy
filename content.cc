/* Copyright (c) 2017, 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "content.h"

#include "root.h"
#include "request.h"
#include "response.h"
#include "status.h"
#include "log.h"
#include "allergy/thumbnail.h"
#include "allergy/page.h"

#include <iostream>

namespace {

    bool wrong_host(const Blob&) { return false; }

    template <class ErrorPage>
    Response* open(const timespec& t, const Root& lib)
    {
	int fd = lib.open(ErrorPage::status::file);
	if (fd==-1) {
	    return new response::Error<typename ErrorPage::status>(t);
	}
	return new ErrorPage{t, fd};
    }

    Response* resp400(const timespec& t, const Root& lib) { return open<response::ErrorPage<Status<400>>>(t, lib); }
    Response* resp404(const timespec& t, const Root& lib) { return open<response::ErrorPage<Status<404>>>(t, lib); }
    Response* resp500(const timespec& t, const Root& lib) { return open<response::ErrorPage<Status<500>>>(t, lib); }
    Response* resp501(const timespec& t, const Root& lib) { return open<response::ErrorPage<Status<501>>>(t, lib); }

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
    Response* open(const timespec& t,
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
	return new R{t, fd, argv ...};
    }
}

Content::Content(std::ostream& err,
		 const std::string& host,
		 const allergy::Index& index,
		 const std::string& root_)
    : host{host},
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

Response* Content::response_of(const Request& req, const timespec& t) const
{
    if(req.method != Request::Property::GET) return resp501(t, lib);

    const auto& host = req.header(Request::Property::Host);
    if(wrong_host(host)) return resp400(t, lib);

    const auto& uri = req.request_uri();

    try {
	if (match<bool>(uri, ""))           return frontpage(t);
	if (match<bool>(uri, "index.html")) return frontpage(t);

	if (match<bool>(uri, "by-date")) return by_date(t);

	if (auto year  = match<allergy::Year>(uri))  return calendar(t, year);
	if (auto month = match<allergy::Month>(uri)) return calendar(t, month);
	if (auto day   = match<allergy::Day>(uri))   return calendar(t, day);

	if (auto name = match<allergy::Photo>(uri))          return photo(t, name);
	if (auto name = match<allergy::Photo>(uri, "thumb")) return thumbnail(t, name);

	const auto ps = match<allergy::Day, allergy::Serial>(uri);
	if (ps.first) return photopage(t, ps.first, ps.second);

	if (match<bool>(uri, "keywords")) return redirect(t, "/key/");
	if (match<bool>(uri, "key"))      return redirect(t, "/key/");
	if (match<bool>(uri, "key", ""))  return keywords(t);

	if (auto key = match<allergy::Key>(uri, "key")) return keyword(t, key);

	if (match<bool>(uri, "css"))         return css(t);
	if (match<bool>(uri, "robots.txt"))  return robots(t);
	if (match<bool>(uri, "icon"))        return favicon(t);
    }
    catch (Status<404>) {
	return resp404(t, lib);
    }
    catch (Status<500>) {
	return resp500(t, lib);
    }

    return resp404(t, lib);
}

namespace {

    template <class Page, class ... Args>
    Response* generated(const timespec& t, Args&& ... argv)
    {
	return new response::Generated<Page> {t, argv ... };
    }
}

Response* Content::frontpage(const timespec& t) const
{
    return generated<allergy::page::Frontpage>(t, index);
}

Response* Content::by_date(const timespec& t) const { return resp404(t, lib); }

Response* Content::calendar(const timespec& t, const allergy::Year& yyyy) const
{
    return generated<allergy::page::Year>(t, index, yyyy);
}

Response* Content::calendar(const timespec& t, const allergy::Month& month) const
{
    return generated<allergy::page::Month>(t, index, month);
}

Response* Content::calendar(const timespec& t, const allergy::Day& day) const
{
    return generated<allergy::page::Day>(t, index, day);
}

Response* Content::redirect(const timespec& t, const std::string&) const { return resp404(t, lib); }

Response* Content::photo(const timespec& t, const allergy::Photo& p) const
{
    const allergy::Entry& e = index.get(p);
    return open<response::Image>(t, root, e);
}

/**
 * Requesting a thumbnail. Like requesting a photo, except if it
 * doesn't exist we try to create it and try again.
 *
 * From a whole-server perspective, this is the only "heavy" request.
 * Everything else is bounded by disk I/O. Acceptable, I hope: every
 * thumbnail should be generated at most once.
 */
Response* Content::thumbnail(const timespec& t, const allergy::Photo& p) const
{
    const allergy::Entry& e = index.get(p);

    const int fd = open(thumb, e);
    if (fd==-1 && errno==ENOENT) {
	if (!allergy::thumbnail(root, thumb, e)) {
	    Warning(Syslog::log) << p << ": failed to thumbnail";
	    throw Status<404> {};
	}
	Info(Syslog::log) << p << ": thumbnailed";
	return open<response::Image>(t, thumb, e);
    }
    return open<response::Image>(t, thumb, fd);
}

Response* Content::photopage(const timespec& t,
			     const allergy::Day& day,
			     const allergy::Serial& serial) const
{
    return generated<allergy::page::Photo>(t, index, day, serial);
}

Response* Content::keywords(const timespec& t) const { return resp404(t, lib); }

Response* Content::keyword(const timespec& t, const allergy::Key& key ) const
{
    return generated<allergy::page::Keyword>(t, index, key);
}

Response* Content::robots(const timespec& t) const { return open<response::File>(t, lib, "robots.txt", "text/plain"); }
Response* Content::css(const timespec& t) const { return open<response::File>(t, lib, "css", "text/css"); }
Response* Content::favicon(const timespec& t) const { return open<response::File>(t, lib,  "icon", "image/svg+xml"); }
