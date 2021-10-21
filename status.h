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
template <unsigned n> class Status;

template <> struct Status<200> {
    static constexpr unsigned val = 200;
    static constexpr const char* text = "200 OK";
};
template <> struct Status<201> {
    static constexpr const char* text = "201 Created";
};
template <> struct Status<202> {
    static constexpr const char* text = "202 Accepted";
};
template <> struct Status<203> {
    static constexpr const char* text = "203 Non-Authoritative Information";
};
template <> struct Status<204> {
    static constexpr const char* text = "204 No Content";
};
template <> struct Status<205> {
    static constexpr const char* text = "205 Reset Content";
};
template <> struct Status<206> {
    static constexpr const char* text = "206 Partial Content";
};

template <> struct Status<300> {
    static constexpr const char* text = "300 Multiple Choices";
};
template <> struct Status<301> {
    static constexpr unsigned val = 301;
    static constexpr const char* text = "301 Moved Permanently";
};
template <> struct Status<302> {
    static constexpr const char* text = "302 Found";
};
template <> struct Status<303> {
    static constexpr const char* text = "303 See Other";
};
template <> struct Status<304> {
    static constexpr const char* text = "304 Not Modified";
};
template <> struct Status<305> {
    static constexpr const char* text = "305 Use Proxy";
};
template <> struct Status<307> {
    static constexpr const char* text = "307 Temporary Redirect";
};

template <> struct Status<400> {
    static constexpr unsigned val = 400;
    static constexpr const char* text = "400 Bad Request";
    static constexpr const char* file = "400";
};
template <> struct Status<403> {
    static constexpr unsigned val = 403;
    static constexpr const char* text = "403 Forbidden";
    static constexpr const char* file = "403";
};
template <> struct Status<404> {
    static constexpr unsigned val = 404;
    static constexpr const char* text = "404 Not Found";
    static constexpr const char* file = "404";
};
template <> struct Status<405> {
    static constexpr const char* text = "405 Method Not Allowed";
};
template <> struct Status<406> {
    static constexpr const char* text = "406 Not Acceptable";
};
template <> struct Status<407> {
    static constexpr const char* text = "407 Proxy Authentication Required";
};
template <> struct Status<408> {
    static constexpr const char* text = "408 Request Timeout";
};
template <> struct Status<409> {
    static constexpr const char* text = "409 Conflict";
};
template <> struct Status<410> {
    static constexpr const char* text = "410 Gone";
};
template <> struct Status<411> {
    static constexpr const char* text = "411 Length Required";
};
template <> struct Status<412> {
    static constexpr const char* text = "412 Precondition Failed";
};
template <> struct Status<413> {
    static constexpr const char* text = "413 Request Entity Too Large";
};
template <> struct Status<414> {
    static constexpr const char* text = "414 Request-URI Too Long";
};
template <> struct Status<415> {
    static constexpr const char* text = "415 Unsupported Media Type";
};
template <> struct Status<416> {
    static constexpr const char* text = "416 Requested Range Not Satisfiable";
};
template <> struct Status<417> {
    static constexpr const char* text = "417 Expectation Failed";
};

template <> struct Status<500> {
    static constexpr unsigned val = 500;
    static constexpr const char* text = "500 Internal Server Error";
    static constexpr const char* file = "500";
};
template <> struct Status<501> {
    static constexpr unsigned val = 501;
    static constexpr const char* text = "501 Not Implemented";
    static constexpr const char* file = "501";
};
template <> struct Status<502> {
    static constexpr const char* text = "502 Bad Gateway";
};
template <> struct Status<503> {
    static constexpr const char* text = "503 Service Unavailable";
};
template <> struct Status<504> {
    static constexpr const char* text = "504 Gateway Timeout";
};
template <> struct Status<505> {
    static constexpr const char* text = "505 HTTP Version Not Supported";
};

#endif
