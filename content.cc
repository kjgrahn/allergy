/* Copyright (c) 2017 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "content.h"

#include "root.h"
#include "request.h"
#include "response.h"
#include "status.h"

namespace {
    bool match(const std::string& s, const std::regex& re)
    {
	return regex_match(s, re);
    }

    bool match(const std::string& s, const std::regex& re, std::smatch& m)
    {
	return regex_match(s, m, re);
    }

    unsigned to_int(const std::string& s) { return std::stoul(s); }

    bool wrong_host(const Blob&) { return false; }

    template <class ErrorPage>
    Response* open(const timespec& t, const Root& root)
    {
	int fd = root.open(ErrorPage::status::file);
	if (fd==-1) {
	    return new response::Error<typename ErrorPage::status>(t);
	}
	return new ErrorPage{t, fd};
    }

    Response* resp400(const timespec& t, const Root& root) { return open<response::ErrorPage<Status<400>>>(t, root); }
    Response* resp404(const timespec& t, const Root& root) { return open<response::ErrorPage<Status<404>>>(t, root); }
    Response* resp500(const timespec& t, const Root& root) { return open<response::ErrorPage<Status<500>>>(t, root); }
    Response* resp501(const timespec& t, const Root& root) { return open<response::ErrorPage<Status<501>>>(t, root); }

    /**
     * R(fd) or a response::Error if the file cannot be opened.
     */
    template <class R, class ... Args>
    Response* open(const timespec& t, const Root& root, const std::string& path, Args&& ... argv)
    {
	int fd = root.open(path);
	if (fd==-1) switch (errno) {
	    case EACCES: // don't give away that the file exists
	    case ENOENT:
	    case ENOTDIR:
		return resp404(t, root);
	    default:
		return resp500(t, root);
	}
	return new R{t, fd, argv ...};
    }
}

Patterns::Patterns()
    : frontpage("/|/index.html"),
      by_date  ("/by-date"),
      year     ("/(\\d{4})"),
      month    ("/(\\d{4})-(\\d{2})"),
      photo    ("/(\\d{4}-\\d{2}-\\d{2}_.+\\.jpg)"),
      thumb    ("/thumb/(\\d{4}-\\d{2}-\\d{2}_.+\\.jpg)"),
      key      ("/key(words)?"),
      keywords ("/key/"),
      keyword  ("/key/(.+)"),
      css      ("/css"),
      robots   ("/robots\\.txt"),
      favicon  ("/favicon\\.ico")
{}

Content::Content(const std::string& host, const std::string& root)
    : host{host},
      root{root}
{}

Content::~Content() = default;

Response* Content::response_of(const Request& req, const timespec& t) const
{
    if(req.method != Request::Method::GET) return resp501(t, root);

    const auto& host = req.header(Request::Property::Host);
    if(wrong_host(host)) return resp400(t, root);

    const auto& uri = req.request_uri();

    if(match(uri, re.frontpage)) return frontpage(t);
    if(match(uri, re.by_date)) return by_date(t);

    std::smatch m;

    if(match(uri, re.year, m)) return year(t, to_int(m[1]));
    if(match(uri, re.month, m)) return month(t,
					     to_int(m[1]),
					     to_int(m[2]));

    if(match(uri, re.photo, m)) return photo(t, m[1]);
    if(match(uri, re.thumb, m)) return thumb(t, m[1]);

    if(match(uri, re.key)) return redirect(t, "/key/");
    if(match(uri, re.keywords)) return keywords(t);

    if(match(uri, re.keyword, m)) return keyword(t, m[1]);

    if(match(uri, re.css)) return css(t);
    if(match(uri, re.robots)) return robots(t);
    if(match(uri, re.favicon)) return favicon(t);

    return resp404(t, root);
}

Response* Content::frontpage(const timespec& t) const { return resp404(t, root); }
Response* Content::by_date(const timespec& t) const { return resp404(t, root); }
Response* Content::year(const timespec& t, unsigned) const { return resp404(t, root); }
Response* Content::month(const timespec& t, unsigned, unsigned) const { return resp404(t, root); }
Response* Content::redirect(const timespec& t, const std::string&) const { return resp404(t, root); }
Response* Content::photo(const timespec& t, const std::string& s) const { return open<response::Image>(t, root, s); }
Response* Content::thumb(const timespec& t, const std::string&) const { return resp404(t, root); }
Response* Content::keywords(const timespec& t) const { return resp404(t, root); }
Response* Content::keyword(const timespec& t, const std::string&) const { return resp404(t, root); }
Response* Content::robots(const timespec& t) const { return open<response::File>(t, root, "robots.txt", "text/plain"); }
Response* Content::css(const timespec& t) const { return open<response::File>(t, root, "css", "text/css"); }
Response* Content::favicon(const timespec& t) const { return resp404(t, root); }
