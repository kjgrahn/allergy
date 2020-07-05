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
    Response* open(const Root& root)
    {
	int fd = root.open(ErrorPage::status::file);
	if (fd==-1) {
	    return new response::Error<typename ErrorPage::status>;
	}
	return new ErrorPage{fd};
    }

    Response* resp400(const Root& root) { return open<response::ErrorPage<Status<400>>>(root); }
    Response* resp404(const Root& root) { return open<response::ErrorPage<Status<404>>>(root); }
    Response* resp500(const Root& root) { return open<response::ErrorPage<Status<500>>>(root); }
    Response* resp501(const Root& root) { return open<response::ErrorPage<Status<501>>>(root); }

    /**
     * R(fd) or a response::Error if the file cannot be opened.
     */
    template <class R, class ... Args>
    Response* open(const Root& root, const std::string& path, Args&& ... argv)
    {
	int fd = root.open(path);
	if (fd==-1) switch (errno) {
	    case EACCES: // don't give away that the file exists
	    case ENOENT:
	    case ENOTDIR:
		return resp404(root);
	    default:
		return resp500(root);
	}
	return new R{fd, argv ...};
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

Response* Content::response_of(const Request& req) const
{
    if(req.method != Request::Method::GET) return resp501(root);

    const auto& host = req.header(Request::Property::Host);
    if(wrong_host(host)) return resp400(root);

    const auto& uri = req.request_uri();

    if(match(uri, re.frontpage)) return frontpage();
    if(match(uri, re.by_date)) return by_date();

    std::smatch m;

    if(match(uri, re.year, m)) return year(to_int(m[1]));
    if(match(uri, re.month, m)) return month(to_int(m[1]),
					     to_int(m[2]));

    if(match(uri, re.photo, m)) return photo(m[1]);
    if(match(uri, re.thumb, m)) return thumb(m[1]);

    if(match(uri, re.key)) return redirect("/key/");
    if(match(uri, re.keywords)) return keywords();

    if(match(uri, re.keyword, m)) return keyword(m[1]);

    if(match(uri, re.css)) return css();
    if(match(uri, re.robots)) return robots();
    if(match(uri, re.favicon)) return favicon();

    return resp404(root);
}

Response* Content::frontpage() const { return resp404(root); }
Response* Content::by_date() const { return resp404(root); }
Response* Content::year(unsigned) const { return resp404(root); }
Response* Content::month(unsigned, unsigned) const { return resp404(root); }
Response* Content::redirect(const std::string&) const { return resp404(root); }
Response* Content::photo(const std::string& s) const { return open<response::Image>(root, s); }
Response* Content::thumb(const std::string&) const { return resp404(root); }
Response* Content::keywords() const { return resp404(root); }
Response* Content::keyword(const std::string&) const { return resp404(root); }
Response* Content::robots() const { return resp404(root); }
Response* Content::css() const { return resp404(root); }
Response* Content::favicon() const { return resp404(root); }
