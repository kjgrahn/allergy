/* Copyright (c) 2013, 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "names.h"

#include <sstream>

namespace {

    using Property = Request::Property;

    const Property init[] = {
	Property::OPTIONS,
	Property::GET,
	Property::HEAD,
	Property::POST,
	Property::PUT,
	Property::DELETE,
	Property::TRACE,
	Property::CONNECT,

	Property::HTTP10,
	Property::HTTP11,

	Property::Request_URI,

	Property::Cache_Control,
	Property::Connection,
	Property::Date,
	Property::Pragma,
	Property::Trailer,
	Property::Transfer_Encoding,
	Property::Upgrade,
	Property::Warning,
	Property::Via,

	Property::Accept_Charset,
	Property::Authorization,
	Property::If_Match,
	Property::Accept_Encoding,
	Property::Expect,
	Property::If_Modified_Since,
	Property::Accept_Language,
	Property::From,
	Property::If_None_Match,
	Property::Accept,
	Property::Host,
	Property::If_Range,
	Property::If_Unmodified_Since,
	Property::Max_Forwards,
	Property::Proxy_Authorization,
	Property::Range,
	Property::Referer,
	Property::TE,
	Property::User_Agent,
	Property::Accept_Ranges,
	Property::Age,
	Property::Content_Disposition,
	Property::ETag,
	Property::Location,
	Property::Proxy_Authenticate,
	Property::Retry_After,
	Property::Server,
	Property::Vary,
	Property::WWW_Authenticate,
	Property::Content_Encoding,
	Property::Allow,
	Property::Content_Language,
	Property::Expires,
	Property::Content_Length,
	Property::Last_Modified,
	Property::Content_Location,
	Property::Content_MD5,
	Property::Content_Range,
	Property::Content_Type,
    };
}


/**
 * Look up [a..b), case-insensitively (XXX not implemented yet).
 * If the lookup fails UNKNOWN is returned.
 */
Property Names::lookup(const char* a, const char* b) const
{
    const std::string s(a, b);
    auto i = map.find(s);
    if(i==map.end()) {
	return Property::UNKNOWN;
    }
    else {
	return i->second;
    }
}


Names::Names()
{
    std::ostringstream oss;

    for (const Property prop : init) {
	oss.str("");
	oss << prop;
	map[oss.str()] = prop;
    }
}


std::ostream& operator<< (std::ostream& os, const Request::Property val)
{
    const char* s;
    switch(val) {
    case Property::OPTIONS: s = "OPTIONS"; break;
    case Property::GET:     s = "GET"; break;
    case Property::HEAD:    s = "HEAD"; break;
    case Property::POST:    s = "POST"; break;
    case Property::PUT:     s = "PUT"; break;
    case Property::DELETE:  s = "DELETE"; break;
    case Property::TRACE:   s = "TRACE"; break;
    case Property::CONNECT: s = "CONNECT"; break;

    case Property::HTTP10: s = "HTTP/1.0"; break;
    case Property::HTTP11: s = "HTTP/1.1"; break;

    case Property::Request_URI: s = "Request-URI"; break;

    default: s = "unknown"; break;

    /* actual RFC 2616 headers */
    case Property::Cache_Control: s = "Cache-Control"; break;
    case Property::Connection: s = "Connection"; break;
    case Property::Date: s = "Date"; break;
    case Property::Pragma: s = "Pragma"; break;
    case Property::Trailer: s = "Trailer"; break;
    case Property::Transfer_Encoding: s = "Transfer-Encoding"; break;
    case Property::Upgrade: s = "Upgrade"; break;
    case Property::Via: s = "Via"; break;
    case Property::Warning: s = "Warning"; break;

    case Property::Accept: s = "Accept"; break;
    case Property::Accept_Charset: s = "Accept-Charset"; break;
    case Property::Accept_Encoding: s = "Accept-Encoding"; break;
    case Property::Accept_Language: s = "Accept-Language"; break;
    case Property::Authorization: s = "Authorization"; break;
    case Property::Expect: s = "Expect"; break;
    case Property::From: s = "From"; break;
    case Property::Host: s = "Host"; break;
    case Property::If_Match: s = "If-Match"; break;
    case Property::If_Modified_Since: s = "If-Modified-Since"; break;
    case Property::If_None_Match: s = "If-None-Match"; break;
    case Property::If_Range: s = "If-Range"; break;
    case Property::If_Unmodified_Since: s = "If-Unmodified-Since"; break;
    case Property::Max_Forwards: s = "Max-Forwards"; break;
    case Property::Proxy_Authorization: s = "Proxy-Authorization"; break;
    case Property::Range: s = "Range"; break;
    case Property::Referer: s = "Referer"; break;
    case Property::TE: s = "TE"; break;
    case Property::User_Agent: s = "User-Agent"; break;

    case Property::Accept_Ranges: s = "Accept-Ranges"; break;
    case Property::Age: s = "Age"; break;
    case Property::ETag: s = "ETag"; break;
    case Property::Location: s = "Location"; break;
    case Property::Proxy_Authenticate: s = "Proxy-Authenticate"; break;
    case Property::Retry_After: s = "Retry-After"; break;
    case Property::Server: s = "Server"; break;
    case Property::Vary: s = "Vary"; break;
    case Property::WWW_Authenticate: s = "WWW-Authenticate"; break;

    case Property::Allow: s = "Allow"; break;
    case Property::Content_Encoding: s = "Content-Encoding"; break;
    case Property::Content_Language: s = "Content-Language"; break;
    case Property::Content_Length: s = "Content-Length"; break;
    case Property::Content_Location: s = "Content-Location"; break;
    case Property::Content_MD5: s = "Content-MD5"; break;
    case Property::Content_Range: s = "Content-Range"; break;
    case Property::Content_Type: s = "Content-Type"; break;
    case Property::Expires: s = "Expires"; break;
    case Property::Last_Modified: s = "Last-Modified"; break;
    }
    return os << s;
}
