/* Copyright (c) 2017 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "content.h"

#include "request.h"
#include "response.h"

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

    response::Error* resp501() { return new response::Error("501 Not Implemented"); }
    response::Error* resp400() { return new response::Error("400 Bad Request"); }
    response::Error* resp404() { return new response::Error("404 Not Found"); }

    Response* frontpage() { return resp404(); }
    Response* by_date() { return resp404(); }
    Response* year(unsigned) { return resp404(); }
    Response* month(unsigned, unsigned) { return resp404(); }
    Response* redirect(const std::string&) { return resp404(); }
    Response* photo(const std::string&) { return resp404(); }
    Response* thumb(const std::string&) { return resp404(); }
    Response* keywords() { return resp404(); }
    Response* keyword(const std::string&) { return resp404(); }
    Response* robots() { return resp404(); }
    Response* css() { return resp404(); }
    Response* favicon() { return resp404(); }
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

Content::Content(const std::string& host, const std::string&)
    : host{host}
{}

Response* Content::response_of(const Request& req) const
{
    if(req.method != Request::Method::GET) return resp501();

    const auto& host = req.header(Request::Property::Host);
    if(wrong_host(host)) return resp400();

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

    return resp404();
}
