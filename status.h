/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_STATUS_H
#define OUTN_STATUS_H

/**
 * HTTP status codes [6.1.1]
 */
namespace status {

    struct S200 {
	static constexpr const char* text = "200 OK";
    };
    struct S201 {
	static constexpr const char* text = "201 Created";
    };
    struct S202 {
	static constexpr const char* text = "202 Accepted";
    };
    struct S203 {
	static constexpr const char* text = "203 Non-Authoritative Information";
    };
    struct S204 {
	static constexpr const char* text = "204 No Content";
    };
    struct S205 {
	static constexpr const char* text = "205 Reset Content";
    };
    struct S206 {
	static constexpr const char* text = "206 Partial Content";
    };

    struct S300 {
	static constexpr const char* text = "300 Multiple Choices";
    };
    struct S301 {
	static constexpr const char* text = "301 Moved Permanently";
    };
    struct S302 {
	static constexpr const char* text = "302 Found";
    };
    struct S303 {
	static constexpr const char* text = "303 See Other";
    };
    struct S304 {
	static constexpr const char* text = "304 Not Modified";
    };
    struct S305 {
	static constexpr const char* text = "305 Use Proxy";
    };
    struct S307 {
	static constexpr const char* text = "307 Temporary Redirect";
    };

    struct S400 {
	static constexpr const char* text = "400 Bad Request";
	static constexpr const char* file = "400";
    };
    struct S403 {
	static constexpr const char* text = "403 Forbidden";
	static constexpr const char* file = "403";
    };
    struct S404 {
	static constexpr const char* text = "404 Not Found";
	static constexpr const char* file = "404";
    };
    struct S405 {
	static constexpr const char* text = "405 Method Not Allowed";
    };
    struct S406 {
	static constexpr const char* text = "406 Not Acceptable";
    };
    struct S407 {
	static constexpr const char* text = "407 Proxy Authentication Required";
    };
    struct S408 {
	static constexpr const char* text = "408 Request Timeout";
    };
    struct S409 {
	static constexpr const char* text = "409 Conflict";
    };
    struct S410 {
	static constexpr const char* text = "410 Gone";
    };
    struct S411 {
	static constexpr const char* text = "411 Length Required";
    };
    struct S412 {
	static constexpr const char* text = "412 Precondition Failed";
    };
    struct S413 {
	static constexpr const char* text = "413 Request Entity Too Large";
    };
    struct S414 {
	static constexpr const char* text = "414 Request-URI Too Long";
    };
    struct S415 {
	static constexpr const char* text = "415 Unsupported Media Type";
    };
    struct S416 {
	static constexpr const char* text = "416 Requested Range Not Satisfiable";
    };
    struct S417 {
	static constexpr const char* text = "417 Expectation Failed";
    };

    struct S500 {
	static constexpr const char* text = "500 Internal Server Error";
	static constexpr const char* file = "500";
    };
    struct S501 {
	static constexpr const char* text = "501 Not Implemented";
	static constexpr const char* file = "501";
    };
    struct S502 {
	static constexpr const char* text = "502 Bad Gateway";
    };
    struct S503 {
	static constexpr const char* text = "503 Service Unavailable";
    };
    struct S504 {
	static constexpr const char* text = "504 Gateway Timeout";
    };
    struct S505 {
	static constexpr const char* text = "505 HTTP Version Not Supported";
    };
}

#endif
